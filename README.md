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

You need a C++ compiler (like g++) and `make` installed on your system.

### Using Make

1.  Navigate to the root directory of the project in your terminal.
2.  Run `make` to compile the project:
    ```bash
    make
    ```
3.  Run the compiled executable:
    ```bash
    make run
    ```
    Alternatively, you can run `./project` directly after `make`.

### Manual Compilation (without Make)

You can also compile the project manually using `g++`:
```bash
g++ -g main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp -o project -std=c++11 -Wall
```
After compilation, run the executable:
```bash
./project
```

## GitHub Actions CI/CD

This project is configured with GitHub Actions for Continuous Integration and Continuous Deployment.

The workflow, defined in `.github/workflows/build.yml`, will automatically:
- Build the project using `make`.
- Run the compiled executable.

This process is triggered on every `push` to the `main` branch and for every `pull_request` targeting the `main` branch, ensuring that the code always builds and runs successfully.
