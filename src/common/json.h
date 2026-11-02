#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <memory>
#include <cstdint>

namespace voicetype {

class JsonValue {
public:
    using Object = std::map<std::string, JsonValue>;
    using Array = std::vector<JsonValue>;
    using Variant = std::variant<
        std::monostate,  // null
        bool,
        int64_t,
        double,
        std::string,
        Object,
        Array
    >;

    JsonValue() = default;
    JsonValue(std::nullptr_t) : value_(std::monostate{}) {}
    JsonValue(bool v) : value_(v) {}
    JsonValue(int v) : value_(static_cast<int64_t>(v)) {}
    JsonValue(int64_t v) : value_(v) {}
    JsonValue(double v) : value_(v) {}
    JsonValue(const char* v) : value_(std::string(v)) {}
    JsonValue(std::string v) : value_(std::move(v)) {}
    JsonValue(Object v) : value_(std::move(v)) {}
    JsonValue(Array v) : value_(std::move(v)) {}

    // Type queries
    bool is_null() const { return std::holds_alternative<std::monostate>(value_); }
    bool is_bool() const { return std::holds_alternative<bool>(value_); }
    bool is_int() const { return std::holds_alternative<int64_t>(value_); }
    bool is_double() const { return std::holds_alternative<double>(value_); }
    bool is_number() const { return is_int() || is_double(); }
    bool is_string() const { return std::holds_alternative<std::string>(value_); }
    bool is_object() const { return std::holds_alternative<Object>(value_); }
    bool is_array() const { return std::holds_alternative<Array>(value_); }

    // Accessors
    bool as_bool() const { return std::get<bool>(value_); }
    int64_t as_int() const { return std::get<int64_t>(value_); }
    double as_double() const { return std::get<double>(value_); }
    const std::string& as_string() const { return std::get<std::string>(value_); }
    const Object& as_object() const { return std::get<Object>(value_); }
    const Array& as_array() const { return std::get<Array>(value_); }
    Object& as_object() { return std::get<Object>(value_); }
    Array& as_array() { return std::get<Array>(value_); }

    // Object access
    JsonValue& operator[](const std::string& key) {
        if (!is_object()) value_ = Object{};
        return std::get<Object>(value_)[key];
    }
    const JsonValue* find(const std::string& key) const {
        if (!is_object()) return nullptr;
        auto it = std::get<Object>(value_).find(key);
        return it != std::get<Object>(value_).end() ? &it->second : nullptr;
    }

    // Array access
    void push_back(const JsonValue& val) {
        if (!is_array()) value_ = Array{};
        std::get<Array>(value_).push_back(val);
    }
    size_t size() const { return is_array() ? std::get<Array>(value_).size() : 0; }
    JsonValue& at(size_t idx) { return std::get<Array>(value_).at(idx); }
    const JsonValue& at(size_t idx) const { return std::get<Array>(value_).at(idx); }

    // Serialization
    std::string dump(int indent = -1) const;
    static std::optional<JsonValue> parse(std::string_view str);

private:
    Variant value_;
    void dump_to(std::string& out, int indent, int current_indent) const;
};

// Convenience helpers
inline JsonValue make_object(std::initializer_list<std::pair<std::string, JsonValue>> list) {
    JsonValue::Object obj;
    for (auto& p : list) obj.emplace(p.first, std::move(p.second));
    return JsonValue(std::move(obj));
}

inline JsonValue make_array(std::initializer_list<JsonValue> list) {
    return JsonValue(JsonValue::Array(list));
}

} // namespace voicetype