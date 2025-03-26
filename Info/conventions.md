# C Programming Conventions

___

## 1. Naming Conventions
- **Variables & Functions:** Use `snake_case` for readability.  
  - ✅ `int total_count;`  
  - ❌ `int totalCount;` *(CamelCase is not allowed)*  
- **Constants & Macros:** Use `UPPER_CASE` with underscores.  
  - ✅ `#define MAX_BUFFER_SIZE 1024`  
  - ❌ `#define MaxBufferSize 1024`  
- **Structs & Enums:** Use `PascalCase`, not `snake_case`.  
  - ✅ `typedef struct MemoryBlock { ... } MemoryBlock;`  
  - ❌ `typedef struct memory_block { ... } memory_block;`  
- **Global Variables:** Prefix with `g_` to indicate global scope.  
  - ✅ `int g_total_users;`  
- **Pointers:** Use the `p_` prefix to indicate a pointer. This is particularly important in this project because there is a `Pointer` struct. Without the `p_` prefix, it would be difficult to differentiate between C pointers and simulation pointers. 
  - ✅ `char *p_name;`
- **Arrays:** Use `arr_` prefix to indicate an array (including pointers to an array).  
  - ✅ `char arr_name[size];`
___

## 2. Formatting & Style
- **Indentation:** Use 4 spaces per indentation level (avoid tabs).  
- **Braces (`{}`) Placement:**  
  - ✅ **K&R Style**  
    ```c
    if (x == 10) {
        printf("X is 10\n");
    } else {
        printf("X is not 10\n");
    }
    ```
- **Line Length:** Keep lines under 80-100 characters.  
- **Spacing:** Use a single space around operators and after commas.  
  - ✅ `if (a == b && c > d) {`  
  - ❌ `if(a==b&&c>d){`  
- **Function Declarations:**  
  - ✅  
    ```c
    int add_numbers(int a, int b);
    ```  
  - ❌  
    ```c
    int add_numbers ( int a , int b ); // Bad, too much spaces
    ```  

___

## 3. Best Practices
- **Use `const` wherever possible**  
  - ✅ `const int MAX_USERS = 100;`
- **Use `enum` instead of `#define` for constants**  when it's a constant for multiple modules.
  - ✅  
    ```c
    typedef enum {
        STATUS_OK,
        STATUS_ERROR,
        STATUS_PENDING
    } Status;
    ```
- **Use `typedef` for complex structs**  
  - ✅  
    ```c
    typedef struct {
        int id;
        char name[50];
    } User;
    ```
- **Use `size_t` for sizes and loop indices**  
  - ✅ `for (size_t i = 0; i < length; i++)`
- **Use `static inline` for small functions**  
  - ✅ **Example:**  
    ```c
    static inline int square(int x) {
        return x * x;
    }
    ```  
  - **Why?**  
    - `static inline` makes the function **inline**, meaning the compiler replaces calls to it with its actual code, reducing function call overhead.  
    - `static` ensures the function is **only visible in the current translation unit**, avoiding conflicts across multiple files.  
    - Use `inline` for **small functions** (typically ≤ 5 lines) that are frequently called.  
    - Use `static` when defining **helper functions** that should not be accessible outside the translation unit.  
- **Avoid Magic Numbers** – use `#define` or `const`  
These are numbers that represent somthing beyond the number itself, eg. array length, a user choices, etc..
  - ✅  
    ```c
    #define MAX_FILES 256
    int files[MAX_FILES] = {0};
    ```  
  - ❌  
    ```c
    int files[256] = {0};  // **Magic number**
    ```  
- **Use `uint8_t` as a Boolean** (`0` = false, `1` = true)
  ```c
  uint8_t is_valid = 1;
  if (!is_valid) {
      printf("Invalid state\n");
  }
  ```
- **Use `free()` for every `malloc()`**  
  ```c
  int *data = malloc(100 * sizeof(int));
  
  ...
  
  if (data) {
      free(data);
  }
  ```
- **Set pointers to `NULL` after `free()`**  
  ```c
  free(ptr);
  ptr = NULL;
  ```

___

## 4. Function Documentation
- **Each time you modify a function (or add a new one), update the function in the documentation as needed.**
- Document function purpose, arguments, return values, and side effects.

___

## 5. Logging and Error Handling
- Instead of using basic error handling, use `print_error`, which will support:
  - Logging errors to a **file**
  - Printing errors to **console**
  - Configurable output based on an argument

Example usage:  
```c
print_error("Memory allocation failed");
```
___

