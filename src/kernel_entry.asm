[bits 32]
[extern _start]

section .text
    start:
        mov [idt_0 + 2], word start

        mov ax, divide_by_zero_handler
        mov [idt_0], al
        mov [idt_0 + 6], ah

        lidt [idt_descriptor]
        call _start
        jmp $

    divide_by_zero_handler:
        pusha
        hlt

    idt_descriptor:
        dw idt_end - idt_start - 1 ;; Size
        dd idt_start               ;; Offset

    idt_start:
        idt_0:
            dw 0
            dw 0
            db 0 
            db 0x8F
            dw 0
    idt_end:

