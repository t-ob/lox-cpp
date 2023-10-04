#include <ostream>
#include "Value.h"

Value::Value(ValueType type, TaggedUnion value) : type_(type), as_(value) {}

Value Value::Number(double value) {
    return Value(ValueType::NUMBER, {.number_ = value});
}

Value Value::Nil() {
    return Value(ValueType::NIL, {.number_ = 0.0});
}

Value Value::Boolean(bool value) {
    return Value(ValueType::BOOL, {.boolean_ = value});
}

bool Value::isBool() const {
    return type_ == ValueType::BOOL;
}

bool Value::isNil() const {
    return type_ == ValueType::NIL;
}

bool Value::isNumber() const {
    return type_ == ValueType::NUMBER;
}

bool Value::asBool() const {
    return as_.boolean_;
}

double Value::asNumber() const {
    return as_.number_;
}

ValueType Value::getType() const {
    return type_;
}

Value::Value() : type_(ValueType::NUMBER), as_{ .number_ = 0.0 } {}

bool operator==(const Value &lhs, const Value &rhs) {
    if (lhs.type_ != rhs.type_) return false;

    switch (lhs.type_) {
        case ValueType::BOOL:   return lhs.as_.boolean_ == rhs.as_.boolean_;
        case ValueType::NIL:    return true;
        case ValueType::NUMBER:  return lhs.as_.number_ == rhs.as_.number_;
        default:         return false; // Unreachable.
    }
}

std::ostream& operator<<(std::ostream& os, const Value& val) {
    switch (val.getType()) {
        case ValueType::BOOL: {
            os << std::boolalpha << val.asBool();
            break;
        }
        case ValueType::NIL: {
            os << "nil";
            break;
        }
        case ValueType::NUMBER: {
            os << val.asNumber();
            break;
        }
    }

    return os;
}
