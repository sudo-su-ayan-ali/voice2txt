#include "x11_injection.h"
#include "log.h"

namespace voicetype {

bool X11Injection::inject(const std::string& text) {
    VT_LOG_WARN("X11Injection::inject: stub, text length={}", text.size());
    return false;
}

} // namespace voicetype