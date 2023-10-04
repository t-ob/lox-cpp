#pragma once

#include <array>

#include "Chunk.h"
#include "Compiler.h"

constexpr size_t STACK_MAX = 1 << 10;

enum class InterpretResult : uint8_t {
    OK = 1,
    COMPILE_ERROR,
    RUNTIME_ERROR
};

enum class VmMode {
    STANDARD,
    DEBUG
};

class Vm {
private:
    Chunk chunk_;
    Compiler compiler_;
    size_t ip_;
    VmMode mode_;
    std::array<Value, STACK_MAX> stack_;
    size_t sp_;
    void pushStack(Value value);
    Value popStack();
    Value peekStack(size_t depth);

    template<typename... Args>
    void runtimeError(Args ...args);

    bool isFalsey(Value value);
public:
    Vm();
    void setMode(VmMode mode);
    InterpretResult interpret(const std::string &source);
    InterpretResult run();
};
