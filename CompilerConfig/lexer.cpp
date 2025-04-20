#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>

enum class TokenType {
    IDENTIFIER,
    KEYWORD,
    NUMBER,
    OPERATOR,
    PUNCTUATION,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t index;
    int line;
    int column;

    char currentChar();
    void advance();
    bool isEnd();
    Token nextToken();
};

#endif // LEXER_H
