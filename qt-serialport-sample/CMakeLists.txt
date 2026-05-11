cmake_minimum_required(VERSION 3.21)
project(QtSerialPortDemo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 让 CMake 找到 MSYS2 的 Qt6
set(CMAKE_PREFIX_PATH "c:/msys64/mingw64")  # <--- 改成你的 msys64 路径

# 查找 Qt 模块
find_package(Qt6 REQUIRED COMPONENTS Core Widgets SerialPort)

# 自动 moc、uic、rcc（必须加）
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 可执行文件
add_executable(${PROJECT_NAME}
        main.cpp
        serialmanager.cpp
)

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Widgets  # 必须加
        Qt6::SerialPort
)