
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
// #define PTE_PRESENT 0x1
// #define PTE_WRITE   0x10

typedef enum {
    PTE_PRESENT = 0x01,
    PTE_RW      = 0x02,
    PTE_USER    = 0x04,
    PTE_PWT     = 0x08, // Page write through
    PTE_PCD     = 0x10, // Cache disabled
    PTE_ACCESSED= 0x20,
    PTE_DIRTY   = 0x40,
    PTE_PAT     = 0x80, // PDEs / PTEs
} PTE_FLAGS;

typedef enum {
    PDE_PRESENT = 0x01,
    PDE_RW      = 0x02,
    PDE_USER    = 0x04,
    PDE_PWT     = 0x08, // Page write through
    PDE_PCD     = 0x10, // Cache disabled
    PDE_ACCESSED= 0x20,
    // PTE_IGNORED = 0x40, 
    PDE_PAT     = 0x80, // PDEs / PTEs
} PDE_FLAGS;

extern usize boot_page_directory[1024]; // Virtual reference to the boot_page_directory
extern usize boot_page_table[1024]; // Virtual reference to the boot_page_table

typedef enum{
    KMAP_SLOT0 = 0, // PTE 1022
    KMAP_SLOT1 = 1, // PTE 1023
} KMAP_SLOT;

#define KMAP_SLOT0_VA  0xC03FE000u
#define KMAP_SLOT1_VA  0xC03FF000u


//------------------------------------------------------------------------------
// Function Definitions


/** Invalidate the TLB entry for the page containing addr. */
void asm_invlpg(const void *addr);

/** Physical address of the current page directory. */
u32int asm_read_cr3(void);

/** Load a new page directory (also flushes the whole TLB). */
void asm_write_cr3(u32int pd_phys);

/** Flush the whole TLB by reloading CR3. */
void asm_flush_tlb(void);

/** Halt forever. */
void asm_halt(void);


#endif