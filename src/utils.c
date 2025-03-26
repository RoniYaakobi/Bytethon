#include "utils.h"

// printf with auto newline
void printlnf(const char *format, ...) {
    va_list args; 
    va_start(args, format); // Store all the additional arguments after format into args
    vprintf(format, args); // Print like printf, but based on a va_list (args) 
    printf("\n");  // Automatically add a newline to the message
    va_end(args); 
}

// printlnf extension that adds an error message 
// (currently prefixes with 'Error: ' and suffixes with " Please type help for more information.\n")
void print_error(const char *format, ...) { 
    va_list args;
    va_start(args, format);
    printf("\033[4;38;5;52m[ERROR]\033[24m\033[38;5;196m "); // Red error prefix and red error text
    vprintf(format, args);
    printf("\033[38;5;88m Enter help to learn more.\033[0m\n");  // Red error suffix with newline
    va_end(args);
}

// Does same as print_error, (but currently prefixes with 'Warning: ' and suffixes with " This action is irreversable, so make sure you are sure.\n")
// Also asks user to confirm if they want to do this action, returning 0 if the user
// Responds with anything that the first character isn't 'c' or 'C'.
uint8_t print_warning(const char *format, ...) { 
    va_list args;
    va_start(args, format);

    printf("\033[4;38;5;178m[WARNING]\033[24m\033[38;5;221m "); // Yellow warning prefix and yellow error text 

    vprintf(format, args);

    printlnf("\033[38;5;184m This action is irreversible. Are you sure?\033[0m");  // Yellow warning suffix with newline

    va_end(args);

    printlnf("Enter c to confirm: "); // Ask a confirmation
    char reply[10];
    fgets(reply, sizeof(reply),stdin);

    return (reply[0] == 'c' || reply[0] == 'C');
}

// Just printlnf with the prefix "Success: "
void print_success(const char *format, ...) { 
    va_list args;
    va_start(args, format);

    printf("\033[4;38;5;28m[SUCCESS]\033[24m\033[38;5;82m "); // Green success prefix and text

    vprintf(format, args);

    printf("\033[0m\n");  // Success suffix with newline

    va_end(args);
}

void exit_program(char *name_of_program) {
    // Print "Thank you for using {program}. Exiting" with some ansi characters to be more fancy
    printf("\033[38;5;45mThank you for using \033[4;38;5;172m%s\033[0m\033[38;5;45m. Exiting",name_of_program); 

    for (int i = 0; i < 3;i++){Sleep(667);printf(".");} // Closing three dots animation (adds a dot every 667 miliseconds). 

    printf("\033[0m"); // Clear formating
    exit(0);
}

uint8_t same_string(const char *str1, const char *str2){return !strcmp(str1, str2);} // Are the strings the same?


// Simple DJB2 hash function
int hash(const char *key, size_t bucket_size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c . bitwise shift 5 is more efficient than * 32

    return hash % bucket_size;
}

// Function to create a new hash node
HashNode* create_node(const char *key, void *p_value) {
    if (key == NULL) {
        print_error("NULL key passed to create_node"); // No name, no key
        exit(1);
    }

    HashNode *p_new_node = (HashNode*)malloc(sizeof(HashNode));  // Dynamically allocate memory to keep the node outside the scope of this function
    if (p_new_node == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);  // Exit the program if memory allocation fails
    }

    // Allocate memory for the key string
    p_new_node->p_key = (char*)malloc(strlen(key) + 1);  // +1 for the null terminator
    if (p_new_node->p_key == NULL) {
        fprintf(stderr, "Memory allocation for key failed!\n");
        exit(1);  // Exit if memory allocation for the key fails
    }

    // Fill the node with information
    strcpy(p_new_node->p_key, key);
    p_new_node->p_value = p_value;
    p_new_node->p_next = NULL;

    return p_new_node;
}

// Remove key-value pair from the hashmap
void hashmap_remove(HashMap *p_map, const char *key, uint8_t silent) {
    int index = hash(key,p_map->bucket_size); // Get the index where the key should be located
    HashNode *p_current = p_map->pp_map[index]; // Pointer to the targetted node
    HashNode *p_prev = NULL;

    while (p_current != NULL) {
        if (same_string(p_current->p_key, key)) { // If this is the targetted node
            
            if (p_prev == NULL) {
                p_map->pp_map[index] = p_current->p_next;  // Remove from the head of the list
            } else {
                p_prev->p_next = p_current->p_next;  // Remove from the middle/end
            }

            free(p_current->p_value);  // Free the memory for the void pointer
            p_current->p_value = NULL;
            free(p_current->p_key);  // Free the memory for the key
            p_current->p_key = NULL;
            free(p_current);  // Free the memory for the node
            return;
        }

        // Move forward in the linked list
        p_prev = p_current; 
        p_current = p_current->p_next;
    }
    if (!silent)
        print_error("Key %s not found", key);
}

// Insert key-value pair into the hashmap
void hashmap_insert(HashMap *p_map, const char *key, void *p_value, uint8_t silent) {
    void *p_check_key = hashmap_get(p_map,key); // Check if the hashmap has this key already
    if (p_check_key != NULL){ // If it does 
        if (!silent){
            uint8_t conf = print_warning("Overriding the old value at key %s.", key);
            if (conf)
                hashmap_remove(p_map,key,(uint8_t)1);
            else {
                return;
            }
        } else {
            hashmap_remove(p_map,key,(uint8_t)1);
        }
        
    }
    int index = hash(key,p_map->bucket_size);
    HashNode *p_new_node = create_node(key, p_value);

    // Insert at the head of the linked list (chaining)
    p_new_node->p_next = p_map->pp_map[index];
    p_map->pp_map[index] = p_new_node;
}

// Find the value for a given key
void* hashmap_get(HashMap *p_map, const char *key) {
    int index = hash(key,p_map->bucket_size);
    HashNode *p_current = p_map->pp_map[index];

    while (p_current != NULL) { // Go over the linked list in the hash map
        if (same_string(p_current->p_key, key)) {
            return p_current->p_value;  // Return the value if found
        }
        p_current = p_current->p_next;
    }

    // print_error("Key not found."); TODO add logging
    return NULL;  // Return NULL if the key is not found
}

void hashmap_free(HashMap *p_map) {
    for (size_t i = 0; i < p_map->bucket_size; i++) {

        HashNode *p_current = p_map->pp_map[i];
        while (p_current != NULL) {

            HashNode *p_next = p_current->p_next;

            free(p_current->p_value);  // Free void pointer
            p_current->p_value = NULL;
            free(p_current->p_key);  // Free key
            p_current->p_key = NULL;
            free(p_current);  // Free node

            p_current = p_next;
        }
    }
    free(p_map->pp_map); // Free the hashmap array itself
}

HashMap init_hashmap(size_t bucket_size) {

    HashNode **pp_hash_nodes = calloc(bucket_size,sizeof(HashNode*));

    HashMap hashmap = {
        .pp_map = pp_hash_nodes,
        .bucket_size = bucket_size
    };

    return hashmap;
}
