# Utilities Module

___

## Overview

The `utils` module provides general-purpose functions that are not specific to any single use case but are useful across multiple parts of the project.

It includes:

 - A `printf()` wrapper with automatic newlining.
 - Debugging functions with **ANSI color formatting**.
 - String comparison utilities.
 - **HashMap** management functions.
This module is intentionally designed to be **self-contained and reusable**, making it suitable for integration into other projects. Because of this, it has its own dedicated `.md` documentation file separate from `functions.md` in `bytethon`.

Since most functions in `utils` are elementary and self-explanatory, this documentation will be **less detailed** than `functions.md`. Many of these functions cover fundamental programming concepts that most developers are already familiar with.

This module includes several dependencies:
 - `<stdio.h>`
 - `<stdlib.h>` For mallocing the `HashNode` structs
 - `<string.h>` For `strcmp()`
 - `<stdint.h>` For `uint8_t` (which are used for the boolean that `print_warning()` returns)
 - `<stdarg.h>` For the `printf()` wrappers' string formating.
 - `<windows.h>` For `Sleep()` in the three dot animation in the `exit_program()` function.

Feel free to **copy, modify, and adapt** this module as needed for your own use.

___

## Console tools

### 1. `exit_program`
Provides a fancier way to exit a program:
 - Takes the name of your program as input.
 - The function will print `"Thank you for using {program} Exiting..."`.
 - Each `'.'` is printed with a 2/3-second delay, resulting in an exit animation that takes 2 seconds to complete.
 - All output sections are styled using **ANSI** colors for a more engaging visual experience.

Example:
``` c
if (input == "exit"){
    exit_program("my cli");
}
```

### 2. `print_error`
Expands upon the `printlnf()`'s behavior:
 - Prefixes the output with an error prefix
 - Suffixes the output with an error suffix
<br>
- The prefix is **`[ERROR]`** (colored red).  
- The suffix is **`Enter help to learn more.`** (also red).  

The main output is also red, and every section has a different shade.

 Example:
``` c
print_error("Division by zero!");
```

### 3. `printlnf`
A `printf` wrapper that **automatically appends a newline** at the end of the output.

- Uses `#include <stdarg.h>` to support **formatted output** with variable arguments.
- Eliminates the need to manually add `\n` at the end of printf calls, even in formatted strings.

Example:
``` c
printlnf("Hello World!");
```

### 5. `print_success`
Expands upon the `printlnf()`'s behavior:
 - Prefixes the output with a success prefix
<br>
- The prefix is **`[SUCCESS]`** (colored green).    

The main output is also green, and the sections have different shades.

Example:
``` c
if (guess == correct){
    print_success("You guessed the right number!");
}
```


### 5. `print_warning`
Expands upon the `printlnf()`'s behavior:
 - Prefixes the output with a warning prefix
 - Suffixes the output with a warning suffix
 - Also prompts the user to confirm if they want to proceed. If they enter any input with the first letter `'c'` or `'C'`, it is parsed as a confirmation.
<br>
- The prefix is **`[WARNING]`** (colored yellow).  
- The suffix is **`This action is irreversible. Are you sure?`** (also yellow).  

The main output is also yellow, and every section has a different shade.

This function returns `uint8_t`, as a boolean (`0` is false, `1` is true). This is done to align with the context of the `Bytethon` project, where uint8_t substitudes booleans. 
Example:
``` c
int multiply_2(int num){
    if (num > 2147483647 / 2){ // Half the integer limit.
        uint8_t conf = print_warning("This will result in an incorrect result,"
        " as multiplying %d times 2 would exceed the integer limit", num);
        if (!conf){
            printlnf("Cancelling...");
            return -1;
        }
    }
    return num*2;
}
```


___

## General tools

### 1. `same_string`
Compares two strings and returns a `uint8_t`:
 - Returns `1` if the strings are identical.
 - Returns `0` if they are not indentical.

Example:
```c
// Returns 1 as they are identical
uint8_t first_check = same_string("Hello World!","Hello World!");

// Returns 0 as they are not identical
uint8_t second_check = same_string("Foo","Bar");
```

___

## Hashmap tools

### Structs

#### 1. HashNode
A `HashNode` represents a node in the hashmap's linked list structure, allowing multiple entries to exist at the same index when hash collisions occur.

