/**
 * @file tui.h
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

#ifndef TUI_H
#define TUI_H

#include "type.h"

typedef struct 
{
    void (* on_newline)();
    void (* on_backspace)();
    void (* on_char)(const char c);
} TUI_keyboard_Callbacks;

void tui_start();
#endif