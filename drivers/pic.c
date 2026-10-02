/**
 * @file pic.h
 * @author Ben Marples
 * @brief Holds all PIC functions
 */

//------------------------------------------------------------------------------
// Includes
#include "pic.h"
#include "io.h"

//------------------------------------------------------------------------------
// Function Declarations
void pic_remap(s32int offset1, s32int offset2);
void pic_acknowledge(u32int interrupt);
//------------------------------------------------------------------------------
// Definitions

static void io_wait()
{
    outb(0x80, 0);
}

void pic_remap(s32int offset1, s32int offset2)
{
    // Store Masks
    u8int m1 = inb(PIC_1_DATA);
    u8int m2 = inb(PIC_2_DATA);

    // ICW1: begin initialzation, ICW4 will follow
    outb(PIC_1_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();
    outb(PIC_2_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    io_wait();

    // ICW2: Vector Offsets
    outb(PIC_1_DATA, offset1);
    io_wait();
    outb(PIC_2_DATA, offset2);
    io_wait();

    // ICW3: Cascade wiring ?

    // ICW4: 8086 mode
    outb(PIC_1_DATA, PIC_ICW4_8086);
    io_wait();
    outb(PIC_2_DATA, PIC_ICW4_8086);
    io_wait();

    // Restore Masks
    outb(PIC_1_DATA, m1);
    outb(PIC_2_DATA, m2);
}

void pic_acknowledge(u32int interrupt)
{
    if ( (interrupt<PIC_1_OFFSET) | (interrupt> PIC_2_END))
    {
        return;
    }

    if (interrupt > PIC_2_OFFSET)
    {
        outb(PIC_2_COMMAND, PIC_ACKNOWLEDGE);
    }

    outb(PIC_1_COMMAND, PIC_ACKNOWLEDGE);
}