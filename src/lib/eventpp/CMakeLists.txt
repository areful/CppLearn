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

add_executable(EventPpDemo eventpp_sample.cpp)
target_link_libraries(EventPpDemo PRIVATE eventpp::eventpp)

add_executable(EventPpDemo2 eventpp_sample2.cpp ${UTILS_FILES})
target_link_libraries(EventPpDemo2 PRIVATE eventpp::eventpp)
