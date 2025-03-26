#include "cli.h"

HashMap init_commands(size_t size) {
   HashMap hashmap = init_hashmap(size);
    // Create manually all the command structs needed for the hashmap of commands. TODO make this automated with json files and a loop instead of manual
    Command my_malloc_cmd = create_cmd(
        2, // size and the pointer to allocate at TODO add a pointer linked list implementation where the free pointers are connected
        (Command_Func)my_malloc_command, 
        "malloc", // Name of command
        "Manually allocate with a pointer a certain amount of bytes,"
        " for exmaple : malloc 10 ptr", // Description of command and usage
        Memory_management
    );

    Command my_free_cmd = create_cmd(
        1,
        (Command_Func)my_free_command,
        "free",
        "Free an allocated pointer, for exmaple : free ptr",
        Memory_management
    );

    Command set_val_cmd = create_cmd(
        2,
        (Command_Func)set_val_command,
        "set_val",
        "Set the value of a pointer's pointed value to a number "
        "(0-255), for example set_val 10 ptr",
        Memory_management
    );

    Command new_pointer_cmd = create_cmd(
        1,
        (Command_Func)new_pointer_command,
        "new_pointer",
        "Create a new pointer with the name you choose, for example new ptr",
        Memory_management
    );

    Command help_cmd = create_cmd(
        0,
        (Command_Func)help_cmds_command,
        "help",
        "Outputs important info about each command",
        Command_managment
    );

    Command exit_cmd = create_cmd(
        0,
        (Command_Func)exit_program_bytethon,
        "exit",
        "Exit the program.",
        ALL
    );

    Command visualize_blocks_cmd = create_cmd(
        0,
        (Command_Func)visualize_blocks_command,
        "visualize_blocks",
        "Show allocated blocks",
        Memory_management
    );
    Command visualize_bytes_cmd = create_cmd(
        0,
        (Command_Func)visualize_bytes_command,
        "visualize_bytes",
        "Show all the memory",
        Memory_management
    );

    #define AMOUNT_OF_CMDS 8 // Macro down here so I remember to edit this number when adding new command until json automation
    // Initialize this command array 
    Command commands[AMOUNT_OF_CMDS] = {my_malloc_cmd, my_free_cmd, set_val_cmd, new_pointer_cmd, help_cmd, exit_cmd, visualize_blocks_cmd, visualize_bytes_cmd};

    for (size_t i = 0; i < AMOUNT_OF_CMDS; i++) {
        Command *curr = malloc(sizeof(Command)); //  Heap allocation for the commands
        if (!curr) { // Alocation failed
            fprintf(stderr, "Memory allocation failed for Command structs!\n");
            for (size_t j = 0; j < i; j++) { // Free all the previous allocations
                free(commands[j].p_name);
                commands[j].p_name = NULL;
                free(commands[j].p_description);
                commands[j].p_description = NULL;
                free(commands[j].cmd); 
                commands[j].cmd = NULL;
            }
            exit(1);
        }
        *curr = commands[i]; //  Copy contents to heap allocated pointer
    
        hashmap_insert(&hashmap, commands[i].p_name, curr,(uint8_t)1); //  Insert heap-allocated pointer, don't print warnings = true (or in this case 1)
    }
    

    return hashmap; 
}

Command create_cmd(size_t amount_of_arguments, Command_Func func, const char *name, const char *description, Command_Classification type_of_function) {
    Command cmd;
    cmd.amount_of_arguments = amount_of_arguments;
    switch(type_of_function) {
        case ALL:
            cmd.ALL_cmd = (ALL_Command_Func)func;
            break;
        case Memory_management:
            cmd.memo_cmd = (Memo_Command_Func)func;
            break;
        case Command_managment:
            cmd.hashmap_cmd = (Hashmap_Command_Func)func;
            break;
        default:
            cmd.cmd = func;
    }
    cmd.type_of_function = type_of_function;

    cmd.p_name = strdup(name);  //  Allocate and copy name 
    cmd.p_description = strdup(description); //  Allocate and copy description

    if (!cmd.p_name || !cmd.p_description) {
        fprintf(stderr, "Memory allocation failed for command strings!\n");
        exit(1);
    }

    return cmd;
}

