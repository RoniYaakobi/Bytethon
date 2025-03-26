# Functions
___

## Developer Documentation
This file is intended for developers and provides information on all functions used in the code. While you're welcome to explore, this documentation assumes familiarity with developer terminology and does not include in-depth explanations of fundamental concepts. **Also you are expected to read the `conventions.md` file before proceeding in this file.**

____

## How to navigate this file?

The functions in this file are sorted by alphabetical order in their modules, and the modules are also sorted in alphabetical order. If you need to find a specific function, you should use the find function (generally `ctrl + f`) of your editor.

**Notice**: when referring to a **CLI** command and not a c function, the command wil be prefixed with `>>> ` like the decoration the CLI prints on the input line.

All modules are dependent on:
 - `"utils.h"`
 - `"memory_structs.h"`
 - `"stdint"`

A standard library is marked by not adding `.h` at the end of the name of the dependency, and a custom module (or external library), is marked by adding `.h` to the end of the name.

<br>
Each module description includes its direct dependencies. Currently, the project is structured with a central module that imports and is imported by all other modules, leading to high interdependencies. Documenting dependencies now allows for easier analysis in the future, should the need arise to reduce unnecessary coupling between modules.   
___

## Modules:

___

### 1. `cli`
The `cli` module is a module that contains the functions that deal with the CLI. This includes creating the commands, dispatching them and sending them to their parsers, printing the help information, and a few more CLI related operations.

Dependencies: `"utils.h"`,`"stdlib"` for `malloc()`, `free()` and `strcpy()`, `"string"` for `strdup()`, `strtok()`, `strncpy()` and `strcspn()`, `"my_malloc.h"` for `my_malloc_command()`, `"my_free.h"` for `my_free_command()`, `"interact_with_memory.h"` for `set_val_command()`, `"pointer_management.h"` for `new_pointer_command()`,`"visualize.h"` for `visualize_bytes_command()` and `visualize_blocks_command()` 
___

#### 1. `create cmd`
 - **Function name :** `create_cmd`
 - **Arguments:**
    - `size_t amount_of_arguments` →  The amount of arguments needed for this command.
    - `Command_Func func` →  Function pointer to the parser for this command, casted into `Command_Func`.
    - `const char* name` →  Name of the command.
    - `const char* description` →  Command description for `>>> help` command.
    - `Command_Classification type_of_function` → Function classification enum so the dispatcher will know if to pass the `Memory` struct pointer, or the command `HashMap` pointer, or both to the parser.
 - **Output :** A `Command` struct that contains the values passed to the function
 - **How does it work?** 
This function initializes a `Command` struct, and sets its parameters based on the values passed into the function arguments. It decides the `cmd.type` with a switch-case block for all the enums that are from type `Command_Classification` type, and for the name and description it uses `strdup()`.

- **Usage example** 
```c
Command my_malloc_cmd = create_cmd(
    2, // Amount of arguments for the command.
    (Command_Func)my_malloc_command, // Pointer to a Parser.
    "malloc", // Name of command.
    // Description of command and usage.
    "Manually allocate with a pointer a certain amount of bytes," 
    "for example : malloc 10 ptr", 
    Memory_management // Classification of the command.
);
```

- **Notes:**
This command might be removed in the future as `init_commands` will become automatically generated.

___

#### 2. `execute command`
 - **Function name :** `execute_command`
 - **Arguments:**
    - `Memory *p_memory` →  Pointer to a `Memory` struct that the user will interact with.
    - `HashMap *p_commands` →  A pointer to a `HashMap` which stores the `Command` structs.
    - `char *input` →  The raw input that the user gave the CLI.
 - **Output :** Sends the arguments to the relevant parser, after confirming the user's input was valid.
 - **How does it work?** 
This function is one of the more complex functions in the program, it does the following:
    1. **Cleans user input :** Removes trailing newline.
    2. **Tokenizes input :** the input string is split into tokens based on spaces, and each token is stored in an array.
    3. **Looks up the relevant command :** Based on the first token, try to get the command struct that its key corresponds to this string (command_name -> Command struct).
    4. **Checks number of arguments** Makes sure that the user entered the same amount of arguments as needed for the command.
    5. **Calls the parser** Calls the parser with casting to the type of function, and extra arguments (`Memory* p_memory`, `Hashmap* p_commands`, etc...) based on the classification type of the command.
    6. **Prints debug information** If/when it fails, or a test is bad, the user gets a message about what made the command fail.

- **Usage example** 
```c
Hashmap cmds = init_commands(10);
Memory mem;
/*
 Initialize memory using whatever steps necessary.
 Since this project is trying to simulate memory management 
 with malloc and free without actually using c built-in malloc or free
 there is no function init_memory().
*/
...

char input[50];
fgets(input, sizeof(input), stdin);
execute_command(&mem,&cmds,input);
```

- **Notes:**
Might change the if statements to a swicth-case in the future, currently doesn't matter too much.
___

#### 3. `exit program bytethon`
 - **Function name :** `exit_program_bytethon`
 - **Arguments:**
    - `Memory *p_memory` → `Memory` struct to free.
    - `HashMap *p_cmds` → The `HashMap` with the command structs.
    - `int args_c` → Amount of arguments that the dispatcher passed.
    - `char *args[10]` → Actual arguments that the dispatcher passed. 
    **Important :** Frees everything that needs freeing, and then calls from utils the function `exit_program()` which prints a goodbye message with a three dot animation.
 - **Output :** Frees the map that stores the `Command` structs.
 - **How does it work?** 
