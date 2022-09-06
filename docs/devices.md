# Devices


## Programmable interrupt controller  (PIC)

disbaling 

```asm
disable_pic:
    mov al, 0xFF                 
    out 0xA1, al
    out 0x21, al
    ret

```