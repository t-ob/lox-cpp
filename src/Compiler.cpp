#include "Compiler.h"

#include <utility>
#include "Scanner.h"

void Compiler::compile(std::string source) {
    Scanner scanner(std::move(source));
    int line = -1;
    auto token = scanner.scanToken();
    }
