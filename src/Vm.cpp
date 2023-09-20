#include <iostream>
#include <utility>

#include "Vm.h"
#include "debug.h"

InterpretResult Vm::interpret(Chunk c) {
    chunk = std::move(c);
    return run();
}

InterpretResult Vm::run() {
    while (true) {
        if (mode == VmMode::DEBUG) {
            std::cout << "Stack contents:\t";
            for (auto i = 0; i < sp; ++i) {
                std::cout << "[ " << stack[i] << " ]";
            }
            std::cout << std::endl;
            disassembleInstruction(chunk, ip);
        }
        auto instruction = chunk.at(ip++);
        switch (static_cast<OpCode>(instruction)) {
            case OpCode::RETURN: {
                auto ret = popStack();
                return InterpretResult::OK;
            }
            case OpCode::CONSTANT: {
                auto constant = chunk.constant_at(chunk.at(ip++));
                pushStack(constant);
                break;
            }
            case OpCode::NEGATE: {
                pushStack(-popStack());
                break;
            }
            case OpCode::ADD: {
                auto b = popStack();
                auto a = popStack();
                pushStack(a + b);
                break;
            }
            case OpCode::SUBTRACT: {
                auto b = popStack();
                auto a = popStack();
                pushStack(a - b);
                break;
            }
            case OpCode::MULTIPLY: {
                auto b = popStack();
                auto a = popStack();
                pushStack(a * b);
                break;
            }
            case OpCode::DIVIDE: {
                auto b = popStack();
                auto a = popStack();
                pushStack(a / b);
                break;
            }
        }
    }
}

Vm::Vm() : ip(0), sp(0), mode(VmMode::STANDARD) {
    stack.fill(0);
}

void Vm::setMode(VmMode mode) {
    this->mode = mode;
}

void Vm::pushStack(Value value) {
    stack[sp++] = value;
}

Value Vm::popStack() {
    return stack[--sp];
}
