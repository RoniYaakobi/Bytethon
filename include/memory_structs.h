#ifndef MEMORY_STRUCTS_H
#define MEMORY_STRUCTS_H

#include <stdint.h>
#include "general_management.h"
#include "utils.h"

// A pointer struct, save the address of the block (index in byte array) it points at in memory
typedef struct {
    int start_index;
} Pointer;

// Block struct, made to make sure that you do not accidentally double allocated,
// and in general used to track memory on the management level
struct Block {
    size_t size;
    size_t start_index;
    struct Block* p_next;
    struct Block* p_prev;
    uint8_t free;
    uint8_t uninitialized;
};

// Block struct, made to make sure that you do not accidentally double allocated,
// and in general used to track memory on the management level
typedef struct Block Block; 

// No clue why I need to typedef like this, gcc gives me warnings otherwise

// Memory struct, used to store the raw memory, the blocks, the length of both of these arrays
// a pointer hashmap so that users can gives their own names to pointers and a flag if it was generate via malloc()
// And then needs to be freed on exiting the program or if it was made in main's stack, and then there is no need.
typedef struct {
    uint8_t *p_bytes;  // 1 byte per slot in the array
    Block *p_blocks;  // Metadata for all the blocks in the memory
    size_t amount_of_blocks;
    size_t memory_size;
    HashMap *p_pointers;
    uint8_t on_heap;
} Memory;


#endif // MEMORY_STRUCTS_H 