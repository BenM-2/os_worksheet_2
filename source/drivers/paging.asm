
global Enable_Paging:
Enable_Paging:
    mov eax,[esp + 4]
    ; address of the page directory -> eax
    mov cr3,eax

    mov ebx,cr4             ; Read current cr4
    or  ebx, 0x00000010     ; Set PSE
    mov cr4, ebx            ; update cr4

    mov ebx,cr0             ; read current cr0
    or  ebx,0x80000000      ; Set PG
    mov cr0,ebx             ; Update cr0

    ; Paging is enabled
    ret