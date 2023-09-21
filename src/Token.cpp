//
// Created by tom on 9/21/23.
//
#include "Token.h"

Token::Token(TokenType type, size_t start, size_t length, size_t line, std::optional<std::string> message) : type(type), start(start), length(length), line(line), message(message) {

}
