/**
 * @file gdt.c
 * @author Ben Marples
 * @brief does intial setup and any interactions with the Global Descriptor table
 */

//------------------------------------------------------------------------------
// Includes
#include "gdt.h"
#include "io.h"
#include "syscalls.h"

//------------------------------------------------------------------------------
// Definitions / Variables

struct gdt_entry
{
    u16int limit_low;
    u16int base_low;
    u8int base_mid;
    u8int access;
    u8int granularity;
    u8int base_high;
} __attribute__((packed));

static struct gdt_entry gdt[6];
static struct gdt_ptr gdt_descriptor;
static TSS tss;

/* Access byte bits */
#define GDT_PRESENT (1 << 7)
#define GDT_DPL(n) (((n) & 0x3) << 5)
#define GDT_CODE_DATA (1 << 4) /* S bit: not a system segment */
#define GDT_EXEC (1 << 3)
#define GDT_RW (1 << 1) /* readable for code, writable for data */

/* Readable shorthand matching the book's table */
#define GDT_DPL0 GDT_DPL(0)
#define GDT_DPL3 GDT_DPL(3)

#define GDT_RX (GDT_PRESENT | GDT_CODE_DATA | GDT_EXEC | GDT_RW)
#define GDT_RWD (GDT_PRESENT | GDT_CODE_DATA | GDT_RW)

/* System descriptor: S bit clear, type 0x9 = 32-bit TSS (available) */
#define GDT_SYS_TSS32 0x9
#define GDT_TSS (GDT_PRESENT | GDT_DPL0 | GDT_SYS_TSS32) /* = 0x89 */

#define GDT_GRAN_4K (1 << 7) /* limit counted in 4 KiB pages */
#define GDT_SIZE_32 (1 << 6) /* 32-bit protected mode segment */

#define GDT_FLAGS_32_4K (GDT_GRAN_4K | GDT_SIZE_32) /* = 0xC0 */
#define KERNEL_DATA_SELECTOR 0x10 /* GDT index 2 */
#define TSS_SELECTOR 0x28 /* GDT index 5 * 8 */
//------------------------------------------------------------------------------
// Function Declarations

//------------------------------------------------------------------------------
// Function Implementations

static void gdt_set_entry(int i, u32int base, u32int limit, u8int access, u8int flags)
{
    gdt[i].base_low = base & 0xFFFF;
    gdt[i].base_mid = (base >> 16) & 0xFF;
    gdt[i].base_high = (base >> 24) & 0xFF;

    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].granularity = ((limit >> 16) & 0x0F) | (flags & 0xF0);

    gdt[i].access = access;
}

void gdt_init(void)
{
    // Set entry
    gdt_set_entry(0, 0, 0, 0x00, 0x00);                                   // Null descriptor
    gdt_set_entry(1, 0, 0xFFFFFFFF, GDT_RX | GDT_DPL0, GDT_FLAGS_32_4K);  // Kernel code
    gdt_set_entry(2, 0, 0xFFFFFFFF, GDT_RWD | GDT_DPL0, GDT_FLAGS_32_4K); // Kernel data
    gdt_set_entry(3, 0, 0xFFFFFFFF, GDT_RX | GDT_DPL3, GDT_FLAGS_32_4K);  // User Code
    gdt_set_entry(4, 0, 0xFFFFFFFF, GDT_RWD | GDT_DPL3, GDT_FLAGS_32_4K); // User Data
    gdt_set_entry(5, (u32int)&tss, sizeof(TSS) - 1, GDT_TSS, 0x00);

    gdt_descriptor.size = sizeof(gdt) - 1;
    gdt_descriptor.address = (u32int)&gdt;

    lgdt_pub(&gdt_descriptor);
    ltr_pub(TSS_SELECTOR);
}

void tss_set_kernel_stack(u32int esp0)
{
    tss.ss0  = KERNEL_DATA_SELECTOR;
    tss.esp0 = esp0;
}