#include "platform.h"
#include "session_detect.h"
#include <cstdlib>
#include <string>

namespace voicetype {

SessionInfo detect_session() {
    return session_detect();
}

std::string get_env(const char* name) {
    const char* val = std::getenv(name);
    return val ? std::string(val) : "";
}

bool set_env(const char* name, const char* value) {
    #ifdef _WIN32
    return _putenv_s(name, value) == 0;
    #else
    return setenv(name, value, 1) == 0;
    #endif
}

std::vector<std::string> get_audio_devices() {
    // TODO: Implement using miniaudio device enumeration
    return {};
}

std::string get_default_audio_device() {
    return "";
}

} // namespace voicetype