Calls the function `free_cmds(p_cmds)`,`hashmap_free(p_cmds)`, and `hashmap_free(p_memory->pointers)` and if user decided to do a heap simulation `free()` for `p_memory->blocks` and `p_memory->bytes`. Then it calls the function `exit_program("Bytethon")`.
- **Usage example** 
```c
int main(){
   Hashmap cmds = init_commands(10);
   Memory mem;
   ... // Initialize memory and do whatever.
   /*
    This part is automatically done by the dispatcher but these 
    Are nessecary arguments for the function if you want to run outside
    execute_command().
   */
   int args_c = 0;
   int *args[10];

   exit_program_bytethon(&mem,&cmds,args_c,args); 
   // Closes program after freeing and animation.
   return 0;
}
```

- **Notes:**
   - This function may include saving mechanics in the future, depending on if a settings file will added.

___

#### 4. `free cmds`
 - **Function name :** `free_cmds`
 - **Arguments:**
    - `HashMap *p_map` → The HashMap with the command structs that you want to free. 
    **Important :** This function only frees the name and description of the `Command` structs, after using this function use the function `free_hashmap()` from the utils module to finish the freeing of the map.
 - **Output :** Frees the map that stores the `Command` structs
 - **How does it work?** 
Loops over all indices of the hashmap, and traverses the linked lists which heads are stored there, as linked lists are the method used to handle hashing collisions, and for each node it frees the pointers to the name and description of that node (created with `strdup()`).
- **Usage example** 
```c
int main(){
   Hashmap cmds = init_commands(10);
   ...
   free_cmds(&cmds); // Free only command struct names and descriptions.
   free_hashmap(&cmds); // Important!!! Call this to free the entire hashmap.

    return 0;
}
```

- **Notes:**
   - **Notice:** This function only frees the name and decription pointers of the `Command` struct, not the entire `HashMap`. This is done so that if there is need, you will be able to reuse this hashmap for future purpose. To safely free the whole commands hashmap use the function `free_hashmap()` from the `utils` module.

___

#### 5. `help cmds`
 - **Function name :** `help_cmd`
 - **Arguments:**
    - `HashMap *p_map` → The commands `HashMap` to print it's information. 
 - **Output :** Prints information about every single command in the `HashMap`.
 - **How does it work?** 
   1. Loops over all the indices of the `HashMap`, and each time it hits a head slot for a linked list it traverses the linked list and prints the relevant information, the name of the command, a short description, the amount of arguments needed for it and its classification type.
   2. After looping over everything tell the user to look at the directory /info for more information.
- **Usage example** 
```c
int main(){
   Hashmap cmds = init_commands(10);

   help_cmds_command(&cmds); // Prints the information
}
```

- **Notes:**
This function might have in the future logic for `>>> help <cmd>`, where cmd is a name of a certain command, but right now it doesn't do anything with the input.

___

#### 6. `help cmds command`
 - **Function name :** `help_cmds_command`
 - **Arguments:**
    - `HashMap *p_map` → The commands `HashMap` to print its information. 
    - `int args_c` → Amount of arguments passed by the dispatcher.
    - `char *args[10]` → The actual arguments the dispatcher passed. 
 - **Output :** Passes the command `HashMap` to function `help_cmds()`
 - **How does it work?** 
Simply passes the `HashMap` as the argument that the function `help_cmds()` needs.
- **Usage example** 
```c
int main(){
   Hashmap cmds = init_commands(10);
   
   /* 
    You can also simply pass the user input into the execute_command() function
    which will detect the word 'help' at the start of the input 
    and call the function automatically.
   */
   int args_c = 0;
   char *args[10];

   help_cmds_command(&cmds,args_c,args); 
   // Calls the function that prints the information
}
```

- **Notes:**
This function might have in the future logic for `>>> help <cmd>`, where cmd is a name of a certain command, but right now it doesn't do anything with the arguments passed.

___


#### 7. `init commands`
 - **Function name :** `init_commands`
 - **Arguments:**
    - `size_t size` →  Bucket size for the `HashMap` used to lookup the commands
 - **Output :** A `HashMap` containing all the commands available to the user in the CLI
 - **How does it work?** 

   1. **Initialize a hashmap** using `init_hashmap(size)`.
   2. **Create command structs** for each command, including:
      - Argument count, function pointer, command name, short description (for `>>> help`), and a classification enum.
      - Uses `create_cmd()`.
   3. **Allocate memory and insert into the hashmap**:
      - Loops over the `Command` structs, attempting `malloc()`.
      - **If `malloc()` fails**, frees allocated memory and exits.
      - Otherwise, copies the struct and inserts it into the hashmap.
- **Usage example** 
```c
Hashmap cmds = init_commands(10);
```

- **Notes:**
This function is currently inefficient and will be replaced by a c file that will be automatically generated by a script, based on a `.json` file which will contain information on each command, and this way we eliminate the issue of needing to write all the command manually.

___

### 2. `general management`
The `general_management` module provides functions for managing memory in multiple scenarios, such as locating a block corresponding to a specific index in a byte array. Also note that this module's header includes all other headers and is included by all other headers.

