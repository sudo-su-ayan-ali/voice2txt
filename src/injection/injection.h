#pragma once

#include <string>
#include <memory>

namespace voicetype {

class TextInjection {
public:
    virtual ~TextInjection() = default;
    virtual bool inject(const std::string& text) = 0;
    virtual std::string name() const = 0;

    static std::unique_ptr<TextInjection> create_best();
};

} // namespace voicetype