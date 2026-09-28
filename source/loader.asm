global loader                           ; entry symbol for ELF
    MAGIC_NUMBER    equ 0x1BADB002      ; Define the magic number constant
    FLAGS           equ 0x0             ; Multiboot flags
    CHECKSUM        equ -MAGIC_NUMBER   ; Calculate the checksum 
                                        ; magic number + checksum + flags should = 0

    section .text:                      ; start of the text (code) section
    align 4                             ; code must be 4 byte alligned
        dd MAGIC_NUMBER                 ; write the magic number to the machine code,
        dd FLAGS                        ; and flags,
        dd CHECKSUM                     ; and the checksum.

    loader:                             ; The loader label (defined as entry point in the linker script)
        mov eax, 0xCAFEBABE             ; place the number 0xCAFEBABE In the eax register

    .loop:
        jmp .loop                       ; loop forever