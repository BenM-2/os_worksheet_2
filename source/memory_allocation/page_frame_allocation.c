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

/**
 * @brief Allocation bitmap: one bit per `4 KiB` physical frame (1 = used) (0 = free)
 */
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
int map_page(usize pd_phys, usize virt, usize phys, u32int flags);

//------------------------------------------------------------------------------
// Function Implementions

/**
 * @brief Reads allocation state of a given index
 * @param index Frame number (Physical address >> 12)
 * @retval `true` if frame is in use
 * @retval `false` if frame is free
 */
static bool ret_page_index(int index)
{
    int bit_index = index % SIZEOF_PAGE_STORAGE;
    page_type bitmask = 1 << bit_index;

    page_type data = pages[index / SIZEOF_PAGE_STORAGE] & bitmask;
    data = data >> bit_index;
    return (bool)data;
}

/**
 * @brief Set allocation state of a given index
 * @param index Frame number (Physical address >> 12)
 * @param value Takes in true or false to set the frame as (1 = used) (0 = free)
 */
static void set_page_index(int index, bool value)
{
    int bit_index = index % SIZEOF_PAGE_STORAGE;
    page_type bitmask = 1 << bit_index;
    page_type bitvalue = value << bit_index;

    page_type data = pages[index / SIZEOF_PAGE_STORAGE] & ~bitmask;
    data = data | bitvalue;
    pages[index / SIZEOF_PAGE_STORAGE] = data;
}

/**
 * @brief Marks every frame touching a physical range as free.
 *
 * The start is rounded down and the end rounded up to 4 KiB boundaries, so
 * any partially covered frame is also released.
 *
 * @param start_addr Physical start address (inclusive).
 * @param end_addr   Physical end address (exclusive).
 *
 * @see reserve_region()
 */
static void release_region(u64int start_addr, u64int end_addr)
{
    u64int Alligned_Start_index = start_addr >> 12;
    u64int Alligned_End_index = (PAGE_4KB + end_addr - 1) >> 12;

    for (u64int i = Alligned_Start_index; i < Alligned_End_index; i++)
    {
        set_page_index(i, EMPTY);
    }
}

/**
 * @brief Marks every frame touching a physical range as in use.
 *
 * The start is rounded down and the end rounded up to 4 KiB boundaries, so
 * any partially covered frame is also reserved.
 *
 * @param start_addr Physical start address (inclusive).
 * @param end_addr   Physical end address (exclusive).
 *
 * @see release_region()
 */
static void reserve_region(u64int start_addr, u64int end_addr)
{
    u64int Alligned_Start_index = start_addr >> 12;
    u64int Alligned_End_index = (PAGE_4KB + end_addr - 1) >> 12;

    for (u64int i = Alligned_Start_index; i < Alligned_End_index; i++)
    {
        set_page_index(i, FULL);
    }
}

/**
 * @brief Frees all usable RAM reported by the multiboot memory map.
 *
 * Walks the multiboot mmap entries and releases every
 * `MULTIBOOT_MEMORY_AVAILABLE` region that starts below 4 GiB, clamping
 * regions that extend past it. The mmap is accessed through the higher-half
 * mapping (`phys + KERNEL_VIRTUAL_BASE`), so it must lie within the memory
 * mapped at boot.
 *
 * @param multiboot_info Multiboot info structure passed by the bootloader.
 *
 * @note Calls panic() if the bootloader did not provide a memory map.
 */
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

/**
 * @brief Initialises the physical page frame allocator.
 *
 * Steps:
 *  -# Mark every frame as in use.
 *  -# Release all usable RAM from the multiboot memory map.
 *  -# Re-reserve memory that must not be handed out: the kernel image
 *     (and everything below it), the multiboot info structure, the memory
 *     map itself, and the frames backing the kmap temporary slots.
 *
 * @param Boot_Vars       Kernel start/end addresses captured at boot.
 * @param multiboot_info  Multiboot info structure (higher-half virtual address).
 *
 * @note Must be called once, before any call to page_alloc() or page_free().
 */
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
 * @brief Frees a single 4 KiB physical page frame.
 *
 * @param page Physical address of the frame, as returned by page_alloc().
 *
 * @warning No validation is performed: the address should be page-aligned
 *          and currently allocated. Double frees are not detected.
 *
 * @see page_alloc()
 */
void page_free(usize page)
{
    release_region(page, page + PAGE_4KB);
}

/**
 * @brief Maps a physical frame into a fixed kernel temporary slot.
 *
 * Writes a present, read/write PTE into `boot_page_table[1022 + slot]` and
 * flushes the TLB entry for the slot's virtual address, so the frame can be
 * accessed without it being permanently mapped. Mapping a new frame into a
 * slot invalidates the previous pointer to that slot.
 *
 * @param slot Slot number: `0` selects KMAP_SLOT0_VA; any non-zero value
 *             selects KMAP_SLOT1_VA.
 * @param phys Physical address of the frame to map (low 12 bits ignored).
 *
 * @return Virtual address through which the frame is now accessible.
 *
 * @note The slots live in the boot page table, so they are visible in every
 *       address space created with pd_create().
 * @note Not reentrant: the slots are global state.
 */
