#pragma once

#include <string>
#include <filesystem>
#include <optional>

namespace voicetype {

namespace fs = std::filesystem;

inline std::optional<fs::path> get_config_dir() {
    #ifdef _WIN32
    const char* appdata = std::getenv("APPDATA");
    if (appdata) return fs::path(appdata) / "voicetype";
    return std::nullopt;
    #else
    const char* xdg = std::getenv("XDG_CONFIG_HOME");
    if (xdg && *xdg) return fs::path(xdg) / "voicetype";
    const char* home = std::getenv("HOME");
    if (home) return fs::path(home) / ".config" / "voicetype";
    return std::nullopt;
    #endif
}

inline std::optional<fs::path> get_data_dir() {
    #ifdef _WIN32
    const char* appdata = std::getenv("APPDATA");
    if (appdata) return fs::path(appdata) / "voicetype";
    return std::nullopt;
    #else
    const char* xdg = std::getenv("XDG_DATA_HOME");
    if (xdg && *xdg) return fs::path(xdg) / "voicetype";
    const char* home = std::getenv("HOME");
    if (home) return fs::path(home) / ".local" / "share" / "voicetype";
    return std::nullopt;
    #endif
}

inline std::optional<fs::path> get_runtime_dir() {
    #ifdef _WIN32
    return get_data_dir(); // Windows doesn't have XDG_RUNTIME_DIR
    #else
    const char* xdg = std::getenv("XDG_RUNTIME_DIR");
    if (xdg && *xdg) return fs::path(xdg) / "voicetype";
    // Fallback to /tmp/voicetype-<uid>
    return fs::path("/tmp") / ("voicetype-" + std::to_string(getuid()));
    #endif
}

inline bool ensure_dir(const fs::path& path) {
    std::error_code ec;
    fs::create_directories(path, ec);
    return !ec;
}

inline std::string read_file(const fs::path& path) {
    std::error_code ec;
    auto size = fs::file_size(path, ec);
    if (ec) return "";
    std::string result(size, '\0');
    std::ifstream f(path, std::ios::binary);
    f.read(result.data(), size);
    return result;
}

inline bool write_file(const fs::path& path, std::string_view content) {
    std::error_code ec;
    fs::create_directories(path.parent_path(), ec);
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    if (!f) return false;
    f.write(content.data(), content.size());
    return f.good();
}

} // namespace voicetype