/**
 * @file kmain.c
 * @author Ben Marples
 * 
 */

//------------------------------------------------------------------------------
// Includes
#include "framebuffer.h"

//------------------------------------------------------------------------------
// Function Implementions
int kernel_main()
{
    FB_set_FG_ALL(RED);
    FB_set_FG_CELL(GREEN, 1,2);
    // BG
    FB_set_BG_ALL(LIGHT_MAGENTA);
    FB_set_BG_CELL(BLACK,2,2);
    // Char
    FB_set_CHAR_ALL(' ');
    FB_set_CHAR_CELL('A',3,2);
    // Full Cell
    FB_set_FULL_CELL('B',BLACK,WHITE, 4,2);
    return 0;
}