#ifndef INTERACT_WITH_MEMORY_H
#define INTERACT_WITH_MEMORY_H

#include "general_management.h"

// Arguments parser for the set_val function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function set_val if all the arguments are valid
void set_val_command(Memory *p_memory, int args_c, char *args[10]);

// Given a pointer to a certain block, set the value of that block to the value passed along with the pointer
//
// Input : A pointer to the memory, a value to set, and a pointer to where to set the value 
//
// Output : Sets the rightmost slot in the block to the value <value>
uint8_t set_val(Memory *p_memory, uint8_t value, Pointer ptr);

#endif // INTERACT_WITH_MEMORY_H