Each node consists of:

 - `*p_key` – A dynamically allocated pointer to the key, enabling key-based lookups.
 - `*p_value` – A `void*` storing the associated value. The recieving function must handle type casting.
 - `*p_next` – A pointer to the next node in the collision-linked list.

This approach ensures efficient storage and retrieval of multiple key-value pairs within the same hash index.

#### 2. HashMap
The `HashMap` consists the following:
 - `**pp_map` - Serves as the primary storage for the hashmap.
 - `bucket_size` The number of buckets (array slots) in the hashmap

The hash function determines an index using `hash % bucket_size`, ensuring values are distributed across the available buckets.

### Functions.

#### 1. create_node
The `create_node` function initializes and returns a new `HashNode` using the following parameters:

`*key` – A pointer to the key for the `HashNode`.
`*p_value` – A `void*` pointing to the value stored in the node.
Function Steps:
**Key Validation** – If `key` is `NULL`, the function prints an error using `print_error` and returns `NULL`.
**Memory Allocation **– Allocates memory for both the `HashNode` and its key using `malloc()`. If allocation fails, it prints an error and returns `NULL`.
**Initialization** – Copies the key into the node’s `key` pointer, assigns `value` to the `.value` field, and sets `next` to `NULL`.
**Return Node** – Returns a pointer to the newly created `HashNode`.

#### 2. hash
This function implements the **DJB2** hashing algorithm, a simple and efficient method for hashing strings.

Process:
1. **Initialize** – Start with a constant hash value of `5381`.
2. **Iterate Over Characters** – For each character in the string:
    - Multiply the current hash by 33 using `(hash << 5) + hash` (equivalent to `hash * 33`).
    - Add the character's ASCII value to the hash.
3. **Ensure Bounds** – Return `hash % bucket_size` to constrain the result within the available storage.

#### 3. hashmap_free
This function frees all allocated memory in the hashmap by iterating over its indices and deallocating nodes.

**Process**:
1. **Iterate Over All Buckets** – Loop through each index in the hashmap array.
2. **Free Nodes in Linked Lists** – For each node:
    - Free the `p_key` and `p_value` pointers.
    - Free the node itself.
    - Move to the `p_next` node (if not `null`) and repeat the process.
3. **Free the HashMap Structure** – Once all nodes are freed, deallocate the hashmap’s storage array.

#### 4. hashmap_get
Retrieves the `p_value` associated with a given `key`.

**Process:**
1. **Compute the Hash** – Use `hash()` to determine the index.
2. **Traverse the Linked List** – Access the corresponding bucket and iterate through the linked list:
    - If a node with a matching key is found, return its `value` (as a `void*`).
    - If the key is not found, return `NULL`.

#### 5. hashmap_insert
Inserts a `HashNode` with a `p_key` and a `p_value` into the `HashMap`.

**Process:**
1. **Initialize a node** – Create a new `HashNode` using `create_node()`, storing the given `key` and `p_value`.
2. **Compute the hash** – Determine the index using `hash(key,bucket_size)`.
3. **Handle insertion:**
    - If the bucket is empty, insert the node directly.
    - If a collision occurs (another node exists at the index), add the new node to the front of the linked list (chaining).

#### 6. hashmap_remove
Removes a `HashNode` associated with the given `p_key` from the `HashMap`.

**Process:**
1. **Compute the hash** – Determine the index using `hash(key,bucket_size)`.
2. **Traverse the linked list** at the computed index:
    - Keep track of the current node and its previous node.
    - If the key matches the current node's key:
        - **Rechain the list:**
            - If it's the head node, update the hashmap's bucket to point to node->next.
            - If it's not the head, update prev->next to skip the node (becomes prev->next->next).
        - **Free the node:**
            - Free `p_key` and `p_value` pointers.
            - Free the `HashNode` itself with `free()`.
    - If the key is not found, return without changes.

### 7. `init_hashmap`  
Initializes a `HashMap` with a specified `bucket_size`.  

#### **Process:**  
1. **Allocate memory** – Use `calloc()` to allocate memory for the `HashMap` structure.  
2. **Initialize the storage** – Allocate an array of `HashNode*` using `calloc(bucket_size, sizeof(HashNode*))`, ensuring all entries start as `NULL`.  
3. **Set metadata** – Assign `bucket_size` to the `HashMap` struct.  
4. **Return the HashMap** – If memory allocation succeeds, return a pointer to the new `HashMap`. If allocation fails, handle the error gracefully.  
