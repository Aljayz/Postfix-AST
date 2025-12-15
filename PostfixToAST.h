#ifndef POSTFIX_TO_AST_H
#define POSTFIX_TO_AST_H

#include "ast_node.h"
#include <vector>
#include <string>
#include <stack>
#include <memory>
#include <unordered_set>

class PostfixToAST {
public:
    // Convert postfix expression to AST
    static ASTNodePtr convert(const std::vector<std::string>& postfixTokens);
    
    // Convert postfix string (space-separated) to AST
    static ASTNodePtr convert(const std::string& postfixExpression);
    
    // Helper functions
    static std::vector<std::string> tokenize(const std::string& expression);
    static bool isOperator(const std::string& token);
    static bool isUnaryOperator(const std::string& token);
    static bool isNumber(const std::string& token);
    static bool isVariable(const std::string& token);
    static bool isFunction(const std::string& token);
    static OperatorType stringToOperator(const std::string& op);
    
    // Variable extraction
    static std::vector<std::string> extractVariables(const std::vector<std::string>& postfixTokens);
    static std::vector<std::string> extractVariables(const std::string& postfixExpression);
    
    // Validation
    static bool validateVariables(const ASTNodePtr& ast, 
                                 const std::vector<std::pair<std::string, double>>& variables);
    
private:
    // Process a token from postfix expression
    static void processToken(const std::string& token, std::stack<ASTNodePtr>& stack);
    
    // Valid function names
    static const std::unordered_set<std::string> validFunctions;
};

#endif // POSTFIX_TO_AST_H