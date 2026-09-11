cmake_minimum_required(VERSION 3.27)

#
# pacman -S mingw-w64-x86_64-gtest
#

# 被测代码
add_library(calc ${CMAKE_SOURCE_DIR}/src/calc.cpp)

# 查找 Google Test
find_package(GTest REQUIRED)
enable_testing()

add_executable(test_calc test_calc.cpp)
target_link_libraries(test_calc
        calc
        GTest::gtest)

add_test(NAME calc COMMAND test_calc)
