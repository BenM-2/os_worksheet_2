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
    FB_set_BG_ALL(DARK_GREY);
    FB_set_FG_ALL(WHITE);
    FB_set_CHAR_ALL(' ');
    return 0;
}