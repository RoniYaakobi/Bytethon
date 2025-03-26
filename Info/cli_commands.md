# CLI Commands
### In this file there is a list of all CLI commands and their purpose. Here is how to read the file:
___
#### What is an argument?
An argument is a value that a command/function needs in order to run. A function's behaviour usually depends on the arguments it gets. In this file the type of the argument needed is given in the "**Usage**" section. There are 3 types of arguments:
- **String** - A sequence of characters enclosed in quotation marks. E.g., `"example"`. It can be a word, a sentence, a paragraph, or even a random combination of keys on your keyboard. Keep in mind that this program assumes that every time you start a newline that your done typing your command, but otherwise feel free to enter whatever you want here, in the constraints of the length. 
For example you can use "FooBarTheBarFoo" as a string.
    <br>
- **Float** - Any number with a decimal point, like `4.23` or `-0.99` There is a limit to how large you can enter a float, but it is unlikely it will interfere with practial needs in this program.
    <br>
- **Int** - An int, short for integer, is a whole number without any decimal point, like 42 or -5. Integers have a lower limit than floats, but still it is unlikely that it will interfere with this program. For example 3 is an integer and so is 100, but 10.42 is not.
___
#### Command information structure

Every command in this file follows the following structure, so if you only are looking for a specific field, it is easier to find:
___
### `command name`:
- **Description :** Short explanation of what the command does
- **Usage :** `command_name [arguments]`
- **Required Arguments:** The number of arguments the command expects, and their types.
- **Function called by the dispatcher :** `parser_for_command_name` This is mainly for developers, not for users
- **Example :** An example of one or more ways to use the command
```
>>> Usage case 1 
// Explanation what happened after running command
...
```
___
In order to specify the type of argument the command takes, the format is `type: name`. For example an `int` argument called **"age"** will be displayed as: `int: age`. In the function's usage section the parameters will be shown as `command <type1: name1> <type2: name2> ...`. 
For example: `malloc <int: size> <string: name>`.

In the code blocks you will see two types of information: 
 1. `>>> command` → The full command being run.
 2. `// blah blah blah` → An explanation of what occurs after running the command above.
There will always be an explanation beneath the command.

___
### Commands
___
### `exit`:
- **Description :** Exit the program safely, freeing all the relevant mallocs in the program (shutdown safely) , and showing the closing animation.
- **Usage :** `exit`
- **Required Arguments:** None
- **Function called by the dispatcher :** `exit_program_bytethon`
- **Example :** 
```
>>> exit 
// Program shuts down itself safely
```
___

### `free`:
- **Description :** Free a pointer's allocation.
- **Usage :** `free <string: name>`
- **Required Arguments:** 1 argument: 
    - Name of pointer to free → `string: name`
- **Function called by the dispatcher :** `free_command`
- **Example :** 
```
>>> free ptr 
// Frees the pointer ptr

>>> free x 
// Frees the pointer x
```
___

### `help`:
- **Description :** Displays a list of available commands with short descriptions. It is meant as a quick reminder, not as a full tutorial.
- **Usage :** `help`
- **Required Arguments:** None
- **Function called by the dispatcher :** `help_cmds_command`
- **Example :** 
```
>>> help 
// Lists all available commands with brief descriptions
```
___

### `malloc`:
- **Description :** Allocates a specified number of bytes of memory and assigns it to the given pointer.
- **Usage :** `malloc <int: size> <string: name>`
- **Required Arguments:** 2 arguments: 
    - Size of allocation → `int: size` 
    - Name of the pointer to allocate to → `string: name`
- **Function called by the dispatcher :** `my_malloc_command`
- **Example :** 
```
>>> malloc 10 ptr 
// Allocates 10 bytes of memory to pointer ptr

>>> malloc 16 x 
// Allocates 16 bytes of memory to pointer x
```
___

### `new pointer`:
- **Description :** Declares a pointer without allocating memory for it. The pointer will need to be initialized before use.
- **Usage :** `new_pointer <string: name>`
- **Required Arguments:** 1 argument: 
    - Name of new pointer → `string: name`
- **Function called by the dispatcher :** `new_pointer_command`
- **Example :** 
```
>>> new_pointer ptr 
// Declares a new pointer with the name ptr

>>> new_pointer x 
// Declares a new pointer with the name x
```
___

### `set value`:
- **Description :** Set the value of the memory location that a pointer is pointing to. The value must be in the range 0-255.
- **Usage :** `set_val <int: value> <string: name>`
- **Required Arguments:** 2 arguments: 
    - Value to set the pointer's value to, must be in range 0-255 → `int: value` 
    - Name of pointer to set its value → `string: name`
- **Function called by the dispatcher :** `set_val_command`
- **Example :** 
```
>>> set_val 10 ptr 
// Set the value that ptr is pointing at to 10

>>> set_val 16 x 
// Set the value that x is pointing at to 16
```
___

### `visualize blocks`:
- **Description :** Show the metadata of all the blocks in the memory.
- **Usage :** `visualize_blocks`
- **Required Arguments:** None
- **Function called by the dispatcher :** `visualize_blocks_command`
- **Example :** 
```
>>> visualize_blocks 
// Prints the metadata for each block in the memory
```
___

### `visualize bytes`:
- **Description :** Prints all the memory. Also prints a legend to explain what different symbols mean.
- **Usage :** `visualize_bytes`
- **Required Arguments:** None
- **Function called by the dispatcher :** `visualize_bytes_command`
- **Example :** 
```
>>> visualize_bytes 
// Prints for every byte in memory information about it
```