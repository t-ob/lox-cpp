#include <string>
#include <iostream>
#include "chunk.h"
#include "debug.h"

OpCode u8_to_op_code(uint8_t value) {
    if (value < static_cast<int>(OpCode::OP_RETURN) || value > static_cast<int>(OpCode::OP_CONSTANT)) {
        throw std::invalid_argument("Invalid OpCode value: " + std::to_string(static_cast<int>(value)));
    }
    return static_cast<OpCode>(value);
}

std::string string_from_op_code(const OpCode &op_code) {
    switch (op_code) {
        case OpCode::OP_RETURN:
            return "OP_RETURN";
        case OpCode::OP_CONSTANT:
            return "OP_CONSTANT";
    }
}

std::pair<std::string, size_t> op_code_info(const OpCode &op_code) {
    switch (op_code) {
        case OpCode::OP_RETURN:
            return { "OP_RETURN", 1 };
        case OpCode::OP_CONSTANT:
            return { "OP_CONSTANT", 2 };
    }
}

void disassembleChunk(const Chunk &chunk, const std::string &name) {
    std::cout << "== " << name << " ==" << std::endl;
    size_t offset = 0;
    while (offset < chunk.size()) {
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
}

