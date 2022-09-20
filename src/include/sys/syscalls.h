#pragma once

#define MAX_SYSCALLS 2

#define SYSCALL_01 0
#define SYSCALL_02 1

typedef void (*Syscall)();

volatile void syscall(int syscall_index);

void handle_syscall(int syscall_index);
void syscall_test01(void);
void syscall_test02(void);