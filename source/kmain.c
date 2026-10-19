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

// Vars / types
// void kernel_main_start(void);
void kernel_virtual_start(void);
void kernel_physical_start(void);
void kernel_virtual_end(void);
void kernel_physical_end(void);

typedef void (*call_module_t)(void);

//------------------------------------------------------------------------------
// Function Implementions

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

    page_init(boot_vars,mbinfo);

    tui_start();

    interrupts_install_idt();

    // FB_write_DEBUG_string("TEST",4);

    enable_hardware_interrupts();

    return ebx;
}