#include "log.h"
#include <cstdarg>
#include <cstdio>
#include <memory>

namespace voicetype {

void Logger::set_output_file(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    open_file(path);
}

void Logger::open_file(const std::string& path) {
    file_stream_ = std::make_unique<std::ofstream>(path, std::ios::app);
    if (!file_stream_->is_open()) {
        file_stream_.reset();
    }
}

const char* Logger::level_string(LogLevel level) {
    switch (level) {
        case LogLevel::Trace: return "TRACE";
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRIT";
        default: return "UNKNOWN";
    }
}

template<typename... Args>
std::string Logger::format(std::string_view fmt, Args&&... args) {
    // Simple format implementation using snprintf
    // For production, consider using fmtlib or std::format (C++20)
    int size = std::snprintf(nullptr, 0, fmt.data(), std::forward<Args>(args)...);
    if (size <= 0) return "";
    std::string result(size + 1, '\0');
    std::snprintf(result.data(), result.size(), fmt.data(), std::forward<Args>(args)...);
    result.pop_back(); // remove null terminator
    return result;
}

// Explicit instantiation for common types
template std::string Logger::format<std::string_view>(std::string_view, std::string_view);
template std::string Logger::format<const char*>(std::string_view, const char*);
template std::string Logger::format<int>(std::string_view, int);
template std::string Logger::format<unsigned long long>(std::string_view, unsigned long long);
template std::string Logger::format<double>(std::string_view, double);

} // namespace voicetype