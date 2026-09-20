cmake_minimum_required(VERSION 3.21)
project(CustomEventBusSample)

## 设置 C++ 标准
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 可执行文件
add_executable(CustomEventBusSample
        custom_event_bus_sample.cpp
        custom_event_bus.hpp
        ${UTILS_FILES}
)
