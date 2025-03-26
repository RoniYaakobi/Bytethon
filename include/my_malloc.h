#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stdio.h>
#include <math.h>
#include <string.h>  // For memmove
#include <stdint.h> // unsigned int with 8 bits array of memory in the bytes
#include "general_management.h"
#include "utils.h"

// Arguments parser for the my_malloc function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function my_malloc if all the arguments are valid
void my_malloc_command(Memory *p_memory, int args_c, char *args[10]); 

// Allocates memory to a block in the memory based on the pointer and size input, 
// does the search, then lets allocate handle the allocation itself
//
// Input : A pointer to the memory, a size of allocation, and a pointer to a pointer struct
//
// Output : Tries to allocate <size> bytes in the memory pointer passed, searches for an index to start the allocation based on best-fit algorithm.
// Returns a boolean (0 or 1) based on success/failure
uint8_t my_malloc(Memory *p_memory, size_t size, Pointer *p_ptr);

// Does the actual allocation logic, gets a size and index for where to allocate, 
// and a pointer to point to the allocation along with a pointer to a memory struct.
//
// Input : A pointer to the memory, a size of allocation, an index for where to start the allocation, and a pointer to a pointer struct
//
// Output : Tries to allocate <size> bytes with pointer struct <*ptr> in the <*memory> pointer where the <index> is the start in the bytes array, 
// creating a new blocks in the memory blocks array, and returns a boolean (0 or 1) if it succeded or not.
uint8_t allocate(Memory *p_memory, size_t size, unsigned int index, Pointer *p_ptr);

// Helper function for allocate: if there is a free block of size s at index <index> in the blocks array, 
// and input <size> < s, then it splits the block into a free block of size s - <size> and allocated block of size <size>
//
// Input : A pointer to the memory, a size of allocation, an index for where to split the block in block array
//
// Output : Split the block that is targeted by <index> based on <size> to a free block and an allocated block
uint8_t split_block(Memory *p_memory, size_t size, unsigned int index);



#endif // MY_MALLOC_H