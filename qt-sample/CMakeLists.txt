cmake_minimum_required(VERSION 3.16)
project(Qt6MinimalDemo)

# C++ 标准
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

# 查找 Qt6 核心组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# 自动处理 MOC、UI、资源
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 生成可执行文件
add_executable(${PROJECT_NAME} main.cpp)

# 链接 Qt6 库
target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
)

# Windows 下控制台黑框隐藏（可选）
if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif ()
