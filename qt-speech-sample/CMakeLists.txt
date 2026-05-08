cmake_minimum_required(VERSION 3.21)
project(Qt6SpeechDemo)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# 更新包数据库
# pacman -Syu
#
# 安装 Qt5 动态库 + Creator
# pacman -S --needed mingw-w64-x86_64-qt5 mingw-w64-x86_64-qt-creator
#
# 或安装 Qt6 全套
# pacman -S --needed mingw-w64-x86_64-qt6 mingw-w64-x86_64-qt6-tools
#
# 如需静态链接
# pacman -S --needed mingw-w64-x86_64-qt5-static
#

# 让 CMake 找到 Qt6（适配 MSYS2）
set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")  # 改成你自己的 msys64 路径

# 查找 Qt6 模块
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets TextToSpeech)

# 启用 Qt 的 MOC 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 可执行文件
add_executable(Qt6SpeechDemo
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# 链接 Qt 库
target_link_libraries(Qt6SpeechDemo PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::TextToSpeech  # 语音模块
)