# Infix to Postfix to AST Converter and Evaluator

This project implements a C++ application that performs the following operations:
- Converts infix mathematical expressions to postfix expressions.
- Converts postfix expressions to an Abstract Syntax Tree (AST).
- Evaluates the AST with given variable values.

It leverages custom data structures for AST nodes and includes functionality for variable collection and basic mathematical functions.

## Project Structure

The core components of the project are:
- `AST_NODE.h` / `AST_NODE.cpp`: Defines the Abstract Syntax Tree (AST) node structure and its functionalities, including evaluation, printing, and variable handling.
- `InfixToPostfix.h` / `InfixToPostfix.cpp`: Implements the conversion logic from infix mathematical expressions to postfix notation.
- `PostfixToAST.h` / `PostfixToAST.cpp`: Handles the conversion of postfix expressions into an AST.
- `main.cpp`: Contains the main application logic, demonstrating the usage of Infix to Postfix conversion, Postfix to AST conversion, and AST evaluation with example expressions and variables.

## How to Build and Run Locally

### Prerequisites

You need a C++ compiler (like g++).

### Using Batch file for Windows CMD

1.  Navigate to the root directory of the project in your terminal.
2.  Run `build.bat` to compile the project:

    ```bash
    build-win
    ```

### Using Shell for MacOS and Linux environment

1. Navigate to the root directory of the project in your terminal
2. Run `build.sh` to compile the project:
    ```bash
    build
    ```

### Manual Compilation (Windows)

You can also compile the project manually using `g++`:
```bash
g++ -g main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp -o project
```
After compilation, run the executable:
```bash
project
```

or 

You can compile and then run in one command
```bash
g++ -g main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp -o project && project.exe
```

### Manual Compilation (macOS/Linux)

You can compile the project manually using `g++`:
```bash
g++ -g main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp -o project
```
After compilation, run the executable:
```bash
./project
```

or 

You can compile and then run in one command:
```bash
g++ -g main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp -o project && ./project
```

**Alternative using make:**
You could also create a simple `Makefile`:
```makefile
CC = g++
CFLAGS = -g
TARGET = project
SOURCES = main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
```

Then use:
```bash
make       # To compile
make run   # To compile and run
./project  # To run after compilation
make clean # To remove the executable
```

# GitHub Repository Cloning Guide

A step-by-step guide to clone a repository from GitHub, from initial setup to successful cloning.

## Prerequisites

Before you begin, you'll need:
- A GitHub account (sign up at [github.com](https://github.com))
- Git installed on your computer
- Command line/terminal access

## Step 1: Install Git

### **Windows**
1. Download Git from [git-scm.com](https://git-scm.com/download/win)
2. Run the installer with default settings (recommended for beginners)
3. Verify installation by opening Command Prompt or PowerShell and typing:
   ```bash
   git --version
   ```

### **macOS**
**Option A: Using Homebrew (recommended)**
1. Install Homebrew if you don't have it:
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```
2. Install Git:
   ```bash
   brew install git
   ```

**Option B: Using Git installer**
1. Download from [git-scm.com](https://git-scm.com/download/mac)
2. Follow installation wizard

### **Linux**
**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install git
```

**Fedora:**
```bash
sudo dnf install git
```

**Arch Linux:**
```bash
sudo pacman -S git
```

## Step 2: Configure Git (First-Time Setup)

Set up your identity for commit tracking:
```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

Optional: Set default branch name and enable color output:
```bash
git config --global init.defaultBranch main
git config --global color.ui auto
```

Verify your configuration:
```bash
git config --list
```

## Step 3: Generate SSH Key (Optional but Recommended)

For secure authentication without passwords:

1. Generate SSH key:
   ```bash
   ssh-keygen -t ed25519 -C "your.email@example.com"
   ```
   Press Enter to accept default file location and set a passphrase if desired.

2. Add SSH key to ssh-agent:
   ```bash
   eval "$(ssh-agent -s)"
   ssh-add ~/.ssh/id_ed25519
   ```

3. Copy your public key:
   - **Windows:** `clip < ~/.ssh/id_ed25519.pub`
   - **macOS:** `pbcopy < ~/.ssh/id_ed25519.pub`
   - **Linux:** `cat ~/.ssh/id_ed25519.pub` and copy the output

4. Add to GitHub:
   - Go to GitHub → Settings → SSH and GPG keys → New SSH key
   - Paste your key and save

## Step 4: Clone a Repository
1. Navigate to the repository on GitHub
2. Click the green "Code" button
3. Select "HTTPS" and copy the URL
4. Open terminal/command prompt
5. Navigate to where you want to clone the repository:
   ```bash
   cd /path/to/desired/location
   ```
6. Clone the repository:
   ```bash
   git clone https://github.com/Aljayz/Postfix-AST.git
   ```
7. Enter your GitHub credentials when prompted
