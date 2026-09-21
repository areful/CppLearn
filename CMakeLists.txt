cmake_minimum_required(VERSION 3.27)
project(CppLearning)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    # 强制指定 MSYS2 Mingw64 下 Qt6 的 cmake 配置目录
    set(Qt6_DIR "C:/msys64/mingw64/lib/cmake/Qt6")
endif ()

# ===================== 公共 utils 引入（核心）=====================
set(UTILS_DIR "${CMAKE_SOURCE_DIR}/src/utils")
include_directories(${UTILS_DIR})
file(GLOB UTILS_FILES ${UTILS_DIR}/*.cpp)
# =================================================================

add_executable(Main main.cpp)

# add test app
add_subdirectory(basic/std)

#add_subdirectory(basic)
add_subdirectory(qt-sample)
add_subdirectory(qt-speech-sample)
add_subdirectory(qt-serialport-sample)
add_subdirectory(qt-serialport-debugger)

# add test app
add_subdirectory(test)

# 自定义EventBus示例
add_subdirectory(src/lib/custom_eventbus)

# 直接添加本地子目录（eventpp 是 INTERFACE 库，无需编译）
add_subdirectory(third_party/eventpp)

add_subdirectory(src/lib/eventpp)
