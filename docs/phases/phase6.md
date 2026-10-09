# Phase 6: Settings & Tray (Qt)

## Goal
Persistent configuration and system tray indicator.

## Duration Estimate
4-5 days

## Tasks & Steps

### Task 6.1: Qt Tray Icon with State Indicator
- [ ] Step 6.1.1: Add Qt6 as dependency (Core, Widgets, DBus) - CMake `find_package(Qt6 REQUIRED COMPONENTS Core Widgets DBus)`
- [ ] Step 6.1.2: Create `src/tray/tray_app.h/cpp`:
  - `QSystemTrayIcon` with context menu
  - Icons for each state: `idle.svg`, `recording.svg`, `processing.svg`, `error.svg` (in `share/icons/`)
  - Tooltip shows current state + last transcript preview
  - Menu actions: Toggle (calls `voicetype toggle`), Settings, Quit
- [ ] Step 6.1.3: IPC client in tray app: connect to daemon socket/pipe for state updates
- [ ] Step 6.1.4: Auto-start daemon if not running (optional, configurable)

### Task 6.2: Settings Dialog
- [ ] Step 6.2.1: Create `src/settings/settings_dialog.h/cpp`:
  - Tab: General (model, language, hotkey hint, auto-start)
  - Tab: Audio (microphone device list, input volume, VAD threshold)
  - Tab: Output (injection method, paste shortcut, clipboard always, notifications)
  - Tab: Logging (enable transcript log, log path, retention)
  - Tab: Advanced (initial prompt, replacements editor, reset to defaults)
- [ ] Step 6.2.2: Model dropdown: list `models/*.bin` + "Download more..." (opens browser to whisper.cpp models)
- [ ] Step 6.2.3: Microphone enumeration: use miniaudio device list
- [ ] Step 6.2.4: Injection method combo: Auto / XTest / wtype / Portal / Clipboard Only
- [ ] Step 6.2.5: Replacements editor: table view (From → To), add/remove rows, regex checkbox

### Task 6.3: Settings Persistence
- [ ] Step 6.3.1: Create `src/settings/config.h/cpp`:
  - Schema: `struct Config { ... }` with all settings
  - Serialization: JSON (Qt `QJsonDocument`)
  - Path: `$XDG_CONFIG_HOME/voicetype/config.json` / `%APPDATA%\voicetype\config.json`
  - Load on daemon startup, watch for changes (QFileSystemWatcher)
- [ ] Step 6.3.2: Daemon applies config changes at runtime where possible (hot-reload):
  - Model change: requires restart (show warning)
  - Injection method: immediate
  - VAD threshold: next recording
  - Replacements: next transcription

### Task 6.4: Daemon Config Reload
- [ ] Step 6.4.1: Add `SIGHUP` handler (Linux) / custom IPC `reload_config` command
- [ ] Step 6.4.2: On config change: validate, apply, emit `config_changed` IPC event
- [ ] Step 6.4.3: Tray app listens for `config_changed` to update UI

## Exit Gate
✅ Every setting persists and survives daemon restart; tray reflects daemon state accurately

## Dependencies
- Phase 5 complete

## Deliverables
- `src/tray/tray_app.h/cpp`
- `src/settings/settings_dialog.h/cpp`
- `src/settings/config.h/cpp`
- `share/icons/idle.svg`, `recording.svg`, `processing.svg`, `error.svg`
- Updated `CMakeLists.txt` (Qt6 integration)
- Daemon integration: config load/watch, hot-reload, IPC events

## Notes
- Qt6 is LGPL v3; ensure dynamic linking for license compliance
- Tray app is optional - daemon must work without it
- Icons: use system theme icons where possible, fallback to bundled SVG
- Config migration: handle missing keys gracefully (use defaults)
- Hotkey display: show current binding (read from GNOME/KDE settings if possible, else show "Set in system settings")