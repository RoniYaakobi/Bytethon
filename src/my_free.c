#include "my_free.h"

// Arguments parser for the my_free function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function my_free if all the arguments are valid
void my_free_command(Memory *p_memory, int args_c, char *args[10]) {
    if (args_c != 1) {
        print_error("Wrong amount of arguments for function free. Expected 1 argument but received %d arguments",args_c);
        return;
    }
    HashMap *p_pointers = p_memory->p_pointers;  // Readability
    Pointer *p_ptr = (Pointer*)hashmap_get(p_pointers, args[0]);
    if (p_ptr == NULL) {
        print_error("Could not locate pointer %s. Please create a pointer with the command new_pointer.",args[0]);
        return;
    }
    uint8_t success = my_free(p_memory, &p_ptr);
    if (success) {
        hashmap_remove(p_memory->p_pointers, args[0],1);
        print_success("Freed pointer %s successfully.", args[0]);
    }
}

// Free a block that a pointer <**ptr> is pointing at.
//
// Input : A pointer to the memory and a pointer to a pointer to a pointer struct (sorry lol)
//
// Output : Frees the memory block that **ptr is pointing at and then frees the actual memory that *ptr 
// (the pointer to the pointer struct that **ptr points at that pointer) uses (as creating pointer structs uses stdlib calloc)
// Returns a boolean (0 or 1) for if it succeeded or not
uint8_t my_free(Memory *p_memory,Pointer **pp_ptr) {
    if (!pp_ptr || !*pp_ptr) {
        print_error("Invalid pointer to pointer recieved. "); 
        return 0;
    }

    Block *p_blockptr; // Pointer to allocated memory for the new block
    Pointer *p_ptr = *pp_ptr; // Address of of pointer struct in **pp_ptr 

    // Stores the block's adress in the pointer and also returns index in block array, unless it wasn't found and then it returns -1
    int index = find_block(p_memory, p_ptr->start_index, &p_blockptr); 
    if (index == -1){
        print_error("Could not locate block at index %d (in the byte array)",
        p_ptr->start_index); return 0;
    }
    
    p_blockptr->free = 1;
    p_blockptr->uninitialized = 1;
    
    // Store next and prev in separate variables before accessing their members (for merging)
    Block *p_next_block = p_blockptr->p_next;
    Block *p_prev_block = p_blockptr->p_prev;

    if (p_next_block && p_next_block->free) { // If then next block exists and is free merge it with the current block
        merge_block_right(p_memory, index);
    }
    if (p_prev_block && p_prev_block->free) { // If then next block exists and is free merge it with the current block
        merge_block_right(p_memory, index - 1);
    }

    p_memory->amount_of_blocks--;

    return 1;
}



// Merges block at index <index> with block at index <index>+1, an attempt to reduce fragmentation in a simple manner
//
// Input : A pointer to the memory and an index to where to merge
//
// Output : Block at index <index> eats block at index <index> + 1
void merge_block_right(Memory *p_memory, unsigned int index){
    Block *p_block = &p_memory->p_blocks[index]; // Get the current block
    Block *p_right_block = p_block->p_next; // Get the block it needs to merge with
    if (!p_right_block) { // If there is no block to merge with, do nothing
        return;
    }
    
    Block *second_right_block = p_right_block->p_next;
    if (second_right_block) { // If there is no successor to right_block, the next line would cause issues 
        second_right_block->p_prev = p_block;
    }
    
    p_block->p_next = p_right_block->p_next; // Rechain the list
    p_block->size += p_right_block->size; // Make the block "eat" the block it merged with
    p_block->free = 1; // Make sure the block is set to free (non free block should not merge)
    p_block->uninitialized = 1; // Free blocks are uninitialized
    p_memory->amount_of_blocks--;

    shift_left(p_memory, index + 1); // Pop the block that was merged with the current index.
}