Dependencies: `"string"` for `memmove()`
___

#### 1. `find block`
 - **Function name :** `find_block`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` struct that you want to find the block that contains the index `index` in the `bytes` array  
    - `unsigned int index` → Search index, the index in the `bytes` array for which you need to find the corresponding block.
    - `Block **pp_block` → A `Block` double pointer to return a pointer to the block found.
 - **Output :** Puts in the `Block` double pointer a pointer to the `Block` that the function found, and returns the index of that block in the `p_blocks` array or `-1` if the block was not found.
 - **How does it work?** 
   Loops over all the blocks and sums their sizes until the sum is more than or equal to the index being searched for, then points the pointer that the double pointer `pp_block` is pointing at, to the block found, and returns the index of the block in the `p_blocks` array. If the search failed, returns -1.
- **Usage example** 
```c
... // Initialize a memory struct, for this example, by the name of mem

int search_index = 10;
Block *p_block;

int block_index = find_block(&mem,search_index,&block)
```

- **Notes:**
This function can be optimized with binary search, currently O(n) in time complexity.

___

#### 2. `shift left`
 - **Function name :** `shift_left`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` struct that you want to shift the block's array in by one index
    - `unsigned int index` → Index that after it (exclusive), every item shifts from index `i` to index `i-1` 
 - **Output :** Shifts the blocks array one left starting one index after `index`, **overwriting the value at `index`**, and returns a uint8_t indicating success (`1`) or failure (`0`).
 - **How does it work?** 
   Effectively like python's `pop(index)` function
   1. Checks that the `index` provided is valid for this operation on the blocks array by making sure it is not exceeding the bounds of the array
   2. Pre-updates the pointers (as `p_blocks` array is both an **array** and a **doubly linked list**)
   3. Shifts all the blocks after `index` (exclusive) one index left (`i` to `i - 1`)
- **Usage example** 
```c
// Initialize memory any way you want, this shows one of the more basic cases
// this function to work
Block arr_blocks[5];
arr_blocks[0] = {...};
arr_blocks[1] = {...};
arr_blocks[2] = {...};

Memory mem;
mem.p_blocks = p_blocks;
mem.amount_of_blocks = 3;
mem.memory_size = 5;

shift_left(&mem,1); 
// After the call, the block originally at index 2 shifts to index 1,
// effectively removing the block at index 1.

mem.amount_of_blocks--; // Important! Updating that an index of a block was overwritten.

shift_left(&mem,0); 
// After the call, the block originally at index 2, and now at index 1, shifts to 
// index 0, effectively removing the block at index 0.

mem.amount_of_blocks--; // Important! Updating that an index of a block was overwritten.
```

- **Notes:**
   - This function effectively duplicates the value at the last index of the movement which in our case doesn't matter, as we decrement `amount_of_blocks` and know not access and index after `amount_of_blocks - 1`. 
   - This function is not simply called `pop` in case there will ever be a need to make it get an end index along with the start index(eg. `shift_left(&mem,1,3)` could shift left from index 1 to index 3), and then it will no longer be like `pop(index)`.
___

#### 3. `shift right`
 - **Function name :** `shift_right`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` struct that you want to shift the block's array in by one index
    - `unsigned int index` → Index that after it (inclusive), every item shifts from index `i` to index `i+1`
 - **Output :** Shifts the blocks array one left starting at `index`, **Leaving the same value at `index` as was before running the function**, and returns an uint8_t indicating success (`1`) or failure (`0`).
 - **How does it work?** 
   Effectively like python's `lst.insert(index,lst[index])` function
   1. Checks that the `index` provided is valid for this operation on the `blocks` array by making sure it is not exceeding the bounds of the array
   2. Pre-updates the pointers (as blocks array is both an **array** and a **doubly linked list**)
   3. Shifts all the blocks after `index` (inclusive) one index right (`i + 1` where `i` is the original index)
   4. `blocks[index]` can effectively be treated as `NULL` without loss of data
- **Usage example** 
```c
// Initialize memory any way you want, 
// This shows a simple example for this function.
Block arr_blocks[5];
arr_blocks[0] = {...};
arr_blocks[1] = {...};
arr_blocks[2] = {...};

Memory mem;
mem.p_blocks = arr_blocks;
mem.amount_of_blocks = 3;
mem.memory_size = 5;

shift_right(&mem,1);
// Block at index 2 shifts to index 3, Block at index 1 duplicates it self 
// into index 2, and indices 4,1, and 0 stay the same. 

... // Insert a block at that index or just use the duplicate but 
// update its pointers, otherwise the linked list breaks.

mem.amount_of_blocks++; // Updating the amount of blocks

shift_right(&mem,0);
// Block at index 3 (originally at index 2) shifts to index 4.
// Block at index 2 (originally at index 1) shifts to index 3.
// Block at index 1 shifts to index 2
// (wasn't impacted by the first call so has the original content).
// Block at index 0 duplicates itself into index 1 (Which also hasn't changed yet).

... // Insert a block at that index or just use the duplicate but 
// Update its pointers, otherwise the linked list breaks.

