#pragma once

#include <array>
#include <string>
#include <map>
#include "Chunk.h"
#include "Token.h"
#include "Scanner.h"

enum class CompileResult { SUCCESS, ERROR };

struct Parser {
    Parser();

    std::optional<Token> current;
    std::optional<Token> previous;
    bool hadError{};
    bool panicMode{};
};

enum class Precedence : uint8_t  {
    NONE = 1,
    ASSIGNMENT,
    OR,
    AND,
    EQUALITY,
    COMPARISON,
    TERM,
    FACTOR,
    UNARY,
    CALL,
    PRIMARY
};

class Compiler {
    using ParseFn = void(Compiler::*)(const std::string &src, Scanner &scanner, Chunk &chunk);

    struct ParseRule {
        ParseFn prefix;
        ParseFn infix;
        Precedence precedence;
    };
private:
    Parser parser_;
    std::map<TokenType, ParseRule> rules_;
public:
    Compiler();

    CompileResult compile(const std::string &source, Chunk &chunk);

    void advance(Scanner &scanner);

    void errorAtCurrent(const Scanner &scanner, const std::string &message);
    void error(const Scanner &scanner, const std::string &message);

    void errorAt(const Scanner &scanner, const Token &token, const std::string &message);

    void consume(Scanner &scanner, TokenType type, const std::string &message);

    void emitByte(Chunk &chunk, uint8_t byte) const;
    void emitByte(Chunk &chunk, OpCode opCode) const;

    void endCompiler(Chunk &chunk) const;

    void emitBytes(Chunk &chunk, uint8_t byte1, uint8_t byte2) const;
    void emitBytes(Chunk &chunk, OpCode opCode, uint8_t byte) const;
    void emitBytes(Chunk &chunk, OpCode opCode1, OpCode opCode2) const;

    void number(const std::string &src, Scanner &scanner, Chunk &chunk);

    void emitConstant(Chunk &chunk, Scanner &scanner, Value value);

    uint8_t makeConstant(Chunk &chunk, Scanner &scanner, Value value);

    void grouping(const std::string &src, Scanner &scanner, Chunk &chunk);

    void binary(const std::string &src, Scanner &scanner, Chunk &chunk);

    void unary(const std::string &src, Scanner &scanner, Chunk &chunk);

    void literal(const std::string &src, Scanner &scanner, Chunk &chunk);

    void expression(const std::string &src, Scanner &scanner, Chunk &chunk);

    void parsePrecedence(const std::string &src, Scanner &scanner, Chunk &chunk, Precedence precedence);

    ParseRule &getRule(TokenType type);
};
