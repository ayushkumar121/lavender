;; Boot sector start
;; Operating in 16 bit real mode

;; Setting the memory origin to 0x7c00 
;; because memory is not counted from 0
[bits 16]
[org 0x7c00]

mov [boot_disk], dl

KERNEL_LOCATION equ 0x1000

;; Setting up the stack for 16 bit real mode (setting everything to zero)
xor ax, ax
mov es, ax ;; Extra segment
mov ds, ax ;; Data segment
mov bp, 0x8000 ;; Base pointer
mov sp, bp     ;; Top pointer

mov bx, KERNEL_LOCATION

;; reading from disk
mov ah, 2 ;; disk reading mode
mov al, 10 ;; no of sectors to read (Loads 10KiB of memory)

;; CSH addressing
mov ch, 0 ;; cylinder no
mov cl, 2 ;; sector no
mov dh, 0 ;; head no

mov dl, [boot_disk]
int 0x13

;; Clear the screen
mov ah, 0x0
mov al, 0x3
int 0x10

CODE_SEG equ gdt.code - gdt
DATA_SEG equ gdt.data - gdt

lgdt [gdt.pointer]

;; Switching to protected mode
mov eax, cr0
or eax, 1
mov cr0, eax

jmp CODE_SEG:start_protected_mode
jmp $

[bits 32]
start_protected_mode:
    ;; Setting up the stack for 32 bit mode
    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000		; 32 bit stack base pointer
	mov esp, ebp

    mov bx, CODE_SEG
    jmp KERNEL_LOCATION

gdt: ;; Flat GDT 
    .null:
        dq 0
    .code:
        dw 0xffff ;; first 16 bit of the limit
        dw 0      ;; first 24 bit of base
        db 0      ;;  
        db 0x9A ;; present, privileage, type
        db 0b11001111 ;; other + limit
        db 0
    .data:
        dw 0xffff
        dw 0
        db 0
        db 0x92
        db 0b11001111
        db 0
    .pointer:
        dw .pointer - gdt - 1
        dq gdt

boot_disk: db 0

times 510-($-$$) db 0
;; Bootloader signature
dw 0xAA55