mem.amount_of_blocks++; // Updating the amount of blocks.
```

- **Notes:**
   - This function effectively duplicates the value at the `index` which enables us to treat `index` as `NULL` without loss of data. - This function is not simply called `insert_null` in case there will ever be a need to make it get an end index along with the start index (eg. `shift_right(&mem,1,3)` could shift right from index 1 to index 3), and then it will no longer be simply like python's `lst.insert(index,lst[index])`

___

### 3. `interact with memory`
The `interact_with_memory` module handles all interactions with memory. This includes dereferencing pointers, performing operations on the values held by two pointers, and setting memory presets for the bytes array, blocks array, and pointer's hashmap.

Currently, the module only contains a function that sets the value a pointer is pointing to, within a range of 0-255.

Dependencies: `"general_management"` for `find_block()`
___

#### 1. `set val`
 - **Function name :** `set_val`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that contains the memory that needs to modified.
    - `uint8_t value` → New value that `ptr` will point to (must be an int between 0-255)
    - `Pointer ptr` → Pointer to the memory index to modify
 - **Output :** Sets the relevant index in the `p_bytes` array to the specified `value`, and returns a uint8_t indicating success (`1`) or failure (`0`).
 - **How does it work?** 
   1. Finds the `Block` in the `p_blocks` array that the pointer is pointing to,using `find_block()` from the `general_management()` module. It ensures that the output index is valid (i.e., not `-1` which is returned to indicate failure). If the index is invalid, a debug message is printed.
   2. Initializes the block (set the `uninitialized` flag to `0`, which represents false) and sets all the values in the block to `0` to prevent garbage values from being stored.
   3. Sets the last (rightmost) byte of the block to the specified `value`.
- **Usage example** 
```c
... // Initialize a Memory struct "mem" and a Pointer struct "ptr"

// Assuming memory layout is as such  *____ptr_____*
 //                         ... XX XX ** ** ** ** ** XX XX ...
// XX represents any memory, and ** represents values in ptr's block

set_val(&mem,10,ptr);

// After this the memory layout:      *____ptr_____*
 //                         ... XX XX 00 00 00 00 0A XX XX ...
// Where '0A' represents the hex value for 10.

```

- **Notes:**
   - In the future, this function will support larger values, and the value will be distributed across multiple bytes when necessary.

___

#### 2. `set val commnad`
 - **Function name :** `set_val_command`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that contains the memory that needs to modified.
    - `int args_c` → Amount of arguments that the `execute_command()` dispatcher tokenized from user input.
    - `char *args[10]` → The actual arguments tokenized by `execute_command()`.
 - **Output :** If parsing the arguments succeeds, passes them to `set_val()`. Otherwise prints an error message.
 - **How does it work?** 
   1. Checks that the correct number of arguments were passed (2)
   2. Uses `strtol()` to parse the first argument in the `args` array, ensuring that the input is a valid integer `0-255`, and that the entire input was a valid number (not partially parsed). If invalid, prints an error message.
   3. Attempts to retrieve a pointer from the `Memory` struct's hashmap using the second argument in `args`. If retrieval fails, prints an error message.
   4. Calls `set_val()` with the parsed arguments. If it prints a success message and returns `1` (success).
- **Usage example** 
```c
// Assuming the user initialized the pointer `ptr` with ">>> new_pointer ptr" in 
// the CLI and malloced it with at least one byte ">>> malloc 1 ptr".
>>> set_val 16 ptr
// Calls the dispatcher, which sends the arguments to this parser,
// which then validates and sends them to the set_val() function.

```

- **Notes:**
   - Future improvements may include handling values larger than 255 if `set_val()` is updated to support multi-byte values.

___

### 4. `main`
In most programs, `main` does not contain much logic. However, due to the nature of this project—avoiding the use of built-in `malloc()` except where absolutely necessary (e.g., hashmaps)—certain responsibilities must remain in `main`. While the core logic of the program is handled elsewhere, `main` is still responsible for key tasks, including:

- Asking the user for simulation settings (currently two questions)
- Initializing a few structs (since using stack allocation in another function would make them invalid upon return)
- Running the main loop (a simple three-line loop that handles user input and sends it to the dispatcher via `execute_command()` from the `cli` module)

Because of these responsibilities, main deserves its own section in the function documentation. 

Dependencies: `"stdlib"` for `malloc()`, `"string"` for `()`, `"cli.h"` for `init_commands()` and `execute_command()`
___

#### 1. `get memory type`
 - **Function name :** `get_memory_type`
 - **Arguments:** None
 - **Output :** A Boolean indicating whether the user selected heap-allocated memory.
 - **How does it work?** 
   Prompts the user to choose between stack allocation and heap allocation.
   
   The function parses the user input as an integer, subtracts 1, and returns:

   - `1` (true) if the user selects heap-based allocation (option `2`)
   - `0` (false) if the user selects stack-based allocation (option `1`)

   If the user inputs an invalid value, they are prompted again until valid input is provided.
- **Usage example** 
```c
uint8_t is_heap_allocated = get_memory_type() // Boolean based on user input
```

- **Notes:**
None

___

#### 2. `get memory size`
 - **Function name :** `get_memory_size`
 - **Arguments:** 
   - `uint8_t is_heap_allocated` → Boolean indicating whether memory will be heap- or stack- allocated. Stack allocation has a much smaller limit for the size.
 - **Output :** Returns a `size_t` value representing the memory size chosen by the user.
 - **How does it work?** 
   Based on `is_heap_allocated`, the function prompts the user to input a memory size, enforcing the following limits:
      - **Stack allocation (`!is_heap_allocated`)** → Maximum size `524,288` bytes (`2^19`)
      - **Heap allocation (`is_heap_allocated`)** → Maximum size `536,870,912` bytes (`2^29`)
   
   The function ensures that the user provides a valid number within the allowed range before returning the selected size. The function attempts to convert user input into an integer. If conversion fails (e.g., non-numeric input), the user is re-prompted until a valid number is provided. If the user enters an invalid input, they are prompted again until a valid one is provided.
- **Usage example** 
```c
uint8_t is_heap_allocated = get_memory_type() // Boolean based on user input

