#pragma once
#include <lib/types.h>

enum Syscalls 
{
  // Puts data into stdout
  SYSCALL_PUTCHAR = 0,
  
  // Wait fors a key press and returns it
  SYSCALL_GETKEY,

  // Total count of all the syscalls
  SYSCALL_COUNT
};

size_t syscall(size_t syscall_index, size_t argument);
