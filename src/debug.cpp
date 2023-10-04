#include <string>
#include <iostream>
#include "Chunk.h"
#include "debug.h"

OpCode u8_to_op_code(uint8_t value) {
    return static_cast<OpCode>(value);
}

std::pair<std::string, size_t> op_code_info(const OpCode &op_code) {
    switch (op_code) {
        case OpCode::RETURN:
            return { "RETURN", 1 };
        case OpCode::CONSTANT:
            return { "CONSTANT", 2 };
        case OpCode::NEGATE:
            return { "NEGATE", 1 };
        case OpCode::ADD:
            return { "ADD", 1 };
        case OpCode::SUBTRACT:
            return { "SUBTRACT", 1 };
        case OpCode::MULTIPLY:
            return { "MULTIPLY", 1 };
        case OpCode::DIVIDE:
            return { "DIVIDE", 1 };
        case OpCode::NIL:
            return { "NIL", 1 };
        case OpCode::TRUE:
            return { "TRUE", 1 };
        case OpCode::FALSE:
            return { "FALSE", 1 };
        case OpCode::NOT:
            return { "NOT", 1 };
        case OpCode::EQUAL:
            return { "EQUAL", 1 };
        case OpCode::GREATER:
            return { "GREATER", 1 };
        case OpCode::LESS:
            return { "LESS", 1 };
    }
}

void disassembleInstruction(const Chunk &chunk, size_t offset) {
    auto byte = chunk.at(offset);
    auto op_code = u8_to_op_code(byte);
    auto info = op_code_info(op_code);
    std::cout << offset << '\t' << chunk.line_at(offset) << '\t' << info.first;
    auto n = info.second;
    for (auto i = 1; i < n; ++i) {
        std::cout << '\t' << chunk.constant_at(chunk.at(offset + i));
    }
    std::cout << std::endl;
    offset += n;
}