// Get memory size based on allocation type and user input
size_t  size_of_memory = get_memory_size(is_heap_allocated);
```

- **Notes:**
   - The maximum stack allocation size (`2^19` or `524,288` bytes) is chosen because it closely approaches typical stack limits.
   - The maximum heap allocation size (`2^29` or `536,870,912` bytes) limits allocation to **0.5 GB**, preventing excessive memory usage in a CLI program.
   - For both allocation types, the macro storing the max size divides the max size by `sizeof(Block)`, ensuring that the `blocks` array does not exceed the intended memory limit.
___

#### 3. `main`
 - **Function name :** `main`
 - **Arguments:** None
 - **How does it work?** 
   1. Asks the user if to use heap- or stack- allocation, and based on that gets the size of the memory.
   2. Initializes the commands and pointers hashmaps.
   3. Initializes the `p_blocks` and `p_bytes` arrays for the `Memory` struct, with `malloc()` or with a `VLA` (Variable-Length Array, an array which size is determined during runtime, based on a variable) based on what user requested.
   4. Initializes the first `Block` in the `p_blocks` array to contain the size of the whole `p_bytes` array.
   5. Initializes the `Memory` struct with the relevant data.
   6. Initalizes the buffer for user input and starts the main loop, which consists of 3 actions: printing `">>> "` (for decoration), gets user input, and calling the dispatcher (`execute_command()`) with the user input, so it can try to dispatch it to the relevant parser function. 
- **Usage example** 
None: it is `main`.

- **Notes:**
   - **`uint8_t` is used instead of `bool` for consistency with other functions, which while they currently only return `0` or `1`, they have potential future expansion.**
   - The program first initializes the `blocks` and `bytes` arrays using a VLA. If the user selects heap allocation, these pointers are reassigned using `malloc()`."

   - Instead of a fixed-size array, a dynamically allocated linked list could allow flexible memory expansion, avoiding the need for large upfront allocations
   
   - Another option could be storing blocks in a struct containing multiple `long long` values, reducing metadata size but increasing complexity in other functions.

   **Example Scenario:**
   **If a user selects heap allocation with a size of `10MB`.**

   - **`p_blocks` and `p_bytes` initially point to small VLAs.**
   - **After confirming heap allocation, `malloc()` assigns 10MB of memory.**
   - **The first `Block` is set to represent the entire `10MB`.**

___

### 5. `my free`
The `my_free` module has one job: implement the c function `free()` for this simulator. It contains 3 functions, one for parsing the input passed by the dispatcher to the main function, one helper function that merges free blocks, and the `my_free()` function.

Dependencies: `"stdlib"` for `strtol()`, `"general_management"` for `shift_left()`
___

#### 1. `merge block right`
 - **Function name :** `merge_block_right`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that contains the blocks to merge.
    - `unsigned int index` → The index in the `blocks` array which is merging the block at `index` with the adjacent block on the right.
 - **Output :** Tries to merge the two blocks, making the block at `index` eat the successor block at `index + 1`, absorbing the block.
 - **How does it work?** 
   1. Validates that there is a block to merge with.
   2. Gets a second pointer to the successor of the successor, and if it is not `NULL` updates it's predeccesor to the block at `index` (instead of `index+1`), which effectively makes the linked list while traversing backwards skip block at `index+1`
   3. Rechains the linked list by linking the block at `index` to the block at `index + 2`, increases the size of the block at `index` by the size of the right block and marks the newly merged block at `index` as free and uninitialized (`block->free = 1`, `block->uninitialized = 1`).`
   4. Decrements the number of allocated blocks in memory, and shifts the blocks after `index + 1`, effectively poping `index + 1` from the list and array and shifts the blocks after `index + 1`, effectively removing `index + 1` from the list and array, causing the following blocks to shift left by one position.
- **Usage example** 
```c
... // For example, somehow get an array of blocks in memory struct mem that 
// are at index 0 and 1 of the block array,
// both are free blocks of size 1 byte

merge_block_right(&mem, 0);

// Now the blocks at index 0 and 1 became one block of size 2 bytes, 
// That is also free, and they are at index 0, and the rest of the 
// Blocks moved back one index
```

- **Notes:**
   - If there is no block to merge with, the function will simply return, which is then handled by the caller. Otherwise there is no possible issues that can occur.
   - This function ensures that there are no issues with derefrencing null pointers `next` of the last block. For example if `index+1` is the last block in the array, and it's `next` pointer is `NULL`, it will not try to update it's `prev` pointer to the block at `index`, and the block at index will get `NULL` as it's new `next` pointer.
   - This function works the same for every location of merging, at the start, middle or end of the blocks array.
   - This function does not perform additional checks for memory overflows or any other issues, other than what is specified in the description


