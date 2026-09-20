//
// Created by Administrator on 2026/4/30.
//
#include <cstdio>
#include "set_console_output_cp.h"

void set_console_output_cp() {
#ifdef _WIN32
    // Windows 设置控制台为 UTF-8（Linux/macOS 通常无需此步骤）
    SetConsoleOutputCP(65001);
#endif
}

/**
 * 初始化控制台输出
 */
void init_console_output() {
    setbuf(stdout, nullptr);
    setbuf(stderr, nullptr);

    set_console_output_cp();
}
