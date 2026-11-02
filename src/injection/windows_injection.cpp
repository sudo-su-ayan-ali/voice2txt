#include "windows_injection.h"
#include "log.h"

namespace voicetype {

bool WindowsInjection::inject(const std::string& text) {
    VT_LOG_WARN("WindowsInjection::inject: stub, text length={}", text.size());
    return false;
}

} // namespace voicetype