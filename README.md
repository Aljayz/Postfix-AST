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