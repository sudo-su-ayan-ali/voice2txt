#pragma once

#include "injection.h"

namespace voicetype {

class X11Injection : public TextInjection {
public:
    bool inject(const std::string&) override;
    std::string name() const override { return "x11"; }
};

} // namespace voicetype