## 6. `volatile` – What Is It and When to Use It?
**What does volatile mean?**  
`volatile` tells the compiler that a variable **can change unexpectedly**, so it should **not optimize it away** (mainly relevant for when **more advanced I/O** is added).

**Example – Without `volatile` (Bug!)**  
```c
int flag = 0;

void wait_for_signal() {
    while (flag == 0) {
        // Compiler may optimize this loop away since flag never seems to change
    }
}
```

**Example – Correct with `volatile`**  
```c
volatile int flag = 0;

void wait_for_signal() {
    while (flag == 0) {
        // Now, the compiler will always check flag's value
    }
}
```

Use `volatile` when dealing with:
- **Shared memory between threads**
- **Interrupt service routines (ISRs)**

___

## 7. `goto` – Why and when to use it
Most of the time, you **shouldn't** use it. But in rare cases, `goto` can **clean up** code by reducing deep nesting, especially for **error handling**. This is not mandatory, but it is helpful.

### Example – Without `goto` (Deep Nesting)
```c
char *buffer = malloc(100);
if (!buffer) return -1;

FILE *fp = fopen("file.txt", "r");
if (!fp) {
    free(buffer);
    return -1;
}

// More resources...

free(buffer);
fclose(fp);
return 0;
```

### Example – With `goto` (Cleaner Cleanup)
```c
char *buffer = malloc(100);
if (!buffer) return -1;

FILE *fp = fopen("file.txt", "r");
if (!fp) goto cleanup_buffer;

// More resources...

cleanup:
    free(buffer);
    fclose(fp);
    return 0;

cleanup_buffer:
    free(buffer);
    return -1;
```

**Why?**  
`goto` **reduces duplicated cleanup code** while keeping things readable. Use it **only for error handling**, never for normal control flow.


___

# Project repository organization:
**Project**
`- /include`
  - All the headers for all the source files excluding main (and more if needed).
- `/Info`
  - All documentation for the project. This includes commands, functions (for developers), and more.
- `/src`
  - All the source code for the project.
- `/Program.zip`
  - Includes the precompiled executable for windows.
- `Makefile`
  - makefile for the project, currently only set up for windows.
- `README.md`
  - The readme file for the repository.


**The project is fully open-source**, and should be accessable to whoever wants to view it's code.

___

## How to Contribute

To ensure that everyone can contribute effectively and that contributions align with the project's goals and structure, please follow the guidelines below:

### 1. **Fork the Repository**
   - Start by forking the repository to your own GitHub account. This allows you to work on the code and make changes independently.

### 2. **Set Up the Development Environment**
   - Ensure you have the necessary tools installed:
     - A C compiler (e.g., GCC or Clang)
     - Make or other build tools (depending on your OS)
     - Your platform's package manager for dependencies (if applicable)
   - Follow the instructions in the [installation guide](/Info/compile_and_install.md) to set up the project on your machine.
   - Test that the project builds correctly and runs without errors after setting it up.

### 3. **Choose an Issue or Feature to Work On**
   - Check the open issues in the repository and see if there’s something you'd like to work on.
   - If you have an idea for a new feature or improvement, feel free to propose it by creating a new issue.
   - If you're unsure, feel free to ask the project maintainers for guidance on where help is needed.

### 4. **Follow Coding Standards**
   - Follow the coding conventions outlined in the above.
   - Use clear, descriptive names for variables, functions, and files.
   - Ensure your code is well-commented, especially if it is implementing new or complex functionality.
   - Keep line lengths manageable (typically no more than 80-100 characters per line).

### 5. **Write Tests**
   - If you’re adding new features or modifying existing ones, please write tests to verify the functionality.
   - Test both your own feature and other features that interact with it.

### 6. **Make Your Changes**
   - Create a feature branch (e.g., `feature/some-new-feature`) and make your changes.
   - Commit your changes with clear, concise messages. Use the format:  
     `type: short description of the change`
     Example:  
     `feat: add malloc support for multiple blocks`

### 7. **Submit a Pull Request**
   - Once your changes are ready, submit a pull request (PR) to the main repository.
   - Provide a detailed description of the changes you made and why.
   - Link to the issue your PR addresses (if applicable) in the PR description.
   - If you're working on a large feature, please open a draft PR early so others can review your progress.

### 8. **Review and Feedback**
   - After submitting a PR, the project maintainers will review your changes and provide feedback.
   - Be open to suggestions and make improvements based on feedback.
   - If necessary, you may be asked to make changes or explain your approach further.

### 9. **Respect the License**
   - By contributing to Bytethon, you acknowledge that your code will be licensed under the **GPL**.
   - If you modify the code, ensure that the **GPL** license remains included and that credit is given.
