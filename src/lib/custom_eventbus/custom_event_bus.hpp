#pragma once
/**
 * @file event_bus.hpp
 * @brief 一个功能完整的自研 C++17 事件总线（EventBus）
 * 
 * 特性：
 *   - 类型安全：以事件类型（class/struct）为键，编译期检查
 *   - 线程安全：内部使用 mutex 保护，发布时拷贝回调避免死锁
 *   - 生命周期管理：返回 Subscription 对象，支持随时取消订阅
 *   - 同步/异步双模式：publish() 立即执行，publish_async() 入队由工作线程处理
 *   - 优先级：HIGH > NORMAL > LOW，同类型事件按优先级排序执行
 *   - 零依赖：仅使用 C++17 标准库
 */

#include <any>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <typeindex>
#include <vector>
#include <algorithm>
#include <iostream>

namespace evt {
    enum class Priority { LOW = 0, NORMAL = 1, HIGH = 2 };

    /**
     * @brief 订阅句柄，用于取消订阅或检查有效性
     */
    class Subscription {
    public:
        Subscription() = default;

        explicit Subscription(std::shared_ptr<bool> flag) : active_(flag) {
        }

        /// 取消订阅，后续事件不再投递到该回调
        void cancel() {
            if (auto sp = active_.lock()) *sp = false;
        }

        /// 检查该订阅是否仍然有效
        bool valid() const {
            if (auto sp = active_.lock()) return *sp;
            return false;
        }

    private:
        std::weak_ptr<bool> active_;
    };

    /**
     * @brief 事件总线核心类
     *
     * 使用示例：
     * @code
     *   struct LoginEvent { std::string user; int id; };
     *
     *   evt::EventBus bus;
     *   auto sub = bus.subscribe<LoginEvent>([](const LoginEvent& e) {
     *       std::cout << e.user << " logged in\n";
     *   }, evt::Priority::HIGH);
     *
     *   bus.publish(LoginEvent{"Alice", 42});
     *   bus.publish_async(LoginEvent{"Bob", 99});   // 异步投递
     * @endcode
     */
    class EventBus {
    public:
        EventBus() : next_id_(1), running_(true) {
            worker_ = std::thread([this] { process_async(); });
        }

        ~EventBus() { shutdown(); }

        EventBus(const EventBus &) = delete;

        EventBus &operator=(const EventBus &) = delete;

        /**
         * @brief 订阅指定类型的事件
         * @tparam Event 事件类型（任意自定义 class/struct）
         * @param handler 回调函数，签名必须是 void(const Event&)
         * @param prio    优先级，默认 NORMAL
         * @return Subscription 订阅句柄，用于后续取消订阅
         */
        template<typename Event>
        Subscription subscribe(std::function<void(const Event &)> handler,
                               Priority prio = Priority::NORMAL) {
            auto active = std::make_shared<bool>(true);

            // 包装器：将 std::any 转回具体类型，并检查订阅是否仍有效
            auto wrapper = [handler, active](const std::any &e) {
                if (*active) {
                    handler(std::any_cast<const Event &>(e));
                }
            };

            std::lock_guard<std::mutex> lock(handlers_mutex_);
            auto &vec = handlers_[std::type_index(typeid(Event))];
            vec.push_back({next_id_++, active, static_cast<int>(prio), wrapper});

            // 按优先级降序排列（HIGH 在前）
            std::sort(vec.begin(), vec.end(), [](const auto &a, const auto &b) {
                return std::get<2>(a) > std::get<2>(b);
            });

            return Subscription(active);
        }

        /**
         * @brief 便利模板：自动推导 Lambda / 函数对象类型
         */
        template<typename Event, typename Func>
        Subscription subscribe(Func &&f, Priority prio = Priority::NORMAL) {
            return subscribe<Event>(
                std::function<void(const Event &)>(std::forward<Func>(f)), prio);
        }

