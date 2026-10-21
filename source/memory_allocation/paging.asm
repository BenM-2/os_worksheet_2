; 32-bit cdecl: args on the stack, return value in eax
section .text

global asm_invlpg
asm_invlpg:
    mov eax, [esp + 4]      ; virtual address
    invlpg [eax]
    ret

global asm_read_cr3
asm_read_cr3:
    mov eax, cr3
    ret

global asm_write_cr3
asm_write_cr3:
    mov eax, [esp + 4]
    mov cr3, eax
    ret

global asm_halt
asm_halt:
    hlt
    ret