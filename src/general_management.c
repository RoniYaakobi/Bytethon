#include "general_management.h"

// Moves all items in the block array in the memory starting from an index one index left
//
// Input : A pointer to the memory, the starting index
//
// Output : Moves all items in the memory blocks array such that if index = 0, memory->blocks[0] = memory->blocks[1], memory->blocks[1] = memory->blocks[2]...
// Ends up overwriting the index <index> in blocks array
// Returns a boolean (0 or 1) based on failure/success
uint8_t shift_left(Memory *p_memory, unsigned int index) {
    if (index >=p_memory->memory_size) return 0; // If the index is out of bounds shift failed

    size_t num_blocks_to_move = (p_memory->amount_of_blocks - index - 1) ; // Calculate the size that needs to be moved in sizeof(Block)

    Block *p_curr = &p_memory->p_blocks[index+1];
    while (p_curr != NULL) { // Preupdate the pointers in the linked list
        Block *next_block = p_curr->p_next;  // Store next block before modifying so you know which block to next update
        
        p_curr->p_prev = p_curr->p_prev ? p_curr->p_prev - 1 : NULL; // Move the pointer by 1
        p_curr->p_next = p_curr->p_next ? p_curr->p_next - 1 : NULL; // Move the pointer by 1

        p_curr = next_block;  // Move to the next block safely
    }

    memmove(&p_memory->p_blocks[index], &p_memory->p_blocks[index + 1], num_blocks_to_move * sizeof(Block)); // Move num_blocks_to_move (bytes) from the index + 1 to the index.

    return 1;
}

// Moves all items in the block array in the memory starting from an index one index right
//
// Input : A pointer to the memory, the starting index
//
// Output : Moves all items in the memory blocks array such that if index = 0, memory->blocks[1] = memory->blocks[0], memory->blocks[2] = memory->blocks[1]...
// Ends up overwriting the last index of the blocks array
// Returns a boolean (0 or 1) based on failure/success
uint8_t shift_right(Memory *p_memory, unsigned int index) {
    if (index >=p_memory->memory_size) return 0; // If the index is out of bounds shift failed

    size_t num_blocks_to_move = (p_memory->amount_of_blocks - index); // Calculate the size that needs to be moved in sizeof(Block)
    if (p_memory->amount_of_blocks >= p_memory->memory_size) { // If the next block's index outside the array
        return 0;  // Prevent moving blocks past the array size
    }
    // Shift memory from the relevant index to the right
    Block *p_curr = &p_memory->p_blocks[index];
    while (p_curr != NULL) { // Premove the pointers
        Block *next_block = p_curr->p_next;  // Store next block before modifying
        
        p_curr->p_prev = p_curr->p_prev ? p_curr->p_prev + 1 : NULL; // Move the pointer by 1 to point at the next 
        p_curr->p_next = p_curr->p_next ? p_curr->p_next + 1 : NULL; // Move the pointer by 1

        p_curr = next_block;  // Move to the next block safely
    }
    memmove(&p_memory->p_blocks[index + 1], &p_memory->p_blocks[index], num_blocks_to_move * sizeof(Block)); // Move the amount of blocks from index+1 to index

    return 1;
}

// Finds a block in the blocks array based on a corresponding index in the bytes array
//
// Input : A pointer to the memory, the lookup index, and a block double pointer to store a pointer for the block 
//(**block is a pointer to a *block pointer which lets you acess the block the function found)
//
// Output : assigns the pointer that the double pointer block holds, to a pointer to the block found, 
// and returns the index of the block in the blocks array as well (or -1 if not found)
int find_block(Memory *p_memory, unsigned int index, Block **pp_block) { 
    size_t sum = 0;

    for (int i = 0; i < p_memory->amount_of_blocks;i++){ // Loop over all the blocks in the array

        sum += p_memory->p_blocks[i].size;

        if (sum >= index) { // If the current block is the block that corresponds to the index given in bytes array

            if (p_memory->p_blocks[i].free) { // The block here is free, so there is no block (effectively, logic treats this as padding) at this index
                return -1; 
            } 
            *pp_block = &p_memory->p_blocks[i]; // Set the block pointer to point at the block found
            return i; // Return index where this block was found in block array in case debugging is needed.
        }
    }
    return -1; // No block found
}