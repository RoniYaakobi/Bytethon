#ifndef VISUALIZE_H
#define VISUALIZE_H

#include "general_management.h"

// These are enums for what stage in the memory a block is (mostly for the visualize_bytes function)
typedef enum { 
    FREE, 
    UNINITIALIZED, 
    USED 
} BlockState; 

// Arguments parser for the visualize_bytes function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function visualize_bytes if all the arguments are valid
void visualize_bytes_command(Memory *p_memory, int args_c, char *args[10]);

// Shows the bytes array items as their hex format. Recommendation : Do not use over 1000 slots of memory if you want to use this function proprerly
//
// Input : A pointer to the memory
//
// Output : Prints all the items in the bytes array in the memory, based on their corresponding block as free, uninitialized, or with a value.
void visualize_bytes(Memory *p_mem);

// Arguments parser for the visualize_blocks function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function visualize_blocks if all the arguments are valid
void visualize_blocks_command(Memory *p_memory, int args_c, char *args[10]);

// Show a list of the blocks and their metadata
//
// Input : A pointer to the memory
//
// Output : Prints all the metadata of all the blocks in the blocks array in the memory passed.
void visualize_blocks(Memory *p_mem);

// Print the legend for the visualize_blocks function, for what each symbol means.
void print_bytes_visualization_legend();
#endif // VISUALIZE_H