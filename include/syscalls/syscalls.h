/**
 * @file syscalls.h
 * @author Ben Marples
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "type.h"

//------------------------------------------------------------------------------
// Definitions

typedef struct __attribute__((packed))
{
    u32int prev_tss;   /**< Previous TSS link (unused) */
    u32int esp0;       /**< Kernel stack pointer for PL3 -> PL0 */
    u32int ss0;        /**< Kernel stack segment selector */
    u32int esp1;
    u32int ss1;
    u32int esp2;
    u32int ss2;
    u32int cr3;
    u32int eip;
    u32int eflags;
    u32int eax;
    u32int ecx;
    u32int edx;
    u32int ebx;
    u32int esp;
    u32int ebp;
    u32int esi;
    u32int edi;
    u32int es;
    u32int cs;
    u32int ss;
    u32int ds;
    u32int fs;
    u32int gs;
    u32int ldt;
    u32int trap_iomap; /**< T flag in the low 16 bits, I/O map base in the high 16 */
} TSS;

void tss_set_kernel_stack(u32int esp0);

#endif