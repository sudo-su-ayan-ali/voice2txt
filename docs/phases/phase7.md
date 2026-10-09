# Phase 7: Packaging & Release

## Goal
Distributable artifacts for Windows and Linux.

## Duration Estimate
4-6 days

## Tasks & Steps

### Task 7.1: Windows Installer
- [ ] Step 7.1.1: Choose installer tool: NSIS (scriptable, lightweight) or Inno Setup
- [ ] Step 7.1.2: Create `packaging/windows/installer.nsi` (or `.iss`):
  - Install: `voicetyped.exe`, `voicetype.exe`, `models/ggml-base.en-q5_1.bin`
  - Install directory: `%LOCALAPPDATA%\VoiceType\` (per-user, no admin)
  - Start Menu shortcuts: VoiceType (tray), Uninstall
  - Register uninstaller
  - Set AppUserModelID for toast notifications
- [ ] Step 7.1.3: First-run model download:
  - Daemon checks for model on startup
  - If missing: download from `https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.en-q5_1.bin`
  - Verify SHA-256 (hardcoded in binary)
  - Show progress via tray/toast
  - Keep previous model on failed download
- [ ] Step 7.1.4: Code signing:
  - Obtain EV code signing certificate (or standard for testing)
  - Sign `voicetyped.exe`, `voicetype.exe`, installer
  - Timestamp signature
  - Document process in `packaging/windows/SIGNING.md`

### Task 7.2: Linux Flatpak
- [ ] Step 7.2.1: Create `packaging/linux/com.voicetype.VoiceType.yml`:
  - Base: `org.freedesktop.Platform//23.08` + `org.freedesktop.Sdk//23.08`
  - Runtime: `org.freedesktop.Platform`, `org.kde.KStyle.Adwaita` (for Qt theming)
  - Finish-args:
    - `--socket=wayland` (Wayland)
    - `--socket=fallback-x11` (X11)
    - `--talk-name=org.freedesktop.portal.Desktop` (portals)
    - `--talk-name=org.freedesktop.portal.RemoteDesktop` (injection)
    - `--talk-name=org.freedesktop.portal.Keyboard` (alternative)
    - `--filesystem=xdg-run/voicetype:create` (daemon socket)
    - `--filesystem=xdg-config/voicetype:rw` (config)
    - `--filesystem=xdg-data/voicetype:rw` (models, logs)
  - Modules: whisper.cpp, miniaudio, Qt6, app build
- [ ] Step 7.2.2: Build locally with `flatpak-builder --force-clean build-dir packaging/linux/com.voicetype.VoiceType.yml`
- [ ] Step 7.2.3: Test on clean Fedora/Ubuntu VM (GNOME + KDE)
- [ ] Step 7.2.4: Publish to Flathub (post v0.1): create `flathub` PR with manifest

### Task 7.3: Release Checklist
- [ ] Step 7.3.1: Create `RELEASE_CHECKLIST.md`:
  - [ ] Clean Windows 11 VM: install → first run → model download → toggle → transcribe → inject → uninstall
  - [ ] Clean Ubuntu 24.04 VM (GNOME): flatpak install → first run → toggle → transcribe → inject → uninstall
  - [ ] Clean Fedora 40 VM (KDE): flatpak install → first run → toggle → transcribe → inject → uninstall
  - [ ] Upgrade test: install v0.1.0 → release v0.1.1 → verify config/models preserved
  - [ ] Portable zip (Windows): extract → run → works without install
  - [ ] Verify signatures (Windows)
  - [ ] Verify Flatpak permissions prompt appears correctly
- [ ] Step 7.3.2: Automate VM tests with GitHub Actions (self-hosted runners or paid macOS/Windows runners)

### Task 7.4: Versioning & Changelog
- [ ] Step 7.4.1: Semantic versioning: `v0.1.0` for MVP
- [ ] Step 7.4.2: Generate `CHANGELOG.md` from conventional commits
- [ ] Step 7.4.3: GitHub Release with artifacts: installer, portable zip, Flatpak bundle, SHA-256 sums

## Exit Gate
✅ Fresh install works on clean Windows 11 VM and clean Ubuntu/Fedora VM (GNOME + KDE)

## Dependencies
- Phase 6 complete

## Deliverables
- `packaging/windows/installer.nsi` (or `.iss`)
- `packaging/windows/SIGNING.md`
- `packaging/linux/com.voicetype.VoiceType.yml`
- `RELEASE_CHECKLIST.md`
- `CHANGELOG.md`
- GitHub Actions release workflow (`.github/workflows/release.yml`)
- Signed binaries and installer (Windows)
- Flatpak bundle (Linux)

## Notes
- Windows per-user install avoids UAC; simpler for MVP
- Model download: use `libcurl` or `WinHTTP` in daemon, not in installer
- Flatpak: portal permissions require user consent on first injection attempt
- Test on Wayland (GNOME/KDE) AND X11 (fallback)
- Keep installer size < 100 MB (model ~40 MB, binaries ~10 MB, Qt ~30 MB)