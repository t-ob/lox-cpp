#include <iostream>
#include "Chunk.h"
#include "debug.h"
#include "Vm.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    Vm vm;
    vm.setMode(VmMode::DEBUG);

    Chunk c;
    c.write_byte(static_cast<uint8_t>(OpCode::CONSTANT), 124);
    c.write_byte(static_cast<uint8_t>(c.add_constant(3.0)), 124);
    c.write_byte(static_cast<uint8_t>(OpCode::CONSTANT), 125);
    c.write_byte(static_cast<uint8_t>(c.add_constant(4.0)), 125);
    c.write_byte(static_cast<uint8_t>(OpCode::NEGATE), 126);
    c.write_byte(static_cast<uint8_t>(OpCode::SUBTRACT), 127);
    c.write_byte(static_cast<uint8_t>(OpCode::RETURN), 128);

    vm.interpret(c);

    return 0;
}
