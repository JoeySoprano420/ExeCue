#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

struct ASTNode {
    std::string value;
    ASTNode* left;
    ASTNode* right;

    ASTNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ASTNode* parse();

private:
    std::vector<Token> tokens;
    size_t currentIndex;

    Token currentToken();
    void advance();
    ASTNode* parseExpression();
};

#endif // PARSER_H
