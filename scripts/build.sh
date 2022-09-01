#!/bin/sh

echo "Compiling kernel"
nasm -f elf ./src/kernel/kernel_entry.asm -o ./target/x86/kernel_entry.o
i386-elf-gcc -ffreestanding -masm=intel -m32 -mgeneral-regs-only -g -c src/kernel/kernel.c -o ./target/x86/kernel.o
i386-elf-ld -o ./target/x86/full_kernel.bin -Ttext 0x1000 ./target/x86/kernel_entry.o ./target/x86/kernel.o --oformat binary

echo "Compiling bootloader"
nasm -f bin ./src/boot/boot.asm -o ./target/x86/boot.bin
nasm -f bin ./src/boot/padding.asm -o ./target/x86/padding.bin

cat  ./target/x86/boot.bin ./target/x86/full_kernel.bin  ./target/x86/padding.bin > ./target/x86/flowos.bin 