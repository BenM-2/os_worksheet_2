USER_MODE_CODE_SEGMENT_SELECTOR equ 0x18 | 0x3
USER_MODE_DATA_SEGMENT_SELECTOR equ 0x20 | 0x3


global asm_enter_user_mode

; void asm_enter_user_mode(u32int pd_phys, u32int entry, u32int user_esp);
; Does not return.
asm_enter_user_mode:
    mov eax,[esp + 4] ; pd
    mov ecx,[esp + 8] ; entry (eip)
    mov edx,[esp + 12]; user_esp

    cli ; make sure interrupts disabled

    mov cr3, eax ; swap pd

    mov ax, USER_MODE_DATA_SEGMENT_SELECTOR 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push USER_MODE_DATA_SEGMENT_SELECTOR ; ss
    push edx                ; esp
    push 0x202              ; eflags: allways 1 bit 0x2 | IF 0x200
    push USER_MODE_CODE_SEGMENT_SELECTOR ; cs
    push ecx                ; eip
    iret