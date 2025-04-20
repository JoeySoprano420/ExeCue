// EXECUE+ Full Instruction Set in C++
// Version 1.0 — VAC Systems
// Description: Complete opcode-level instruction interface with dominion-aware execution layers

#ifndef EXECUE_INSTRUCTION_SET_H
#define EXECUE_INSTRUCTION_SET_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace execue {

// Core instruction format
struct Instruction {
    uint8_t opcode;
    std::string mnemonic;
    std::vector<std::string> operands;
    std::function<void()> execute;
};

// Instruction category
enum class OpcodeType {
    READ,
    WRITE,
    LOGIC,
    JUMP,
    DIAGNOSTIC,
    CONTROL,
    MEMORY,
    DEVICE
};

// Execution context
struct ExecutionContext {
    uint32_t program_counter;
    std::unordered_map<std::string, int32_t> registers;
    std::vector<uint8_t> memory;
    std::vector<std::string> dominion_stack;
    std::vector<std::string> log_stack;
};

// Instruction registry
class InstructionSet {
public:
    void register_instruction(uint8_t opcode, const std::string& mnemonic, OpcodeType type, const std::function<void(ExecutionContext&)>& exec_func);
    void execute(uint8_t opcode, ExecutionContext& context);
    std::string get_mnemonic(uint8_t opcode) const;

private:
    struct RegisteredOp {
        std::string mnemonic;
        OpcodeType type;
        std::function<void(ExecutionContext&)> exec;
    };

    std::unordered_map<uint8_t, RegisteredOp> instructions;
};

// Macros to define instructions
#define DEFINE_INSTRUCTION(set, code, name, type, func) \
    set.register_instruction(code, name, type, func)

// Sample Instructions
inline void define_core_instructions(InstructionSet& set) {
    DEFINE_INSTRUCTION(set, 0x01, "LOAD", OpcodeType::READ, [](ExecutionContext& ctx) {
        ctx.registers["A"] = ctx.memory[ctx.program_counter + 1];
        ctx.program_counter += 2;
    });

    DEFINE_INSTRUCTION(set, 0x02, "STORE", OpcodeType::WRITE, [](ExecutionContext& ctx) {
        ctx.memory[ctx.program_counter + 1] = static_cast<uint8_t>(ctx.registers["A"]);
        ctx.program_counter += 2;
    });

    DEFINE_INSTRUCTION(set, 0x10, "JMP", OpcodeType::JUMP, [](ExecutionContext& ctx) {
        ctx.program_counter = ctx.memory[ctx.program_counter + 1];
    });

    DEFINE_INSTRUCTION(set, 0xFF, "HALT", OpcodeType::CONTROL, [](ExecutionContext& ctx) {
        throw std::runtime_error("Execution Halted");
    });
}

} // namespace execue

#endif // EXECUE_INSTRUCTION_SET_H