void *kmap(int slot, usize phys)
{
    usize mask = 0xFFFFF000;

    boot_page_table[1022 + slot] = (phys & mask) | PTE_PRESENT | PTE_RW;
    void *page = (void *)(slot ? KMAP_SLOT1_VA : KMAP_SLOT0_VA);
    asm_invlpg(page);
    return page;
}

/**
 * @brief Creates a new page directory.
 *
 * Allocates a frame, zeroes it, and copies the kernel-space entries
 * (PDEs 768-1023, i.e. virtual addresses from 0xC0000000 up) from
 * `boot_page_directory` so the kernel stays mapped in the new address space.
 * User-space entries (0-767) are left empty.
 *
 * @return Physical address of the new page directory, or `0` if no frame
 *         could be allocated.
 *
 * @note Uses kmap slot 0, invalidating any earlier pointer to that slot.
 * @todo Set PDE 1023 to point at the new directory itself if using
 *       recursive page table mapping.
 */
usize pd_create(void)
{
    usize phys = page_alloc(); // returns Physical mem
    if (phys == 0)
    {
        return 0;
    }

    usize *pd = kmap(KMAP_SLOT0, phys); // return virtual address to the page table and flushes the page

    memset(pd, 0, PAGE_4KB);

    usize *bpd = (usize *)boot_page_directory;

    for (usize i = 768; i < 1023; i++)
    {
        pd[i] = (bpd)[i];
    }

    pd[1023] = (phys & 0xFFFFF000) | PTE_PRESENT | PTE_RW;

    return phys;
}

/**
 * @brief Maps one 4 KiB virtual page to a physical frame in a page directory.
 *
 * Splits @p virt into a PD index (`virt >> 22`) and a PT index
 * (`(virt >> 12) & 0x3FF`), allocating and zeroing the page table first if
 * the PDE is not present.
 *
 * Virtual address layout:
 * @code
 *  31          22 21          12 11           0
 * +--------------+--------------+--------------+
 * |   PD index   |   PT index   |    offset    |
 * |   (10 bits)  |   (10 bits)  |   (12 bits)  |
 * +--------------+--------------+--------------+
 * @endcode
 *
 * PDE / PTE layout:
 * @code
 *  31                          12 11          0
 * +------------------------------+-------------+
 * |   physical frame address     |    flags    |
 * +------------------------------+-------------+
 * @endcode
 *
 * The PTE is written as `(phys & 0xFFFFF000) | (flags & 0xFFF) | PTE_PRESENT`:
 * `0xFFFFF000` keeps the address bits (and page-aligns @p phys), `0xFFF`
 * keeps the flags from overwriting the address.
 *
 * New PDEs are `PDE_PRESENT | PDE_RW`, with only the user bit copied from
 * @p flags, so each PTE decides the final permissions.
 *
 * @param pd_phys Physical address of the page directory.
 * @param virt    Virtual address to map (low 12 bits ignored).
 * @param phys    Physical frame address (low 12 bits masked off).
 * @param flags   PTE flags, e.g. `PTE_RW`, `PTE_USER` (bits 0-11 only).
 *
 * @retval 0 Mapping written.
 * @retval 1 A page table was needed but allocation failed.
 *
 * @note Uses kmap slots 0 (PD) and 1 (PT); not reentrant. The TLB entry is
 *       flushed only if @p pd_phys is the active directory.
 * @warning Do not map into 0xC0000000-0xC03FFFFF (shared `boot_page_table`).
 *          Existing mappings are overwritten silently.
 */
int map_page(usize pd_phys, usize virt, usize phys, u32int flags)
{
    usize page_directory_index = virt >> 22;
    usize page_table_index = (virt >> 12) & 0x3FF;

    usize *page_directory = kmap(KMAP_SLOT0, pd_phys);
    usize page_directory_entry = page_directory[page_directory_index];

    if (!(page_directory_entry & PDE_PRESENT))
    {
        usize page_table_phys = page_alloc();
        if (page_table_phys == 0)
        {
            return 1;
        }
        usize *page_table = kmap(KMAP_SLOT1, page_table_phys);
        memset(page_table, 0, PAGE_4KB);

        page_directory[page_directory_index] = page_table_phys | PDE_PRESENT | PDE_RW | (flags & PTE_USER);
        page_directory_entry = page_directory[page_directory_index];
    }

    usize *page_table = kmap(KMAP_SLOT1, page_directory_entry & 0xFFFFF000);
    page_table[page_table_index] = (phys & 0xFFFFF000) | (flags & 0xFFF) | PTE_PRESENT;

    if (pd_phys == asm_read_cr3())
    {
        asm_invlpg((void *)virt);
    }

    return 0;
}
