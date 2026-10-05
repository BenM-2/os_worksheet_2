global load_idt
; load_idt- Loadsthe interrupt descriptortable (IDT).
; stack:[esp +4] the addressof thefirst entry inthe IDT
; [esp ]the return address

load_idt:
    mov eax,[esp+4]
    lidt[eax]
    ret