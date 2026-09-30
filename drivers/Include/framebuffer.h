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

#ifdef NO_NAMESPACE
// Cursor
static inline void move_cursor(unsigned short pos) {
    FB_move_cursor(pos);
}
static inline void move_cursor_row_col(const unsigned int col, const unsigned int row) {
    FB_move_cursor_row_col(col, row);
}
static inline unsigned short get_cursor(void) {
    return FB_get_cursor();
}
static inline void increment_cursor(void) {
    FB_increment_cursor();
}
static inline void decrement_cursor(void) {
    FB_decrement_cursor();
}
static inline void write_string(const char *const buff, const unsigned int length) {
    FB_write_string(buff, length);
}

// FG
static inline void set_FG_ALL(const FB_COLOUR c) {
    FB_set_FG_ALL(c);
}
static inline void set_FG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row) {
    FB_set_FG_CELL(c, col, row);
}
static inline void set_FG_CELL_INDEX(const FB_COLOUR c, const unsigned int index) {
    FB_set_FG_CELL_INDEX(c, index);
}

// BG
static inline void set_BG_ALL(const FB_COLOUR c) {
    FB_set_BG_ALL(c);
}
static inline void set_BG_CELL(const FB_COLOUR c, const unsigned int col, const unsigned int row) {
    FB_set_BG_CELL(c, col, row);
}
static inline void set_BG_CELL_INDEX(const FB_COLOUR c, const unsigned int index) {
    FB_set_BG_CELL_INDEX(c, index);
}

// CHAR
static inline void set_CHAR_ALL(const char c) {
    FB_set_CHAR_ALL(c);
}
static inline void set_CHAR_CELL(const char c, const unsigned int col, const unsigned int row) {
    FB_set_CHAR_CELL(c, col, row);
}
static inline void set_CHAR_CELL_INDEX(const char c, const unsigned int index) {
    FB_set_CHAR_CELL_INDEX(c, index);
}
#endif

#endif