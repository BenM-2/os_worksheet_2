
/**
 * @file tui.c
 * @author Ben Marples
 * @brief Sends all interupts to their related callback defined under interpt_callbacks.h and instantiated used interrupt_cb_config()
 */

//------------------------------------------------------------------------------
// Includes

#include "tui.h"
#include "tui_keyboard.h"
#include "tui_apps.h"
#define NO_NAMESPACE
#include "framebuffer.h"
#include "interrupts.h"
#include "kstring.h"

//------------------------------------------------------------------------------
// Definitions

#define MAX_PATH_LENGTH 10

static const char *const PATH = "~:>";
static int FB_CMD_START_INDEX = 0;

//------------------------------------------------------------------------------
// Function Declarations
void screen_init();
void tui_start();
void on_backspace();
void on_newline();
void on_char(const char ascii);
void write_path();

//------------------------------------------------------------------------------
// Function Implementations
static void update_FB_CMD_START_INDEX()
{
    FB_CMD_START_INDEX = FB_get_cursor();
}

void tui_start()
{
    // Keyboard setup
    TUI_keyboard_Callbacks TUI_cb = {
        .on_backspace = on_backspace,
        .on_newline = on_newline,
        .on_char = on_char,
    };
    tui_init_cb(&TUI_cb);

    // Interrupt Config
    Interrupt_callbacks ITR_cb = {
        .keyboard_event = tui_keyboard_callback,
    };
    interrupts_cb_config(&ITR_cb);

    screen_init();
    write_path();
    update_FB_CMD_START_INDEX();
}

void on_backspace()
{
    decrement_cursor();

    if (FB_get_cursor() < FB_CMD_START_INDEX)
    {
        increment_cursor();
        return;
    }
    set_CHAR_CELL_INDEX(' ', FB_get_cursor());
}

#define MAX_LINE 10
#define LINE_LENGTH 32

void on_newline()
{
    int command_ran = 0;
    int buf_len = 0;
    char buf[256];
    // Get args
    int argc = 0;
    char *argv[LINE_LENGTH];

    // Readline
    int fb_index = FB_get_cursor();
    FrameBuffer *fb = (FrameBuffer *)FRAME_BUFFER_START;
    // int start_row_index = get_current_row() * FB_MAX_COL;
    for (int i = FB_CMD_START_INDEX; i < fb_index; i++)
    {
        buf[buf_len] = fb[i].asci_char;
        buf_len++;
    }
    buf[buf_len] = '\0'; // end of arr
    // mutate array in place

    char *arg = &buf[0];
    while (*arg != '\0')
    {
        while (*arg == ' ')
        {
            *arg++ = '\0';
        }

        if (*arg == '\0') // only spaces were left
        {
            break;
        }

        argv[argc++] = arg;

        while (*arg && *arg != ' ')
        {
            arg++;
        }
    }

    argv[argc] = 0;
    
    FB_write_DEBUG_string(buf,buf_len);

    int cmd_length = sizeof(TUI_app_table) / sizeof(TUI_app_table[0]);
    for (int i = 0; i < cmd_length; i++)
    {
        if ((strcmp(argv[0], TUI_app_table[i].cmd)) == 0)
        {
            TUI_app_table[i].cmd_cb(argc, argv);
            command_ran++;
            break;
        }
    }
    //  put program on newline
    if (!command_ran)
    {
        FB_set_cursor_newline();
    }

    write_path();
    update_FB_CMD_START_INDEX();
}

void on_char(const char ascii)
{
    // Add the new character to the display
    set_CHAR_CELL_INDEX(ascii, FB_get_cursor());
    increment_cursor();
}

void write_path()
{
    write_string(PATH, strlen(PATH));
}

void screen_init()
{
    set_BG_ALL(DARK_GREY);
    set_FG_ALL(WHITE);
    set_CHAR_ALL(' ');

    move_cursor(0);
}