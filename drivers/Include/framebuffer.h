/**
 * @file framebuffer.h
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

#ifndef FRAME_BUFFER_API
#define FRAME_BUFFER_API

//------------------------------------------------------------------------------
// Definitions
typedef struct __attribute__((packed))
{
    unsigned char asci_char; // Ascii character
    unsigned char FG_BG;     // FG + BG
} FrameBuffer;

typedef enum
{
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHT_GREY,
    DARK_GREY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_BROWN,
    WHITE,
} FB_COLOUR;

#define FRAME_BUFFER_START 0x000B8000

#define FB_MAX_COL 80
#define FB_MAX_ROW 25

#define FB_lower_mask 0xF0
#define FB_upper_mask 0x0F


//------------------------------------------------------------------------------
// Cursor implementation
#ifndef FRAME_BUFFER_CUSTOM_CURSOR /* VGA cursor */

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15
/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the given position
 *
 * @param pos The new position of the cursor
 */
static void FB_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}

#else /* Custom Cursor */

static struct
{
    unsigned short row;
    unsigned short col;
    unsigned short index;
} FB_cursor;

static void FB_move_cursor(const unsigned short pos)
{
    FB_cursor.row = pos / FB_MAX_COL;
    FB_cursor.col = pos % FB_MAX_COL;
    FB_cursor.index = pos;
}

#endif

//------------------------------------------------------------------------------
// Function Declarations

// // FG
// void FB_set_FG_ALL(const FB_COLOUR c);
// void FB_set_FG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
// // BG
// void FB_set_BG_ALL(const FB_COLOUR c);
// void FB_set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row);
// // Char
// void FB_set_CHAR_ALL(const char c);
// void FB_set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row);
// // Full Cell
// void FB_set_FULL_CELL(const char c, const FB_COLOUR FG, const FB_COLOUR BG, const unsigned int col, const unsigned int row);

// // Final Functions
// void FB_print_string(const char *const s);

#endif