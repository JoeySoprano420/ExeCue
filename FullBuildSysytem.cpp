// File: CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(ExecueCompiler)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "include/*.h"
)

add_executable(execue ${SOURCES})

target_include_directories(execue PUBLIC include)

# Link libraries or rendering backends here if extended

# File: include/render/RenderLayer.h
#pragma once
#include <string>

class RenderLayer {
public:
    void render(const std::string& content);
};

// File: src/render/RenderLayer.cpp
#include "render/RenderLayer.h"
#include <iostream>

void RenderLayer::render(const std::string& content) {
    std::cout << "[EXECUE RENDER]:\n" << content << std::endl;
}

// File: include/interpreter/ExecInterpreter.h
#pragma once
#include <string>
#include "render/RenderLayer.h"
#include "stdlib/MathStdLib.h"

class ExecInterpreter {
public:
    ExecInterpreter();
    void interpret(const std::string& exuSource);
private:
    RenderLayer renderer;
    std::string preloadStdLib();
};

// File: src/interpreter/ExecInterpreter.cpp
#include "interpreter/ExecInterpreter.h"
#include <sstream>

ExecInterpreter::ExecInterpreter() {}

void ExecInterpreter::interpret(const std::string& exuSource) {
    std::string fullSource = preloadStdLib() + "\n" + exuSource;
    std::stringstream ss;
    ss << "[INTERPRETATION RESULT]\n" << fullSource;
    renderer.render(ss.str());
}

std::string ExecInterpreter::preloadStdLib() {
    return MathStdLib::getBuiltIn();
}

// File: include/stdlib/MathStdLib.h
#pragma once
#include <string>

class MathStdLib {
public:
    static std::string getBuiltIn();
};

// File: src/stdlib/MathStdLib.cpp
#include "stdlib/MathStdLib.h"

std::string MathStdLib::getBuiltIn() {
    return R"EXU(
routine add {
    operand a
    operand b
    return a + b
}

routine multiply {
    operand x
    operand y
    return x * y
}

routine subtract {
    operand x
    operand y
    return x - y
}

routine divide {
    operand x
    operand y
    return x / y
}
)EXU";
}

// File: src/main.cpp
#include "interpreter/ExecInterpreter.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: execue <filename.exu>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    std::string exuSource((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    ExecInterpreter interpreter;
    interpreter.interpret(exuSource);

    return 0;
}

// File: examples/hello.exu
routine main {
    write "Hello from Execue!"
    flush()
}
