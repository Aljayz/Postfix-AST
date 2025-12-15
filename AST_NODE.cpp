#include "ast_node.h"
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cctype>

// Constructor for number node
ASTNode::ASTNode(double value) : type(NodeType::NUMBER) {
    number.value = value;
}

// Constructor for variable node
ASTNode::ASTNode(const std::string& varName) : type(NodeType::VARIABLE) {
    new(&variable) VariableData{varName};
}

// Constructor for operator node
ASTNode::ASTNode(OperatorType op, ASTNodePtr left, ASTNodePtr right) : type(NodeType::BINARY_OP) {
    new(&this->op) OpData{op, left, right};
}

// Constructor for function call node
ASTNode::ASTNode(const std::string& funcName, const std::vector<ASTNodePtr>& args) : type(NodeType::FUNCTION_CALL) {
    new(&function) FunctionData{funcName, args};
}

// Destructor - needs to properly clean up union members
ASTNode::~ASTNode() {
    switch(type) {
        case NodeType::VARIABLE:
            variable.~VariableData();
            break;
        case NodeType::BINARY_OP:
            op.~OpData();
            break;
        case NodeType::UNARY_OP:
            op.~OpData();
            break;
        case NodeType::FUNCTION_CALL:
            function.~FunctionData();
            break;
        case NodeType::NUMBER:
            // No destructor needed for primitive type
            break;
    }
}

// Factory function for number node
ASTNodePtr ASTNode::createNumber(double value) {
    return std::make_shared<ASTNode>(value);
}

// Factory function for variable node
ASTNodePtr ASTNode::createVariable(const std::string& name) {
    return std::make_shared<ASTNode>(name);
}

// Factory function for binary operator node
ASTNodePtr ASTNode::createBinaryOp(OperatorType op, ASTNodePtr left, ASTNodePtr right) {
    return std::make_shared<ASTNode>(op, left, right);
}

// Factory function for unary operator node
ASTNodePtr ASTNode::createUnaryOp(OperatorType op, ASTNodePtr operand) {
    auto node = std::make_shared<ASTNode>(op, operand, nullptr);
    node->type = NodeType::UNARY_OP;
    return node;
}

// Factory function for function call node
ASTNodePtr ASTNode::createFunctionCall(const std::string& funcName, const std::vector<ASTNodePtr>& args) {
    return std::make_shared<ASTNode>(funcName, args);
}

// Evaluate the AST with VariableMap
double ASTNode::evaluate(const VariableMap& variables) const {
    switch(type) {
        case NodeType::NUMBER:
            return number.value;
            
        case NodeType::VARIABLE: {
            // Find variable value in map
            std::string varName = variable.name;
            auto it = variables.find(varName);
            if (it != variables.end()) {
                return it->second;
            }
            throw std::runtime_error("Undefined variable: " + varName);
        }
            
        case NodeType::BINARY_OP: {
            double leftVal = op.left->evaluate(variables);
            double rightVal = op.right->evaluate(variables);
            
            switch(op.op) {
                case OperatorType::ADD: return leftVal + rightVal;
                case OperatorType::SUBTRACT: return leftVal - rightVal;
                case OperatorType::MULTIPLY: return leftVal * rightVal;
                case OperatorType::DIVIDE: 
                    if (rightVal == 0) throw std::runtime_error("Division by zero");
                    return leftVal / rightVal;
                case OperatorType::POWER: return std::pow(leftVal, rightVal);
                default: throw std::runtime_error("Unknown binary operator");
            }
        }
            
        case NodeType::UNARY_OP: {
            double val = op.left->evaluate(variables);
            switch(op.op) {
                case OperatorType::NEGATIVE: return -val;
                default: throw std::runtime_error("Unknown unary operator");
            }
        }
            
        case NodeType::FUNCTION_CALL: {
            // For now, we'll handle basic functions
            std::string funcName = function.functionName;
            std::vector<double> args;
            for (const auto& arg : function.arguments) {
                args.push_back(arg->evaluate(variables));
            }
            
            // Basic function implementations
            if (funcName == "sin" && args.size() == 1) {
                return std::sin(args[0]);
            } else if (funcName == "cos" && args.size() == 1) {
                return std::cos(args[0]);
            } else if (funcName == "sqrt" && args.size() == 1) {
                if (args[0] < 0) throw std::runtime_error("Square root of negative number");
                return std::sqrt(args[0]);
            } else if (funcName == "log" && args.size() == 1) {
                if (args[0] <= 0) throw std::runtime_error("Log of non-positive number");
                return std::log(args[0]);
            } else if (funcName == "exp" && args.size() == 1) {
                return std::exp(args[0]);
            } else if (funcName == "abs" && args.size() == 1) {
                return std::abs(args[0]);
            } else {
                throw std::runtime_error("Unknown function or wrong number of arguments: " + funcName);
            }
        }
            
        default:
            throw std::runtime_error("Unknown node type");
    }
}

