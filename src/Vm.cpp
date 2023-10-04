#include <iostream>

#include "Vm.h"
#include "debug.h"

InterpretResult Vm::interpret(const std::string &source) {
    Chunk chunk;

    auto compileResult = compiler_.compile(source, chunk);

    if (compileResult == CompileResult::ERROR) {
        return InterpretResult::COMPILE_ERROR;
    }

    chunk_ = chunk;
    ip_ = 0;

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
                auto v = peekStack(0);
                if (!v.isNumber()) {
                    runtimeError("Operand must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                pushStack(Value::Number(-popStack().asNumber()));
                break;
            }
            case OpCode::ADD: {
                auto rhs = peekStack(0);
                if (!rhs.isNumber()) {
                    runtimeError("Right hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto lhs = peekStack(1);
                if (!lhs.isNumber()) {
                    runtimeError("Left hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto b = popStack().asNumber();
                auto a = popStack().asNumber();
                pushStack(Value::Number(a + b));
                break;
            }
            case OpCode::SUBTRACT: {
                auto rhs = peekStack(0);
                if (!rhs.isNumber()) {
                    runtimeError("Right hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto lhs = peekStack(1);
                if (!lhs.isNumber()) {
                    runtimeError("Left hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto b = popStack().asNumber();
                auto a = popStack().asNumber();
                pushStack(Value::Number(a - b));
                break;
            }
            case OpCode::MULTIPLY: {
                auto rhs = peekStack(0);
                if (!rhs.isNumber()) {
                    runtimeError("Right hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto lhs = peekStack(1);
                if (!lhs.isNumber()) {
                    runtimeError("Left hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto b = popStack().asNumber();
                auto a = popStack().asNumber();
                pushStack(Value::Number(a * b));
                break;
            }
            case OpCode::DIVIDE: {
                auto rhs = peekStack(0);
                if (!rhs.isNumber()) {
                    runtimeError("Right hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto lhs = peekStack(1);
                if (!lhs.isNumber()) {
                    runtimeError("Left hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto b = popStack().asNumber();
                auto a = popStack().asNumber();
                pushStack(Value::Number(a / b));
                break;
            }
            case OpCode::NIL: {
                pushStack(Value::Nil()); break; }
            case OpCode::TRUE: {
                pushStack(Value::Boolean(true)); break; }
            case OpCode::FALSE: {
                pushStack(Value::Boolean(false)); break; }
            case OpCode::NOT: {
                pushStack(Value::Boolean(isFalsey(popStack())));
                break;
            }
            case OpCode::EQUAL: {
                auto b = popStack();
                auto a = popStack();
                pushStack(Value::Boolean(a == b));
                break;
            }
            case OpCode::GREATER: {
                auto rhs = peekStack(0);
                if (!rhs.isNumber()) {
                    runtimeError("Right hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto lhs = peekStack(1);
                if (!lhs.isNumber()) {
                    runtimeError("Left hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto b = popStack().asNumber();
                auto a = popStack().asNumber();
                pushStack(Value::Boolean(a > b));
                break;
            }
            case OpCode::LESS: {
                auto rhs = peekStack(0);
                if (!rhs.isNumber()) {
                    runtimeError("Right hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto lhs = peekStack(1);
                if (!lhs.isNumber()) {
                    runtimeError("Left hand side to operator must be a number.");
                    return InterpretResult::RUNTIME_ERROR;
                }
                auto b = popStack().asNumber();
                auto a = popStack().asNumber();
                pushStack(Value::Boolean(a < b));
                break;
            }
        }
    }
}

Vm::Vm() : ip_(0), sp_(0), mode_(VmMode::STANDARD), stack_() {
    stack_.fill(Value::Number(0.0));
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

Value Vm::peekStack(size_t depth) {
    return stack_[sp_ - 1 - depth];
}

bool Vm::isFalsey(Value value) {
    return value.isNil() || (value.isBool() && !value.asBool());
}

// Base case for recursion
void runtimeErrorImpl(std::ostream& os) {}

template <typename T, typename... Args>
void runtimeErrorImpl(std::ostream& os, T value, Args... args) {
    os << value;
    runtimeErrorImpl(os, args...);
}

template<typename... Args>
void Vm::runtimeError(Args... args) {
    runtimeErrorImpl(std::cerr, args...);
    std::cerr << std::endl;

    auto instruction = chunk_.at(ip_ - 1);
    auto line = chunk_.line_at(instruction);
    std::cerr << "[line " << line << "] in script" << std::endl;

    sp_ = 0;
}
