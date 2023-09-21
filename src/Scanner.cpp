
#include "Scanner.h"
#include "Token.h"

#include <utility>

Scanner::Scanner(std::string source) {
    this->source = std::move(source);
    start = 0;
    current = 0;
    line = 1;
}

Token Scanner::scanToken() {
    start = current;

    if (isAtEnd()) return makeToken(TokenType::END);

    auto c = advance();

    switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        default: {
            std::string message = "Unexpected character: ";
            message += c;
            return makeToken(TokenType::ERROR, message);
        }
    }

    return makeToken(TokenType::ERROR, "Unexpected character.");
}

Token Scanner::makeToken(TokenType type, std::optional<std::string> message) const {
    return {type, start, current - start, line, std::move(message)};
}

Token Scanner::makeToken(TokenType type) const {
    return {type, start, current - start, line, {}};
}

bool Scanner::isAtEnd() {
    return current == source.length();
}

char Scanner::advance() {
    current++;
    return source[current - 1];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    return true;
}