// Overloaded evaluate function for vector of pairs
double ASTNode::evaluate(const std::vector<std::pair<std::string, double>>& variables) const {
    // Convert vector of pairs to unordered_map for faster lookup
    VariableMap varMap;
    for (const auto& [name, value] : variables) {
        varMap[name] = value;
    }
    return evaluate(varMap);
}

// Print the AST with indentation
void ASTNode::print(int indent, bool tree) const {
    if (tree) {
        // Print root value with initial indentation
        printIndent(indent);
        switch (type) {
            case NodeType::NUMBER:
                std::cout << number.value << std::endl;
                break;
            case NodeType::VARIABLE:
                std::cout << variable.name << std::endl;
                break;
            case NodeType::BINARY_OP:
            case NodeType::UNARY_OP:
                std::cout << opToString(op.op) << std::endl;
                break;
            case NodeType::FUNCTION_CALL:
                std::cout << function.functionName << "()" << std::endl;
                break;
        }

        // Get children
        std::vector<ASTNodePtr> children;
        if (type == NodeType::BINARY_OP) {
            if (op.left) children.push_back(op.left);
            if (op.right) children.push_back(op.right);
        } else if (type == NodeType::UNARY_OP) {
            if (op.left) children.push_back(op.left);
        } else if (type == NodeType::FUNCTION_CALL) {
            children = function.arguments;
        }
        
        std::string prefix(indent, ' ');
        // Recurse on children
        for (size_t i = 0; i < children.size(); ++i) {
            children[i]->printTree(prefix, i == children.size() - 1);
        }

    } else {
        printIndent(indent);
        
        switch(type) {
            case NodeType::NUMBER:
                std::cout << "Number: " << number.value << std::endl;
                break;
            
            case NodeType::VARIABLE:
                std::cout << "Variable: " << variable.name << std::endl;
                break;
                
            case NodeType::BINARY_OP:
                std::cout << "Binary Op: " << opToString(op.op) << std::endl;
                op.left->print(indent + 2);
                op.right->print(indent + 2);
                break;
                
            case NodeType::UNARY_OP:
                std::cout << "Unary Op: " << opToString(op.op) << std::endl;
                op.left->print(indent + 2);
                break;
                
            case NodeType::FUNCTION_CALL:
                std::cout << "Function Call: " << function.functionName << "(";
                for (size_t i = 0; i < function.arguments.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << function.arguments[i]->toString();
                }
                std::cout << ")" << std::endl;
                for (const auto& arg : function.arguments) {
                    arg->print(indent + 2);
                }
                break;
        }
    }
}

// Convert AST to string representation
std::string ASTNode::toString() const {
    std::stringstream ss;
    
    switch(type) {
        case NodeType::NUMBER:
            // Remove trailing zeros
            ss << number.value;
            {
                std::string str = ss.str();
                size_t dotPos = str.find('.');
                if (dotPos != std::string::npos) {
                    // Remove trailing zeros after decimal point
                    str = str.substr(0, str.find_last_not_of('0') + 1);
                    if (str.back() == '.') str.pop_back();
                }
                return str;
            }
            
        case NodeType::VARIABLE:
            ss << variable.name;
            break;
            
        case NodeType::BINARY_OP: {
            bool needParentheses = false;
            
            // Check if we need parentheses for left child
            if (op.left->type == NodeType::BINARY_OP || op.left->type == NodeType::UNARY_OP) {
                int leftPrecedence = getPrecedence(
                    op.left->type == NodeType::BINARY_OP ? op.left->op.op : 
                    op.left->type == NodeType::UNARY_OP ? op.left->op.op : OperatorType::NONE
                );
                int currentPrecedence = getPrecedence(op.op);
                needParentheses = leftPrecedence < currentPrecedence;
            }
            
            if (needParentheses) ss << "(";
            ss << op.left->toString();
            if (needParentheses) ss << ")";
            
            ss << " " << opToString(op.op) << " ";
            
            needParentheses = false;
            if (op.right->type == NodeType::BINARY_OP || op.right->type == NodeType::UNARY_OP) {
                int rightPrecedence = getPrecedence(
                    op.right->type == NodeType::BINARY_OP ? op.right->op.op : 
                    op.right->type == NodeType::UNARY_OP ? op.right->op.op : OperatorType::NONE
                );
                int currentPrecedence = getPrecedence(op.op);
                needParentheses = rightPrecedence <= currentPrecedence;
            }
            
            if (needParentheses) ss << "(";
            ss << op.right->toString();
            if (needParentheses) ss << ")";
            break;
        }
            
        case NodeType::UNARY_OP:
            if (op.op == OperatorType::NEGATIVE) {
                ss << "-" << op.left->toString();
            } else {
                ss << opToString(op.op) << "(" << op.left->toString() << ")";
            }
            break;
            
        case NodeType::FUNCTION_CALL:
            ss << function.functionName << "(";
            for (size_t i = 0; i < function.arguments.size(); ++i) {
                if (i > 0) ss << ", ";
                ss << function.arguments[i]->toString();
            }
            ss << ")";
            break;
    }
    
    return ss.str();
}

