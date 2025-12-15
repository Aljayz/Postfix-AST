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

// Simple demonstration function
void demonstrateConversion() {
    InfixToPostfix converter;
    std::vector<std::string> testExpressions = {
        "a + b",
        "a + b * c",
        "(a + b) * c",
        "a + b * c - d",
        "3 + 4 * 2 / (1 - 5)",
        "a * (b + c) / d"
    };
    std::cout << "INFIX TO POSTFIX CONVERSION DEMO\n";
    std::cout << "================================\n";
    
    for (const auto& infix : testExpressions) {
        try {
            std::string postfix = converter.convertInfixToPostfix(infix);
            std::cout << "Infix: " << infix << "\n";
            std::cout << "Postfix: " << postfix << "\n\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error converting: " << infix << " - " << e.what() << "\n\n";
        }
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
    
    // Bulk usage
    // demonstrateConversion();
    

    // Postfix to AST
    try {
        // Infix to Postfix to AST
        printHeader("INF TO POST TO AST");
        std::string postAST = postfixExpression;
        ASTNodePtr astPost = PostfixToAST::convert(postAST);
        printExpressionInfo(postAST, astPost);
        printHeader("AST STRUCTURE");
        astPost->print();

        // Define Variables
        std::vector<std::pair<std::string, double>> postVars = {
            {"a", 2}, {"b", 3}, {"c", 1}, {"d", 4}, {"e", 5}
        };
        evaluateWithVariables(astPost, postVars);

        // Example 1: Expression with 5 variables
        printHeader("Example 1: Complex Expression with 5 Variables");
        std::string expr1 = "a b + c d - * e /";
        ASTNodePtr ast1 = PostfixToAST::convert(expr1);
        printExpressionInfo(expr1, ast1);
        
        // Define 5 variables
        std::vector<std::pair<std::string, double>> vars1 = {
            {"a", 2}, {"b", 3}, {"c", 1}, {"d", 4}, {"e", 5}
        };
        evaluateWithVariables(ast1, vars1);
        
        // Test with different values
        std::vector<std::pair<std::string, double>> vars1b = {
            {"a", 10}, {"b", 20}, {"c", 5}, {"d", 2}, {"e", 3}
        };
        evaluateWithVariables(ast1, vars1b, "\nWith different values:");
        
        // Example 2: Expression with power and multiple variables
        printHeader("Example 2: Expression with Power Operator");
        std::string expr2 = "x y ^ z *";
        ASTNodePtr ast2 = PostfixToAST::convert(expr2);
        printExpressionInfo(expr2, ast2);
        
        std::vector<std::pair<std::string, double>> vars2 = {
            {"x", 2}, {"y", 3}, {"z", 4}
        };
        evaluateWithVariables(ast2, vars2);
        
        // Example 3: More complex expression with unary minus
        printHeader("Example 3: Expression with Unary Minus");
        std::string expr3 = "a b c * + ~ d /";  // ~ represents unary minus
        ASTNodePtr ast3 = PostfixToAST::convert(expr3);
        printExpressionInfo(expr3, ast3);
        
        std::vector<std::pair<std::string, double>> vars3 = {
            {"a", 5}, {"b", 2}, {"c", 3}, {"d", 2}
        };
        evaluateWithVariables(ast3, vars3);
        
        // Example 4: Missing variable test
        printHeader("Example 4: Handling Missing Variables");
        std::string expr4 = "x y z + *";
        ASTNodePtr ast4 = PostfixToAST::convert(expr4);
        printExpressionInfo(expr4, ast4);
        
        // Try with missing variable
        std::vector<std::pair<std::string, double>> vars4 = {
            {"x", 2}, {"y", 3}  // z is missing
        };
        evaluateWithVariables(ast4, vars4);
        
        // Example 5: Simple arithmetic (no variables)
        printHeader("Example 5: Pure Arithmetic (No Variables)");
        std::string expr5 = "3 4 + 2 * 5 /";
        ASTNodePtr ast5 = PostfixToAST::convert(expr5);
        printExpressionInfo(expr5, ast5);
        
        // Evaluate without variables
        std::cout << "Result: " << ast5->evaluate() << "\n";
        
        // Example 6: Extract variables from expression
        printHeader("Example 6: Variable Extraction");
        std::string expr6 = "temp pressure volume * + R /";
        std::vector<std::string> extractedVars = PostfixToAST::extractVariables(expr6);
        std::cout << "Expression: " << expr6 << "\n";
        std::cout << "Extracted variables: ";
        for (const auto& var : extractedVars) {
            std::cout << var << " ";
        }
        std::cout << "\n";
        
        // Example 7: Expression with all operations
        printHeader("Example 7: All Operations");
        std::string expr7 = "a b + c * d e / - f ^";
        ASTNodePtr ast7 = PostfixToAST::convert(expr7);
        printExpressionInfo(expr7, ast7);
        
        std::vector<std::pair<std::string, double>> vars7 = {
            {"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 2}, {"f", 0.5}
        };
        evaluateWithVariables(ast7, vars7);
        
        // Example 8: AST Structure visualization
        printHeader("Example 8: AST Structure");
        std::string expr8 = "a b c * +";
        ASTNodePtr ast8 = PostfixToAST::convert(expr8);
        std::cout << "Postfix: " << expr8 << "\n";
        std::cout << "AST Structure:\n";
        ast8->print();
        
        // Example 9: Edge cases
        printHeader("Example 9: Edge Cases");
        
        // Single variable
        std::string expr9a = "x";
        ASTNodePtr ast9a = PostfixToAST::convert(expr9a);
        printExpressionInfo(expr9a, ast9a);
        std::vector<std::pair<std::string, double>> vars9a = {{"x", 42}};
        evaluateWithVariables(ast9a, vars9a);
        
        // Complex nested expression
        std::cout << "\nComplex nested expression:\n";
        std::string expr9b = "a b c + * d e * / f +";
        ASTNodePtr ast9b = PostfixToAST::convert(expr9b);
        printExpressionInfo(expr9b, ast9b);
        std::vector<std::pair<std::string, double>> vars9b = {
            {"a", 10}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}, {"f", 1}
        };
        evaluateWithVariables(ast9b, vars9b);
        
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}