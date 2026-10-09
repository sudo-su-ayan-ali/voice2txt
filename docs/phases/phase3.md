# Phase 3: Output & Clipboard

## Goal
Reliable clipboard write + user feedback on every transcription.

## Duration Estimate
2 days

## Tasks & Steps

### Task 3.1: Implement Clipboard Write
- [ ] Step 3.1.1: Create `src/clipboard/clipboard.h` (abstract interface)
- [ ] Step 3.1.2: Implement `WindowsClipboard`:
  - `OpenClipboard(NULL)`, `EmptyClipboard()`, `SetClipboardData(CF_UNICODETEXT, hGlobal)`
  - `GlobalAlloc(GMEM_MOVEABLE)`, `GlobalLock`, `wcscpy`, `GlobalUnlock`, `CloseClipboard()`
  - Retry loop (max 3 attempts, 10ms delay) for clipboard contention
- [ ] Step 3.1.3: Implement `LinuxClipboard`:
  - Wayland: `wl-copy` subprocess (requires `wl-clipboard` package)
  - X11: `xclip -selection clipboard` or `xsel -b` subprocess
  - Detect session type via `$XDG_SESSION_TYPE` and `$WAYLAND_DISPLAY`
  - Fallback chain: Wayland → X11 → error
- [ ] Step 3.1.4: Unit test: write known string, read back, verify match

### Task 3.2: Desktop Notifications
- [ ] Step 3.2.1: Create `src/notify/notify.h` (abstract interface)
- [ ] Step 3.2.2: Implement `WindowsToast`:
  - Use `IUserNotification2` or `Shell_NotifyIcon` with `NIF_INFO` (balloon tooltip)
  - Fallback: simple message box if toast unavailable
  - Template: "VoiceType" title, transcript preview (first 80 chars) as body
- [ ] Step 3.2.3: Implement `LinuxNotify`:
  - `libnotify` (`notify-send`) subprocess
  - Icon: custom icon in `share/icons/hicolor/scalable/apps/voicetype.svg`
  - Urgency: normal (success), critical (error)
- [ ] Step 3.2.4: Notification types: `recording_started`, `transcription_success`, `transcription_failed`, `error`

### Task 3.3: Transcript Logging (Opt-In)
- [ ] Step 3.3.1: Create `src/logging/transcript_log.h/cpp`:
  - Config: `enabled` (default false), `path` (default `$XDG_DATA_HOME/voicetype/transcripts.log` / `%APPDATA%\voicetype\transcripts.log`)
  - Format: JSON Lines, one entry per transcription:
    ```json
    {"timestamp":"2026-10-09T12:34:56Z","duration_ms":1234,"text":"hello world","model":"base.en"}
    ```
  - Rotation: max 10 MB, keep 5 files
  - Thread-safe append (mutex + `std::ofstream` with `std::ios::app`)
- [ ] Step 3.3.2: CLI command `voicetype log enable|disable|path|show`

### Task 3.4: CLI Status Enhancement
- [ ] Step 3.4.1: Extend `status` command response to include:
  - Daemon state (idle/recording/processing/error)
  - Last transcript text (truncated)
  - Last transcript timestamp
  - Clipboard status (written/failed)
  - Model loaded
  - Uptime

## Exit Gate
✅ Transcript reaches clipboard in 100% of successful runs; notifications fire on success/failure; opt-in logging works

## Dependencies
- Phase 2 complete

## Deliverables
- `src/clipboard/clipboard.h/cpp`
- `src/clipboard/windows_clipboard.h/cpp`
- `src/clipboard/linux_clipboard.h/cpp`
- `src/notify/notify.h/cpp`
- `src/notify/windows_toast.h/cpp`
- `src/notify/linux_notify.h/cpp`
- `src/logging/transcript_log.h/cpp`
- `share/icons/hicolor/scalable/apps/voicetype.svg`
- Updated `src/cli/main.cpp` (enhanced `status` + `log` commands)

## Notes
- Clipboard: handle Unicode correctly (UTF-16 on Windows, UTF-8 on Linux)
- Windows toast: requires AppUserModelID registration for persistent toasts
- Linux: `notify-send` is widely available; `libnotify` dev headers in CI
- Log rotation: implement simple size-based rotation on write
- All clipboard/notification calls must be non-blocking (async or short timeout)