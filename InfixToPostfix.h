#ifndef INFIX_TO_POSTFIX_H
#define INFIX_TO_POSTFIX_H

#include <string>
#include <stack>
#include <unordered_map>

class InfixToPostfix {
private:
    std::unordered_map<char, int> precedence;
    std::stack<char> opStack;
    std::string output;
    
    void initPrecedence();
    bool isOp(char c);
    bool isOperandChar(char c);
    int prec(char op);
    void handleOperator(char op);
    void handleRightParen();

public:
    std::string convertInfixToPostfix(const std::string& infix);
};

#endif // INFIX_TO_POSTFIX_H