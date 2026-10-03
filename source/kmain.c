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
#include "kstring.h"
#include "keyboard.h"
#include "io.h"

//------------------------------------------------------------------------------
// Function Implementions

void screen_init()
{
    set_BG_ALL(DARK_GREY);
    set_FG_ALL(WHITE);
    set_CHAR_ALL(' ');

    move_cursor(0);
}

void keyboard_cb(const u32int interrupt)
{
    u8int input;
    u8int ascii;

    while ((inb(0x64) & 1))
    {
        input = keyboard_read_scan_code();
        // Only process if it's not a break code
        if ((input & 0x80))
        {
            continue;
        }

        if (input > KEYBOARD_MAX_ASCII)
        {
            continue;
        }

        ascii = keyboard_scan_code_to_ascii(input);
        if (ascii == 0)
        {
            continue;
        }

        switch (ascii)
        {
        case '\b': // case backspace
            break;
        case '\n': // case backspace
            break;

        default: // normal ascii char
            // Add the new character to the display
            FB_set_CHAR_CELL_INDEX(ascii, FB_get_cursor());
            FB_increment_cursor();
            break;
        }
    }
    pic_acknowledge(interrupt);
}

int kernel_main()
{
    screen_init();

    // Interrupt Config
    Interrupt_callbacks cb = {
        .keyboard_event = keyboard_cb,
    };
    interrupts_cb_config(&cb);

    interrupts_install_idt();

    enable_hardware_interrupts();

    return 0;
}