

#ifndef PAGE_FRAME_ALLOCATION_H
#define PAGE_FRAME_ALLOCATION_H

#include "type.h"
#include "kbool.h"
#include "multiboot.h"


typedef struct
{
    u32int k_v_start;
    u32int k_v_end;
    u32int k_p_start;
    u32int k_p_end;
} start_vars;

#define NULL 0
#define MAX_NODES 20

#define PAGE_4KB 4096
#define PAGE_8KB 8192
#define PAGE_32KB 32768
#define PAGE_128KB 131072
#define PAGE_512KB 524288
#define FOUR_GIGABYTES 0x100000000ULL

#define PAGES_PER_FRAME 20
#define FRAME_SIZE (PAGES_PER_FRAME * PAGE_512KB)

#define FULL true
#define FULL_PAGE 0xFF
#define EMPTY false

#define page_type u8int
#define SIZEOF_PAGE_STORAGE (sizeof(page_type) * 8) // sizeof(*size of page data*)
#define MAX_PAGES (0xC0000000u / PAGE_4KB)
#define BITMAP_BYTES (MAX_PAGES / SIZEOF_PAGE_STORAGE)
#define KERNEL_VIRTUAL_BASE 0xC0000000

void page_init(start_vars Boot_Vars, const struct multiboot_info *multiboot_info);


#endif