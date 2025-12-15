#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>
#include "InfixToPostfix.h"
#include "PostfixToAST.h"
#include <iomanip>

using namespace std;

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void printExpressionInfo(const std::string& postfix, const ASTNodePtr& ast) {
    std::cout << "Postfix expression: " << postfix << "\n";
    std::cout << "Infix expression: " << ast->toString() << "\n";
    
    // Collect and display variables
    std::vector<std::string> variables = ast->collectVariables();
    std::cout << "Variables in expression: ";
    if (variables.empty()) {
        std::cout << "(none)";
    } else {
        for (size_t i = 0; i < variables.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << variables[i];
        }
    }
    std::cout << "\n";
}

void evaluateWithVariables(const ASTNodePtr& ast, 
                          const std::vector<std::pair<std::string, double>>& variables,
                          const std::string& description = "") {
    if (!description.empty()) {
        std::cout << description << "\n";
    }
    
    std::cout << "Variable values: ";
    for (size_t i = 0; i < variables.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << variables[i].first << "=" << variables[i].second;
    }
    std::cout << "\n";
    
    try {
        // Validate variables
        bool valid = PostfixToAST::validateVariables(ast, variables);
        if (!valid) {
            std::cout << "Warning: Variable mismatch detected!\n";
        }
        
        double result = ast->evaluate(variables);
        std::cout << "Result: " << std::fixed << std::setprecision(6) << result << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}


// Main function - usage example
int main() {
    // Simple usage
    InfixToPostfix converter;
    
    std::string infixExpression = "a + b * (c - d) / e";
    std::string postfixExpression = converter.convertInfixToPostfix(infixExpression);
    
    std::cout << "Input (infix): " << infixExpression << std::endl;
    std::cout << "Output (postfix): " << postfixExpression << std::endl;
    
    // Postfix to AST
    try {
        // Infix to Postfix to AST
        printHeader("INF TO POST TO AST");
        std::string postAST = postfixExpression;
        ASTNodePtr astPost = PostfixToAST::convert(postAST);
        printExpressionInfo(postAST, astPost);
        printHeader("AST STRUCTURE (Default)");
        astPost->print();
        printHeader("AST STRUCTURE (Tree)");
        astPost->print(0, true);

        // Define Variables
        std::vector<std::pair<std::string, double>> postVars = {
            {"a", 2}, {"b", 3}, {"c", 1}, {"d", 4}, {"e", 5}
        };
        evaluateWithVariables(astPost, postVars);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}


