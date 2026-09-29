/**
 * @file framebuffer.c
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

//------------------------------------------------------------------------------
// Includes
#include "framebuffer.h"
//------------------------------------------------------------------------------
// Definitions
#define FRAME_BUFFER_START 0x000B8000
#define FB_MAX_COL 80
#define FB_MAX_ROW 25
#define FB_lower_mask 0xF0
#define FB_upper_mask 0x0F

//------------------------------------------------------------------------------
// Function Declarations
static int FB_ROW_COL_TO_INDEX(const unsigned int col, const unsigned int row);
// FG
void FB_set_FG_ALL(const FB_COLOUR c);
void FB_set_FG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
// BG
void FB_set_BG_ALL(const FB_COLOUR c);
void FB_set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
// Char
void FB_set_CHAR_ALL(const char c);
void FB_set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row);
// Full Cell
void FB_set_FULL_CELL(const char c,const FB_COLOUR FG,const FB_COLOUR BG, const unsigned int col, const unsigned int row);

//------------------------------------------------------------------------------
// Function Implementations

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
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index];            // Store current Char and FG color
    temp.FG_BG = temp.FG_BG & FB_lower_mask; // set lower bits to 0
    temp.FG_BG = temp.FG_BG | c;
    fb[index] = temp;
}

void FB_set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row)
{
    int index = FB_ROW_COL_TO_INDEX(col, row);
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index];            // Store current Char and FG color
    temp.FG_BG = temp.FG_BG & FB_upper_mask; // set lower bits to 0
    temp.FG_BG = temp.FG_BG | (c << 4);
    fb[index] = temp;
}

void FB_set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row)
{
    const int index = FB_ROW_COL_TO_INDEX(col, row);
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index]; // Store current Char and BG color
    temp.asci_char = c;
    fb[index] = temp;
}

void FB_set_FULL_CELL(const char c,const FB_COLOUR FG,const FB_COLOUR BG, const unsigned int col, const unsigned int row)
{
    const int index = FB_ROW_COL_TO_INDEX(col, row);
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = {
        .asci_char = c,
        .FG_BG = (FG) | (BG << 4),
    };
    fb[index] = temp;
}