/**
 * @file process.h
 * @author Ben Marples
 * @brief Holds Paging interface
 */

#ifndef PROCESS_H
#define PROCESS_H

#include "type.h"

//------------------------------------------------------------------------------
// Definitions

typedef struct
{
    usize pd_phys;          // physical address to the page descriptor table
    usize code_frame_count; // count of code frames
    usize *code_frames;     // phys address of code frames
    usize stack_frame;      // phys address
    usize entry;
    usize user_esp;
} process_t;

//------------------------------------------------------------------------------
// Function Declarations

int process_alloc_frame(process_t *proc, usize bin_size);
void process_load_binary(process_t *proc, usize mod_start, usize mod_end);
int process_map(process_t *proc);
void asm_enter_user_mode(u32int pd_phys, u32int entry, u32int user_esp);
process_t *process_create(usize mod_start, usize mod_end);
void process_enter(const process_t *proc);

#endif