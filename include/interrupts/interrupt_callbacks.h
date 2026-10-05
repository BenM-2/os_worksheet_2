/**
 * @file interrupt_callbacks.h
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

#ifndef INTERRUPTS_CALLBACKS_H
#define INTERRUPTS_CALLBACKS_H

//------------------------------------------------------------------------------
// Includes
#include "type.h"

//------------------------------------------------------------------------------
// Definitions

typedef struct
{
    void (*keyboard_event)(const u32int interrupt);
} Interrupt_callbacks;

#endif