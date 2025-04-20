// src/main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "interpreter/interpreter.h"
#include "render/renderer.h"
#include "stdlib/loader.h"

using namespace execue;

void runFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        renderError("Could not open file: " + filename);
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();

    Interpreter interpreter;
    interpreter.loadStandardLibrary();
    interpreter.execute(code);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: execue <filename.exu>\n";
        return 1;
    }

    std::string filename = argv[1];
    runFile(filename);

    return 0;
}
