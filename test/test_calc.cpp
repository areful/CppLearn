//
// Created by areful on 2026/9/11.
//
#include <gtest/gtest.h>
#include "../src/calc.hpp"

// 加法
TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(add(-2, -3), -5);
}

// 整数除法
TEST(DivideTest, Normal) {
    EXPECT_EQ(divide(10, 2), 5);
}

TEST(DivideTest, ByZeroReturnsZero) {
    EXPECT_EQ(divide(10, 0), 0);
}

// 浮点除法
TEST(DividefTest, Approximate) {
    EXPECT_NEAR(dividef(10.0, 3.0), 3.3333, 0.001);
}

// 字符串
TEST(GreetingTest, Content) {
    EXPECT_STREQ(greeting(), "hello");
}

int main(int argc, char **argv) {
    // init_console_output();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
