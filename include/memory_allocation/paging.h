
/**
 * @file paging.h
 * @author Ben Marples
 * @brief Holds Paging interface
 */

#ifndef PAGING_H
#define PAGING_H

#include "type.h"
//------------------------------------------------------------------------------
// Definitions


/** Invalidate the TLB entry for the page containing addr. */
void cpu_invlpg(const void *addr);

/** Physical address of the current page directory. */
u32int cpu_read_cr3(void);

/** Load a new page directory (also flushes the whole TLB). */
void cpu_write_cr3(u32int pd_phys);

/** Flush the whole TLB by reloading CR3. */
void cpu_flush_tlb(void);

/** Halt forever. */
void cpu_halt_forever(void);


#endif