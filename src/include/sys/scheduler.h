#pragma once
#include <sys/program.h>

void scheduler_init();
void scheduler_addtask(Program *program);
void scheduler_start();