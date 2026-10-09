# Project Overview: voice2txt

## Vision
A cross-platform (Linux + Windows) push-to-talk voice dictation application that runs fully offline, transcribes speech to text using local models, and injects text into the currently focused application.

## Core Requirements (MVP v0.1)
- **Global hotkey activation** (toggle: press to start, press to stop) on Windows and Linux (X11 + Wayland)
- **Offline transcription** using whisper.cpp with quantized models (default: `base.en` Q5)
- **Clipboard output** on every successful transcription
- **Text injection** into focused window where supported
- **Zero phantom text** from silence (VAD + hallucination filtering)
- **Single-instance daemon** with CLI control (`voicetype toggle|start|stop|status|quit`)

## Architecture
Three-process design communicating via JSON over Unix domain socket (Linux) / named pipe (Windows):
1. **`voicetyped`** - Daemon: owns microphone, VAD, transcription, clipboard, text injection
2. **`voicetype`** - CLI: sends commands to daemon, called by hotkeys
3. **Tray/Settings App** (Qt, Phase 6+) - Optional GUI client

State machine: `Idle → Recording → Processing → Idle` (with `Error` reachable from any state)

## Target Platforms
- **Windows 10/11**: Native `RegisterHotKey`, `SendInput` for injection
- **Linux**: GNOME/KDE on Wayland (primary), X11, wlroots compositors
  - Injection backends: XTest (X11), wtype (wlroots), RemoteDesktop portal (GNOME/KDE), clipboard fallback

## Distribution
- **Windows**: Portable zip + installer (signed), model downloads on first run with SHA-256 verification
- **Linux**: Flatpak (required for portal permissions)

## Non-Goals for v0.1
- AI correction / post-processing
- Always-on listening / wake words
- Multiple languages in one session
- Streaming partial results
- Polished GUI (Phase 6+)

## Key Technical Decisions
| Decision | Choice |
|----------|--------|
| Model | `base.en` quantized Q5 (upgradeable to `small.en`) |
| Hardware | CPU-only baseline; GPU optional acceleration |
| Latency target | < 1.5s stop-to-text for 10s utterance |
| Audio | 16 kHz mono float PCM via miniaudio |
| VAD | Silero (built into whisper.cpp) |
| IPC | JSON over Unix socket / named pipe |
| Single instance | Lock file (Linux) / named mutex (Windows) |

## Risk Mitigation
- Wayland injection blocked → Clipboard always written first; portal backend for GNOME/KDE
- Phantom text on silence → VAD gate + energy threshold + hallucination filter + CI test corpus
- Hotkey failures → `status` command, daemon logs, desktop notifications
- Microphone leaks → RAII handles + watchdog timeout

## Success Metrics (Phase Gates)
Each phase has explicit exit gates (see `docs/roadmap.md` and phase files). No phase begins until previous gate passes.