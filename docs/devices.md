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

## Advanced Programmable Interrupt Controller (APIC)


## Serial

Most modern computers have a serial interface to communicate with devices such as USB,
serial is a port mapped I/O  that means you need to send some data to send some specific ports