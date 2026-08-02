cmake_minimum_required(VERSION 3.27)
project(CppLearning)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    # 强制指定 MSYS2 Mingw64 下 Qt6 的 cmake 配置目录
    set(Qt6_DIR "C:/msys64/mingw64/lib/cmake/Qt6")
endif ()

add_executable(Main main.cpp)

#add_subdirectory(basic)
add_subdirectory(qt-sample)
add_subdirectory(qt-speech-sample)
add_subdirectory(qt-serialport-sample)
add_subdirectory(qt-serialport-debugger)
