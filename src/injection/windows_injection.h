#pragma once

#include "injection.h"

namespace voicetype {

class WindowsInjection : public TextInjection {
public:
    bool inject(const std::string&) override;
    std::string name() const override { return "windows"; }
};

} // namespace voicetype