void execute_command(Memory *p_memory, HashMap *p_commands, char *input){
    if (input == NULL || input[0] == '\0') {
        print_error("Empty input received. No command to execute.");
        return;
    }

    input[strcspn(input, "\n")] = '\0';  // Remove trailing newline
    
    char input_copy[MAX_INPUT_SIZE];  
    strncpy(input_copy, input, MAX_INPUT_SIZE - 1);  
    input_copy[MAX_INPUT_SIZE - 1] = '\0';

    
    int args_c = 0;
    char *args[10];  
    char *token = strtok(input, " "); // Tokenize the first word (the command's name)

    if (token == NULL) { 
        print_error("Could not detect a command.");
        return;
    }

    while (token != NULL && args_c < 10) {  // If token detecteed and still enough space to store more arguments
        args[args_c++] = token; // Store the token and then increment args_c
        token = strtok(NULL, " "); // Get new token from same string
    } 

    args[args_c] = NULL;  // Ensure null termination

    // Lookup the command in the hashmap
    Command *p_cmd = (Command*)hashmap_get(p_commands, args[0]);

    if (p_cmd == NULL) {
        print_error("No command '%s' found.", args[0]);
        return;
    }

    // Check argument count (excluding command name)
    if (args_c - 1 != p_cmd->amount_of_arguments) {
        print_error("Wrong amount of arguments ( %d ) for %s. Expected %d args.", args_c - 1, p_cmd->p_name, p_cmd->amount_of_arguments);
        return;
    }

    // Execute the command function
    if (p_cmd->memo_cmd) {
        if(p_cmd->type_of_function == Memory_management && p_cmd->memo_cmd){
            p_cmd->memo_cmd(p_memory, args_c - 1, args + 1);  // Exclude command name and use the args array excluding the command name
            return;
        }
        else if (p_cmd->type_of_function == Command_managment && p_cmd->hashmap_cmd){
            p_cmd->hashmap_cmd(p_commands, args_c - 1, args + 1);  // Exclude command name and use the args array excluding the command name
            return;
        }
        else if (p_cmd->type_of_function == ALL && p_cmd->ALL_cmd){
            p_cmd->ALL_cmd(p_memory, p_commands , args_c - 1, args + 1);  // Exclude command name and use the args array excluding the command name
            return;
        }
    }

    print_error("Could not access the function '%s'", p_cmd->p_name);
}

// Free the pointers to the names and description of the functions
void free_cmds(HashMap *p_map) {
    for (size_t i = 0; i < p_map->bucket_size; i++) { // Go over all the indexes in the hashmap

        HashNode *p_current = p_map->pp_map[i]; // Readability

        while (p_current != NULL) { // While the linked list is not finished
            
            Command *p_cmd = (Command*)p_current->p_value; // The command hashmap stores pointers to command structs
            if (p_cmd) {
                free(p_cmd->p_name);  //  Free allocated name
                p_cmd->p_name = NULL; 
                free(p_cmd->p_description);  //  Free allocated description
                p_cmd->p_description = NULL;
            }
            p_current = p_current->p_next;
        }
    }
}


// Gets the arguments from the execute_command 
// Converts the arguments to a usable form for the function
void help_cmds_command(HashMap *p_hashmap,int args_c, char *args[10]) {
    // No reason to make any checks here, it doesn't matter and anyways the user will not know at this point the syntax
    help_cmds(p_hashmap);
}

void help_cmds(HashMap *p_map) {

    for (size_t i = 0; i < p_map->bucket_size; i++) { // Go over all the indexes in the hashmap

        HashNode *p_current = p_map->pp_map[i];
        while (p_current != NULL) { // While the linked list is not finished
            
            Command *p_cmd = (Command*)p_current->p_value; // The command hashmap stores pointers to command structs

            if (p_cmd) {
                char management_type[20]; // Buffer to store the management type needed to print
                switch (p_cmd->type_of_function){
                    case ALL:
                        strcpy(management_type,"all");
                        break;
                    case Memory_management:
                        strcpy(management_type,"memmory");
                        break;
                    case Command_managment:
                        strcpy(management_type,"commands");
                        break;
                    default:
                        strcpy(management_type,"none");
                }

                printlnf("Command Name : %s", p_cmd->p_name);
                printlnf("Command Description : %s", p_cmd->p_description);
                printlnf("Amount of arguments : %d", p_cmd->amount_of_arguments);
                printlnf("Management type : %s", management_type);
                printlnf("-------------------------------------------------------------------------------------------");
            }
            p_current = p_current->p_next; // Next node
        }
    }
    printlnf("Look in directory /info for the file cli_commands.md for a more detailed explaination of each function");
}

void exit_program_bytethon(Memory *p_memory, HashMap *p_cmds, int args_c, char *args[10]) {

    free_cmds(p_cmds); // Frees the pointers to the name and description of the commmands, but not the nodes themselves
    hashmap_free(p_cmds); // Frees the whole hashmap (including all nodes)
    hashmap_free(p_memory->p_pointers); // Frees the whole hashmap (including all nodes)

    if (p_memory->on_heap) {
        free(p_memory->p_blocks);
        p_memory->p_blocks = NULL;
        free(p_memory->p_bytes);
        p_memory->p_bytes = NULL;
    }

    exit_program("Bytethon."); // Nice closing animation function from utils.h.
}