global outb
    ; make the label outb visible outside this file
    ; outb- send a byte to an I/O port
    ; stack: [esp + 8] the data byte
    ; [esp + 4] the I/O port
    ; [esp ] return address
outb: 
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al
    ret
; move the data to be sent into the al register
; move the address of the I/O port into the dx register
; send the data to the I/O port
; return to the calling function

global inb
; inb- returns a byte from the given I/O port
; stack: [esp + 4] The address of the I/O port
; [esp] The return address 
inb:
    mov dx, [esp + 4]
    in al, dx
    ret

global lgdt_pub
; lgdt_pub loads the gdt entry provided from the stack
; stack: [esp + 4] pointer to the gdt entry
; [esp] The return address 
lgdt_pub:
    mov     eax,[esp + 4]
    lgdt    [eax] 
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush_cs

.flush_cs:
    ret

