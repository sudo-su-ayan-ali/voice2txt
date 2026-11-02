#include "session_detect.h"
#include <cstdlib>
#include <string>
#include <algorithm>

namespace voicetype {

SessionInfo session_detect() {
    SessionInfo info;

    // Get XDG_SESSION_TYPE
    std::string session_type = get_env("XDG_SESSION_TYPE");
    std::string wayland_display = get_env("WAYLAND_DISPLAY");
    std::string desktop = get_env("XDG_CURRENT_DESKTOP");
    std::string desktop_session = get_env("DESKTOP_SESSION");

    info.wayland_display = wayland_display;

    // Determine display server
    if (!wayland_display.empty() || session_type == "wayland") {
        info.display_server = "wayland";
    } else {
        info.display_server = "x11";
    }

    // Normalize desktop name
    std::string desktop_lower = desktop;
    std::transform(desktop_lower.begin(), desktop_lower.end(), desktop_lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (desktop_lower.find("gnome") != std::string::npos) {
        info.desktop = "GNOME";
    } else if (desktop_lower.find("kde") != std::string::npos || 
               desktop_lower.find("plasma") != std::string::npos) {
        info.desktop = "KDE";
    } else if (desktop_lower.find("sway") != std::string::npos) {
        info.desktop = "sway";
    } else if (desktop_lower.find("hyprland") != std::string::npos) {
        info.desktop = "Hyprland";
    } else if (!desktop_session.empty()) {
        info.desktop = desktop_session;
    }

    // Determine session type
    if (info.display_server == "x11") {
        info.type = SessionType::X11;
    } else if (info.display_server == "wayland") {
        if (info.desktop == "GNOME") {
            info.type = SessionType::GNOME_Wayland;
        } else if (info.desktop == "KDE") {
            info.type = SessionType::KDE_Wayland;
        } else if (info.desktop == "sway" || info.desktop == "Hyprland") {
            info.type = SessionType::WLRoots;
        } else {
            info.type = SessionType::WLRoots; // Default for unknown Wayland
        }
    }

    return info;
}

} // namespace voicetype