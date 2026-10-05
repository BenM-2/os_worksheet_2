;GenericInterrupt Handler
;
extern interrupt_handler


%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0 ;push 0 as errorcode
    push dword %1 ;push theinterrupt number
    jmp common_interrupt_handler ; jumpto thecommon handler
%endmacro


%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1 ;push theinterrupt number
    jmp common_interrupt_handler ;jump tothe commonhandler
%endmacro

common_interrupt_handler: ;the commonparts ofthe generic interrupt handler 
    ; savethe registers
    push eax
    push ebx
    push ecx
    push edx
    push ebp
    push esi
    push edi
    call interrupt_handler ;call theC function
    ;restore the registers
    pop edi
    pop esi
    pop ebp
    pop edx
    pop ecx
    pop ebx
    pop eax
    ; restore the esp
    add esp,8
    ; returnto thecode that gotinterrupted
    iret
no_error_code_interrupt_handler 33 ; createhandler forinterrupt 1
