[bits 32]

extern _start

global idt_load
extern idt_descriptor

section .text
    start:
        call _start
        jmp $

    idt_load:
        lidt [idt_descriptor]
        ret