#include "Compiler.h"

#include <utility>
#include <iostream>
#include "Scanner.h"

CompileResult Compiler::compile(const std::string &source, Chunk &chunk) {
    Scanner scanner(source);

    parser_.hadError = false;
    parser_.panicMode = false;

    advance(scanner);
    expression(source, scanner, chunk);
    consume(scanner, TokenType::END, "Expect end of expression.");

    endCompiler(chunk);

    if (parser_.hadError) return CompileResult::ERROR;

    return CompileResult::SUCCESS;
    }

void Compiler::advance(Scanner &scanner) {
    parser_.previous = parser_.current;

    while (true) {
        parser_.current = scanner.scanToken();
        if (parser_.current->getType() != TokenType::ERROR) break;

        errorAtCurrent(scanner, std::to_string(parser_.current->getStart()));
    }
}

void Compiler::errorAtCurrent(const Scanner &scanner, const std::string &message) {
    errorAt(scanner, parser_.current.value(), message);
}

void Compiler::error(const Scanner &scanner, const std::string &message) {
    errorAt(scanner, parser_.previous.value(), message);
}

void Compiler::errorAt(const Scanner &scanner, const Token &token, const std::string &message) {
    if (parser_.panicMode) return;
    parser_.panicMode = true;
    std::cerr << "[line " << token.getLine() << "] Error";

    auto tokenType = token.getType();
    if (tokenType == TokenType::END) {
        std::cerr << " at end";
    } else if (tokenType == TokenType::ERROR) {
        // nothing
    } else {
        std::cerr << " at " << std::string(scanner.getSource().data() + token.getStart(), token.getLength());
    }

    std::cerr << ": " << message << std::endl;
    parser_.hadError = true;
}

void Compiler::consume(Scanner scanner, TokenType type, const std::string &message) {
    if (parser_.current->getType() == type) {
        advance(scanner);
        return;
    }

    errorAtCurrent(scanner, message);
}

void Compiler::emitByte(Chunk &chunk, uint8_t byte) const {
    chunk.write_byte(byte, parser_.previous->getLine());
}

void Compiler::emitBytes(Chunk &chunk, uint8_t byte1, uint8_t byte2) const {
    emitByte(chunk, byte1);
    emitByte(chunk, byte2);
}

void Compiler::endCompiler(Chunk &chunk) const {
    emitByte(chunk, static_cast<uint8_t>(OpCode::RETURN));
}

void Compiler::binary(const std::string &src, Scanner &scanner, Chunk &chunk) {
    auto operatorType = parser_.previous->getType();
    auto rule = getRule(operatorType);
    auto p = static_cast<uint8_t>(rule.precedence);
    parsePrecedence(src, scanner, chunk, static_cast<Precedence>(p + 1));
    switch (operatorType) {
        case TokenType::PLUS: {
            emitByte(chunk, static_cast<uint8_t>(OpCode::ADD));
            break;
        }
        case TokenType::MINUS: {
            emitByte(chunk, static_cast<uint8_t>(OpCode::SUBTRACT));
            break;
        }
        case TokenType::STAR: {
            emitByte(chunk, static_cast<uint8_t>(OpCode::MULTIPLY));
            break;
        }
        case TokenType::SLASH: {
            emitByte(chunk, static_cast<uint8_t>(OpCode::DIVIDE));
            break;
        }
    }
}

void Compiler::grouping(const std::string &src, Scanner &scanner, Chunk &chunk) {
    expression(src, scanner, chunk);
    consume(scanner, TokenType::RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::number(const std::string &src, Scanner &scanner, Chunk &chunk) {
    auto startIdx = parser_.previous->getStart();
    auto val = std::stod(src.data() + startIdx);
    emitConstant(chunk, scanner, val);
}

void Compiler::unary(const std::string &src, Scanner &scanner, Chunk &chunk) {
    auto operatorType = parser_.previous->getType();

    parsePrecedence(src, scanner, chunk, Precedence::UNARY);

    switch (operatorType) {
        case TokenType::MINUS: {
            emitByte(chunk, static_cast<uint8_t>(OpCode::NEGATE));
            break;
        }
        default:
            return; // unreachable
    }
}

void Compiler::parsePrecedence(const std::string &src, Scanner &scanner, Chunk &chunk, Precedence precedence) {
    advance(scanner);
    auto prefixRule = getRule(parser_.previous->getType()).prefix;
    if (prefixRule == nullptr) {
        error(scanner, "Expect expression.");
    }

    (this->*prefixRule)(src, scanner, chunk);

    while (precedence <= getRule(parser_.current->getType()).precedence) {
        advance(scanner);
        auto infixRule = getRule(parser_.previous->getType()).infix;
        (this->*infixRule)(src, scanner, chunk);
    }
}

void Compiler::emitConstant(Chunk &chunk, Scanner &scanner, Value value) {
    emitBytes(chunk, static_cast<uint8_t>(OpCode::CONSTANT), makeConstant(chunk, scanner, value));
}

uint8_t Compiler::makeConstant(Chunk &chunk, Scanner &scanner, Value value) {
    auto constant = chunk.addConstant(value);
    if (constant > UINT8_MAX) {
        error(scanner, "Too many constants in one chunk.");
    }
    return static_cast<uint8_t>(constant);
}

void Compiler::expression(const std::string &src, Scanner &scanner, Chunk &chunk) {
    parsePrecedence(src, scanner, chunk, Precedence::ASSIGNMENT);
}

Compiler::Compiler() {
    rules_.insert({TokenType::LEFT_PAREN, {&Compiler::grouping, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::RIGHT_PAREN, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::LEFT_BRACE, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::RIGHT_BRACE, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::COMMA, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::DOT, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::MINUS, {&Compiler::unary, &Compiler::binary, Precedence::TERM }});
    rules_.insert({TokenType::PLUS, {nullptr, &Compiler::binary, Precedence::TERM }});
    rules_.insert({TokenType::SEMICOLON, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::SLASH, {nullptr, &Compiler::binary, Precedence::FACTOR }});
    rules_.insert({TokenType::STAR, {nullptr, &Compiler::binary, Precedence::FACTOR }});
    rules_.insert({TokenType::BANG, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::BANG_EQUAL, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::EQUAL, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::EQUAL_EQUAL, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::GREATER, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::GREATER_EQUAL, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::LESS, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::LESS_EQUAL, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::IDENTIFIER, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::NUMBER, {&Compiler::number, nullptr, Precedence::TERM }});
    rules_.insert({TokenType::AND, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::CLASS, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::ELSE, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::FALSE, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::FOR, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::FUN, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::IF, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::NIL, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::OR, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::PRINT, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::RETURN, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::SUPER, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::THIS, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::TRUE, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::VAR, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::WHILE, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::ERROR, {nullptr, nullptr, Precedence::NONE }});
    rules_.insert({TokenType::END, {nullptr, nullptr, Precedence::NONE }});
}

Compiler::ParseRule &Compiler::getRule(TokenType type) {
    return rules_[type];
};

Parser::Parser() {
    current = std::nullopt;
    previous = std::nullopt;
}