#### 2. `my free`
 - **Function name :** `my_free`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that contains the pointer that needs to be freed
    - `Pointer **pp_ptr` → Double pointer to the `Pointer` struct to free, is a double pointer so that my_free can nullify the pointer after freeing it, preventing dangling references.
 - **Output :** Marks the memory block that `**pp_ptr` points to as free, and attempts to merge with surrounding blocks, sets `*ptr` to `NULL`, and returns a uint8_t indicating success (`1`) or failure (`0`).
 - **How does it work?** 
   1. Validates that `pp_ptr` is not `NULL` and `**pp_ptr` is a valid pointer, printing an error message if this check is invalid.
   2. Finds the block that the Pointer struct `**ptr` points at using the `find_block` function from the `general_management` module. Ensures that a block was found, and prints an error message otherwise.
   3. Attempts to merge the block found with adjacent free blocks. It first attempts to merge with the next block using `merge_block_right()`, then with the previous block if possible. This order ensures optimal memory defragmentation by prioritizing forward merging.
   4. Decrements the number of allocated blocks in memory.
- **Usage example** 
```c
... // initalize a memory struct "mem"

// Create a new pointer "ptr"
new_pointer(&mem,"ptr");

// Get its address
Pointer *p_ptr = (Pointer*)hashmap_get(mem.pointers, args[0]);

// Free the pointer
my_free(&mem,&p_ptr);
```

- **Notes:**
   - In the future, this function may have more advanced garbage collection, but currently only merges adjacent free blocks.
   - The function's parser, `my_free_command()`, also removes `ptr` from the `HashMap` of pointers, which removes the `Pointer` struct from the heap.

___

#### 3. `my free command`
 - **Function name :** `my_free_command`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that contains the pointer to free.
    - `int args_c` → Amount of arguments that the `execute_command()` dispatcher tokenized from user input.
    - `char *args[10]` → The actual arguments tokenized by `execute_command()`.
 - **Output :** If parsing the arguments succeeds, passes them to `my_free()`. Otherwise prints an error message.
 - **How does it work?** 
   1. Validates that exactly one argument was provided (`args_c == 1`). If not, prints an error message.
   2. Attempts to retrieve a pointer from the `Memory` struct's hashmap using the first argument in `args`. If retrieval fails, prints an error message.
   3. Calls `my_free()` with the parsed argument. 
   4. If `my_free()` succeeds (returns `1`) 
      - Prints a success message, 
      - Removes the pointer from the hashmap using `hashmap_remove(args[0])`.
- **Usage example** 
```c
// Assuming the user initialized the pointer `ptr` with ">>> new_pointer ptr" in the CLI
>>> free ptr
// Calls the dispatcher, which sends the argument to this parser,
// which then validates and sends it to the my_free() function.

```

- **Notes:**
   - This function ensures that both the pointer and its associated struct are removed from memory, preventing memory leaks and dangling references

___

___

### 6. `my malloc`
The `my_malloc` module is responsible for implementing the `malloc()` function in this memory simulator. It handles finding suitable memory locations, performing allocations, and splitting blocks when necessary.

Dependencies: `"stdlib"` for `strtol()`, `"general_management.h"` for `shift_right()`
___

#### 1. `allocate`
 - **Function name :** `allocate`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that the allocation will take place in.
    - `size_t size` → The size of the allocation in bytes.
    - `unsigned int index` → The index the search function (`my_malloc()`) found for the allocation in the `blocks` array.
    - `Pointer *p_ptr` → A `Pointer` struct to store the index of the start of the allocated block
 - **Output :** Tries to create a new allocated block at `index` of the `p_blocks` array, `size` bytes in size and set `*p_ptr` to point at the relevant index in the `p_bytes` array. Returns a uint8_t indicating success (`1`) or failure (`0`).
 - **How does it work?** 
    1. Finds the starting index of the block in the `p_bytes` array.
    2. If the block found was a perfect match (same size exactly), marks the block as allocated by setting its `free` flag to `0`.
    3. Otherwise splits the block at `index` into two: an allocated block of `size` bytes and a remaining free block.
    4. Returns `0` if allocation failed, and `1` if allocation succeeded.
- **Usage example** 
```c
...
// Say you have a memory struct mem and a pointer struct ptr
// To allocate 10 bytes at index 10 of the blocks array

uint8_t success = allocate(&mem, 10, 10, &ptr);

if (success){
   // Assuming allocation succeeded, the block at index 10 
   // is now pointed to by ptr
}
else {
   // If the allocation failed, then everything remains the same. 
   // This might occur due to the failure of the split_block function, 
   // or due to no index being found for the block's start in the bytes array, 
   // however, in this project, these failure cases are unlikely to occur.
} 
```

- **Notes:**
   - This function generally is called by `my_malloc()`


