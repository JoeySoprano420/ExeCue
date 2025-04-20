#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(ASTNode* ast) : ast(ast) {}

void Interpreter::executeNode(ASTNode* node) {
    if (node->left && node->right) {
        executeNode(node->left);
        executeNode(node->right);
    }
    std::cout << node->value << " ";
}

void Interpreter::interpret() {
    executeNode(ast);
    std::cout << std::endl;
}
