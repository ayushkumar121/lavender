#pragma once
#include <lib/types.h>

enum Registers
{
  REGISTER_A,
  REGISTER_B,
  REGISTER_C,
  REGISTER_D,
  REGISTER_RESULT,
  REGISTER_COUNT,
};

enum Channels
{
  CHANNEL_0,
  CHANNEL_1,
  CHANNEL_2,
  CHANNEL_3,
  CHANNEL_COUNT,
};

enum Instructions 
{
  // Marks the start of your program
  PROGRAM_START = 0,

  // Does nothing
  PROGRAM_NOOP,

  // usage: [PROGRAM_LOAD, REGISTER_A, 100]
  // it load the data into register A
  PROGRAM_LOAD,

  // usage: [PROGRAM_ADD REGISTER_A, REGISTER_B] 
  // moves the data of register B into A
  PROGRAM_MOV,
  
  // usage: [PROGRAM_ADD REGISTER_A, REGISTER_B] 
  // it adds values in A and B together and stores them in A
  PROGRAM_ADD,

  // usage: [PROGRAM_INC REGISTER_A, amount] 
  // increments the value stored in register by amount
  PROGRAM_INC,

  // usage [PROGRAM_PRINT REGISTER_A]
  // it prints value A to screen
  PROGRAM_PRINT, 

  // usage [PROGRAM_SYSCALL, SYSCALL_ID, REGISTER_A]
  // calls syscall with the given id and stores the result in
  // REGISTER_RESULT
  PROGRAM_SYSCALL,

  // usage [PROGRAM_SYSCALL, SYSCALL_ID, SYSCALL_DATA]
  // calls syscall with the given id and stores the result in
  // REGISTER_RESULT
  PROGRAM_SYSCALL_IMMEDIATE,

  // usage [PROGRAM_TEST, REGISTER_A, REGISTER_B]
  // sets zero flag if REGISTER_A == REGISTER_B
  PROGRAM_TEST,

  // usage [PROGRAM_TEST, REGISTER_A, VALUE]
  // sets zero flag if REGISTER_A == VALUR
  PROGRAM_TEST_IMMEDIATE,

  // resets flags
  PROGRAM_RESET,

  // usage [PROGRAM_JUMP, ip]
  // jumps to instruction
  PROGRAM_JUMP,
  
  // usage [PROGRAM_JUMP_IF, ip]
  // jumps to instruction zero flag is set
  PROGRAM_JUMP_IF,

  // usage [PROGRAM_LOAD_MEMORY_BASE, REGISTER_A]
  // load the memory base to register_a
  PROGRAM_LOAD_MEMORY_BASE,

  // usage [PROGRAM_DEREF, REGISTER_A, REGISTER_B]
  // deref the memory address stored in b and stores
  // the value in A
  PROGRAM_DEREF,

  PROGRAM_PUBLISH,
  PROGRAM_SUBSCRIBE,
  
  // Marks the end of your program as well base base of 
  // of fixed memory base
  PROGRAM_END
};

enum ProgramFlags 
{
  ZERO_FLAG = 0b00000001,
};

#define MAX_PROGRAM_CAPACITY 512

typedef struct Program {
    size_t *program_data;
    size_t program_capacity;
    size_t program_length;

    // A, B, C, D, RESULT registers
    int64_t registers[REGISTER_COUNT];
    int64_t pid;
    size_t ip;
    char flags;
} Program;

#define PACKET_SIZE 64

typedef struct Subscriber {
  size_t *packet; 
  Program *program;
  struct Subscriber *next;
} Subscriber;

void program_init(Program *program);
void program_step(Program *program);
