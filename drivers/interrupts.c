
/**
 * @file interrupts.c
 * @author Ben Marples
 * @brief Sends all interupts to their related callback defined under interpt_callbacks.h and instantiated used interrupt_cb_config()
 */

//------------------------------------------------------------------------------
// Includes
#include "interrupts.h"
#include "io.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "interrupt_callbacks.h"

//------------------------------------------------------------------------------
// Definitions
#define INTERRUPTS_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD 33
// #define INPUT_BUFFER_SIZE 256

static Interrupt_callbacks callbacks; // Keep callbacks constant though different calls

// u8int input_buffer[INPUT_BUFFER_SIZE];
// u8int buffer_index = 0;

struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;
u32int BUFFER_COUNT;

//------------------------------------------------------------------------------
// Function Declarations
void interrupts_init_descriptor(s32int index, u32int address);
void interrupts_install_idt();
void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, u32int interrupt, __attribute__((unused)) struct stack_state stack);

//------------------------------------------------------------------------------
// Function Implementations
void interrupts_init_descriptor(s32int index, u32int address)
{
    idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; // offset bits 0..15
    idt_descriptors[index].offset_low = (address & 0xFFFF);        // offsetbits 16..31 ↪
    idt_descriptors[index].segment_selector = 0x08;                // The second(code) segment selector in GDT : one segment is64b. ↪
    idt_descriptors[index].reserved = 0x00;                        // Reserved.

    /*
↪
Bit:
| 31
5 | 4 3 2 1 0 |
↪
↪
Content: | offset high
0 0 | reserved
16 | 15 | 14 13 | 12 | 11
10 9 8 | 7 6
| P | DPL | S | D and GateType | 0
P If the handler is present in memory or not (1 = present, 0 = not
present). Set to 0 for unused interrupts or for Paging.
↪
}
DPL Descriptor Privilige Level, the privilege level the handler can
be called from (0, 1, 2, 3).
S Storage Segment. Set to 0 for interrupt gates.
D Size of gate, (1 = 32 bits, 0 = 16 bits).
*/

    idt_descriptors[index].type_and_attr = (0x01 << 7) |
                                           (0x00 << 6) | (0x00 << 5) | // DPL
                                           0xe;
}

void interrupts_install_idt()
{

    interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (u32int)interrupt_handler_33);

    idt.address = (s32int)&idt_descriptors;
    idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;
    load_idt((s32int)&idt);

    /*pic_remap(PIC_PIC1_OFFSET, PIC_PIC2_OFFSET);*/
    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);

    outb(PIC_1_DATA, 0xFF);
    outb(PIC_2_DATA, 0xFF);
    // Unmask keyboard interrupt (IRQ1)
    outb(0x21, inb(0x21) & ~(1 << 1));
}

void interrupts_cb_config(const Interrupt_callbacks *const callbacks_)
{
    callbacks = *callbacks_;
}

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, u32int interrupt, __attribute__((unused)) struct stack_state stack)
{
    switch (interrupt)
    {
    case INTERRUPTS_KEYBOARD:
        callbacks.keyboard_event(interrupt);
        break;
    default:
        break;
    }
}
