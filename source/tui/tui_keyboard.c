
/**
 * @file tui_keyboard.c
 * @author Ben Marples
 * @brief defines the callback functions for the tui
 */

//------------------------------------------------------------------------------
// Includes
#include "tui.h"
#include "interrupts.h"
#include "io.h"
#include "keyboard.h"
#include "framebuffer.h"
//------------------------------------------------------------------------------
// Definititions / Vars
static TUI_keyboard_Callbacks callbacks;

//------------------------------------------------------------------------------
// Function Declarations

void tui_init_cb(const TUI_keyboard_Callbacks * const callbacks_);
void tui_keyboard_callback(const u32int interrupt);

//------------------------------------------------------------------------------
// Function Implementations
void tui_init_cb(const TUI_keyboard_Callbacks * const callbacks_)
{
    callbacks = *callbacks_;
}

void tui_keyboard_callback(const u32int interrupt)
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
            callbacks.on_backspace();
            break;
        case '\n': // case backspace
            callbacks.on_newline();
            break;

        default: // normal ascii char
            callbacks.on_char(ascii);
            break;
        }
    }
    pic_acknowledge(interrupt);
}


