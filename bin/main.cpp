#include <iostream>
#include "chunk.h"
#include "debug.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Chunk c;
    c.write_byte(static_cast<uint8_t>(OpCode::OP_RETURN), 123);
    c.write_byte(static_cast<uint8_t>(OpCode::OP_CONSTANT), 124);
    auto i = c.add_constant(2.8);
    c.write_byte(static_cast<uint8_t>(i), 124);
    disassembleChunk(c, "test chunk");

    return 0;
}
