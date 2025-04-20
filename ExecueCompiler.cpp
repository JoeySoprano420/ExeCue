// EXECUE: Full Compiler Core + Rendering System + .exu Executor
// Language: C++17+

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <thread>
#include <chrono>

namespace ExecueCore {
    using namespace std;

    enum class Opcode {
        NOP, LOAD, STORE, ADD, SUB, JMP, JZ, PRINT, HALT
    };

    struct Instruction {
        Opcode opcode;
        int operand;
    };

    class DominionVM {
        vector<Instruction> program;
        map<int, int> memory;
        int pc = 0;
        bool halted = false;
    public:
        void load(const vector<Instruction>& prog) {
            program = prog;
            pc = 0;
            halted = false;
        }

        void execute() {
            while (!halted && pc < program.size()) {
                const auto& instr = program[pc];
                switch (instr.opcode) {
                    case Opcode::NOP: break;
                    case Opcode::LOAD: memory[0] = instr.operand; break;
                    case Opcode::STORE: memory[instr.operand] = memory[0]; break;
                    case Opcode::ADD: memory[0] += instr.operand; break;
                    case Opcode::SUB: memory[0] -= instr.operand; break;
                    case Opcode::JMP: pc = instr.operand - 1; break;
                    case Opcode::JZ: if (memory[0] == 0) pc = instr.operand - 1; break;
                    case Opcode::PRINT: cout << "[VM] OUT: " << memory[instr.operand] << endl; break;
                    case Opcode::HALT: halted = true; break;
                }
                ++pc;
                this_thread::sleep_for(chrono::milliseconds(10));
            }
        }
    };

    class Parser {
    public:
        static vector<Instruction> parseEXU(const string& filename) {
            ifstream file(filename);
            if (!file) throw runtime_error("Cannot open file.");
            vector<Instruction> instructions;
            string line;
            while (getline(file, line)) {
                istringstream ss(line);
                string op;
                int val = 0;
                ss >> op >> val;
                Opcode code;
                if (op == "NOP") code = Opcode::NOP;
                else if (op == "LOAD") code = Opcode::LOAD;
                else if (op == "STORE") code = Opcode::STORE;
                else if (op == "ADD") code = Opcode::ADD;
                else if (op == "SUB") code = Opcode::SUB;
                else if (op == "JMP") code = Opcode::JMP;
                else if (op == "JZ") code = Opcode::JZ;
                else if (op == "PRINT") code = Opcode::PRINT;
                else if (op == "HALT") code = Opcode::HALT;
                else throw runtime_error("Invalid opcode: " + op);
                instructions.push_back({ code, val });
            }
            return instructions;
        }
    };

    class Renderer {
    public:
        static void splashScreen() {
            cout << "\nEXECUE - Dominion Engine Compiler and Runtime v1.0" << endl;
            cout << "Rendering Subsystem Initialized..." << endl;
            cout << "[✓] Terminal Render Layer Loaded" << endl;
        }
    };
}

int main(int argc, char** argv) {
    using namespace ExecueCore;
    Renderer::splashScreen();
    if (argc < 2) {
        cerr << "Usage: executar <file.exu>" << endl;
        return 1;
    }

    try {
        auto program = Parser::parseEXU(argv[1]);
        DominionVM vm;
        vm.load(program);
        vm.execute();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
