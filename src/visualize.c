#include "visualize.h"

// Arguments parser for the visualize_bytes function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function visualize_bytes if all the arguments are valid
void visualize_bytes_command(Memory *p_memory, int args_c, char *args[10]) {
    if (args_c != 0) { // Might want to remove this in the future right now is here for consistency between parsers
        print_error("Wrong amount of arguments for function visualize_bytes. Expected 0 arguments but received %d arguments",args_c);
        return;
    }
    
    visualize_bytes(p_memory);
}

// Shows the bytes array items as their hex format. Recommendation : Do not use over 1000 slots of memory if you want to use this function proprerly
//
// Input : A pointer to the memory
//
// Output : Prints all the items in the bytes array in the memory, based on their corresponding block as free, uninitialized, or with a value.
void visualize_bytes(Memory *p_mem) {
    // We don't really need a pointer to a memory struct, it can just be a memory struct, but its more uniform like this (with other functions)
    Memory mem = *p_mem; 

    unsigned int block_index = 0; // Index in blocks array
    int biggest_index_for_block = -1; // What index does the current block end at in the bytes array
    int stage_to_print; // Print __, ** or XX
    
    for (int i = 0; i < mem.memory_size;i++) { // Loop over all the memory's bytes
        
        if (i > biggest_index_for_block){ // If outside the current block update the block you are in
            
            Block curr = mem.p_blocks[block_index]; // Increase readability
            
            biggest_index_for_block = curr.start_index + curr.size - 1; // When does this block end in bytes array
            block_index++; // Next block's index in blocks array
            
            if (curr.free) {
                stage_to_print = FREE;
            } else if (curr.uninitialized) {
                stage_to_print = UNINITIALIZED;
            } else { 
                stage_to_print = USED;
            }
        }

        switch (stage_to_print) {
            case FREE:
                printf("__ "); // If free print two underscore instead of two hex digits
                break;
            case UNINITIALIZED:
            printf("** "); // If uninitialized print two * instead of two hex digits
                break;
            default:
                printf("%02X ", mem.p_bytes[i]); // Else print the hex of the memory's slot
                break;
            }
    }
    print_bytes_visualization_legend(); // Show what symbol means what to the user
    printlnf(""); // Seperate from the next command line for aesthetics
}

// Print the legend for the visualize_blocks function, for what each symbol means. 
void print_bytes_visualization_legend() {
    printlnf("\nHere is the legend for the different types of blocks:");
    printlnf(" ___________________________________________");
    printlnf("|                                  |        |");
    printlnf("|              Meaning             | Symbol |");
    printlnf("|__________________________________|________|");
    printlnf("|                                  |        |");
    printlnf("|               Free               |   __   |");
    printlnf("|__________________________________|________|");
    printlnf("|                                  |        |");
    printlnf("|   Uninitialized (but not free)   |   **   |");
    printlnf("|__________________________________|________|");
    printlnf("|                                  |        |");
    printlnf("| Used (XX is a generic hex value) |   XX   |");
    printlnf("|__________________________________|________|");
}
// Arguments parser for the visualize_blocks function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function visualize_blocks if all the arguments are valid
void visualize_blocks_command(Memory *p_memory, int args_c, char *args[10]) {
    if (args_c != 0){ // Might want to remove this in the future right now is here for consistency between parsers
        print_error("Wrong amount of arguments for function visualize_blocks. Expected 0 arguments but received %d arguments",args_c);
        return;
    }
    visualize_blocks(p_memory);
}

// Show a list of the blocks and their metadata
//
// Input : A pointer to the memory
//
// Output : Prints all the metadata of all the blocks in the blocks array in the memory passed.
void visualize_blocks(Memory *p_mem) {
    // We don't really need a pointer to a memory struct, it can just be a memory struct, but its more uniform like this (with other functions)
    Memory mem = *p_mem;

    Block curr; // Increase readablity

    printlnf("Here is the metadata of the blocks in the memory\n");
    for (int i = 0; i < mem.amount_of_blocks; i++){ // Print the metadata for all the blocks in the blocks array.
        curr = mem.p_blocks[i];

        printlnf("Block number: %d", i + 1);
        printlnf(" - Size: %d",curr.size);
        printlnf(" - Free: %s", curr.free ? "Yes":"No");
        printlnf(" - Initialized: %s",curr.uninitialized ? "No" : "Yes");
        printlnf(" - Start in bytes array: %d",curr.start_index);
        printlnf(""); // One line padding between Block's info
    }
}