/**
 * @file framebuffer.c
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

//------------------------------------------------------------------------------
// Includes
#include "framebuffer.h"
#include "io.h"

//------------------------------------------------------------------------------
// Function Declarations

// Direct FB access
static int FB_ROW_COL_TO_INDEX(const unsigned int col, const unsigned int row);
// FG
static void FB_set_FG_ALL(const FB_COLOUR c);
static void FB_set_FG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
// BG
static void FB_set_BG_ALL(const FB_COLOUR c);
static void FB_set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
// Char
static void FB_set_CHAR_ALL(const char c);
static void FB_set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row);
// Full Cell
static void FB_set_FULL_CELL(const char c,const FB_COLOUR FG,const FB_COLOUR BG, const unsigned int col, const unsigned int row);

// FB cursor 


// Public Facing functions


//------------------------------------------------------------------------------
// Function Implementations

static int FB_ROW_COL_TO_INDEX(const unsigned int col, const unsigned int row)
{
    return (col % FB_MAX_COL) + ((row % FB_MAX_ROW) * FB_MAX_COL);
}

// Set All
static void FB_set_FG_ALL(const FB_COLOUR c)
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

static void FB_set_BG_ALL(const FB_COLOUR c)
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

static void FB_set_CHAR_ALL(const char c)
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
static void FB_set_FG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row)
{
    int index = FB_ROW_COL_TO_INDEX(col, row);
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index];            // Store current Char and FG color
    temp.FG_BG = temp.FG_BG & FB_lower_mask; // set lower bits to 0
    temp.FG_BG = temp.FG_BG | c;
    fb[index] = temp;
}

static void FB_set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row)
{
    int index = FB_ROW_COL_TO_INDEX(col, row);
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index];            // Store current Char and FG color
    temp.FG_BG = temp.FG_BG & FB_upper_mask; // set lower bits to 0
    temp.FG_BG = temp.FG_BG | (c << 4);
    fb[index] = temp;
}

static void FB_set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row)
{
    const int index = FB_ROW_COL_TO_INDEX(col, row);
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = fb[index]; // Store current Char and BG color
    temp.asci_char = c;
    fb[index] = temp;
}

// Set FULL CELL
static void FB_set_FULL_CELL(const char c,const FB_COLOUR FG,const FB_COLOUR BG, const unsigned int col, const unsigned int row)
{
    const int index = FB_ROW_COL_TO_INDEX(col, row);
    FrameBuffer *const fb = (FrameBuffer *)FRAME_BUFFER_START;
    FrameBuffer temp = {
        .asci_char = c,
        .FG_BG = (FG) | (BG << 4),
    };
    fb[index] = temp;
}


// Cursor



// Public facing api

void FB_print_string(const char *const s, const unsigned int length)
{

}