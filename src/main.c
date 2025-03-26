#include "general_management.h"

#define MAX_SIZE_STACK ((1 << 19) / sizeof(Block))  // 2^19 = 512 KB
#define MAX_SIZE_HEAP ((1 << 29) / sizeof(Block)) // 2^29 = 512 MB
uint8_t get_memory_type(){
    printlnf("Please choose where you want to allocate your memory for the simulation: \n");
    printlnf(" 1. Stack: Smaller, but faster \n");
    printlnf(" 2. Heap, Larger but slower. \n");
    printlnf("Heap option also uses c built-in malloc and free, so for seeing the project fully \nwithout builtin malloc/free (excluding dynamic hashes for the cli commands) use stack.");

    // The reply should not be more than 10 charcters
    char option[10];
    uint8_t is_heap_allocated;
    do {
        fgets(option,sizeof(option),stdin);
        int valid = sscanf(option,"%hhu", &is_heap_allocated); 

        if (valid && (is_heap_allocated == 1 || is_heap_allocated == 2)){
            is_heap_allocated--; // (convert into boolean: 1 (Stack option) -> 0 (false)...)
            return is_heap_allocated;
        }

        printlnf("\033[4;38;5;52m[ERROR]\033[24m\033[38;5;196m Please enter an option, 1 or 2.\033[0m"); // Red error prefix + red error message

    } while (1);
}

size_t get_size_of_memory(uint8_t is_heap_allocated) {
    size_t size;
    char input[50]; // Buffer for input

    do {
        printlnf("Please enter a number from 1 - %zu for the size of your memory in the simulation: ", is_heap_allocated ?  MAX_SIZE_HEAP : MAX_SIZE_STACK);
        fgets(input, sizeof(input), stdin);
        
        int success = sscanf(input, "%zu", &size); // Read input as size_t
        
        // If sscanf fails or if input is out of range, show the input and explain the error
        if (success && size > 0 && size <= (is_heap_allocated == 1 ? MAX_SIZE_HEAP : MAX_SIZE_STACK)) {
            return size; // Valid input, return the size
        } else {
            // Show the user their invalid input
            input[strcspn(input, "\n")] = '\0';  // Remove trailing newline
            print_error("Invalid input: '%s'. Please enter a valid number between 1 and %zu!", input, is_heap_allocated ? MAX_SIZE_HEAP : MAX_SIZE_STACK );
        }
    } while (1); // Repeat until valid input
}

int main(){
    uint8_t is_heap_allocated = get_memory_type();
    // Can make an array the size that is inputted here (can't use constructor function without using malloc)
    size_t size_of_memory = get_size_of_memory(is_heap_allocated);

    HashMap commands = init_commands(100); // Bucket size of the hashmap for commands

    HashMap pointers = init_hashmap(10); // Bucket size is 10

    Block *p_blocks;  // Declare the pointer for blocks (this will be used for heap allocation)
    uint8_t *p_bytes; // Declare the pointer for bytes (this will be used for heap allocation)
    
    // Stack allocation for blocks, because c is annoying I need to create stack arrays outside of if blocks, and replace them later with malloc if I want both options
    Block blocks_stack[is_heap_allocated? 1 : size_of_memory];  

   // Stack allocation for bytes, because c is annoying I need to create stack arrays outside of if blocks, and replace them later with malloc if I want both options
    uint8_t bytes_stack[is_heap_allocated? 1 : size_of_memory]; 

    // Assign stack arrays to pointers
    p_blocks = blocks_stack; 
    p_bytes = bytes_stack;

    if (is_heap_allocated) {
        // Heap allocation for both blocks and bytes
        p_blocks = (Block *)malloc(size_of_memory * sizeof(Block)); // Allocate memory for blocks on the heap
        p_bytes = (uint8_t *)malloc(size_of_memory * sizeof(uint8_t)); // Allocate memory for bytes on the heap
        if (p_blocks == NULL || p_bytes == NULL) {
            // Handle allocation failure (for safety)
            printf("Memory allocation failed!\n");
            exit(1);
        }
    } 
    
    // Now, you can use `blocks` and `bytes` throughout the function
    
    
    p_blocks[0] = (Block){ // Initiailize the first index as a big free uninitialized block
        .size = size_of_memory, // With the size the requested
        .start_index =  0, // Starts at the index 0 in the memory (the actual memory, not the blocks memory)
        .p_prev = NULL, // No previous block
        .p_next = NULL, // No next block yet
        .free = 1, // It is free
        .uninitialized = 1 // And it is uninitialzed
    };

    Memory memory = {
        .p_bytes = p_bytes, // Initialize the memory with bytes as mem.bytes
        .p_blocks = p_blocks, // The blocks array as the mem.blocks
        .amount_of_blocks = 1, // There is only one block currently
        .memory_size = size_of_memory, // The size of the memory
        .p_pointers = &pointers,
        .on_heap = is_heap_allocated
    };

    char input[MAX_INPUT_SIZE]; // Input buffer

    printlnf("Welcome to the Bytethon terminal. Please enter help to see a list of commands and how they work. For more information look for the documentation in dir /info.\n\n");
    while (1) {
        printf(">>> ");
        fgets(input,sizeof(input),stdin);
        execute_command(&memory,&commands,input); 
    }
  return 0;
}