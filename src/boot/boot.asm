;; Boot sector start
;; Operating in 16 bit real mode

;; Setting the memory origin to 0x7c00 
;; because memory is not counted from 0
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
mov al, 20 ;; no of sectors to read

;; CSH addressing
mov ch, 0 ;; cylinder no
mov cl, 2 ;; sector no
mov dh, 0 ;; head no

mov dl, [boot_disk]
int 0x13

;; CLear the screen
mov ah, 0x0
mov al, 0x3
int 0x10

CODE_SEG equ kernel_code_descriptor - gdt_start
DATA_SEG equ kernel_data_descriptor - gdt_start


cli ;; Disable all interupts
lgdt [gdt_descriptor]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp CODE_SEG:start_protected_mode
jmp $

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ;; Size
    dd gdt_start               ;; Start


;; Flat memory model
gdt_start:
    null_descriptor:
        dd 0
        dd 0
    kernel_code_descriptor:
        dw 0xffff ;; first 16 bit of the limit
        dw 0      ;; first 24 bit of base
        db 0      ;;  
        db 0x9A ;; present, privileage, type
        db 0b11001111 ;; other + limit
        db 0
    kernel_data_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0x92
        db 0b11001111
        db 0
    user_code_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0xFA
        db 0b11001111
        db 0
    user_data_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0xF2
        db 0b11001111
        db 0   
gdt_end:

boot_disk: db 0


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


times 510-($-$$) db 0
dw 0xAA55

; ;; Real mode Procedures
; proc_print_str:
;     mov ah, 0x0e ;; Switch to teletype mode
; proc_print_str_loop:
;     mov al, [bx]
;     cmp al, 0
;     je proc_print_str_end
;     int 0x10
;     inc bx
;     jmp proc_print_str_loop
; proc_print_str_end:
;     ret ;; returns to previous memory location

; proc_keyboard_input:
;     mov ah, 0 ;; Read keyboard scancode (blocking) 
;     int 0x16
;     mov [keyboard_char], al ;; *keyboard_char = al
; proc_keyboard_input_end:
;     ret
