/**
 * @file tui_keyboard.h
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

#ifndef TUI_KEYBOARD_H
#define TUI_KEYBOARD_H

#include "type.h"


void tui_init_cb(const TUI_keyboard_Callbacks * const callbacks_);
void tui_keyboard_callback(const u32int interrupt);

#endif