# Phase 4: Text Injection

## Goal
Type transcribed text into focused application across all target platforms.

## Duration Estimate
4-6 days (riskiest phase)

## Tasks & Steps

### Task 4.1: Windows SendInput Injection
- [ ] Step 4.1.1: Create `src/injection/injection.h` (abstract interface: `inject_text(const std::wstring&)`)
- [ ] Step 4.1.2: Implement `WindowsInjection`:
  - Convert UTF-8 transcript → UTF-16 (`MultiByteToWideChar`)
  - For each code point: `INPUT` with `type=INPUT_KEYBOARD`, `ki.wScan=codepoint`, `ki.dwFlags=KEYEVENTF_UNICODE`
  - Send in batches of ~50 chars with `SendInput`
  - Handle surrogate pairs (code points > 0xFFFF)
  - Rate limiting: ~1000 chars/sec to avoid overflow
- [ ] Step 4.1.3: Test in: Notepad, Chrome address bar, VS Code, Windows Terminal, password field (should fail gracefully)

### Task 4.2: Linux Backend Detection
- [ ] Step 4.2.1: Create `src/platform/session_detect.h/cpp`:
  - Read `XDG_SESSION_TYPE` (x11/wayland)
  - Read `WAYLAND_DISPLAY` (non-empty = Wayland)
  - Read `XDG_CURRENT_DESKTOP` (GNOME, KDE, sway, Hyprland, etc.)
  - Read `DESKTOP_SESSION` as fallback
  - Return `enum SessionType { X11, WLROOTS, GNOME_WAYLAND, KDE_WAYLAND, UNKNOWN }`
- [ ] Step 4.2.2: Log detected session at daemon startup
- [ ] Step 4.2.3: Allow override via config: `injection_backend = "auto|x11|wtype|portal|clipboard"`

### Task 4.3: X11 Backend (XTest)
- [ ] Step 4.3.1: Implement `X11Injection`:
  - Link against `X11` and `Xtst`
  - `XOpenDisplay(NULL)`, `XTestFakeKeyEvent` for each Unicode char
  - Use `XkbKeycodeToKeysym` for ASCII; for Unicode > 0xFFFF: compose key sequences or fallback
  - `XFlush()`, `XCloseDisplay()`
- [ ] Step 4.3.2: Handle display connection failure gracefully (fallback to clipboard)

### Task 4.4: wlroots Backend (wtype)
- [ ] Step 4.4.1: Implement `WtypeInjection`:
  - Spawn `wtype -` subprocess, write transcript to stdin
  - `wtype` handles Unicode natively via virtual keyboard protocol
  - Check `wtype` availability at startup (`which wtype`)
  - Timeout: 5 seconds max per injection

### Task 4.5: GNOME/KDE Wayland Portal (RemoteDesktop)
- [ ] Step 4.5.1: Implement `PortalInjection`:
  - Use `libportal` (GDBus) or `dbus-send` subprocess
  - Call `org.freedesktop.portal.RemoteDesktop.SendText` (or `org.freedesktop.portal.Keyboard.SendText` if available)
  - Requires Flatpak permission: `org.freedesktop.portal.RemoteDesktop` or `org.freedesktop.portal.Keyboard`
  - Handle portal not available / permission denied → fallback
- [ ] Step 4.5.2: Flatpak manifest snippet for permissions (document for Phase 7)

### Task 4.6: Clipboard-Paste Fallback
- [ ] Step 4.6.1: Implement `ClipboardPasteInjection`:
  - Write transcript to clipboard (reuse Phase 3)
  - Simulate paste shortcut:
    - Default: `Ctrl+V` (Windows/Linux X11)
    - Terminal default: `Ctrl+Shift+V`
    - Configurable: `paste_shortcut = "ctrl+v|ctrl+shift+v|shift+insert|custom"`
  - Windows: `SendInput` with `VK_CONTROL` + `VK_V`
  - Linux X11: `XTestFakeKeyEvent` for Control+V
  - Linux Wayland: `wtype -M ctrl -P v -m ctrl` or portal `SendText` with paste semantics
- [ ] Step 4.6.2: Configurable per-application override (future: window class detection)

### Task 4.7: Injection Test Matrix Execution
- [ ] Step 4.7.1: Create `scripts/injection_test.py`:
  - Automated test using `xdotool`/`ydotool` (Linux) or `pywinauto` (Windows)
  - Test each target app × each backend
  - Verify text appears correctly, no extra chars, no focus loss
- [ ] Step 4.7.2: Manual test matrix (document results in `injection_test_results.md`):

| Target App | Windows | X11 | Sway/Hyprland | GNOME Wayland | KDE Wayland |
|------------|---------|-----|---------------|---------------|-------------|
| Plain text editor (Notepad/Gedit) | ☐ | ☐ | ☐ | ☐ | ☐ |
| Browser text field (Chrome/Firefox) | ☐ | ☐ | ☐ | ☐ | ☐ |
| Terminal emulator | ☐ | ☐ | ☐ | ☐ | ☐ |
| Electron app (VS Code, Slack) | ☐ | ☐ | ☐ | ☐ | ☐ |
| Password field (expect refuse/warn) | ☐ | ☐ | ☐ | ☐ | ☐ |
| Focus change during processing | ☐ | ☐ | ☐ | ☐ | ☐ |

- [ ] Step 4.7.3: Document known failures and workarounds per platform

## Exit Gate
✅ Injection test matrix passes on every claimed target platform (all ✅ for at least Windows + one Linux session)

## Dependencies
- Phase 3 complete

## Deliverables
- `src/injection/injection.h`
- `src/injection/windows_injection.h/cpp`
- `src/injection/x11_injection.h/cpp`
- `src/injection/wtype_injection.h/cpp`
- `src/injection/portal_injection.h/cpp`
- `src/injection/clipboard_paste_injection.h/cpp`
- `src/platform/session_detect.h/cpp`
- `scripts/injection_test.py`
- `injection_test_results.md`
- Updated daemon integration (call injection after clipboard write)

## Notes
- Injection runs AFTER clipboard write (clipboard is primary, injection is best-effort)
- Password field detection: heuristic - check if focused window class suggests password field
- Focus change during processing: capture target window at `Recording→Processing` transition, inject to that window
- Wayland: portal is async; injection may complete after daemon returns to Idle
- All injection methods must handle empty string, very long strings (>10k chars), and Unicode gracefully