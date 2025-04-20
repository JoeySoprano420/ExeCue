#include "compiler.h"
#include <iostream>

Compiler::Compiler(ASTNode* ast) : ast(ast) {}

void Compiler::generateCode(ASTNode* node) {
    if (node->left && node->right) {
        generateCode(node->left);
        generateCode(node->right);
    }
    std::cout << "Generating code for: " << node->value << std::endl;
}

void Compiler::compile() {
    generateCode(ast);
    std::cout << "Compilation complete." << std::endl;
}
