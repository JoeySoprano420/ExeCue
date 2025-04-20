#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

class Interpreter {
public:
    Interpreter(ASTNode* ast);
    void interpret();

private:
    ASTNode* ast;
    void executeNode(ASTNode* node);
    std::string output;
};

#endif // INTERPRETER_H
