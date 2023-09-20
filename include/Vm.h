#pragma once

#include <array>

#include "Chunk.h"

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
    Chunk chunk;
    size_t ip;
    VmMode mode;
    std::array<Value, STACK_MAX> stack;
    size_t sp;
    void pushStack(Value value);
    Value popStack();
public:
    Vm();
//    Vm(VmMode mode);
    void setMode(VmMode mode);
    InterpretResult interpret(Chunk chunk);
    InterpretResult run();
};