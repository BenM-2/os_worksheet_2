# OS Worksheet 2

## Prerequisites

| Tool  | Purpose                   |
|-------|---------------------------|
| Make  | Build system              |
| QEMU  | Emulation                 |
| GCC   | C compilation and linking |
| NASM  | Assembly compilation      |

## Task 1 
### Overview
To complete task 1 `0xCAFEBABE` needs to be placed into the `eax` register. To do this we use GRUB as a bootloader 

### Run
To run the program and be able to seee that 0xCAFEBABE is in the register use 
``` bash
make qemu_run
```
to run the os silently just run 
``` bash
make qemu_run_quiet
```
