#ifndef GENERAL_MANAGEMENT_H
#define GENERAL_MANAGEMENT_H

// General management module that imports all the modules and is imported by all the modules
// Ineffcient for the long term, but started using before I knew the issues with this, might have to change import architecture in the future

#include <stdio.h>
#include <math.h>
#include <string.h>  // For memmove
#include <stdint.h> // unsigned int with 8 bits array of memory in the bytes
#include "memory_structs.h"
#include "utils.h"
#include "visualize.h"
#include "my_malloc.h"
#include "my_free.h"
#include "interact_with_memory.h"
#include "cli.h"
#include "pointer_management.h"

// Moves all items in the block array in the memory starting from an index one index left
//
// Input : A pointer to the memory, the starting index
//
// Output : Moves all items in the memory blocks array such that if index = 0, memory->blocks[0] = memory->blocks[1], memory->blocks[1] = memory->blocks[2]...
// Ends up overwriting the index <index> in blocks array
// Returns a boolean (0 or 1) based on failure/success
uint8_t shift_left(Memory *p_memory, unsigned int index);


// Moves all items in the block array in the memory starting from an index one index right
//
// Input : A pointer to the memory, the starting index
//
// Output : Moves all items in the memory blocks array such that if index = 0, memory->blocks[1] = memory->blocks[0], memory->blocks[2] = memory->blocks[1]...
// Ends up overwriting the last index of the blocks array
// Returns a boolean (0 or 1) based on failure/success
uint8_t shift_right(Memory *p_memory, unsigned int index);

// Finds a block in the blocks array based on a corresponding index in the bytes array
//
// Input : A pointer to the memory, the lookup index, and a block double pointer to store a pointer for the block 
//(**block is a pointer to a *block pointer which lets you acess the block the function found)
//
// Output : assigns the pointer that the double pointer block holds, to a pointer to the block found, 
// and returns the index of the block in the blocks array as well (or -1 if not found)
int find_block(Memory *p_memory, unsigned int index, Block **pp_block);

#endif // GENERAL_MANAGEMENT_H