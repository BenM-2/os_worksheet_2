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

//------------------------------------------------------------------------------
// Function Implementions

int kernel_main()
{
    tui_start();

    interrupts_install_idt();

    enable_hardware_interrupts();

    return 0;
}