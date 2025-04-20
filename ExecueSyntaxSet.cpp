// EXECUE+ Language Specification Sheet in C++

#ifndef EXECUE_SPEC_HPP
#define EXECUE_SPEC_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>

namespace ExecueSpec {

// Token types
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    LITERAL,
    BRACKET,
    SEPARATOR,
    COMMENT,
    DIRECTIVE,
    WHITESPACE
};

// Opcode class
enum class Opcode {
    NOP,
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    JMP,
    JNZ,
    CALL,
    RET,
    HALT
};

// Memory Allocation Modes
enum class MemoryMode {
    VIRTUAL,
    STATIC,
    DYNAMIC,
    DOMINION
};

// Instruction Set Definition
struct Instruction {
    Opcode opcode;
    std::vector<std::string> operands;
    std::optional<std::string> label;
};

// Execution Profile
struct ExecProfile {
    size_t frameWidth = 1920;
    size_t frameHeight = 1080;
    size_t latencyNS = 5000;
    size_t cycleTimeNS = 2500;
    size_t maxFallbacks = 5;
    size_t memoryBytes = 4096;
};

// Semantic Rule
struct SemanticRule {
    std::string keyword;
    std::string structure;
    std::string constraint;
};

// Execution Pipeline
struct ExecutionPipeline {
    std::vector<Instruction> hotpath;
    std::vector<Instruction> fallback;
    std::vector<Instruction> dominionBind;
    std::vector<Instruction> renderStream;
};

// Parser Rule
struct SyntaxRule {
    std::string name;
    std::string startToken;
    std::string endToken;
    std::string matchPattern;
};

// Token
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
};

// Execution Directive
struct ExecDirective {
    std::string command;
    std::vector<std::string> parameters;
};

// Language Tagline
constexpr const char* TAGLINE = "EXECUE+ — Precision-Crafted Execution for Dominion-Synthetic Systems";

// Opcode Mapping Table
const std::unordered_map<std::string, Opcode> OpcodeMap = {
    {"NOP", Opcode::NOP},
    {"LOAD", Opcode::LOAD},
    {"STORE", Opcode::STORE},
    {"ADD", Opcode::ADD},
    {"SUB", Opcode::SUB},
    {"MUL", Opcode::MUL},
    {"DIV", Opcode::DIV},
    {"JMP", Opcode::JMP},
    {"JNZ", Opcode::JNZ},
    {"CALL", Opcode::CALL},
    {"RET", Opcode::RET},
    {"HALT", Opcode::HALT}
};

} // namespace ExecueSpec

#endif // EXECUE_SPEC_HPP
