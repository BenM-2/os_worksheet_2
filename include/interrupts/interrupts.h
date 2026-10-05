#ifndef INCLUDE_INTERRUPTS
#define INCLUDE_INTERRUPTS
#include "type.h"
#include "interrupt_callbacks.h"
#include "pic.h"
struct IDT
{
    u16int size;
    u32int address;
}__attribute__((packed));
struct IDTDescriptor
{
    /* Thelowest 32bits */
    u16int offset_low;       // offset bits0..15
    u16int segment_selector; // acode segmentselector in GDTor LDT
    /* Thehighest 32bits */
    u8int reserved;      // Just 0.
    u8int type_and_attr; // type andattributes
    u16int offset_high;   // offsetbits 16..31
}__attribute__((packed));
struct cpu_state
{
    u32int eax;
    u32int ebx;
    u32int ecx;
    u32int edx;
    u32int ebp;
    u32int esi;
    u32int edi;
}__attribute__((packed));
struct stack_state
{
    u32int error_code;
    u32int eip;
    u32int cs;
    u32int eflags;
}__attribute__((packed));

void interrupt_handler(struct cpu_state cpu, u32int interrupt, struct stack_state stack);

void interrupts_install_idt();

void interrupts_cb_config(const Interrupt_callbacks *const callbacks_);

// Wrappersaround ASM.
void load_idt(u32int idt_address);
void interrupt_handler_33();
void interrupt_handler_14();
#endif /* INCLUDE_INTERRUPTS */