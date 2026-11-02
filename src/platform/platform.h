#pragma once

#include <string>
#include <optional>
#include <vector>

namespace voicetype {

enum class SessionType {
    Unknown,
    X11,
    WLRoots,
    GNOME_Wayland,
    KDE_Wayland
};

struct SessionInfo {
    SessionType type = SessionType::Unknown;
    std::string display_server;  // "x11" or "wayland"
    std::string desktop;         // "GNOME", "KDE", "sway", "Hyprland", etc.
    std::string wayland_display; // $WAYLAND_DISPLAY value
};

SessionInfo detect_session();

// Platform-specific helpers
std::string get_env(const char* name);
bool set_env(const char* name, const char* value);
std::vector<std::string> get_audio_devices();
std::string get_default_audio_device();

} // namespace voicetype