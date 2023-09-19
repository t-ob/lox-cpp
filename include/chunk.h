#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "value.h"

enum class OpCode : uint8_t { OP_RETURN = 1, OP_CONSTANT };

class Chunk {
private:
    std::vector<uint8_t> code;
    std::vector<Value> constants;
    std::vector<size_t> lines;
public:
    void write_byte(uint8_t byte, size_t line);
    size_t add_constant(Value value);
    [[nodiscard]] uint8_t at(size_t offset) const;
    [[nodiscard]] Value constant_at(size_t offset) const;
    [[nodiscard]] size_t line_at(size_t offset) const;
    [[nodiscard]] size_t size() const;
};