/**
 * @file page_frame_allocation.c
 * @author Ben Marples
 * @brief
 */

//------------------------------------------------------------------------------
// Includes
#include "page_frame_allocation.h"
#include "paging.h"
#include "err.h"
#include "kbool.h"
#include "type.h"
#include "kmem.h"
#include "io.h"


static page_type pages[BITMAP_BYTES];

//------------------------------------------------------------------------------
// Function Declarations
static bool ret_page_index(int index);
static void set_page_index(int index, bool value);
static void release_region(u64int start_addr, u64int end_addr);
static void reserve_region(u64int start_addr, u64int end_addr);
static void Release_All_Usable_Mem(const struct multiboot_info *multiboot_info);
void page_init(start_vars Boot_Vars, const struct multiboot_info *multiboot_info);
usize page_alloc(void);
void page_free(usize page);
void *kmap(int slot, usize phys);
usize pd_create(void);

//------------------------------------------------------------------------------
// Function Implementions
static bool ret_page_index(int index)
{
    int bit_index = index % SIZEOF_PAGE_STORAGE;
    page_type bitmask = 1 << bit_index;

    page_type data = pages[index / SIZEOF_PAGE_STORAGE] & bitmask;
    data = data >> bit_index;
    return (bool)data;
}

static void set_page_index(int index, bool value)
{
    int bit_index = index % SIZEOF_PAGE_STORAGE;
    page_type bitmask = 1 << bit_index;
    page_type bitvalue = value << bit_index;

    page_type data = pages[index / SIZEOF_PAGE_STORAGE] & ~bitmask;
    data = data | bitvalue;
    pages[index / SIZEOF_PAGE_STORAGE] = data;
}

static void release_region(u64int start_addr, u64int end_addr)
{
    u64int Alligned_Start_index = start_addr >> 12;
    u64int Alligned_End_index = (PAGE_4KB + end_addr - 1) >> 12;

    for (u64int i = Alligned_Start_index; i < Alligned_End_index; i++)
    {
        set_page_index(i, EMPTY);
    }
}

static void reserve_region(u64int start_addr, u64int end_addr)
{
    u64int Alligned_Start_index = start_addr >> 12;
    u64int Alligned_End_index = (PAGE_4KB + end_addr - 1) >> 12;

    for (u64int i = Alligned_Start_index; i < Alligned_End_index; i++)
    {
        set_page_index(i, FULL);
    }
}

static void Release_All_Usable_Mem(const struct multiboot_info *multiboot_info)
{
    if (!(multiboot_info->flags & MULTIBOOT_INFO_MEM_MAP))
    {
        panic();
    }

    u32int physical_start = multiboot_info->mmap_addr;
    u32int physical_end = physical_start + multiboot_info->mmap_length;

    u32int current_physical = physical_start;

    while (current_physical < physical_end)
    {
        const struct multiboot_mmap_entry *current_entry = (struct multiboot_mmap_entry *)(current_physical + KERNEL_VIRTUAL_BASE);

        bool is_usable_ram = (current_entry->type == MULTIBOOT_MEMORY_AVAILABLE) && (current_entry->addr < FOUR_GIGABYTES);

        if (is_usable_ram)
        {
            u64int region_end = current_entry->addr + current_entry->len;

            if (region_end > FOUR_GIGABYTES)
            {
                region_end = FOUR_GIGABYTES;
            }

            release_region(current_entry->addr, region_end);
        }

        current_physical += current_entry->size + 4;
    }
}

void page_init(start_vars Boot_Vars, const struct multiboot_info *multiboot_info)
{
    for (u32int i = 0; i < BITMAP_BYTES; i++)
    {
        pages[i] = FULL_PAGE;
    }

    Release_All_Usable_Mem(multiboot_info);

    reserve_region(0, Boot_Vars.k_p_end);

    u32int multiboot_info_physical_address = (u32int)multiboot_info - KERNEL_VIRTUAL_BASE;
    reserve_region(multiboot_info_physical_address,
                   multiboot_info_physical_address + sizeof(struct multiboot_info));

    reserve_region(multiboot_info->mmap_addr,
                   multiboot_info->mmap_addr + multiboot_info->mmap_length);

    reserve_region(0x3FE000, 0x400000); // Temp slot frames
}

/**
 * @brief Returns the first free `4KB` page available.
 * @return `0` if no free pages exist; returns Physical page address otherwise
 */
usize page_alloc(void)
{
    usize page = 0; // Err page

    for (usize pos = 1; pos < MAX_PAGES; pos++)
    {
        if (ret_page_index(pos) == EMPTY)
        {
            set_page_index(pos, FULL);
            return pos * PAGE_4KB;
        }
    }

    return page;
}

/**
 * @brief Frees the Region on memory from page -> Page + `4KB`
 */
void page_free(usize page)
{
    release_region(page, page + PAGE_4KB);
}

void *kmap(int slot, usize phys)
{
    usize mask = 0xFFFFF000;

    boot_page_table[1022 + slot] = (phys & mask) | PTE_PRESENT | PTE_RW;
    void *page = (void *)(slot ? KMAP_SLOT1_VA : KMAP_SLOT0_VA);
    asm_invlpg(page);
    return page;
}

usize pd_create(void)
{
    usize phys = page_alloc(); // returns Physical mem 
    if (phys == 0){return 0;}

    usize* pd = kmap(KMAP_SLOT0, phys); // return virtual address to the page table and flushes the page

    memset(pd, 0, PAGE_4KB);

    usize *bpd = (usize *)boot_page_directory ;

    for (usize i = 768; i < 1024; i++)
    {
        pd[i] = (bpd)[i];
    }

    return phys;
}