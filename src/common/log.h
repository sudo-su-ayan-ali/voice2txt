#pragma once

#include <string>
#include <string_view>
#include <chrono>
#include <source_location>
#include <mutex>
#include <fstream>
#include <memory>

namespace voicetype {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger {
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void set_level(LogLevel level) { level_ = level; }
    void set_output_file(const std::string& path);
    void set_console_output(bool enabled) { console_output_ = enabled; }

    template<typename... Args>
    void log(LogLevel level, std::string_view fmt, Args&&... args) {
        if (level < level_) return;
        std::lock_guard<std::mutex> lock(mutex_);
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        char time_buf[32];
        std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&time_t));

        std::string message = format(fmt, std::forward<Args>(args)...);
        std::string line = std::string(time_buf) + "." + std::to_string(ms.count()).substr(0,3) 
            + " [" + level_string(level) + "] " + message + "\n";

        if (console_output_) {
            fputs(line.c_str(), level >= LogLevel::Warning ? stderr : stdout);
        }
        if (file_stream_ && file_stream_->is_open()) {
            *file_stream_ << line;
            file_stream_->flush();
        }
    }

private:
    Logger() : level_(LogLevel::Info), console_output_(true) {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void open_file(const std::string& path);
    static const char* level_string(LogLevel level);
    template<typename... Args>
    static std::string format(std::string_view fmt, Args&&... args);

    LogLevel level_;
    bool console_output_;
    std::mutex mutex_;
    std::unique_ptr<std::ofstream> file_stream_;
};

// Convenience macros
#define VT_LOG_TRACE(...)   voicetype::Logger::instance().log(voicetype::LogLevel::Trace, __VA_ARGS__)
#define VT_LOG_DEBUG(...)   voicetype::Logger::instance().log(voicetype::LogLevel::Debug, __VA_ARGS__)
#define VT_LOG_INFO(...)    voicetype::Logger::instance().log(voicetype::LogLevel::Info, __VA_ARGS__)
#define VT_LOG_WARN(...)    voicetype::Logger::instance().log(voicetype::LogLevel::Warning, __VA_ARGS__)
#define VT_LOG_ERROR(...)   voicetype::Logger::instance().log(voicetype::LogLevel::Error, __VA_ARGS__)
#define VT_LOG_CRITICAL(...) voicetype::Logger::instance().log(voicetype::LogLevel::Critical, __VA_ARGS__)

} // namespace voicetype