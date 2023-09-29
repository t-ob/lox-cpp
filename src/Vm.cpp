#include <iostream>
#include <utility>

#include "Vm.h"
#include "debug.h"

InterpretResult Vm::interpret(const std::string &source) {
    compiler_.compile(source);
    return run();
}

InterpretResult Vm::run() {
    while (true) {
        if (mode_ == VmMode::DEBUG) {
            std::cout << "Stack contents:\t";
            for (auto i = 0; i < sp_; ++i) {
                std::cout << "[ " << stack_[i] << " ]";
            }
            std::cout << std::endl;
            disassembleInstruction(chunk_, ip_);
        }
        auto instruction = chunk_.at(ip_++);
        switch (static_cast<OpCode>(instruction)) {
            case OpCode::RETURN: {
                auto ret = popStack();
                return InterpretResult::OK;
            }
            case OpCode::CONSTANT: {
                auto constant = chunk_.constant_at(chunk_.at(ip_++));
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

Vm::Vm() : ip_(0), sp_(0), mode_(VmMode::STANDARD) {
    stack_.fill(0);
}

void Vm::setMode(VmMode mode) {
    this->mode_ = mode;
}

void Vm::pushStack(Value value) {
    stack_[sp_++] = value;
}

Value Vm::popStack() {
    return stack_[--sp_];
}
