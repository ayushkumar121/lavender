#pragma once
#include <sys/program.h>

void scheduler_init();
void scheduler_addtask(Program *program);

// NOTE: This takes control from kernel and start executing 
// tasks 
void scheduler_start();