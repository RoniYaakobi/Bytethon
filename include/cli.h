#ifndef CLI_H
#define CLI_H


#include "general_management.h"
#define MAX_INPUT_SIZE 2048


// Defining different types of commands based on what the parser they correspond to needs
typedef void (*Command_Func)(int, char**);
typedef void (*Memo_Command_Func)(Memory*, int, char**);
typedef void (*Hashmap_Command_Func)(HashMap*, int, char**);
typedef void (*ALL_Command_Func)(Memory*, HashMap*, int, char**);

// Which type of function type to cast the parser function
typedef enum {
    ALL = 0,
    Memory_management = 1,
    Command_managment = 2
} Command_Classification;

// Stores help info along with info needed for the dispatcher (func field)
struct Command {
    char *p_name;
    char *p_description;
    union func {
        Command_Func cmd;
        Memo_Command_Func memo_cmd;
        Hashmap_Command_Func hashmap_cmd;
        ALL_Command_Func ALL_cmd;
    };
    size_t amount_of_arguments;
    Command_Classification type_of_function;
};

// Stores help info along with info needed for the dispatcher (func field)
typedef struct Command Command; 

// ^^^^ No clue why I need to typedef like this, gcc doesn't like it otherwise

// Initiate the commands name to Commands struct Hashmap with a bucket size of <size>
HashMap init_commands(size_t size);

// Constructer for Command struct
Command create_cmd(size_t amount_of_arguments, Command_Func func, const char *name, const char *description, Command_Classification type_of_function);

// General dispatcher for all commands, passes the input in the form of an argument count and arguments array to the parser for the relevant function
//
// Input : A memory struct pointer, a commands Hashmap and the command you want to run
//
// Output : Passes the arguments on to the relevent parser, which then handles it accordingly
void execute_command(Memory *p_memory, HashMap *p_commands, char *input);


// Free the cmds structs internal pointers like *name and *description, still need to use free_hashmap function afterwards
// This is done so that the Hashmap can be reused for a different usage afterwards (if needed)
void free_cmds(HashMap *p_map);

// Arguments parser for the help_cmds function
//
// Input : A pointer to the hashmap of commands and the amount of arguments and the arguments
//
// Output : Calls the function help_cmds if all the arguments are valid
void help_cmds_command(HashMap *p_hashmap,int args_c, char *args[10]);

// Prints help info for all the commands in *map
// For each command prints the name , the description, the amount of arguments needed, and the catagory it belongs to in terms of management
// Memory, Commands, etc...
void help_cmds(HashMap *p_map);

// Frees all the pointers malloced, and does a closing... animation.
void exit_program_bytethon(Memory *p_memory, HashMap *p_cmds,int args_c, char *args[10]); 

#endif // CLI_HS