

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
    ERR_t (*cmd_cb)(const int argc, char *argv[]);  // arg count and arg values 
} TUI_CMD;

//------------------------------------------------------------------------------
// Apps

ERR_t TUI_CMD_bg(const int argc, char *argv[]);
ERR_t TUI_CMD_fg(const int argc, char *argv[]);

//------------------------------------------------------------------------------
// App Table

static const TUI_CMD TUI_app_table[] = {
    {.cmd="bg",.cmd_cb=TUI_CMD_bg},
    {.cmd="fg",.cmd_cb=TUI_CMD_fg}
};

#endif