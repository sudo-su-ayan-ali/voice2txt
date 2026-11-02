#pragma once

#include "injection.h"

namespace voicetype {

class WtypeInjection : public TextInjection {
public:
    bool inject(const std::string&) override;
    std::string name() const override { return "wtype"; }
};

} // namespace voicetype