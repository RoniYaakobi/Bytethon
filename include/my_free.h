#ifndef MY_FREE_H
#define MY_FREE_H

#include "general_management.h"

// Arguments parser for the my_free function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function my_free if all the arguments are valid
void my_free_command(Memory *p_memory, int args_c, char *args[10]);

// Free a block that a pointer <**ptr> is pointing at.
//
// Input : A pointer to the memory and a pointer to a pointer to a pointer struct (sorry lol)
//
// Output : Frees the memory block that **ptr is pointing at and then frees the actual memory that *ptr 
// (the pointer to the pointer struct that **ptr points at that pointer) uses (as creating pointer structs uses stdlib calloc)
// Returns a boolean (0 or 1) for if it succeeded or not
uint8_t my_free(Memory *p_memory,Pointer **pp_ptr);

// Merges block at index <index> with block at index <index>+1, an attempt to reduce fragmentation in a simple manner
//
// Input : A pointer to the memory and an index to where to merge
//
// Output : Block at index <index> eats block at index <index> + 1
void merge_block_right(Memory *p_memory, unsigned int index); // Merges blocks (left side eats right side), for memory reducing fragmentation

#endif // MY_FREE_H