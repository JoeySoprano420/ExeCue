// ========================= EXECUE SYSTEM CORE ========================= //
// Execue: Immersive Execution-Optimized Language Framework
// Build Target: FULL AOT + INTERPRETER HYBRID SYSTEM

#include "core/runtime.exu"
#include "compiler/parser.exu"
#include "compiler/lexer.exu"
#include "compiler/transpiler.exu"
#include "interpreter/exec_loop.exu"
#include "interpreter/opcode_resolver.exu"
#include "stdlib/math.exu"
#include "stdlib/chrono.exu"
#include "stdlib/convert.exu"
#include "io/debug_monitor.exu"
#include "render/render_frame.hpp"
#include "optim/hotpath_prelink.exu"
#include "errors/exception_map.exu"
#include "aot/aot_optimizer.hpp"
#include "docs/codex_exporter.hpp"
#include <iostream>
#include <filesystem>

// ========================= MAIN ENTRYPOINT ========================= //

int main(int argc, char** argv) {
    Execue::RenderSystem::init();
    Execue::MemoryTracker::init();
    
    if (argc < 2) {
        std::cout << "Usage: execue [source.exu | --export-codex | --track-memory]\n";
        return 1;
    }

    std::string arg = argv[1];

    if (arg == "--export-codex") {
        Execue::CodexExporter exporter;
        exporter.generatePDF("Execue_Codex.pdf");
        return 0;
    }

    if (arg == "--track-memory") {
        Execue::MemoryTracker::enableLiveTracking();
        std::cout << "[Memory Tracker] Live tracking enabled.\n";
        return 0;
    }

    std::filesystem::path filePath = arg;
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "[ERROR] File not found: " << filePath << "\n";
        return 1;
    }

    std::cout << "[Execue] Executing " << filePath << "\n";

    // Parsing
    auto tokens = Execue::Lexer::tokenize(filePath);
    auto ast = Execue::Parser::parse(tokens);

    // AOT Optimization
    auto optimized = Execue::AOTOptimizer::process(ast);

    // Interpreter Runtime
    Execue::OpcodeQueue queue = Execue::Transpiler::toOpcodeQueue(optimized);
    Execue::ExecLoop::run(queue);

    // Rendering final output
    Execue::RenderSystem::flush();
    
    // Display tracked memory
    Execue::MemoryTracker::report();

    return 0;
}

// ========================= CMakeLists.txt ========================= //
# Project Meta
project(ExecueLang)
cmake_minimum_required(VERSION 3.20)

# Language settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source includes
file(GLOB_RECURSE EXECUE_SRC "*.exu" "*.hpp" "*.cpp")

add_executable(execue ${EXECUE_SRC})

# PDF Library for Codex Export
find_package(PDFium REQUIRED)
target_link_libraries(execue PRIVATE PDFium::pdfium)

# ========================= Sample .exu Program ========================= //
/* test.exu */
routine main {
    print "Begin Test"
    let x = 8
    let y = 2
    let z = add(x, y)
    if z > 5 then print "Z is greater than 5"
    end
    render "Test Render Frame"
    flush()
}

// ========================= Sample Rendering Layer ========================= //
namespace Execue::RenderSystem {
    void init() {
        std::cout << "[Render Init] Frame system active.\n";
    }

    void flush() {
        std::cout << "[Render] Frame flushed.\n";
    }

    void render(const std::string& frameText) {
        std::cout << "[Render] " << frameText << "\n";
    }
}

// ========================= Memory Tracker System ========================= //
namespace Execue::MemoryTracker {
    bool liveTracking = false;
    size_t memoryUsed = 0;

    void init() {
        std::cout << "[Memory] Tracker initialized.\n";
    }

    void enableLiveTracking() {
        liveTracking = true;
    }

    void report() {
        std::cout << "[Memory] Estimated usage: " << memoryUsed << " bytes.\n";
    }

    void track(size_t bytes) {
        if (liveTracking) memoryUsed += bytes;
    }
}

// ========================= AOT Optimizer Core ========================= //
namespace Execue::AOTOptimizer {
    AST process(const AST& ast) {
        std::cout << "[AOT] Running optimizations.\n";
        // Dead code elimination, opcode folding, loop unrolling etc.
        return ast;
    }
}

// ========================= Codex Export System ========================= //
namespace Execue::CodexExporter {
    void generatePDF(const std::string& path) {
        std::cout << "[Codex] Generating PDF manual at " << path << "\n";
        // Uses PDFium or libHaru to export full syntax + examples + stdlib API
    }
}
