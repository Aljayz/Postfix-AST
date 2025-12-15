#ifndef AST_NODE_H
#define AST_NODE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

// Forward declaration
class ASTNode;

// shared_ptr for automatic memory management
using ASTNodePtr = std::shared_ptr<ASTNode>;

// variable map type for easier lookup
using VariableMap = std::unordered_map<std::string, double>;

// Types of AST nodes
enum class NodeType {
    NUMBER,
    VARIABLE,
    BINARY_OP,
    UNARY_OP,
    FUNCTION_CALL
};

// Types of operations
enum class OperatorType {
    ADD,        // +
    SUBTRACT,   // -
    MULTIPLY,   // *
    DIVIDE,     // /
    POWER,      // ^
    NEGATIVE,   // Unary minus
    NONE
};

// AST Node class
class ASTNode {
public:
    NodeType type;
    
    // Union-like structure for different node types
    struct NumberData {
        double value;
    };
    
    struct VariableData {
        std::string name;
    };
    
    struct OpData {
        OperatorType op;
        ASTNodePtr left;
        ASTNodePtr right;
    };
    
    struct FunctionData {
        std::string functionName;
        std::vector<ASTNodePtr> arguments;
    };
    
    // Data can be one of these
    union {
        NumberData number;
        VariableData variable;
        OpData op;
        FunctionData function;
    };
    
    // Constructors
    ASTNode(double value);
    ASTNode(const std::string& varName);
    ASTNode(OperatorType op, ASTNodePtr left, ASTNodePtr right);
    ASTNode(const std::string& funcName, const std::vector<ASTNodePtr>& args);
    
    // Destructor
    ~ASTNode();
    
    // Prevent copying (use shared_ptr instead)
    ASTNode(const ASTNode&) = delete;
    ASTNode& operator=(const ASTNode&) = delete;
    
    // Helper functions
    static ASTNodePtr createNumber(double value);
    static ASTNodePtr createVariable(const std::string& name);
    static ASTNodePtr createBinaryOp(OperatorType op, ASTNodePtr left, ASTNodePtr right);
    static ASTNodePtr createUnaryOp(OperatorType op, ASTNodePtr operand);
    static ASTNodePtr createFunctionCall(const std::string& funcName, const std::vector<ASTNodePtr>& args);
    
    // Evaluation functions
    double evaluate(const VariableMap& variables = {}) const;
    double evaluate(const std::vector<std::pair<std::string, double>>& variables) const;
    
    // Display functions
    void print(int indent = 0, bool tree = false) const;
    std::string toString() const;
    
    // Helper functions
    static bool isUnaryOperator(OperatorType op);
    static int getPrecedence(OperatorType op);
    static std::string opToString(OperatorType op);
    
    // Variable collection
    std::vector<std::string> collectVariables() const;
    
    // Check if expression contains variables
    bool hasVariables() const;
    
private:
    void printTree(const std::string& prefix, bool isTail) const;
    void printIndent(int indent) const;
    void collectVariablesRecursive(std::vector<std::string>& vars) const;
    bool hasVariablesRecursive() const;
};

#endif // AST_NODE_H