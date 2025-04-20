#include "parser.h"

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentIndex(0) {}

Token Parser::currentToken() {
    return tokens[currentIndex];
}

void Parser::advance() {
    if (currentIndex < tokens.size()) {
        currentIndex++;
    }
}

ASTNode* Parser::parseExpression() {
    // Simplified parsing logic, just a demonstration
    ASTNode* left = new ASTNode(currentToken().value);
    advance();
    
    if (currentToken().type == TokenType::OPERATOR) {
        std::string op = currentToken().value;
        advance();
        ASTNode* right = parseExpression();
        ASTNode* expr = new ASTNode(op);
        expr->left = left;
        expr->right = right;
        return expr;
    }
    return left;
}

ASTNode* Parser::parse() {
    return parseExpression();
}
