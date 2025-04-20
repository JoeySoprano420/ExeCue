#ifndef RUNTIME_H
#define RUNTIME_H

#include <string>

class Runtime {
public:
    Runtime();
    void initialize();
    void execute(const std::string& code);
    void cleanup();

private:
    std::string currentCode;
};

#endif // RUNTIME_H
