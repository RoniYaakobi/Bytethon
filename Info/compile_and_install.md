# Compilation and Installation

In order to use this project you have two options:

1. **Use the precompiled executable** 
This is the simpler option, especially for Windows users. Just extract the executable from [`executable.zip`](/executable.zip).
<br><br>
2. **Manually compile** 
This option is more complex but allows you to make changes to the project or use it on macOS/Linux (currently only supporting Windows). The project comes with a Makefile for Windows, but you can also manually compile or create your own Makefile.

___

## Before choosing your approach:
You should install the project from git or github.
___

#### Using Git
First, make sure Git is installed on your system:

- To check if Git is installed, run the following command in your terminal:
    ```bash
    git --version
    ```
-  If Git is installed, it will output the version:
    ```bash
    git version 2.x.x
    ```
- If Git is not installed, you will see something like:
    ```bash
    command not found: git
    ```
In this case, follow the instructions below based on your operating system:

- **Linux**: Open a terminal and run:
```bash
sudo apt install git
```

- **macOS**: Open a terminal and run:
```bash
brew install git
```
 - **Windows**: Download and install from the [official Git website](https://git-scm.com/).
<br><br>
___

After confirming that Git is installed, run the following command to clone the repository:

```bash
git clone https://github.com/RoniYaakobi/Bytethon.git
```

This will clone the repository into `path/Bytethon`, where `path` is the directory you are currently in (e.g., `C:/Users/user`). Ensure you're in the correct directory before running the command.
<br><br>
___

#### Using Github
Alternatively, you can download the project directly from GitHub:
1. Go to [Bython's Repository on Github](https://github.com/RoniYaakobi/Bytethon)
2. Click the green [<> Code] button
3. Click `Download ZIP`
4. Once the download is complete, right click the file **Bytethon.zip** and select **"Extract All..."** (on **Windows**), or use the appropriate method for your OS.
<br><br>
___

## After downloading the files:
You can choose how to install/compile the project.

___

### 1. Using the precompiled executable (for Windows)
- If you're using **Windows**, go to the project directory and extract `executable.zip`.
- Inside this folder, you will find `Bytethon.exe`. You can move this file wherever you want on your **Windows machine** since it’s statically linked to the necessary libraries.
<br><br>
___

### 2. Manually compile (for advanced users)
If you want to manually compile the project or you're using macOS/Linux, follow these steps:
 - **Ensure you have a C compiler** installed on your computer.
 - If you're on macOS/Linux and/or not using GCC, you may need to modify the `Makefile` for your system/compiler. Note that this program uses **VLAs**, so make sure to enable them.
 - Alternatively, you can manually compile the files without using the `Makefile`.

___

Once you're ready, navigate to the folder containing the `Makefile` (for example for me the path is `C:\Users\user\Desktop\Bytethon\` which is the folder that contains the Makefile):
```bash
cd C:/path/to/Makefile/
```
Then, simply run the following command:
```bash
make
```

After this, the project will be compiled and ready to use.
<br><br>
___

**Note**: This project was primarily tested on **Windows**. If you're using **macOS** or **Linux**, you may need to modify some parts of the code or Makefile before compiling.
<br><br>
___
## Thank you for installing Bytethon!