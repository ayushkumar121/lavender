#pragma once

#define MAX_PROGRAM_LEN 128

typedef struct Program {
    int data[MAX_PROGRAM_LEN];
    int len;
    int ip;
} Program;

void scheduler_init();

/* returns the process ID */
int scheduler_addtask(Program program);
void scheduler_start();