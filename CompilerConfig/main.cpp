#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "renderer.h"
#include "runtime.h"
#include "compiler.h"

// Function to read a file into a string
std::string readFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: execue <filename.exu>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string source = readFile(filename);

    // Tokenization (lexing)
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    // Parsing
    Parser parser(tokens);
    ASTNode* ast = parser.parse();

    // Interpretation or Compilation
    Interpreter interpreter(ast);
    interpreter.interpret();

    // Rendering (this could be done through a rendering layer)
    Renderer renderer;
    renderer.render(interpreter.getOutput());

    return 0;
}