#### 2. `my malloc`
 - **Function name :** `my_malloc`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that the allocation will be done on.
    - `size_t size` → Size of allocation, used to determine the location to allocate at.
    - `Pointer *p_ptr` → Pointer to a `Pointer` struct, which will store the allocated block’s index in the `p_bytes` array upon success.
 - **Output :** After locating a suitable memory block, calls `allocate()`. If `allocate()` returns `1`, updates `*p_ptr` with the allocated block’s index and returns `1`. Otherwise, returns `0`.
 - **How does it work?** 
   - Implements best-fit algorithm: Looks for the block that is closest to the requested size. 
   1. If an exact-size block is found, immediately calls `allocate()`.
   2. Otherwise, after completing the search, calls `allocate()` with the best block's index found.
   3. If no suitable block is found, prints an error message and returns `0`.
- **Usage example** 
```c
... // initalize a memory struct "mem"

// Declare a Pointer struct "ptr"
Pointer ptr;

// Attempts to allocate 10 bytes and store the result in ptr
uint8_t success =  my_malloc(&mem, 10, &ptr);

// Handle the success flag as needed.
```

- **Notes:**
   - Future updates may introduce additional allocation strategies, such as first-fit.

___

#### 3. `my malloc command`
 - **Function name :** `my_malloc_command`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` struct where the allocation should occur.
    - `int args_c` → Amount of arguments that the `execute_command()` dispatcher tokenized from user input.
    - `char *args[10]` → Array of tokenized arguments from `execute_command()`.
 - **Output :** If parsing the arguments succeeds, passes them to `my_malloc()`. Otherwise prints an error message.
 - **How does it work?** 
    1. Validates that exactly two arguments were provided (`args_c == 2`). If not, prints an error message.
    2. Uses `strtol()` to parse the first argument in the `args` array, ensures the input is a valid positive integer (`> 0`) and fully parsed (not partially valid).. If invalid, prints an error message.
    3. Retrieves a pointer from `p_memory`'s hashmap using `args[1]`. If unsuccessful, prints an error.
    4. Passes the parsed arguments to `my_malloc()`. Prints a success message if allocation succeeds (`1` returned.)
- **Usage example** 
```c
// Assuming the user initialized the pointer `ptr` with ">>> new_pointer ptr" in the CLI
>>> malloc 10 ptr
// Calls the dispatcher, which sends the argument to this parser,
// which then validates and sends it to the my_malloc() function.

```

- **Notes:**
   None

___

#### 4. `split block`
 - **Function name :** `split_block`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` that contains the block that is going to be split.
    - `size_t size` → Size of allocated portion.
    - `unsigned int index` → Index of the block to split.
 - **Output :** Attempts to split the block at `index` into two blocks: one allocated block of `size` bytes and another free block containing the remaining bytes. Returns `1` if successful, or `0` if unsuccessful.

 - **How does it work?** 
   1. Checks that `index` does not exceed the size of the `p_blocks` array. If the index is out of bounds, it prints an error message and returns `0`.
   2. Attempts to shift the `p_blocks` array one index for all blocks after `index`, effectively inserting `NULL` at the `index`'s position.
   3. Rechains the linked list, creating a new free block and the allocated block with the specified `size` in bytes. Initializes both blocks accordingly.
- **Usage example** 
```c
... // Initialize a memory struct "mem"

// Split the block at index 5 into a block of 10 allocated bytes, 
// and the leftover bytes are assigned to a new free block.

uint8_t success = split_block(&mem, 10, 5);

// Handle the success flag as needed.
...
```

- **Notes:**
   - Ensure that the `p_blocks` array has enough space to accommodate the split before calling this function.

___

### 7. `pointer management`
The `pointer_management` module is responsible for managing pointers within the simulation’s `HashMap`. It currently handles pointer creation and may be expanded in the future to support variable creation and type management for both pointers and variables.

Dependencies: `"utils.h"`,`"stdlib.h"` for `malloc()` 
___

#### 1. `new pointer`
 - **Function name :** `new_pointer`
 - **Arguments:**
    - `HashMap *p_pointer_map` → A `HashMap` that stores the new pointer.
    - `char *name` → The name of the new pointer
 - **Output :** Creates a new `Pointer` struct and inserts it into the `HashMap` `pointer_map` 
 - **How does it work?** 
   - Allocates memory for a `Pointer` struct using `malloc()`.
   - Uses `hashmap_insert()` from the `utils` module to store the `Pointer` struct in the `HashMap`, with `name` as the key.

- **Usage example** 
```c
HashMap ptrs = init_hashmap(10); // Initialize the hashmap for pointers

new_pointer(&ptrs, "ptr"); // Creates a new pointer "ptr" in the hashmap
```

- **Notes:**

   - In the future, when pointer types are introduced, this function will also initialize the `pointer.type` field.

___

#### 2. `new pointer command`
 - **Function name :** `new_pointer_command`
 - **Arguments:**
    - `Memory *p_memory` → Pointer to the `Memory` struct where the new pointer will be stored.
    - `int args_c` → Number of arguments tokenized from user input by `execute_command()`.
    - `char *args[10]` → Array of tokenized arguments from `execute_command()`.
 - **Output :** 
   - If argument parsing succeeds, passes them to `new_pointer()`. 
   - Otherwise prints an error message.
 - **How does it work?** 
    1. Validates that exactly one argument was provided (`args_c == 1`). If not, prints an error message.
    2. Checks whether a `Pointer` with the same name already exists in the `HashMap`:
      - If a pointer with the same name exists, prints a warning using `print_warning()` from the `utils` module.
      - The user is then prompted whether to continue despite the warning.
         - If they decline, the function returns without calling `new_pointer()`.
         - If they confirm, the function proceeds.
    3. Calls `new_pointer()` with the validated arguments.
