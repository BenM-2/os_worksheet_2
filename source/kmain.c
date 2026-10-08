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

//------------------------------------------------------------------------------
// Function Implementions

int kernel_main()
{

    gdt_init();

    tui_start();

    interrupts_install_idt();

    // FB_write_DEBUG_string("TEST",4);

    enable_hardware_interrupts();

    return 0;
}