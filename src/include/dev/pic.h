#pragma once

#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET PIC_1_OFFSET + 8

#define TIMER_INT_INDEX PIC_1_OFFSET
#define KEYBOARD_INT_INDEX (PIC_1_OFFSET + 1)

void pic_init();
void pic_eoi(int interrupt_index);

void pic_disable();