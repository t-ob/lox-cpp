#pragma once

#include <cstddef>
#include <optional>
#include <string>

enum class TokenType {
    ERROR, END, LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, SEMICOLON, COMMA, DOT, MINUS, PLUS, SLASH, STAR,
    BANG_EQUAL, BANG, EQUAL_EQUAL, EQUAL, LESS_EQUAL, LESS, GREATER_EQUAL, GREATER, STRING, NUMBER, IDENTIFIER, AND,
    CLASS, ELSE, IF, NIL, OR, PRINT, RETURN, SUPER, VAR, WHILE, FALSE, FOR, FUN, THIS, TRUE
};

class Token {
private:
    TokenType type_;
    size_t start_;
    size_t length_;
    size_t line_;
    std::optional<std::string> message_;
public:
    Token(TokenType type, size_t start, size_t length, size_t line, std::optional<std::string> message);
    [[nodiscard]] TokenType getType() const;
    [[nodiscard]] size_t getStart() const;
    [[nodiscard]] size_t getLine() const;
    [[nodiscard]] size_t getLength() const;
};