#pragma once

#include <vector>

enum class ValueType {
    BOOL, NIL, NUMBER
};

union TaggedUnion {
    bool boolean_;
    double number_;
};

class Value {
    ValueType type_;
    TaggedUnion as_;

private:
    Value(ValueType type, TaggedUnion value);

public:
    Value();
    static Value Boolean(bool value);
    static Value Number(double value);
    static Value Nil();

    [[nodiscard]] ValueType getType() const;

    [[nodiscard]] bool isBool() const;
    [[nodiscard]] bool isNil() const;
    [[nodiscard]] bool isNumber() const;

    [[nodiscard]] bool asBool() const;
    [[nodiscard]] double asNumber() const;

    friend bool operator==(const Value& lhs, const Value& rhs);
};

using ValueArray = std::vector<Value>;

std::ostream& operator<<(std::ostream& os, const Value& val);