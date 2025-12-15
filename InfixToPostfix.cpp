#include "InfixToPostfix.h"
#include <iostream>
#include <cctype>
#include <stdexcept>

void InfixToPostfix::initPrecedence() {
    precedence = {{'^', 4}, {'*', 3}, {'/', 3}, {'%', 3}, {'+', 2}, {'-', 2}, {'(', 1}};
}

bool InfixToPostfix::isOp(char c) { 
    return precedence.find(c) != precedence.end() && c != '('; 
}

bool InfixToPostfix::isOperandChar(char c) { 
    return std::isalnum(c); 
}

int InfixToPostfix::prec(char op) { 
    return precedence[op]; 
}

void InfixToPostfix::handleOperator(char op) {
    while (!opStack.empty() && prec(opStack.top()) >= prec(op)) {
        output += opStack.top();
        output += ' ';
        opStack.pop();
    }
    opStack.push(op);
}

void InfixToPostfix::handleRightParen() {
    while (!opStack.empty() && opStack.top() != '(') {
        output += opStack.top();
        output += ' ';
        opStack.pop();
    }
    
    if (opStack.empty()) throw "Mismatched parentheses";
    opStack.pop();  // Remove '('
}

std::string InfixToPostfix::convertInfixToPostfix(const std::string& infix) {
    initPrecedence();
    opStack = std::stack<char>();  // Clear stack
    output.clear();
    std::cout << "Infix: " << infix << std::endl;
    
    for (size_t i = 0; i < infix.length(); ++i) {
        char token = infix[i];
        if (token == ' ') continue;
        
        if (isOperandChar(token)) {
            // Handle multi-character operands (like numbers or variable names)
            while (i < infix.length() && isOperandChar(infix[i])) {
                output += infix[i++];
            }
            output += ' ';
            i--;
        }
        else if (token == '(') {
            opStack.push(token);
        }
        else if (token == ')') {
            handleRightParen();
        }
        else if (isOp(token)) {
            handleOperator(token);
        }
        else {
            throw std::runtime_error("Invalid character in expression");
        }
    }
    
    // Pop remaining operators
    while (!opStack.empty()) {
        if (opStack.top() == '(') throw "Mismatched parentheses";
        output += opStack.top();
        output += ' ';
        opStack.pop();
    }
    
    return output;
}