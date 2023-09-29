#pragma once

#include <string>
#include "Token.h"

class Scanner {
private:
    std::string source_;
    size_t start_;
    size_t current_;
    size_t line_;
    [[nodiscard]] Token makeToken(TokenType type) const;
    [[nodiscard]] Token makeToken(TokenType type, std::optional<std::string>) const;
    bool isAtEnd();
    char advance();
    bool match(char expected);
    char peek();
    std::optional<char> peekNext();
    void skipWhitespace();

    Token string();

    bool isDigit(char c);

    Token number();

    bool isAlpha(char c);

    Token identifier();

    TokenType identifierType();

    TokenType checkKeyword(int start, int length, const char *rest, TokenType type);

public:

    Scanner() = delete;

    Scanner(std::string source);

    Token scanToken();

    [[nodiscard]] const std::string &getSource() const;
};