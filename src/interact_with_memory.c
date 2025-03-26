#include "interact_with_memory.h"

// Arguments parser for the set_val function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function set_val if all the arguments are valid
void set_val_command(Memory *p_memory, int args_c, char *args[10]) {
    if (args_c != 2) {
        print_error("Wrong amount of arguments for function set_val. Expected 2 arguments but received %d arguments",args_c);
        return;
    }

    char *endptr;
    long value = strtol(args[0],&endptr,10);
    // In an array of uint8_t, every slot can be 0-255 TODO implement the ability to spread the data on a certain amount of bytes so user can enter bigger numbers
     if(*endptr != '\0' || value < 0 || value > 255) { 
        print_error("First argument in set_val must be an integer (no decimal dot) between 0-255 <value>, not %d.", value);
        return;
    }
    
    HashMap *p_pointers = p_memory->p_pointers; // Readability
    Pointer *p_ptr = (Pointer*)hashmap_get(p_pointers, args[1]);
    if (p_ptr == NULL) {
        print_error("Could not locate pointer %s. Please create a pointer with the command new_pointer.",args[1]);
        return;
    }
    uint8_t success = set_val(p_memory, (uint8_t)value, *p_ptr);
    if (success) {
        print_success("Set %ld to the value pointer %s is pointing at successfully.",value,args[1]);
    }
}

// Given a pointer to a certain block, set the value of that block to the value passed along with the pointer
//
// Input : A pointer to the memory, a value to set, and a pointer to where to set the value 
//
// Output : Sets the rightmost slot in the block to the value <value>
uint8_t set_val(Memory *memory, uint8_t value,Pointer ptr) {
    Block block;
    Block *ptr2block = &block; // find_block function needs a pointer to a pointer to a block struct

    int index_of_block = find_block(memory,ptr.start_index, &ptr2block);
    
    if (index_of_block >= 0) {
        block = memory->p_blocks[index_of_block]; // Double check to make sure you have the right block
    } else {
        print_error("Could not find allocated block at address %d", ptr.start_index); // If the index was not found print an error message
        return 0;
    }
    
    ptr2block->uninitialized = 0; // Mark block as initialized

    for (int i = ptr.start_index; i < ptr.start_index + block.size - 1;i++) {
        memory->p_bytes[i] = 0;
    }

    memory->p_bytes[ptr.start_index + block.size - 1] = value; // Set the end of the allocated block to the value
    return 1;
}