# OS Worksheet 2

## Prerequisites

| Tool  | Purpose                   |
|-------|---------------------------|
| Make  | Build system              |
| QEMU  | Emulation                 |
| GCC   | C compilation and linking |
| NASM  | Assembly compilation      |

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

## Frame Buffer Api

### Requirements
> [!NOTE]
> Due to C not having namespacing all APIs/Drivers will have all functions prefixed with their acronym e.g `framebuffer` -> `FB_` as a prefix 
#### Structs / Definitions
| Name | Brief | Implemented |
|------|-------|-------------|
|`FB_Colour`|The colour value corresponding with the documentented value|Yes|
|`FrameBuffer`| Holds the packed structure of what the FrameBuffer actually holds|Yes|


#### Functions
Here is a list of all functions I want my framebuffer API to have eventually and the rough function signature that i want them to have. 

| Name | Brief | Return | Parameters | Implemented |
|------|-------|--------|------------|-------------|
| `ROW_COL_TO_INDEX` (static) | Converts a column/row pair into a linear index into the frame buffer, wrapping out-of-range values | `int` (cell index) | `const unsigned int col`, `const unsigned int row` | Yes |
| `set_FG_ALL` | Sets the foreground colour of every cell, keeping the background and character | `void` | `const FB_COLOUR c` | Yes |
| `set_FG_CELL` | Sets the foreground colour of a single cell, keeping the background and character | `void` | `const FB_COLOUR c`, `const unsigned int col`, `const unsigned int row` | Yes |
| `set_BG_ALL` | Sets the background colour of every cell, keeping the foreground and character | `void` | `const FB_COLOUR c` | Yes |
| `set_BG_CELL` | Sets the background colour of a single cell, keeping the foreground and character | `void` | `const FB_COLOUR c`, `const unsigned int col`, `const unsigned int row` | Yes |
| `set_CHAR_ALL` | Sets the character of every cell, keeping the colours | `void` | `const char c` | Yes |
| `set_CHAR_CELL` | Sets the character of a single cell, keeping the colours | `void` | `const char c`, `const unsigned int col`, `const unsigned int row` | Yes |
| `set_FULL_CELL` | Sets the character, foreground and background of a single cell in one write | `void` | `const char c`, `const FB_COLOUR FG`, `const FB_COLOUR BG`, `const unsigned int col`, `const unsigned int row` | Yes |
| `write_string`| Writes the inputed string at the current cursor position | `void` | `const char *const s` | No |
| `clear`| Clears the whole framebuffer to be empty and reset cursor position to 0,0 | `void` | `None` | No |
| `mov_cursor`| Writes current cursor position | `void` | `const unsigned int col`, `const unsigned int row` | No |


