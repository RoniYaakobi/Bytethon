# Bytethon

___

## What is Bytethon?

**Have you ever wondered** how a VM can be built with an integrated interpreted language? Bytethon aims to create something like this.

___

## TL;DR:
This is a memory simulator virtual machine which aims to integrate a custom scripting language (Bytethon like Python) licensed under the GPL. For developer documentation, look at [The info directory.](/Info/)
___

**Bytethon is a project written in c**, which aims to simulate **memory management**. The goal is to in time turn the project into **a full scripting language**, with **simulated RAM** (an array/malloced pointer as the memory pool) and a **VM (Virtual Machine)**. The commands are executed on the virtual machine line by line, similar to how python interprets line by line. This project aims to minimize the use of `malloc()` from `stdlib`, only for things like dynamic hashmaps for the CLI and such. On starting the simulation, you will be prompted, asking if you want to use stack memory or heap memory for the simulation, explaining the difference between them. Keep in mind that if you choose **heap**, the program uses `malloc()`, so treat the stack memory option as proof of concept.

___


## How to use this program?

There is an in-depth explanation in [`/Info/compile_and_install.md`](/Info/compile_and_install.md) for how to install on **Linux**, **Mac**, or **Windows**. **Only if you are on Windows**, simply right-click the file **program.zip** in the app **"File Explorer"** (in the same folder as this file), click `extract all`, and wait for the `.zip` to extract. Once the folder has been extracted, open the folder program, and double click program.exe. This will open the program as a console application, which means you write a command and press enter to make the program do something. There is a list of all available commands **[here](Info/cli_commands.md)**. 

**When you start up the application**, you need to enter which type of memory you want. This shouldn't matter to much generally, but if you want **more memory**, you need to use the **heap** option **(over 1000 times more memory available)**.

In examples for commands, `>>> cmd` means that the program will run a command `cmd`, and `//` mean that this section is a comment, an explanation of what happens when running the command. Here is an example for a session:
```
>>> new_pointer ptr
// Create a pointer "ptr" which allows you to interact with the memory

>>> malloc 16 ptr
// Get 16 bytes of allocated memory, which effectively means you have 
// 16 bytes (or 128 bits) to use for your needs

>>> set_val 255 ptr
// The last byte in ptr's block of allocated memory becomes 255
// This command limits your value size to 255 currently, but will let you 
// store larger values in the future

>>> free ptr
// Free the pointer's allocated memory, which is important as if you don't do it
// You create a memory leak, where the memory you allocated can't be used, 
// but also isn't being used.
// Don't worry though, when you close the program all of the memory 
// is automatically freed.
```

These are the most important currently supported commands, but there will be more in the future. Errors are printed to the user in the console.

___


## How does it work?

Currently the project uses an array of `uint8_t` in order to store raw memory, and an array of `Block` structs in order to track which blocks of memory are allocated.
You traverse the array of blocks in order to locate a place to allocate for malloc, and free is made by effectively removing the pointer from the hashmap of pointers (name -> pointer), and removing the Block from the `blocks` array which corresponds to the pointer. For more info look at directory [Info](/Info/), espcially at the [Structs enums and Macro Doumentation](/Info/structs,%20enums,%20and%20macros.md).


This project allows **customization** for how much **memory** it uses, it is **designed to limit the memory** so that if you use the max amount allowed, and your comupter crashed from memory overload, it likely is at least 15 years old. If it does crash anyways from memory overload, simply choose a smaller size for the memory the next time.

___


## Why make this project?

You may be wondering why I made this project, and also maybe why the name of the project is `Bytethon`. The reason why I made their project is that, I decided that I want to make a custom coding language, and wanted to design my own interpreter with a custom VM. Right now this project is a **very** early prototype of the VM, but at some point the goal is to make the ability to write external code, to be parsed into a bytecode type of file, and then for this bytecode to be run on the VM.

As for why the name `Bytethon`, there are a few reasons:
- Byte, since I intend to include low level functions like malloc and free, which allow you to interact with the enviroment on a byte level
- thon, since I want to model the syntax after python (at least the names of the core functions eg print, input ...).
- Also byte sounds like bite, and I have a funny image of the computer biting into the instructions of the program in order to execute them (I know that this is not how it actually works, or even close to it, but it's still funny).

All in all I think about this project as something fun that I can make, and something that I can learn a lot from. 

___


## What is the timeline for this project:
This project is currently in the pre-alpha stage. This means that some of the core of the project is done, but it is mostly still a work in progress. Here is a general timeline for the project's features:

___

### Pre-Alpha:

- [x] Created the basic memory management functions (95% done)
- [x] Built the base of the CLI
- [x] Implemented basic garbage collection system (merging adjacent block on freeing a block)
- [x] Built documentation for all the current features

___


### Alpha:

- [ ] Multi-byte values in blocks
- [ ] Adding arithmetic and bitwise operations
- [ ] Writing code in a file 
- [ ] Advanced debugging using logging to files
- [ ] More advanced garbage collection
- [ ] Creating a more efficient method to create commands for the CLI (likely a script that will generate a file based on a json of commands)
- [ ] Implementing conditionals, loops, and functions
- [ ] Creating command aliases

___


### Beta:

- [ ] Building better syntax
- [ ] Including more advanced I/O (Files, Sockets, etc...)
- [ ] Improving documentation
- [ ] Multi-platform support

___


### Stable Release (and complete):

- [ ] Allowing custom syntax
- [ ] Creating libraries for the language
- [ ] Allowing sys calls if needed
- [ ] Adding the ability to use custom c or assembly code in the Bytethon script.
- [ ] Maybe adding dynamic typing.

___

### Where could this project be used? 

If there will ever be a use for Bytethon, it will likely be purely educational, as this language will likely lack high optimization, but it will hopefully include in depth explanations for basic to intermidate concepts of **Computer Science**.

___

### What are the guidelines for thie project?
If you want to contribue to the project, feel free to do so, but first read the documentation at [/Info/conventions.md](/Info/conventions.md). **A contribution which lacks these conventions will not be accepted.** Also keep in mind that this project is licensed under the **GPL**. This means that you can effectively do anything with the code, as long as you acknowledge the origin and the **GPL** license is included in the modifyied / copied code. **For a full version of the license go to** [The GNU's official website](https://www.gnu.org/licenses/quick-guide-gplv3.html).