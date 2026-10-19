
global loader                               ; entry symbol for ELF
extern kernel_main
MAGIC_NUMBER        equ 0x1BADB002      ; Define the magic number constant
ALIGN_MODULES       equ 0x00000003      ; Tell Grub to align modules + get meminfo
CHECKSUM            equ -(MAGIC_NUMBER+ALIGN_MODULES)   ; Calculate the checksum 
                                        ; magic number + checksum + flags should = 0
KERNEL_STACK_SIZE   equ 4096            ; Size of the stack in Bytes
KERNEL_VBASE        equ 0xC0000000      ; Offset

section .boot progbits alloc exec nowrite align=4
    dd MAGIC_NUMBER                 ; write the magic number to the machine code,
    dd ALIGN_MODULES                ; Write the align modules instruction
    dd CHECKSUM                     ; and the checksum.


loader: 
    ; Runs at its physical address because paging is off
    ; ebx = multiboot info pointer 
    
    ; Fill the page page table: identity map the first 4MB (Present | Writable)
    mov edi, boot_page_table - KERNEL_VBASE
    mov esi, 0x3
    mov ecx, 1024

.fill:
    mov [edi],esi
    add esi, 0x1000
    add edi, 4
    loop .fill

    mov edx, (boot_page_table - KERNEL_VBASE) + 0x3
    mov [boot_page_directory - KERNEL_VBASE], edx
    mov [boot_page_directory - KERNEL_VBASE + 768 * 4], edx

    mov edx, boot_page_directory - KERNEL_VBASE
    mov cr3, edx
    mov edx, cr0
    or edx, 0x80000000
    mov cr0, edx

    lea edx, [higher_half]
    jmp edx

section .text
higher_half:
    mov dword [boot_page_directory],0
    invlpg [0]

    mov esp, kernel_stack_top
    mov eax, MAGIC_NUMBER
    add ebx, KERNEL_VBASE
    push dword ebx
    call kernel_main

.loop:
        jmp .loop
    
section .bss
align 4096
boot_page_directory:    resb 4096
boot_page_table:        resb 4096
kernel_stack:           resb KERNEL_STACK_SIZE
kernel_stack_top:
