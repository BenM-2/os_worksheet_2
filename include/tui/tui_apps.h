

// NOTES TO REMEMEBR FUTURE ME

// USE a dict like {.cmd="bg_color",.callback=setbg(int argc, char **argv[])}

/**
 * @file tui_apps.h
 * @author Ben Marples
 * @brief
 */

#ifndef TUI_APPS_H
#define TUI_APPS_H

//------------------------------------------------------------------------------
// Includes
#include "err.h"

typedef struct
{
    const char *cmd;
    ERR_t (*cmd_cb)(const int argc, char *argv[]); // arg count and arg values
    const char *usage;
} TUI_CMD;

//------------------------------------------------------------------------------
// Apps

ERR_t TUI_CMD_bg(const int argc, char *argv[]);
ERR_t TUI_CMD_fg(const int argc, char *argv[]);
ERR_t TUI_CMD_clear(const int argc, char *argv[]);
ERR_t TUI_CMD_help(const int argc, char *argv[]);
ERR_t TUI_CMD_shutdown(const int argc,char *argv[]);
//------------------------------------------------------------------------------
// App Table

typedef enum  {
    TUI_BG,
    TUI_FG,
    TUI_CLEAR,
    TUI_HELP,
    TUI_SHUTDOWN,
}TUI_app_table_Index;

static const TUI_CMD TUI_app_table[] = {
    {.cmd = "bg",   .cmd_cb = TUI_CMD_bg,   .usage = "bg <color>  - sets the background color"},
    {.cmd = "fg",   .cmd_cb = TUI_CMD_fg,   .usage = "fg <color>  - set foreground colour"},
    {.cmd = "clear",.cmd_cb = TUI_CMD_clear,.usage = "clear       - clear the screen"},
    {.cmd = "help", .cmd_cb = TUI_CMD_help, .usage = "help [cmd]  - list commands, or show usage for cmd"},
    {.cmd = "shutdown", .cmd_cb = TUI_CMD_shutdown, .usage = "shutdown    - shuts down computer"},
};

#endif