- **Usage example** 
```c

// This is effectively the first interaction with the simulation
>>> new_pointer ptr
// Calls the dispatcher, which sends the argument to this parser,
// which validates and then calls new_pointer().

```

- **Notes:**
   - If the user confirms pointer creation despite a name conflict, the function prints `"Proceeding..."`.

___

### 8. `utils`
This module contains helper functions used throughout the `HashMap` implementation and debugging. To maintain modularity and ease of import, it is documented separately.  

See [`utils.md`](utils.md) for detailed documentation.  

**Key functions in `utils`:**  
- `print_error()` – Handles error messaging.  
- `same_string()` – Check if two strings are the same.  
- `free_hashmap()` – Safely frees a hashmap created with functions in the module.  

___

### 9. `visualize`
This module provides tools for debugging and visualizing key parts of the `Memory` struct.  

**Current features:**  
- **Byte state visualization** – Uses symbols to represent the state of each byte.  
- **Memory block inspection** – Prints relevant metadata about allocated memory blocks.  

Dependencies: None extra
___

#### 1. `visualize blocks`
- **Function name:** `visualize_blocks`
- **Arguments:**
  - `Memory *p_mem` → Pointer to the target `Memory` struct for blocks visualization.
- **Output:**  
  Prints metadata of all blocks in the `p_blocks` array to the user.

- **How does it work?**  
  Loops over all blocks in the `p_blocks` array and prints the following details:
  - Block index (index + 1).
  - Block size.
  - Whether the block is free.
  - Whether the block is uninitialized.
  - Starting index of the block in the bytes array.

- **Usage example:**  
```c
...
// Assuming you initialize the memory like in main.

visualize_blocks(&memory); 
// Prints 1 block with a starting index of 0 and a size of memory.size, 
// uninitialized and free.
```

- **Notes:**
   - More block parameters may be added for visualization in the future.

___

#### 2. `visualize blocks command`
- **Function name:** `visualize_blocks_command`
- **Arguments:**
  - `Memory *p_memory` → Pointer to the target `Memory` struct for blocks visualization.
  - `int args_c` → Number of arguments tokenized from user input by `execute_command()`.
  - `char *args[10]` → Array of tokenized arguments from `execute_command()`.

- **Output:**  
  - If argument parsing succeeds, passes them to `visualize_blocks()`.  
  - Otherwise, prints an error message.

- **How does it work?**  
  1. Validates that exactly **0 arguments** were provided (`args_c == 0`). If not, prints an error message.  
  2. Calls `visualize_blocks()` with the `p_memory` pointer.

- **Usage example:**  
```c
// This command displays the allocated blocks and their metadata.
>>> visualize_blocks
// Calls the dispatcher, which invokes this function,
// validates input, and then calls visualize_blocks().
```

- **Notes:**
   - This command may be replaced with `>>> debug {arg: infoType}` in the future.

___

#### 3. `visualize bytes`
- **Function name:** `visualize_bytes`
- **Arguments:**
  - `Memory *p_mem` → Pointer to the target `Memory` struct for bytes visualization.
- **Output:**  Prints the state of each index in the `bytes` array.

- **How does it work?**  
 1. Loops over all indices in the `bytes` array:
   - If the value at the index is in a free block, print `"__"`
   - If the value at the index is in an uninitialized block but not free, print `"**"`
   - If the value at the index is in an initialized block, print the number stored there, as a uppercase hex (`XX`)
 2. Calls `print_bytes_visualization_legend()` which explains the symbols that were printed.
- **Usage example:**  
```c
...
// Assuming you initialize the memory like in main

visualize_bytes(&memory); 

// Prints "__" for each index in memory.bytes.
```

- **Notes:**
   - Block highlighting may be added in the future, where all indices of the same block are a certain color.

___

#### 4. `visualize bytes command`
- **Function name:** `visualize_bytes_command`
- **Arguments:**
  - `Memory *p_memory` → Pointer to the target `Memory` struct for bytes visualization.
  - `int args_c` → Number of arguments tokenized from user input by `execute_command()`.
  - `char *args[10]` → Array of tokenized arguments from `execute_command()`.

- **Output:**  
  - If argument parsing succeeds, passes them to `visualize_bytes()`.  
  - Otherwise, prints an error message.

- **How does it work?**  
  1. Validates that exactly **0 arguments** were provided (`args_c == 0`). If not, prints an error message.  
  2. Calls `visualize_bytes()` with the `p_memory` pointer.

- **Usage example:**  
```c
// This command displays the byte states.
>>> visualize_bytes
// Calls the dispatcher, which invokes this function,
// validates input, and then calls visualize_bytes().
```

- **Notes:**
   - This command may be replaced with `>>> debug {arg: infoType}` in the future.

___

#### 5. `print bytes visualization legend`
- **Function name:** `print_bytes_visualization_legend`
- **Arguments:**
  - None

- **Output:**  
  - Prints a table that shows what every symbol means in the bytes visualization.

- **How does it work?**  
Simply prints a large table of information.

- **Usage example:**  
```c
print_bytes_visualization_legend();
```

- **Notes:**
   None

___