#pragma once

#define MAX_SYSCALLS 2

#define SYSCALL_01 1
#define SYSCALL_02 2

volatile void syscall(int syscall_index);
void syscall_test01(void);
void syscall_test02(void);