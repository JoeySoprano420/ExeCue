#include "lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source)
    : source(source), index(0), line(1), column(1) {}

char Lexer::currentChar() {
    return index < source.size() ? source[index] : '\0';
}

void Lexer::advance() {
    if (currentChar() == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    index++;
}

bool Lexer::isEnd() {
    return currentChar() == '\0';
}

Token Lexer::nextToken() {
    while (!isEnd()) {
        char c = currentChar();

        if (isspace(c)) {
            advance();
            continue;
        }

        if (isdigit(c)) {
            std::string number;
            while (isdigit(currentChar())) {
                number += currentChar();
                advance();
            }
            return Token{TokenType::NUMBER, number, line, column};
        }

        if (isalpha(c)) {
            std::string identifier;
            while (isalnum(currentChar()) || currentChar() == '_') {
                identifier += currentChar();
                advance();
            }
            return Token{TokenType::IDENTIFIER, identifier, line, column};
        }

        if (c == '+' || c == '-' || c == '*' || c == '/') {
            advance();
            return Token{TokenType::OPERATOR, std::string(1, c), line, column};
        }

        // Handle punctuation (parentheses, etc.)
        if (c == '(' || c == ')') {
            advance();
            return Token{TokenType::PUNCTUATION, std::string(1, c), line, column};
        }

        advance();
    }
    return Token{TokenType::END_OF_FILE, "", line, column};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!isEnd()) {
        Token token = nextToken();
        if (token.type != TokenType::END_OF_FILE) {
            tokens.push_back(token);
        }
    }
    return tokens;
}
