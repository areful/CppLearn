cmake_minimum_required(VERSION 3.27)
project(CppLearning)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(Main main.cpp)

#add_subdirectory(basic)
add_subdirectory(qt-sample)
add_subdirectory(qt-speech-sample)
add_subdirectory(qt-serialport-sample)
add_subdirectory(qt-serialport-debugger)
