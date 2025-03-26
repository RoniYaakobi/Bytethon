#ifndef POINTER_MANAGEMENT_H
#define POINTER_MANAGEMENT_H

#include "general_management.h"

// Arguments parser for the new_pointer function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function new_pointer if all the arguments are valid
void new_pointer_command(Memory *p_memory, int args_c, char *args[10]);

// Creates a new pointer struct and adds it into the pointer struct hashmap in the memory struct
//
// Input : A pointer to a pointer struct hashmap (Usually inside a memory struct) and a name for the new pointer
//
// Output : Initializes a new pointer struct into the pointer struct map
void new_pointer(HashMap *p_pointer_map, char *name);

#endif // POINTER_MANAGEMENT_H