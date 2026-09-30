/**
 * @file kmain.c
 * @author Ben Marples
 * 
 */

//------------------------------------------------------------------------------
// Includes
#define NO_NAMESPACE
#include "framebuffer.h"
#include "kstring.h"
//------------------------------------------------------------------------------
// Function Implementions


int kernel_main()
{
    set_BG_ALL(DARK_GREY);
    set_FG_ALL(WHITE);
    set_CHAR_ALL(' ');

    static char *const buf = "~:$";
    move_cursor(0);
    write_string(buf,strlen(buf));
    move_cursor(80);
    write_string(buf,strlen(buf));
    return 0;
}