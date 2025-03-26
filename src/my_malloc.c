#include "my_malloc.h"

// Arguments parser for the my_malloc function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function my_malloc if all the arguments are valid
void my_malloc_command(Memory *p_memory, int args_c, char *args[10]) {
    if (args_c != 2) {
        print_error("Wrong amount of arguments for function malloc. Expected 2 arguments but received %d arguments",args_c);
        return;
    }
    
    char *endptr;
    long size = strtol(args[0],&endptr,10); // Convert the string argument that user gave to a long based on decimal notation
     if(*endptr != '\0' || size < 0) { // Invalid input
        print_error("First argument in malloc must be a positive integer (no decimal point) non zero number <size>");
        return;
    }

    HashMap *p_pointers = p_memory->p_pointers; // Readability
    Pointer *p_ptr = (Pointer*)hashmap_get(p_pointers, args[1]);
    if (p_ptr == NULL) { // Pointer does not exsist
        print_error("Could not locate pointer %s. Please create a pointer with the command new_pointer.",args[1]);
        return;
    }
    uint8_t success = my_malloc(p_memory, (size_t)size ,p_ptr);
    if (success) {
        print_success("Allocated %ld bytes for pointer %s successfully.", size,args[1]);
    }
}

// Allocates memory to a block in the memory based on the pointer and size input, 
// does the search, then lets allocate handle the allocation itself
//
// Input : A pointer to the memory, a size of allocation, and a pointer to a pointer struct
//
// Output : Tries to allocate <size> bytes in the memory pointer passed, searches for an index to start the allocation based on best-fit algorithm.
// Returns a boolean (0 or 1) based on success/failure
uint8_t my_malloc(Memory *p_memory, size_t size, Pointer *p_ptr) {
    
    if (p_memory->amount_of_blocks > p_memory->memory_size){
        print_error("Could not allocate memory: all slots are taken."); 
        return 0;
    }

    Block *p_blocks = p_memory->p_blocks;
    int index = -1; // Initialize index to -1 so if the index is not found, the index is invalid
    unsigned int best_bytes = p_memory->memory_size + 1; // Make sure the initial value of best bytes is worst than any other vaild option in the memory
    uint8_t success = 0;
    for (int i = 0; i < p_memory->amount_of_blocks; i++) { 
        if (p_blocks[i].free == 0){continue;} // Avoid double allocation
        if (p_blocks[i].size == size){ // If the size of the block is exactly the size requested, it is the best option
            success = allocate(p_memory, size, i, p_ptr);
            return 1;
        }
        if (p_blocks[i].size > size && p_blocks[i].size < best_bytes) { // If the current block is better fit use it instead of previous best
            index = i;
            best_bytes = p_blocks[i].size;
        }
    }
    if (index == -1) { // No index found
        return 0;
    }

    success = allocate(p_memory, size,(unsigned int)index, p_ptr);
    if (!success) {
        print_error("Could not allocate enough memory for size %zu.", size);
        return 0;
    }
    return 1;
}

// Does the actual allocation logic, gets a size and index for where to allocate, 
// and a pointer to point to the allocation along with a pointer to a memory struct.
//
// Input : A pointer to the memory, a size of allocation, an index for where to start the allocation, and a pointer to a pointer struct
//
// Output : Tries to allocate <size> bytes with pointer struct <*ptr> in the <*memory> pointer where the <index> is the start in the bytes array, 
// creating a new blocks in the memory blocks array, and returns a boolean (0 or 1) if it succeded or not.
uint8_t allocate(Memory *p_memory, size_t size, unsigned int index, Pointer *p_ptr) {
    int byte_index = 0; // Stores the index of the pointer in the bytes array

    for (int i = 0; i < index; i++) { // Get the index where the allocated memory will start in memory the bytes array
        byte_index += p_memory->p_blocks[i].size; // Sum the size of the blocks up to the relevant block
    }
    if (size == p_memory->p_blocks[index].size) { // If the block and the size of allocation are the same then just toggle free off for the blocki
        p_memory->p_blocks[index].free = 0;
        p_ptr->start_index = (int)byte_index;
        return 1;
    }
    uint8_t success = split_block(p_memory, size, index); // Try to split the block in memory that is needed for the allocation
    if (success) {
        p_ptr->start_index = (int)byte_index ;
        return 1;
    }
    return 0;
}

// Helper function for allocate: if there is a free block of size s at index <index> in the blocks array, 
// and input <size> < s, then it splits the block into a free block of size s - <size> and allocated block of size <size>
//
// Input : A pointer to the memory, a size of allocation, an index for where to split the block in block array
//
// Output : Split the block that is targeted by <index> based on <size> to a free block and an allocated block
uint8_t split_block(Memory *p_memory, size_t size, unsigned int index) {
    // Makes sure that you can actually split the block.
    if (index >= p_memory->amount_of_blocks) {
        print_error("Could not split block at index %u: out of bounds index.", index); 
        return 0;
    }

    Block *p_block = &p_memory->p_blocks[index]; // Increase readability
    
    // Try to shift all the memory one slot to the right to make place for the new block
    uint8_t success = shift_right(p_memory,index + 1); 
    if(!success) {
        // TODO add a logger which lets you see all the errors including this one.
        return 0;
    }
    Block new_free; // Create a new free block that is the leftover of the previous block
    
    // Chain the new_free struct to list
    new_free.p_next = p_block->p_next;
    new_free.p_prev = p_block;

    // Move remaining memory to the new block
    new_free.size = p_block->size - size;
    p_block->size = size; 

    // Set the index of the start of the free block to be directly after the end of the allocated block
    new_free.start_index = p_block->start_index + size;

    // Mark the free block as free and the allocated block as taken
    new_free.free = 1;
    p_block->free = 0;

    // All blocks start unintialized
    new_free.uninitialized = 1;
    p_block->uninitialized = 1;

    // Put the new free block in the array
    p_memory->p_blocks[index + 1] = new_free;

    // Finish chaining by making the block before and after the new free block recognize the block
    p_block->p_next = &p_memory->p_blocks[index + 1];
    
    Block *p_next_block_after_new_free = p_memory->p_blocks[index + 1].p_next;
    if (p_next_block_after_new_free) { // If there is a block after the new_free block
        p_next_block_after_new_free->p_prev = &p_memory->p_blocks[index + 1]; // Chain it too
    }
    // Amount of blocks cannot exceed the size of the memory bytes array
    p_memory->amount_of_blocks = (p_memory->amount_of_blocks + 1) <p_memory->memory_size ? p_memory->amount_of_blocks + 1 :p_memory->memory_size; 
    return 1;
}
