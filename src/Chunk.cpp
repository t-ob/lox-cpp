#include "Chunk.h"

void Chunk::write_byte(uint8_t byte, size_t line) {
    code_.emplace_back(byte);
    lines_.emplace_back(line);
}

size_t Chunk::size() const {
    return code_.size();
}

uint8_t Chunk::at(size_t offset) const {
    return code_.at(offset);
}

size_t Chunk::add_constant(Value value) {
    constants_.emplace_back(value);
    return constants_.size() - 1;
}

Value Chunk::constant_at(size_t offset) const {
    return constants_.at(offset);
}

size_t Chunk::line_at(size_t offset) const {
    return lines_.at(offset);
}
