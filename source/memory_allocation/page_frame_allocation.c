
#include "page_frame_allocation.h"
#include "paging.h"

static page_type pages[BITMAP_BYTES];

#include "io.h"
static void serial_putc(char c) {
    outb(0x3F8, c);
}

static void serial_puts(const char *s) {
    while (*s) serial_putc(*s++);
}

static void panic(void) {
    serial_puts("PANIC at page_init\n");
    while (1) asm volatile("hlt");
}

// static bool ret_page_index(int index)
// {
//     int bit_index = index % SIZEOF_PAGE_STORAGE;
//     page_type bitmask = 1 << bit_index;

//     page_type data = pages[index / SIZEOF_PAGE_STORAGE] & bitmask;
//     data = data >> bit_index;
//     return (bool)data;
// }

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

        bool is_usable_ram = (current_entry->type & MULTIBOOT_MEMORY_AVAILABLE) && (current_entry->addr < FOUR_GIGABYTES);

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
}