#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <thread>
#include <mutex>

// Constants
const size_t MEMORY_SIZE = 1024;
const size_t STACK_SIZE = 512;

// Error Codes
enum ExecutionError {
    ERR_NONE = 0,
    ERR_INVALID_OPCODE,
    ERR_MEMORY_OVERFLOW,
    ERR_DIVIDE_BY_ZERO,
    ERR_STACK_OVERFLOW,
    ERR_UNKNOWN_ERROR
};

// Memory management class (simulating virtual memory)
class VirtualMemory {
public:
    VirtualMemory() : memory(MEMORY_SIZE, 0) {}

    void write(size_t address, int value) {
        if (address >= MEMORY_SIZE) {
            throw std::out_of_range("Memory address out of range");
        }
        memory[address] = value;
    }

    int read(size_t address) {
        if (address >= MEMORY_SIZE) {
            throw std::out_of_range("Memory address out of range");
        }
        return memory[address];
    }

private:
    std::vector<int> memory;
};

// The Instruction class represents the individual actions to be performed during execution
class Instruction {
public:
    std::string opcode;
    std::vector<int> operands;

    Instruction(const std::string& opcode, const std::vector<int>& operands)
        : opcode(opcode), operands(operands) {}

    void execute(class ExecEngine& engine);
};

// Execution Engine: Core of the virtual machine
class ExecEngine {
public:
    ExecEngine() : stack(STACK_SIZE), memory(std::make_unique<VirtualMemory>()), programCounter(0), running(true) {}

    void loadProgram(const std::vector<Instruction>& program) {
        instructions = program;
    }

    void run() {
        while (running && programCounter < instructions.size()) {
            const Instruction& instruction = instructions[programCounter];
            try {
                instruction.execute(*this);  // Execute the current instruction
            } catch (const std::exception& e) {
                std::cerr << "Execution Error: " << e.what() << std::endl;
                break;
            }
            programCounter++;
        }
    }

    void halt() {
        running = false;
    }

    void pushStack(int value) {
        if (stackPointer >= STACK_SIZE) {
            throw std::overflow_error("Stack Overflow");
        }
        stack[stackPointer++] = value;
    }

    int popStack() {
        if (stackPointer == 0) {
            throw std::underflow_error("Stack Underflow");
        }
        return stack[--stackPointer];
    }

    int readMemory(size_t address) {
        return memory->read(address);
    }

    void writeMemory(size_t address, int value) {
        memory->write(address, value);
    }

private:
    std::vector<Instruction> instructions;
    size_t programCounter;
    bool running;
    std::vector<int> stack;
    size_t stackPointer = 0;
    std::unique_ptr<VirtualMemory> memory;
};

// Executor for individual instructions
void Instruction::execute(ExecEngine& engine) {
    if (opcode == "ADD") {
        int operand1 = operands[0];
        int operand2 = operands[1];
        engine.pushStack(operand1 + operand2);  // Perform addition and push result
    }
    else if (opcode == "SUB") {
        int operand1 = operands[0];
        int operand2 = operands[1];
        engine.pushStack(operand1 - operand2);  // Perform subtraction and push result
    }
    else if (opcode == "MUL") {
        int operand1 = operands[0];
        int operand2 = operands[1];
        engine.pushStack(operand1 * operand2);  // Perform multiplication and push result
    }
    else if (opcode == "DIV") {
        int operand1 = operands[0];
        int operand2 = operands[1];
        if (operand2 == 0) {
            throw std::runtime_error("Division by zero");
        }
        engine.pushStack(operand1 / operand2);  // Perform division and push result
    }
    else if (opcode == "LOAD") {
        int address = operands[0];
        engine.pushStack(engine.readMemory(address));  // Load value from memory to stack
    }
    else if (opcode == "STORE") {
        int address = operands[0];
        int value = operands[1];
        engine.writeMemory(address, value);  // Store value into memory
    }
    else if (opcode == "HALT") {
        engine.halt();  // Halting the program
    }
    else {
        throw std::runtime_error("Unknown opcode: " + opcode);  // Invalid opcode
    }
}

// Execution management class
class ExecutionManager {
public:
    void executeProgram(const std::vector<Instruction>& program) {
        ExecEngine engine;
        engine.loadProgram(program);
        engine.run();
    }
};

// ErrorLogger class for managing runtime errors
class ErrorLogger {
public:
    static void logExecutionError(ExecutionError code, const std::string& message) {
        std::cerr << "Runtime Error (" << code << "): " << message << std::endl;
    }
};

// Example program to be executed
int main() {
    std::vector<Instruction> program = {
        Instruction("ADD", {5, 3}),
        Instruction("SUB", {10, 4}),
        Instruction("MUL", {2, 3}),
        Instruction("DIV", {9, 3}),
        Instruction("LOAD", {0}),  // Assuming value at address 0 is 12
        Instruction("STORE", {1, 99}),  // Storing 99 at memory address 1
        Instruction("HALT", {})
    };

    ExecutionManager execMgr;
    try {
        execMgr.executeProgram(program);
    } catch (const std::exception& e) {
        ErrorLogger::logExecutionError(ERR_UNKNOWN_ERROR, e.what());
    }

    return 0;
}
