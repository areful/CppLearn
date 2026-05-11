cmake_minimum_required(VERSION 3.21)
project(QtSerialPortDebugger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 开启Qt自动编译
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 改成你自己的MSYS2路径
set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")

# 控制台窗口保留，方便看日志
set(CMAKE_WIN32_EXECUTABLE OFF)

# 依赖模块
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Widgets
        SerialPort
)

add_executable(${PROJECT_NAME}
        main.cpp
        mainwindow.h
        mainwindow.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::SerialPort
)