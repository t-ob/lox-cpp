#include "Chunk.h"

void Chunk::write_byte(uint8_t byte, size_t line) {
    code.emplace_back(byte);
    lines.emplace_back(line);
}

size_t Chunk::size() const {
    return code.size();
}

uint8_t Chunk::at(size_t offset) const {
    return code.at(offset);
}

size_t Chunk::add_constant(Value value) {
    constants.emplace_back(value);
    return constants.size() - 1;
}

Value Chunk::constant_at(size_t offset) const {
    return constants.at(offset);
}

size_t Chunk::line_at(size_t offset) const {
    return lines.at(offset);
}