// Check if operator is unary
bool ASTNode::isUnaryOperator(OperatorType op) {
    return op == OperatorType::NEGATIVE;
}

// Get operator precedence
int ASTNode::getPrecedence(OperatorType op) {
    switch(op) {
        case OperatorType::POWER: return 4;
        case OperatorType::NEGATIVE: return 3; // Unary minus has high precedence
        case OperatorType::MULTIPLY:
        case OperatorType::DIVIDE: return 2;
        case OperatorType::ADD:
        case OperatorType::SUBTRACT: return 1;
        default: return 0;
    }
}

// Convert operator to string
std::string ASTNode::opToString(OperatorType op) {
    switch(op) {
        case OperatorType::ADD: return "+";
        case OperatorType::SUBTRACT: return "-";
        case OperatorType::MULTIPLY: return "*";
        case OperatorType::DIVIDE: return "/";
        case OperatorType::POWER: return "^";
        case OperatorType::NEGATIVE: return "-";
        default: return "?";
    }
}

// Collect all variable names in the expression
std::vector<std::string> ASTNode::collectVariables() const {
    std::vector<std::string> variables;
    collectVariablesRecursive(variables);
    
    // Remove duplicates and sort
    std::sort(variables.begin(), variables.end());
    variables.erase(std::unique(variables.begin(), variables.end()), variables.end());
    
    return variables;
}

// Recursive helper to collect variables
void ASTNode::collectVariablesRecursive(std::vector<std::string>& vars) const {
    switch(type) {
        case NodeType::VARIABLE:
            vars.push_back(variable.name);
            break;
            
        case NodeType::BINARY_OP:
            op.left->collectVariablesRecursive(vars);
            op.right->collectVariablesRecursive(vars);
            break;
            
        case NodeType::UNARY_OP:
            op.left->collectVariablesRecursive(vars);
            break;
            
        case NodeType::FUNCTION_CALL:
            for (const auto& arg : function.arguments) {
                arg->collectVariablesRecursive(vars);
            }
            break;
            
        default:
            // Numbers don't contain variables
            break;
    }
}

// Check if expression contains any variables
bool ASTNode::hasVariables() const {
    return hasVariablesRecursive();
}

// Recursive helper to check for variables
bool ASTNode::hasVariablesRecursive() const {
    switch(type) {
        case NodeType::VARIABLE:
            return true;
            
        case NodeType::BINARY_OP:
            return op.left->hasVariablesRecursive() || op.right->hasVariablesRecursive();
            
        case NodeType::UNARY_OP:
            return op.left->hasVariablesRecursive();
            
        case NodeType::FUNCTION_CALL:
            for (const auto& arg : function.arguments) {
                if (arg->hasVariablesRecursive()) return true;
            }
            return false;
            
        default:
            return false;
    }
}

// Helper function for printing indentation
void ASTNode::printIndent(int indent) const {
    for (int i = 0; i < indent; ++i) {
        std::cout << " ";
    }
}
void ASTNode::printTree(const std::string& prefix, bool isTail) const {
    std::cout << prefix << (isTail ? "|--- " : "|--- ");

    // Print node value
    switch (type) {
        case NodeType::NUMBER:
            std::cout << number.value << std::endl;
            break;
        case NodeType::VARIABLE:
            std::cout << variable.name << std::endl;
            break;
        case NodeType::BINARY_OP:
        case NodeType::UNARY_OP:
            std::cout << opToString(op.op) << std::endl;
            break;
        case NodeType::FUNCTION_CALL:
            std::cout << function.functionName << "()" << std::endl;
            break;
    }

    // Get children
    std::vector<ASTNodePtr> children;
    if (type == NodeType::BINARY_OP) {
        if (op.left) children.push_back(op.left);
        if (op.right) children.push_back(op.right);
    } else if (type == NodeType::UNARY_OP) {
        if (op.left) children.push_back(op.left);
    } else if (type == NodeType::FUNCTION_CALL) {
        children = function.arguments;
    }

    // Recurse on children
    for (size_t i = 0; i < children.size(); ++i) {
        children[i]->printTree(prefix + (isTail ? "    " : "|   "), i == children.size() - 1);
    }
}