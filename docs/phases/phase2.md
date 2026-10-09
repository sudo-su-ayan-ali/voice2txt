# Phase 2: Daemon, IPC & Hotkey

## Goal
Persistent daemon with CLI control and global hotkey activation.

## Duration Estimate
3-4 days

## Tasks & Steps

### Task 2.1: Implement Daemon State Machine
- [ ] Step 2.1.1: Define `DaemonState` enum: `Idle`, `Recording`, `Processing`, `Error`
- [ ] Step 2.1.2: Create `src/daemon/state_machine.h/cpp`:
  - `StateMachine` class with `transition(State, Event)` → `Result<State>`
  - Valid transitions: Idle→Recording, Recording→Processing, Processing→Idle, Any→Error
  - Callbacks: `on_enter(State)`, `on_exit(State)`, `on_error(ErrorCode)`
  - Thread-safe (mutex-protected)
- [ ] Step 2.1.3: Integrate with audio capture and transcriber:
  - Idle→Recording: start audio capture
  - Recording→Processing: stop capture, start transcription
  - Processing→Idle: deliver result, reset for next cycle
- [ ] Step 2.1.4: Unit test all valid/invalid transitions

### Task 2.2: Implement IPC Server (Unix Socket / Named Pipe)
- [ ] Step 2.2.1: Define JSON message protocol in `src/ipc/protocol.h`:
  ```json
  // Request
  { "id": 1, "cmd": "toggle|start|stop|status|quit", "params": {} }
  // Response
  { "id": 1, "ok": true, "result": {...} }  // or { "id": 1, "ok": false, "error": "msg" }
  // Async notification (daemon → client)
  { "event": "state_changed", "state": "recording", "timestamp_ms": 12345 }
  ```
- [ ] Step 2.2.2: Create `src/ipc/server.h/cpp` (abstract interface)
- [ ] Step 2.2.3: Implement `UnixSocketServer` (Linux): `socket(AF_UNIX)`, bind to `$XDG_RUNTIME_DIR/voicetype/socket`
- [ ] Step 2.2.4: Implement `NamedPipeServer` (Windows): `CreateNamedPipe(\\.\pipe\voicetype)`
- [ ] Step 2.2.5: Message framing: length-prefixed (4-byte LE uint32) + JSON UTF-8
- [ ] Step 2.2.6: Handle multiple concurrent clients (thread pool or async I/O)
- [ ] Step 2.2.7: Implement command handlers mapping to state machine events

### Task 2.3: Implement Single-Instance Lock
- [ ] Step 2.3.1: Linux: lock file at `$XDG_RUNTIME_DIR/voicetype/lock` with `flock(LOCK_EX|LOCK_NB)`
- [ ] Step 2.3.2: Windows: named mutex `CreateMutex(NULL, FALSE, "Global\\voicetype_daemon_mutex")`
- [ ] Step 2.3.3: On lock failure: print "Another instance is running" and exit(1)
- [ ] Step 2.3.4: Release lock on clean shutdown (atexit + signal handlers)

### Task 2.4: Build CLI `voicetype` with Commands
- [ ] Step 2.4.1: Extend `src/cli/main.cpp` with subcommands: `toggle`, `start`, `stop`, `status`, `quit`
- [ ] Step 2.4.2: Implement IPC client: connect to socket/pipe, send request, wait for response (with timeout)
- [ ] Step 2.4.3: Pretty-print responses (JSON for `--json`, human-readable default)
- [ ] Step 2.4.4: Exit codes: 0=success, 1=daemon not running, 2=command failed, 3=timeout

### Task 2.5: Windows Global Hotkey in Daemon
- [ ] Step 2.5.1: Choose default hotkey: `Ctrl+Shift+Space` (VK_SPACE, MOD_CONTROL|MOD_SHIFT)
- [ ] Step 2.5.2: In daemon main thread: `RegisterHotKey(hwnd, HOTKEY_ID, MOD_CONTROL|MOD_SHIFT, VK_SPACE)`
- [ ] Step 2.5.3: Message loop: `GetMessage` → `WM_HOTKEY` → send `toggle` command to state machine
- [ ] Step 2.5.4: Handle hotkey conflicts: if `RegisterHotKey` fails, log error, notify via tray (later), continue

### Task 2.6: Linux Hotkey Binding Documentation
- [ ] Step 2.6.1: Document GNOME: Settings → Keyboard → Custom Shortcuts → `voicetype toggle`
- [ ] Step 2.6.2: Document KDE: System Settings → Shortcuts → Custom Shortcuts → `voicetype toggle`
- [ ] Step 2.6.3: Document Sway/Hyprland: `bindsym $mod+Shift+space exec voicetype toggle`
- [ ] Step 2.6.4: Create `docs/linux-hotkeys.md` with step-by-step instructions + screenshots placeholders

### Task 2.7: Soak Test (100 Toggle Cycles)
- [ ] Step 2.7.1: Create `scripts/soak_test.py`:
  - Start daemon
  - Loop 100x: send `toggle`, wait for `state_changed: recording`, send `toggle`, wait for `state_changed: idle`
  - Verify no crashes, no stuck states, no leaked file descriptors (check `/proc/<pid>/fd` count)
- [ ] Step 2.7.2: Run on Linux and Windows (CI if possible, otherwise local)
- [ ] Step 2.7.3: Add memory leak check: run under `valgrind --leak-check=full` (Linux) / Dr. Memory (Windows)

## Exit Gate
✅ 100 consecutive toggle cycles with no crash, no stuck state, no orphaned microphone handle

## Dependencies
- Phase 1 complete

## Deliverables
- `src/daemon/state_machine.h/cpp`
- `src/daemon/daemon.h/cpp` (main entry point)
- `src/ipc/protocol.h`
- `src/ipc/server.h/cpp`
- `src/ipc/unix_socket_server.h/cpp`
- `src/ipc/named_pipe_server.h/cpp`
- `src/ipc/client.h/cpp`
- `src/cli/main.cpp` (extended with all commands)
- `docs/linux-hotkeys.md`
- `scripts/soak_test.py`

## Notes
- Daemon runs in foreground (systemd/user service or Windows service later)
- IPC socket path: respect `$XDG_RUNTIME_DIR`, fallback `/tmp/voicetype-<uid>/socket`
- Windows pipe name: `\\.\pipe\voicetype_<session_id>` for multi-user support
- Signal handling: SIGTERM/SIGINT → graceful shutdown (state→Error→cleanup→exit)
- CLI timeout: 5 seconds default, configurable via `--timeout`