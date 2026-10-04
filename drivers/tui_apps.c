/**
 * @file tui_apps.c
 * @author Ben Marples
 * @brief
 */

//------------------------------------------------------------------------------
// Includes
#include "tui_apps.h"
#include "framebuffer.h"
#include "kstring.h"

//------------------------------------------------------------------------------
// Function Declarations
ERR_t TUI_CMD_bg(const int argc, char *argv[]);

//------------------------------------------------------------------------------
// Function Implementations

typedef struct
{
    const char *opt_str;
    void (*opt_cb)(const FB_COLOUR c);
    FB_COLOUR opt_value;
} TUI_CMD_bg_otps_t;

static TUI_CMD_bg_otps_t TUI_CMD_bg_opts[] = {
    {.opt_str = "black",         .opt_cb = FB_set_BG_ALL, .opt_value = BLACK},
    {.opt_str = "blue",          .opt_cb = FB_set_BG_ALL, .opt_value = BLUE},
    {.opt_str = "green",         .opt_cb = FB_set_BG_ALL, .opt_value = GREEN},
    {.opt_str = "cyan",          .opt_cb = FB_set_BG_ALL, .opt_value = CYAN},
    {.opt_str = "red",           .opt_cb = FB_set_BG_ALL, .opt_value = RED},
    {.opt_str = "magenta",       .opt_cb = FB_set_BG_ALL, .opt_value = MAGENTA},
    {.opt_str = "brown",         .opt_cb = FB_set_BG_ALL, .opt_value = BROWN},
    {.opt_str = "light-grey",    .opt_cb = FB_set_BG_ALL, .opt_value = LIGHT_GREY},
    {.opt_str = "dark-grey",     .opt_cb = FB_set_BG_ALL, .opt_value = DARK_GREY},
    {.opt_str = "light-blue",    .opt_cb = FB_set_BG_ALL, .opt_value = LIGHT_BLUE},
    {.opt_str = "light-green",   .opt_cb = FB_set_BG_ALL, .opt_value = LIGHT_GREEN},
    {.opt_str = "light-cyan",    .opt_cb = FB_set_BG_ALL, .opt_value = LIGHT_CYAN},
    {.opt_str = "light-red",     .opt_cb = FB_set_BG_ALL, .opt_value = LIGHT_RED},
    {.opt_str = "light-magenta", .opt_cb = FB_set_BG_ALL, .opt_value = LIGHT_MAGENTA},
    {.opt_str = "light-brown",   .opt_cb = FB_set_BG_ALL, .opt_value = LIGHT_BROWN},
    {.opt_str = "white",         .opt_cb = FB_set_BG_ALL, .opt_value = WHITE},
};

ERR_t TUI_CMD_bg(const int argc, char *argv[])
{
    static const char *usage_msg = "Usage of bg is bg -color-; use bg -h to get all possible colors";
    if (argc != 2)
    {
        FB_write_string(usage_msg, strlen(usage_msg));
        return ERR_GENERIC;
    }

    int cap = sizeof(TUI_CMD_bg_opts)/sizeof(TUI_CMD_bg_opts[0]);

    for (int i = 0; i < cap; i++)
    {
        if ((strcmp(argv[1], TUI_CMD_bg_opts[i].opt_str))==0)
        {
            TUI_CMD_bg_opts[i].opt_cb(TUI_CMD_bg_opts[i].opt_value);
        }
    }
    return ERR_NONE;
}
