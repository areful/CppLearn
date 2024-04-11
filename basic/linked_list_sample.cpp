#include <iostream>

// 定义链表节点
template<typename T>
struct Node {
    T data;
    Node *next;

    explicit Node(const T &data) : data(data), next(nullptr) {}
};

// 定义链表类
template<typename T>
class LinkedList {
private:
    Node<T> *head;

public:
    LinkedList() : head(nullptr) {}

    // 在链表头部插入新节点
    void insertAtBeginning(const T &data) {
        auto *newNode = new Node<T>(data);
        newNode->next = head;
        head = newNode;
    }

    // 在链表尾部插入新节点
    void insertAtEnd(const T &data) {
        auto *newNode = new Node<T>(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        Node<T> *temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // 删除指定数据的节点
    void deleteNode(const T &data) {
        if (head == nullptr) {
            return;
        }
        if (head->data == data) {
            Node<T> *temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node<T> *prev = nullptr;
        Node<T> *curr = head;
        while (curr != nullptr && curr->data != data) {
            prev = curr;
            curr = curr->next;
        }
        if (curr != nullptr) {
            prev->next = curr->next;
            delete curr;
        }
    }

    // 打印链表
    void printList() {
        Node<T> *temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    // 析构函数释放链表内存
    ~LinkedList() {
        Node<T> *temp = head;
        while (temp != nullptr) {
            Node<T> *next = temp->next;
            delete temp;
            temp = next;
        }
        head = nullptr;
    }
};

int main() {
    LinkedList<int> list;

    list.insertAtBeginning(3);
    list.insertAtBeginning(2);
    list.insertAtBeginning(1);
    list.printList();

    list.insertAtEnd(4);
    list.insertAtEnd(5);
    list.printList();

    list.deleteNode(3);
    list.deleteNode(1);
    list.printList();

    return 0;
}
