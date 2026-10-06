/**
 * @file framebuffer.c
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

//------------------------------------------------------------------------------
// Includes
#include "framebuffer.h"
#ifndef FRAME_BUFFER_CUSTOM_CURSOR /* VGA cursor */
#include "io.h"
#include "kstring.h"
#endif
//------------------------------------------------------------------------------
// Function Declarations

// Cursor
void FB_move_cursor(unsigned short pos);
void FB_move_cursor_row_col(const unsigned int col, const unsigned int row);
unsigned short FB_get_cursor();
void FB_increment_cursor();
void FB_decrement_cursor();
void FB_write_string(const char *const buff, const unsigned int length);

// ROW to index
static int FB_ROW_COL_TO_INDEX(const unsigned int col, const unsigned int row);

// FG
void FB_set_FG_ALL(const FB_COLOUR c);
void FB_set_FG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
void FB_set_FG_CELL_INDEX(const FB_COLOUR c, const unsigned int index);

// BG
void FB_set_BG_ALL(const FB_COLOUR c);
void FB_set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
void FB_set_BG_CELL_INDEX(const FB_COLOUR c, const unsigned int index);

// CHAR
void FB_set_CHAR_ALL(const char c);
void FB_set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row);
void FB_set_CHAR_CELL_INDEX(const char c, const unsigned int index);

void FB_scroll();
void FB_set_cursor_newline();
int FB_get_current_row();
void FB_write_DEBUG_string(const char *const buff, const unsigned int length);
//------------------------------------------------------------------------------
// Function Implementations

#ifndef FRAME_BUFFER_CUSTOM_CURSOR
/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position
 *
 * @param pos The new position of the cursor
 */
void FB_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

unsigned short FB_get_cursor()
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    unsigned short pos = inb(FB_DATA_PORT) << 8;
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    return pos | inb(FB_DATA_PORT);
}

#else
static struct
{
    unsigned short row;
    unsigned short col;
    unsigned short index;
} FB_cursor;

void FB_move_cursor(const unsigned short pos)
{
    FB_cursor.row = pos / FB_MAX_COL;
    FB_cursor.col = pos % FB_MAX_COL;
    FB_cursor.index = pos;
}

unsigned short FB_get_cursor()
{
    return FB_cursor.index;
}

#endif

void FB_move_cursor_row_col(const unsigned int col, const unsigned int row)
{
    FB_move_cursor(FB_ROW_COL_TO_INDEX(col, row));
}

static int FB_ROW_COL_TO_INDEX(const unsigned int col, const unsigned int row)
{
    return (col % FB_MAX_COL) + ((row % FB_MAX_ROW) * FB_MAX_COL);
}

// Set All
void FB_set_FG_ALL(const FB_COLOUR c)
{
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    for (int i = 0; i < (FB_MAX_ROW * FB_MAX_COL); i++)
    {
        FrameBuffer temp = fb[i];                // Store current Char and FG color
        temp.FG_BG = temp.FG_BG & FB_lower_mask; // set lower bits to 0
        temp.FG_BG = temp.FG_BG | c;
        fb[i] = temp;
    }
}

void FB_set_BG_ALL(const FB_COLOUR c)
{
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    for (int i = 0; i < (FB_MAX_ROW * FB_MAX_COL); i++)
    {
        FrameBuffer temp = fb[i];                // Store current Char and BG color
        temp.FG_BG = temp.FG_BG & FB_upper_mask; // set lower bits to 0
        temp.FG_BG = temp.FG_BG | (c << 4);
        fb[i] = temp;
    }
}

void FB_set_CHAR_ALL(const char c)
{
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    for (int i = 0; i < (FB_MAX_ROW * FB_MAX_COL); i++)
    {
        FrameBuffer temp = fb[i]; // Store current Char and BG color
        temp.asci_char = c;
        fb[i] = temp;
    }
}

// Set CELL

void FB_set_FG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row)
{
    int index = FB_ROW_COL_TO_INDEX(col, row);
    FB_set_FG_CELL_INDEX(c, index);
}

void FB_set_FG_CELL_INDEX(const FB_COLOUR c, const unsigned int index)
{
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index];            // Store current Char and FG color
    temp.FG_BG = temp.FG_BG & FB_lower_mask; // set lower bits to 0
    temp.FG_BG = temp.FG_BG | c;
    fb[index] = temp;
}

void FB_set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row)
{
    int index = FB_ROW_COL_TO_INDEX(col, row);
    FB_set_BG_CELL_INDEX(c, index);
}

void FB_set_BG_CELL_INDEX(const FB_COLOUR c, const unsigned int index)
{
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index];            // Store current Char and FG color
    temp.FG_BG = temp.FG_BG & FB_upper_mask; // set lower bits to 0
    temp.FG_BG = temp.FG_BG | (c << 4);
    fb[index] = temp;
}

void FB_set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row)
{
    const int index = FB_ROW_COL_TO_INDEX(col, row);
    FB_set_CHAR_CELL_INDEX(c, index);
}

void FB_set_CHAR_CELL_INDEX(const char c, const unsigned int index)
{
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index]; // Store current Char and BG color
    temp.asci_char = c;
    fb[index] = temp;
}

// Cursor
void FB_increment_cursor()
{
    FB_move_cursor(FB_get_cursor() + 1);
}

void FB_decrement_cursor()
{
    FB_move_cursor(FB_get_cursor() - 1);
}

void FB_write_string(const char *const buff, const unsigned int length)
{
    for (unsigned i = 0; i < length; i++)
    {
        FB_set_CHAR_CELL_INDEX(buff[i], FB_get_cursor());
        FB_increment_cursor();
    }
}

void FB_scroll()
{
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    for (int row = 0; row < (FB_MAX_ROW - 1); row++)
    {
        for (int col = 0; col < FB_MAX_COL; col++)
        {
            fb[FB_ROW_COL_TO_INDEX(col, row)] = fb[FB_ROW_COL_TO_INDEX(col, row + 1)];
        }
    }

    /* clear the new bottom row */
    for (int col = 0; col < FB_MAX_COL; col++)
    {
        fb[(FB_MAX_ROW - 1) * FB_MAX_COL + col].asci_char = ' ';
    }
}

int FB_get_current_row()
{
    unsigned short index = FB_get_cursor();
    int current_row = index / FB_MAX_COL;
    return current_row;
}

void FB_set_cursor_newline()
{
    int current_row = FB_get_current_row();  
    if ((current_row < (FB_MAX_ROW - 1)))
    {
        current_row++;
    }
    else
    {
        FB_scroll();
        current_row = FB_MAX_ROW - 1;
    }

    FB_move_cursor(FB_MAX_COL * current_row);
}


void FB_write_DEBUG_string(const char *const buff, const unsigned int length)
{
    for (int i = (FB_MAX_ROW - 1) * FB_MAX_COL;i < FB_MAX_COL * FB_MAX_ROW;i++)
    {
        FB_set_CHAR_CELL_INDEX(' ',i);
    }
    const char *DEBUG_MSG = "DEBUG: ";
    const int cursor = FB_get_cursor();
    FB_move_cursor((FB_MAX_ROW - 1) * FB_MAX_COL);
    FB_write_string(DEBUG_MSG,strlen(DEBUG_MSG));
    FB_write_string(buff,length);

    FB_move_cursor(cursor);
}
