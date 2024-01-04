#include "StringValue.h"
#include <cassert>
#include <cstring>

StringValue::StringValue(std::string s) {
    this->s = s;
}

Any StringValue::getValue() {
    return s;
}

void StringValue::setValue(Any value) {
    s = std::any_cast<std::string>(value);
}

uint8 StringValue::size() {
    return s.size();
}

void StringValue::serialize(void *data) {
    memcpy(data, s.data(), s.size());
}

ValueType StringValue::getType() {
    return STRING_V;
}