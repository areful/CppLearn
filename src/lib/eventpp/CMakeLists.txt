cmake_minimum_required(VERSION 3.27)
project(EventPpSample)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#include(FetchContent)
#FetchContent_Declare(
#        eventpp
#        GIT_REPOSITORY https://github.com/wqking/eventpp.git
#        GIT_TAG master
#)
#FetchContent_MakeAvailable(eventpp)

function(add_event_exe target src)
    add_executable(${target} ${src} ${UTILS_FILES})
    target_link_libraries(${target} PRIVATE eventpp::eventpp)
endfunction()

# 基础用法示例
add_event_exe(EventPpDemo1 eventpp_sample.cpp)

# 简单示例
add_event_exe(EventPpDemo2 eventpp_sample2.cpp)

# 泛型事件示例
add_event_exe(EventPpDemo3 eventpp_sample3.cpp)

# 泛型，以类型本身作为 key（真正的"按数据类型分发"）
add_event_exe(EventPpDemo4 eventpp_sample4.cpp)

# 泛型 + 单例模式
add_event_exe(EventPpDemo5 eventpp_sample5.cpp)
