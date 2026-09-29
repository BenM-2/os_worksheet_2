/**
 * @file framebuffer.h
 * @author Ben Marples
 * @brief Holds the whole frame buffer api
 */

#ifndef FRAME_BUFFER_API
#define FRAME_BUFFER_API

//------------------------------------------------------------------------------
// Definitions
typedef struct __attribute__((packed)) {
    unsigned char asci_char;    // Ascii character
    unsigned char FG_BG;        // FG + BG
} FrameBuffer ;

typedef enum {
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
}FB_COLOUR;


//------------------------------------------------------------------------------
// Function Declarations
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
#endif