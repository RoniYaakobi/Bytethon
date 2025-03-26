#include "pointer_management.h"

// Arguments parser for the new_pointer function
//
// Input : A pointer to the memory and the amount of arguments and the arguments
//
// Output : Calls the function new_pointer if all the arguments are valid
void new_pointer_command(Memory *p_memory, int args_c, char *args[10]) {
    if (args_c != 1) {
        print_error("Wrong amount of arguments for function new_pointer. Expected 1 argument but received %d arguments",args_c);
        return;
    }

    HashMap *p_pointers = p_memory->p_pointers; // Readability
    char *name = args[0];
    if (hashmap_get(p_pointers,name) != NULL) { // If the pointer name already exsists
        uint8_t conf = print_warning("This will overwrite the previous %s pointer.", name); // Get confirmation from the print_warning function
        
        if (!conf) {
            printlnf("Keeping previous pointer.");
            return;
        }
        printlnf("Proceeding...");
    }
    new_pointer(p_pointers, name);
}

// Creates a new pointer struct and adds it into the pointer struct hashmap in the memory struct
//
// Input : A pointer to a pointer struct hashmap (Usually inside a memory struct) and a name for the new pointer
//
// Output : Initializes a new pointer struct into the pointer struct map
void new_pointer(HashMap *p_pointer_map, char *name) {
    Pointer *p_ptr = malloc(sizeof(Pointer)); // Put the new pointer on the heap (so it can be used in the hashmap)
    
    if(!p_ptr) {
        fprintf(stderr, "Memory allocation failed for Command struct!\n");
        exit(1);
    }

    hashmap_insert(p_pointer_map, name, p_ptr, (uint8_t)1); //  Insert heap-allocated pointer, don't print warnings
    print_success("Declared pointer %s successfully.", name);
}