        /**
         * @brief 同步发布事件（在当前线程立即调用所有订阅者）
         *
         * 实现细节：先加锁拷贝回调列表，再解锁后逐个调用，
         * 避免在回调执行期间持有锁，防止死锁。
         */
        template<typename Event>
        void publish(const Event &event) {
            std::vector<std::function<void(const std::any &)> > cbs;
            {
                std::lock_guard<std::mutex> lock(handlers_mutex_);
                auto it = handlers_.find(std::type_index(typeid(Event)));
                if (it == handlers_.end()) return;
                for (const auto &item: it->second) {
                    cbs.push_back(std::get<3>(item));
                }
            }

            std::any wrapped = event;
            for (auto &cb: cbs) {
                cb(wrapped);
            }
        }

        /**
         * @brief 异步发布事件（入队，由内部工作线程处理）
         * @tparam Event 事件类型
         * @param event  事件对象（会被拷贝到队列中）
         * @param prio   队列内优先级（注：当前为 FIFO，可扩展为优先队列）
         */
        template<typename Event>
        void publish_async(const Event &event, Priority prio = Priority::NORMAL) {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            async_queue_.push({
                std::type_index(typeid(Event)),
                std::any(event),
                static_cast<int>(prio)
            });
            cv_.notify_one();
        }

        /**
         * @brief 停止异步工作线程，等待队列处理完毕（当前实现为立即退出）
         */
        void shutdown() {
            bool expected = true;
            if (running_.compare_exchange_strong(expected, false)) {
                cv_.notify_all();
                if (worker_.joinable()) worker_.join();
            }
        }

        /**
         * @brief 清理已取消（失效）的订阅，释放内存
         *
         * 建议在事件稀疏或大量动态订阅/取消的场景下定期调用。
         */
        void gc() {
            std::lock_guard<std::mutex> lock(handlers_mutex_);
            for (auto &[type, vec]: handlers_) {
                vec.erase(
                    std::remove_if(vec.begin(), vec.end(), [](const auto &item) {
                        return !(*std::get<1>(item));
                    }),
                    vec.end()
                );
            }
        }

    private:
        using HandlerId = size_t;
        using ActiveFlag = std::shared_ptr<bool>;
        using Callback = std::function<void(const std::any &)>;
        using HandlerItem = std::tuple<HandlerId, ActiveFlag, int, Callback>;

        struct AsyncItem {
            std::type_index type;
            std::any event;
            int priority;
        };

        void process_async() {
            while (running_) {
                std::unique_lock<std::mutex> lock(queue_mutex_);
                cv_.wait(lock, [this] { return !async_queue_.empty() || !running_; });

                while (!async_queue_.empty()) {
                    auto item = std::move(async_queue_.front());
                    async_queue_.pop();
                    lock.unlock();

                    // 同样：先拷贝回调，再释放锁，最后执行
                    std::vector<Callback> cbs;
                    {
                        std::lock_guard<std::mutex> hlock(handlers_mutex_);
                        auto it = handlers_.find(item.type);
                        if (it != handlers_.end()) {
                            for (const auto &h: it->second) {
                                cbs.push_back(std::get<3>(h));
                            }
                        }
                    }

                    for (auto &cb: cbs) {
                        cb(item.event);
                    }

                    lock.lock();
                }
            }
        }

        std::atomic<HandlerId> next_id_;
        std::map<std::type_index, std::vector<HandlerItem> > handlers_;
        std::mutex handlers_mutex_;

        std::queue<AsyncItem> async_queue_;
        std::mutex queue_mutex_;
        std::condition_variable cv_;
        std::atomic<bool> running_;
        std::thread worker_;
    };

    /**
     * @brief 全局单例事件总线
     *
     * 项目中通常只需要一个总线实例，通过此函数获取。
     * 若需要多总线隔离（如 UI 总线 vs 数据总线），可直接构造 EventBus。
     */
    inline EventBus &global_bus() {
        static EventBus bus;
        return bus;
    }
} // namespace evt
