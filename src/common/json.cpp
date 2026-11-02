#include "json.h"
#include <charconv>
#include <cctype>
#include <stdexcept>

namespace voicetype {

namespace {

struct Parser {
    std::string_view str;
    size_t pos = 0;

    void skip_ws() {
        while (pos < str.size() && std::isspace(static_cast<unsigned char>(str[pos]))) ++pos;
    }

    char peek() const { return pos < str.size() ? str[pos] : '\0'; }
    char get() { return pos < str.size() ? str[pos++] : '\0'; }
    bool match(char c) { if (peek() == c) { ++pos; return true; } return false; }
    void expect(char c) { if (!match(c)) throw std::runtime_error("Expected '" + std::string(1, c) + "'"); }

    std::optional<JsonValue> parse_value() {
        skip_ws();
        char c = peek();
        if (c == 'n') return parse_null();
        if (c == 't' || c == 'f') return parse_bool();
        if (c == '"') return parse_string();
        if (c == '[') return parse_array();
        if (c == '{') return parse_object();
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return parse_number();
        throw std::runtime_error("Unexpected character: " + std::string(1, c));
    }

    std::optional<JsonValue> parse_null() {
        if (str.substr(pos, 4) == "null") { pos += 4; return JsonValue(nullptr); }
        throw std::runtime_error("Expected 'null'");
    }

    std::optional<JsonValue> parse_bool() {
        if (str.substr(pos, 4) == "true") { pos += 4; return JsonValue(true); }
        if (str.substr(pos, 5) == "false") { pos += 5; return JsonValue(false); }
        throw std::runtime_error("Expected 'true' or 'false'");
    }
};

} // anonymous namespace

std::optional<JsonValue> JsonValue::parse(std::string_view str) {
    Parser parser{str};
    auto val = parser.parse_value();
    if (val) {
        parser.skip_ws();
        if (parser.pos != str.size()) return std::nullopt;
    }
    return val;
}

} // namespace voicetype