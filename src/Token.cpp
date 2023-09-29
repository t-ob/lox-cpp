//
// Created by tom on 9/21/23.
//
#include "Token.h"

Token::Token(TokenType type, size_t start, size_t length, size_t line, std::optional<std::string> message) : type_(type), start_(start), length_(length), line_(line), message_(message) {

}

TokenType Token::getType() const {
    return type_;
}

size_t Token::getStart() const {
    return start_;
}

size_t Token::getLine() const {
    return line_;
}

size_t Token::getLength() const {
    return length_;
}
