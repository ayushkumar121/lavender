#pragma once

typedef int (*Task)();

void scheduler_init();
void scheduler_taskwait();

/* returns the process ID */
int scheduler_addtask(Task task);
