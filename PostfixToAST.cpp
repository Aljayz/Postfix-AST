#include "PostfixToAST.h"
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <unordered_set>

// Initialize valid functions set
const std::unordered_set<std::string> PostfixToAST::validFunctions = {
    "sin", "cos", "tan", "sqrt", "log", "exp", "abs", "min", "max"
};

// Convert postfix expression (vector of tokens) to AST
ASTNodePtr PostfixToAST::convert(const std::vector<std::string>& postfixTokens) {
    std::stack<ASTNodePtr> stack;
    
    for (const auto& token : postfixTokens) {
        processToken(token, stack);
    }
    
    if (stack.size() != 1) {
        throw std::runtime_error("Invalid postfix expression: Stack has " + 
                                std::to_string(stack.size()) + " elements instead of 1");
    }
    
    return stack.top();
}

// Convert postfix string to AST
ASTNodePtr PostfixToAST::convert(const std::string& postfixExpression) {
    std::vector<std::string> tokens = tokenize(postfixExpression);
    return convert(tokens);
}

// Tokenize a space-separated postfix expression
std::vector<std::string> PostfixToAST::tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    std::stringstream ss(expression);
    std::string token;
    
    while (ss >> token) {
        // Handle special case for negative numbers that might not be space-separated
        if (token.length() > 1 && token[0] == '-' && std::isdigit(token[1])) {
            tokens.push_back(token);
        } else {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

// Check if token is an operator
bool PostfixToAST::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || 
           token == "/" || token == "^" || token == "~"; // ~ for unary minus
}

// Check if token is a unary operator
bool PostfixToAST::isUnaryOperator(const std::string& token) {
    return token == "~";
}

// Check if token is a number (including negative numbers)
bool PostfixToAST::isNumber(const std::string& token) {
    if (token.empty()) return false;
    
    std::string_view view(token);
    
    // Handle negative numbers
    bool hasSign = false;
    if (view[0] == '-') {
        hasSign = true;
        view = view.substr(1);
        if (view.empty()) return false; // Just "-" is not a number
    }
    
    bool hasDecimal = false;
    bool hasDigit = false;
    
    for (char c : view) {
        if (c == '.') {
            if (hasDecimal) return false; // Multiple decimals
            hasDecimal = true;
        } else if (std::isdigit(c)) {
            hasDigit = true;
        } else {
            return false; // Invalid character
        }
    }
    
    return hasDigit; // Must have at least one digit
}

// Check if token is a variable (starts with letter, contains letters/numbers/underscores)
bool PostfixToAST::isVariable(const std::string& token) {
    if (token.empty() || !std::isalpha(token[0]) && token[0] != '_') {
        return false;
    }
    
    for (char c : token) {
        if (!std::isalnum(c) && c != '_') {
            return false;
        }
    }
    
    return true;
}

// Check if token is a function name
bool PostfixToAST::isFunction(const std::string& token) {
    return validFunctions.find(token) != validFunctions.end();
}

// Convert string operator to OperatorType
OperatorType PostfixToAST::stringToOperator(const std::string& op) {
    if (op == "+") return OperatorType::ADD;
    if (op == "-") return OperatorType::SUBTRACT;
    if (op == "*") return OperatorType::MULTIPLY;
    if (op == "/") return OperatorType::DIVIDE;
    if (op == "^") return OperatorType::POWER;
    if (op == "~") return OperatorType::NEGATIVE;
    
    throw std::runtime_error("Unknown operator: " + op);
}

// Extract variables from postfix expression
std::vector<std::string> PostfixToAST::extractVariables(const std::vector<std::string>& postfixTokens) {
    std::vector<std::string> variables;
    
    for (const auto& token : postfixTokens) {
        if (isVariable(token)) {
            variables.push_back(token);
        }
    }
    
    // Remove duplicates and sort
    std::sort(variables.begin(), variables.end());
    variables.erase(std::unique(variables.begin(), variables.end()), variables.end());
    
    return variables;
}

// Extract variables from postfix string
std::vector<std::string> PostfixToAST::extractVariables(const std::string& postfixExpression) {
    std::vector<std::string> tokens = tokenize(postfixExpression);
    return extractVariables(tokens);
}

// Validate that all variables in AST are provided
bool PostfixToAST::validateVariables(const ASTNodePtr& ast, 
                                     const std::vector<std::pair<std::string, double>>& variables) {
    // Extract variables from AST
    std::vector<std::string> astVars = ast->collectVariables();
    
    // Convert provided variables to set for quick lookup
    std::unordered_set<std::string> providedVars;
    for (const auto& [name, value] : variables) {
        providedVars.insert(name);
    }
    
    // Check if all AST variables are provided
    for (const auto& var : astVars) {
        if (providedVars.find(var) == providedVars.end()) {
            std::cerr << "Warning: Variable '" << var << "' is not provided in the variables list" << std::endl;
            return false;
        }
    }
    
    // Check for extra variables that aren't needed
    for (const auto& [name, value] : variables) {
        if (std::find(astVars.begin(), astVars.end(), name) == astVars.end()) {
            std::cerr << "Warning: Variable '" << name << "' is provided but not used in the expression" << std::endl;
        }
    }
    
    return true;
}

// Process a single token
void PostfixToAST::processToken(const std::string& token, std::stack<ASTNodePtr>& stack) {
    if (isNumber(token)) {
        // Push number node
        double value = std::stod(token);
        stack.push(ASTNode::createNumber(value));
    } 
    else if (isVariable(token)) {
        // Push variable node
        stack.push(ASTNode::createVariable(token));
    }
    else if (isOperator(token)) {
        if (isUnaryOperator(token)) {
            // Unary operator (e.g., unary minus)
            if (stack.empty()) {
                throw std::runtime_error("Not enough operands for unary operator: " + token);
            }
            
            ASTNodePtr operand = stack.top();
            stack.pop();
            stack.push(ASTNode::createUnaryOp(stringToOperator(token), operand));
        } 
        else {
            // Binary operator
            if (stack.size() < 2) {
                throw std::runtime_error("Not enough operands for binary operator: " + token);
            }
            
            ASTNodePtr right = stack.top();
            stack.pop();
            ASTNodePtr left = stack.top();
            stack.pop();
            
            stack.push(ASTNode::createBinaryOp(stringToOperator(token), left, right));
        }
    }
    else if (isFunction(token)) {
        // Function call - for postfix, functions come after their arguments
        // For simplicity, we'll assume functions take one argument in postfix
        if (stack.empty()) {
            throw std::runtime_error("Not enough arguments for function: " + token);
        }
        
        ASTNodePtr arg = stack.top();
        stack.pop();
        std::vector<ASTNodePtr> args = {arg};
        stack.push(ASTNode::createFunctionCall(token, args));
    }
    else {
        throw std::runtime_error("Invalid token: " + token);
    }
}