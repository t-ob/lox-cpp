#pragma once

#include <cstddef>
#include <optional>
#include <string>

enum class TokenType {
    ERROR, END, LEFT_PAREN, RIGHT_PAREN
};

class Token {
private:
    TokenType type;
    size_t start;
    size_t length;
    size_t line;
    std::optional<std::string> message;
public:
    Token() = delete;
    Token(TokenType type, size_t start, size_t length, size_t line, std::optional<std::string> message);
};