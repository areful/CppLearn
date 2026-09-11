//
// Created by areful on 2026/9/11.
//
#include "calc.hpp"

int add(int a, int b) {
    return a + b;
}

int divide(int a, int b) {
    if (b == 0) {
        return 0;
    }
    return a / b;
}

double dividef(double a, double b) {
    if (b == 0.0) {
        return 0.0;
    }
    return a / b;
}

const char *greeting() {
    return "hello";
}
