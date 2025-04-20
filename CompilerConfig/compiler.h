#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"

class Compiler {
public:
    Compiler(ASTNode* ast);
    void compile();

private:
    ASTNode* ast;
    void generateCode(ASTNode* node);
};

#endif // COMPILER_H
