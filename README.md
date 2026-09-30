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

## Interupt Handlers