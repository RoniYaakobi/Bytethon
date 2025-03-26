#ifndef UTILS_H
#define UTILS_H

// This file is meant to be a general utils file that I can use whereever I want. Feel free to copy paste lol.

#include <stdio.h>
#include <stdlib.h> // For the hashmaps
#include <string.h>
#include <stdint.h> // Using uint8_t in all the project
#include <stdarg.h> // For the printlnf function which needs to accept multiple parameters like the printf function
#include <windows.h> // For Sleep() in the exit animstion

// printf with auto newline
void printlnf(const char *format, ...); 


// printlnf extension that adds an error message 
// (currently prefixes with 'Error: ' and suffixes with " Please type help for more information.\n")
void print_error(const char *format, ...);

// Does same as print_error, (but currently prefixes with 'Warning: ' and suffixes with " This action is irreversable, so make sure you are sure.\n")
// Also askes the user for a confirmation and returns a boolean based on the confirmation (conf == "c" or 'c' --> return 1, else return 0)
uint8_t print_warning(const char *format, ...);

// Just printlnf with the prefix "Success: "
void print_success(const char *format, ...);

// Checks string equality
uint8_t same_string(const char *str1, const char *str2); //String equivelence check

// Closing animation for the program with the name of the program as input
void exit_program(char *name_of_program);

// A node for the hashmap, must contain it's key (for the linked list searching), its value, and a pointer to the next HashNode in the linked list (for the linked list searching)
struct HashNode{
    char *p_key;
    void *p_value;
    struct HashNode *p_next;
};

typedef struct HashNode HashNode; // gcc gives me a warning if I typedef on the struct's definition here :-(

// The actual hashmap struct, map is a pointer to an array of HashNodes 
// (each HashNode is part of a linked list of keys that got hashed to the same value)
// This implementation makes the key's that have the same hash go to the same index, but then they simply become a linked list
// (This is the only reason why I have stdlib in here)
// The function traverses the linked list until it finds a node with the exact key that it is looking for, and ends up returning a value based on that.
typedef struct {
    HashNode **pp_map;
    size_t bucket_size;
} HashMap;

// Hashing function to generate the keys for the hashmap.
// Limits the key to be in the range 0 <= key < size
int hash(const char *key, size_t size);

// Initialize a hashmap based on the input size
// size is the bucket size of the hashmap
HashMap init_hashmap(size_t size);

// Creates a HashNode for the hashmap_insert function
HashNode* create_node(const char *key, void *p_value);

// Insert into an exsisting HashMap a new value
// Creates a new HashNode struct and puts it in the map array based on the key's hash in hash()
void hashmap_insert(HashMap *p_map, const char *key, void *p_value, uint8_t silent) ;

// Get the value at a certain key in the HashMap and return a void pointer to it.
// This allows the hashmap to effectively store any value you want, as long as you cast back to that type
// Including but not limited to functions, pointers, structs, and arrays
// Or return NULL in the void pointer if the value was not found 
void* hashmap_get(HashMap *p_map, const char *key);

// Remove the HashNode for a certain key, freeing all relevant mallocs
void hashmap_remove(HashMap *p_map, const char *key, uint8_t silent);

// Free the entire hashmap, all the HashNodes and their mallocs, the map itself which is malloced
void hashmap_free(HashMap *p_map);

#endif // UTILS_H