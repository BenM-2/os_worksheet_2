# OS Worksheet 2

## Prerequisites

| Tool  | Purpose                   |
|-------|---------------------------|
| Make  | Build system              |
| QEMU  | Emulation                 |
| GCC   | C compilation and linking |
| NASM  | Assembly compilation      |

## Todo
|Task| Desc | Time | Priority |
|-|-|-|-|
|Finish Frame buffer api| Complete full api spec as listed under [FB API](#functions)|2 Days|5|
|Interupts | Implement Interupt Handler under [TBD](#interupt-handlers) | TBD | 4 |
|Keyboard Handler| Implement full keyboard support | TBD | 3 |
|TUI| Implement a Set of instructions for tinyOS | TBD | 2 |

## Task 1 
### Overview
To complete task 1 `0xCAFEBABE` needs to be placed into the `eax` register. To do this we use GRUB as a bootloader 

### Run
To run the program and be able to seee that 0xCAFEBABE is in the register use 
``` bash
make qemu_run
```
to run the os silently just run 
``` bash
make qemu_run_quiet
```

## Task 2+

## Running 
To run the os from task 2 onwards first run 
``` Bash
make clean && make qemu_run_curses 
```
then in a separate terminal run 
``` Bash
make telnet 
```
This is best done using either 2 diferent tabs or via TMUX to allow for ease of viewing both terminals.

## Frame Buffer Api
### Design
The Frame buffer api does the the groundwork for writing to the screen but does not handle any functions in specific e.g `clear_screen()` would have be implemented in the program / driver that wants to use it.

To use the api just include the header
``` c
#include "framebuffer.h"
```
If the namespace isnt poluted you can use:
``` c
#define NO_NAMESPACE
#include "framebuffer.h"
```
to remove the `FB_` prefix from all the functions

### Structs / Definitions
| Name | Brief | Implemented |
|------|-------|-------------|
|`FB_Colour`|The colour value corresponding with the documentented value|Yes|
|`FrameBuffer`| Holds the packed structure of what the FrameBuffer actually holds|Yes|


### Framebuffer API endpoints

| Category | Function | Parameters | Returns | Description |
|---|---|---|---|---|
| Cursor | `FB_move_cursor` | `unsigned short pos` | `void` | Moves the cursor to a linear cell index. |
| Cursor | `FB_move_cursor_row_col` | `unsigned int col`, `unsigned int row` | `void` | Moves the cursor to a column/row position (wraps via modulo). |
| Cursor | `FB_get_cursor` | `NONE` | `unsigned short` | Returns the current cursor index. |
| Cursor | `FB_increment_cursor` | `NONE` | `void` | Moves the cursor forward by one cell. |
| Cursor | `FB_decrement_cursor` | `NONE` | `void` | Moves the cursor back by one cell. |
| Cursor | `FB_write_string` | `const char *buff`, `unsigned int length` | `void` | Writes `length` characters starting at the cursor, advancing it after each one. |
| FG colour | `FB_set_FG_ALL` | `FB_COLOUR c` | `void` | Sets the foreground colour of every cell. |
| FG colour | `FB_set_FG_CELL` | `FB_COLOUR c`, `unsigned int col`, `unsigned int row` | `void` | Sets the foreground colour of one cell by column/row. |
| FG colour | `FB_set_FG_CELL_INDEX` | `FB_COLOUR c`, `unsigned int index` | `void` | Sets the foreground colour of one cell by linear index. |
| BG colour | `FB_set_BG_ALL` | `FB_COLOUR c` | `void` | Sets the background colour of every cell. |
| BG colour | `FB_set_BG_CELL` | `FB_COLOUR c`, `unsigned int col`, `unsigned int row` | `void` | Sets the background colour of one cell by column/row. |
| BG colour | `FB_set_BG_CELL_INDEX` | `FB_COLOUR c`, `unsigned int index` | `void` | Sets the background colour of one cell by linear index. |
| Character | `FB_set_CHAR_ALL` | `char c` | `void` | Sets the character of every cell. |
| Character | `FB_set_CHAR_CELL` | `char c`, `unsigned int col`, `unsigned int row` | `void` | Sets the character of one cell by column/row. |
| Character | `FB_set_CHAR_CELL_INDEX` | `char c`, `unsigned int index` | `void` | Sets the character of one cell by linear index. |

### Internal (not part of the public API)

| Function | Parameters | Returns | Description |
|---|---|---|---|
| `FB_ROW_COL_TO_INDEX` | `unsigned int col`, `unsigned int row` | `int` | `static` helper converting col/row to a linear index, with modulo wrapping on both axes. |


### Cursor
There are 2 cursor implementations one using the vga buffer and the other using a static struct to hold the place of the cursor by default vga cursor will be enabled but to enable the static struct cursor above the include for the framebuffer in main 

To use my cursor implementation run 
``` bash
make clean && make EXPERIMENTAL=1 qemu_run_curses
```
otherwise use 
``` bash
make clean && make qemu_run_curses
```

## Interrupt Handlers

Interrupts are set up in `interrupts.c`. The file builds the Interrupt Descriptor Table (IDT), remaps the PICs, and forwards each interrupt to a callback you register. The callback type lives in `interrupt_callbacks.h`.

### How it works

1. `interrupts_install_idt()` fills in the IDT entries, loads the table with `load_idt()`, and remaps the PICs.
2. All IRQs are masked, then the keyboard (IRQ1) is unmasked.
3. When an interrupt fires, the assembly stub calls `interrupt_handler()`, which switches on the interrupt number and calls the matching callback.

The table has 256 descriptors. Each one is a 32-bit interrupt gate (present, DPL 0) using the code segment selector `0x08`.

### Supported interrupts

| Vector | Source   | Callback                      |
|--------|----------|-------------------------------|
| 33     | Keyboard | `callbacks.keyboard_event()`  |

Any other vector is ignored by the default case.

### Usage

Register your callbacks, then install the IDT:

```c
#include "interrupts.h"
#include "interrupt_callbacks.h"

static void on_keyboard(u32int interrupt)
{
    // read the scancode and handle it
}

void kernel_setup(void)
{
    Interrupt_callbacks cbs = {
        .keyboard_event = on_keyboard,
    };

    interrupts_cb_config(&cbs);
    interrupts_install_idt();
}
```

`interrupts_cb_config()` copies the struct, so it doesn't need to outlive the call.

### Adding a new interrupt

1. Add a vector define in `interrupts.c` (e.g. `#define INTERRUPTS_TIMER 32`).
2. Add a callback field to `Interrupt_callbacks` in `interrupt_callbacks.h`.
3. Write an assembly stub for the vector (like `interrupt_handler_33`) and register it with `interrupts_init_descriptor()` in `interrupts_install_idt()`.
4. Add a `case` to the switch in `interrupt_handler()`.
5. Unmask the IRQ on the PIC.



# TUI Apps: 
## Implemented commands

| Command | Description                                  | Usage              | Source                          |
|---------|----------------------------------------------|--------------------|---------------------------------|
| [`bg`](#tui-apps-bg-command) | Set the framebuffer background colour | `bg <color>` | [`tui_apps.c`](./tui_apps.c) |

Each command is implemented as a function named `TUI_CMD_<name>` with the signature:

```c
ERR_t TUI_CMD_<name>(const int argc, char *argv[]);
```

`argv[0]` is the command name, and the function returns `ERR_NONE` on success or an error code on failure.

### Adding a command to this list

1. Implement `TUI_CMD_<name>` in `tui_apps.c` and declare it in `tui_apps.h`.
2. Add a row to the table above.
3. Add a section below documenting its usage and return values (see the `bg` section for the format).


## `bg` command

Part of the kernel's text UI (`tui_apps.c`). Implements the `bg` shell command, which sets the background colour of the whole framebuffer.

### Usage

```
bg <color>
```

| Argument  | Description                                |
|-----------|--------------------------------------------|
| `<color>` | One of the colour names listed below       |

Example:

```
bg blue
bg light-grey
```

If the wrong number of arguments is given, the command prints a usage message and returns `ERR_GENERIC`.

### Supported colours

| Standard  | Light / bright  |
|-----------|-----------------|
| `black`   | `light-grey`    |
| `blue`    | `light-blue`    |
| `green`   | `light-green`   |
| `cyan`    | `light-cyan`    |
| `red`     | `light-red`     |
| `magenta` | `light-magenta` |
| `brown`   | `light-brown`   |
| `dark-grey` | `white`       |

### How it works

Colours are defined in a lookup table, `TUI_CMD_bg_opts[]`, where each entry holds:

- `opt_str`: the colour name typed by the user
- `opt_cb`: the framebuffer function to call (currently `FB_set_BG_ALL` for every entry)
- `opt_value`: the `FB_COLOUR` value passed to that function

`TUI_CMD_bg()` checks `argc`, then walks the table comparing `argv[1]` against each `opt_str` with `strcmp`. On a match it calls the entry's callback with its colour value.

### Adding a colour

New Colours Cannot be added due to the limit of the VGA text buffer

### Return values

| Value         | Meaning                          |
|---------------|----------------------------------|
| `ERR_NONE`    | Command completed                |
| `ERR_GENERIC` | Wrong number of arguments        |

### Dependencies

- `tui_apps.h`: command and error type declarations
- `framebuffer.h`: `FB_set_BG_ALL`, `FB_write_string`, `FB_COLOUR`
- `kstring.h`: `strcmp`, `strlen`