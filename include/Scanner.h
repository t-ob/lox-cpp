#pragma once

#include <string>
#include "Token.h"

class Scanner {
private:
    std::string source;
    size_t start;
    size_t current;
    size_t line;
    [[nodiscard]] Token makeToken(TokenType type) const;
    [[nodiscard]] Token makeToken(TokenType type, std::optional<std::string>) const;
    bool isAtEnd();
    char advance();
    bool match(char expected);
public:
    Scanner() = delete;
    Scanner(std::string source);

    Token scanToken();
};