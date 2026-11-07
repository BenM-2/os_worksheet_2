/**
 * @file kmain.c
 * @author Ben Marples
 *
 */

//------------------------------------------------------------------------------
// Includes
#define NO_NAMESPACE
#include "framebuffer.h"
#include "hardware_interrupt_enabler.h"
#include "interrupts.h"
#include "io.h"
#include "tui.h"
#include "gdt.h"
#include "multiboot.h"
#include "page_frame_allocation.h"
#include "kstring.h"

// Vars / types
// void kernel_main_start(void);
void kernel_virtual_start(void);
void kernel_physical_start(void);
void kernel_virtual_end(void);
void kernel_physical_end(void);

typedef void (*call_module_t)(void);

//------------------------------------------------------------------------------
// Function Implementions

/**
 * @brief Self-test for the kmap temporary mapping slots.
 *
 * Allocates two physical frames and verifies that kmap() correctly maps
 * frames into slots and that remapping a slot updates the translation
 * (i.e. the TLB entry is flushed).
 *
 * The test runs in two stages:
 *  -# **Aliasing check:** frame A is mapped into slots 0 and 1. A pattern
 *     (0xDEAD0000 + i) is written through slot 0 and must read back
 *     identically through slot 1, confirming both slots reach the same
 *     physical frame.
 *  -# **Remap/flush check:** slot 0 is remapped to frame B and a different
 *     pattern (0xBEEF0000 + i) is written through it. Slot 1, still mapped
 *     to frame A, must continue to hold the original 0xDEAD0000 + i data.
 *     This fails if the remap did not take effect (stale TLB entry) or
 *     if the write leaked into frame A.
 *
 * Progress and results are printed to the framebuffer console.
 *
 * @note Each frame is treated as 1024 words, which assumes a 4 KiB page
 *       and a 32-bit usize.
 * @note The allocated frames are not freed before returning, so each
 *       call leaks two frames.
 *
 * @retval 0 All checks passed.
 * @retval 1 A frame allocation failed, or a read/write check mismatched.
 *
 * @see kmap()
 * @see page_alloc()
 */
int kmap_test(void)
{
    // Allocate 2 test frames
    usize frame_a = page_alloc();
    usize frame_b = page_alloc();
    if (!frame_a | !frame_b)
    {
        FB_write_string("Failed page_alloc", strlen("Failed page_alloc"));
        FB_set_cursor_newline();
        return 1;
    }

    volatile usize *s0 = kmap(0, frame_a);
    volatile usize *s1 = kmap(1, frame_a);

    // check slots are different
    // set slot 0 -> 0xDEAD0000 + i for all slots
    for (int i = 0; i < 1024; i++)
    {
        s0[i] = 0xDEAD0000 + i;
    }
    for (int i = 0; i < 1024; i++)
    {
        if (s1[i] != 0xDEAD0000 + i)
        {
            FB_write_string("Failed Read_write from frame", strlen("Failed Read_write from frame"));
            FB_set_cursor_newline();
            return 1;
        }
    }
    FB_write_string("Passed Read_write from frame", strlen("Passed Read_write from frame"));
    FB_set_cursor_newline();

    // remap slot 0 to the other frame; Tests flush
    s0 = kmap(0, frame_b);
    for (int i = 0; i < 1024; i++)
    {
        s0[i] = 0xBEEF0000 + i;
    }

    // slot 1 should hold frame_a data
    for (int i = 0; i < 1024; i++)
    {
        if (s1[i] != 0xDEAD0000 + i)
        {
            FB_write_string("Failed ut write from frame", strlen("Failed ut write from frame"));
            FB_set_cursor_newline();
            return 1;
        }
    }
    FB_write_string("Passed ut write from frame", strlen("Passed ut write from frame"));
    FB_set_cursor_newline();



    return 0;
}

int kernel_main(unsigned int ebx)
{
    multiboot_info_t *mbinfo = (multiboot_info_t *)ebx;

    // if ((mbinfo->flags & MULTIBOOT_INFO_MODS) && mbinfo->mods_count == 1 && mbinfo->mods_count == 1)
    // {
    //     multiboot_module_t *modules = (multiboot_module_t *)mbinfo->mods_addr;
    //     call_module_t start_progam = (call_module_t)modules[0].mod_start;
    // start_progam();
    // return 0;
    // }

    disable_hardware_interrupts();

    gdt_init();

    start_vars boot_vars = {
        .k_p_start = (u32int)&kernel_physical_start,
        .k_p_end = (u32int)&kernel_physical_end,
        .k_v_start = (u32int)&kernel_virtual_start,
        .k_v_end = (u32int)&kernel_virtual_end,
    };

    page_init(boot_vars, mbinfo);

    tui_start();

    interrupts_install_idt();

    // FB_write_DEBUG_string("TEST",4);

    enable_hardware_interrupts();

    kmap_test();

    return ebx;
}