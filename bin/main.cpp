#include <iostream>
#include <stdexcept>
#include "Chunk.h"
#include "debug.h"
#include "Vm.h"

static void repl(Vm &vm) {
    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (std::cin.eof()) {
            break;
        }
        vm.interpret(line);
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

    return 0;
}
