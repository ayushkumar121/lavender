# Devices


## Programmable interrupt controller  (PIC)

PIC controls the various hardware interrupt fired 
by diff devices connected to CPU.

```
                     ____________                          ____________
Real Time Clock --> |            |   Timer -------------> |            |
ACPI -------------> |            |   Keyboard-----------> |            |      _____
Available --------> | Secondary  |----------------------> | Primary    |     |     |
Available --------> | Interrupt  |   Serial Port 2 -----> | Interrupt  |---> | CPU |
Mouse ------------> | Controller |   Serial Port 1 -----> | Controller |     |_____|
Co-Processor -----> |            |   Parallel Port 2/3 -> |            |
Primary ATA ------> |            |   Floppy disk -------> |            |
Secondary ATA ----> |____________|   Parallel Port 1----> |____________|
```

### Disbaling PIC 

```asm
mov al, 0xFF                 
out 0xA1, al
out 0x21, al
```


## Advanced Programmable Interrupt Controller (APIC)