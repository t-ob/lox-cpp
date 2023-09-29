
#include "Scanner.h"
#include "Token.h"

#include <utility>

Scanner::Scanner(std::string source) {
    this->source_ = std::move(source);
    start_ = 0;
    current_ = 0;
    line_ = 1;
}

Token Scanner::scanToken() {
    skipWhitespace();
    start_ = current_;

    if (isAtEnd()) return makeToken(TokenType::END);

    auto c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return makeToken(TokenType::LEFT_PAREN);
        case ')': return makeToken(TokenType::RIGHT_PAREN);
        case '{': return makeToken(TokenType::LEFT_BRACE);
        case '}': return makeToken(TokenType::RIGHT_BRACE);
        case ';': return makeToken(TokenType::SEMICOLON);
        case ',': return makeToken(TokenType::COMMA);
        case '.': return makeToken(TokenType::DOT);
        case '-': return makeToken(TokenType::MINUS);
        case '+': return makeToken(TokenType::PLUS);
        case '/': return makeToken(TokenType::SLASH);
        case '*': return makeToken(TokenType::STAR);
        case '!': return makeToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        case '=': return makeToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        case '<': return makeToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        case '>': return makeToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        case '"': return string();
        default: {
            std::string message = "Unexpected character: ";
            message += c;
            return makeToken(TokenType::ERROR, message);
        }
    }

    return makeToken(TokenType::ERROR, "Unexpected character.");
}

Token Scanner::makeToken(TokenType type, std::optional<std::string> message) const {
    return {type, start_, current_ - start_, line_, std::move(message)};
}

Token Scanner::makeToken(TokenType type) const {
    return {type, start_, current_ - start_, line_, {}};
}

bool Scanner::isAtEnd() {
    return current_ == source_.length();
}

char Scanner::advance() {
    current_++;
    return source_[current_ - 1];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source_[current_] != expected) return false;

    current_++;
    return true;
}

void Scanner::skipWhitespace() {
    while (true) {
        auto c = peek();
        switch (c) {
        case ' ': case '\r': case '\t': advance(); break;
        case '\n': line_++; advance(); break;
        case '/':
        if (peekNext() == '/') {
            while (peek() != '\n' && !isAtEnd()) advance();
        } else {
            return;
        }
        default: return;
        }
    }
}

char Scanner::peek() {
    return source_[current_];
}

std::optional<char> Scanner::peekNext() {
    if (isAtEnd()) return std::nullopt;
    return {source_[current_] };
}

Token Scanner::string() {
    while (peek() != '=' && !isAtEnd()) {
        if (peek() == '\n') line_++;
        advance();
    }

    if (isAtEnd()) return makeToken(TokenType::ERROR, "Unterminated string.");

    // closing quote
    advance();
    return makeToken(TokenType::STRING);
}

bool Scanner::isDigit(char c) {
    return '0' <= c && c <= '9';
}

Token Scanner::number() {
    while (isDigit(peek())) advance();

    if (peek() == '.') {
        auto c = peekNext();
        if (c.has_value() && isDigit(c.value())) {
            // Consume dot
            advance();
            while (isDigit(peek())) advance();
        }
    }
    return makeToken(TokenType::NUMBER);
}

bool Scanner::isAlpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_');
}

Token Scanner::identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

TokenType Scanner::identifierType() {
    switch (source_[start_]) {
        case 'a': return checkKeyword(1, 2, "nd", TokenType::AND);
        case 'c': return checkKeyword(1, 4, "lass", TokenType::CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TokenType::ELSE);
        case 'f':
            if (current_ - start_ > 1) {
                switch (source_[start_ + 1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TokenType::FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TokenType::FOR);
                    case 'u': return checkKeyword(2, 1, "n", TokenType::FUN);
                }
            }
            break;
        case 'i': return checkKeyword(1, 1, "f", TokenType::IF);
        case 'n': return checkKeyword(1, 2, "il", TokenType::NIL);
        case 'o': return checkKeyword(1, 1, "r", TokenType::OR);
        case 'p': return checkKeyword(1, 4, "rint", TokenType::PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TokenType::RETURN);
        case 's': return checkKeyword(1, 4, "uper", TokenType::SUPER);
        case 't':
            if (current_ - start_ > 1) {
                switch (source_[start_ + 1]) {
                    case 'h': return checkKeyword(2, 2, "is", TokenType::THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TokenType::TRUE);
                }
            }
            break;
        case 'v': return checkKeyword(1, 2, "ar", TokenType::VAR);
        case 'w': return checkKeyword(1, 4, "hile", TokenType::WHILE);
    }
    return TokenType::IDENTIFIER;
}

TokenType Scanner::checkKeyword(int start, int length, const char *rest, TokenType type) {
    if (current_ - start_ == start + length && source_.compare(start_ + start, length, rest) == 0) return type;
    return TokenType::IDENTIFIER;
}

const std::string &Scanner::getSource() const {
    return source_;
}
