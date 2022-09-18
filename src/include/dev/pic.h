#pragma once

#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET PIC_1_OFFSET + 8

void pic_init();
void pic_eoi(int interrupt_index);

void pic_disable();