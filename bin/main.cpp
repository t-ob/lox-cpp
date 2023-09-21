#include <iostream>
#include <stdexcept>
#include "Chunk.h"
#include "debug.h"
#include "Vm.h"

static void repl(Vm &vm) {
    std::string line;
    while (true) {
        std::cout << "> ";
        std::cin >> line;
        if (std::cin.eof()) {
            break;
        }
    }
}

int main(int argc, const char* argv[]) {
    Vm vm;
    vm.setMode(VmMode::DEBUG);

    if (argc == 1) {
        repl(vm);
    } else if (argc == 2) {
//        runFile(argv[1]);
    } else {
        throw(std::runtime_error("Usage: loc-cpp [path]\n"));
    }


//    Chunk c;
//    c.write_byte(static_cast<uint8_t>(OpCode::CONSTANT), 124);
//    c.write_byte(static_cast<uint8_t>(c.add_constant(3.0)), 124);
//    c.write_byte(static_cast<uint8_t>(OpCode::CONSTANT), 125);
//    c.write_byte(static_cast<uint8_t>(c.add_constant(4.0)), 125);
//    c.write_byte(static_cast<uint8_t>(OpCode::NEGATE), 126);
//    c.write_byte(static_cast<uint8_t>(OpCode::SUBTRACT), 127);
//    c.write_byte(static_cast<uint8_t>(OpCode::RETURN), 128);
//
//    vm.interpret(c);

    return 0;
}
