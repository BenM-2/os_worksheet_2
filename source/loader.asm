
global loader                               ; entry symbol for ELF
extern sum_of_three
    MAGIC_NUMBER        equ 0x1BADB002      ; Define the magic number constant
    FLAGS               equ 0x0             ; Multiboot flags
    CHECKSUM            equ -MAGIC_NUMBER   ; Calculate the checksum 
                                            ; magic number + checksum + flags should = 0
    KERNEL_STACK_SIZE   equ 4096            ; Size of the stack in Bytes
    section .bss
    align 4                                 ; Align at 4 Bytes
    kernel_stack:                           ; label points to begging of memory
        resb    KERNEL_STACK_SIZE           ; reserve stack for the kernel


    section .text                       ; start of the text (code) section
    align 4                             ; code must be 4 byte alligned
        dd MAGIC_NUMBER                 ; write the magic number to the machine code,
        dd FLAGS                        ; and flags,
        dd CHECKSUM                     ; and the checksum.

    loader:                             ; The loader label (defined as entry point in the linker script)
        mov eax, 0xCAFEBABE             ; place the number 0xCAFEBABE In the eax register
        mov esp, kernel_stack + KERNEL_STACK_SIZE ; point esp to the start of the stack (end of memory area)
        push dword 3
        push dword 2
        push dword 1
        call sum_of_three
    