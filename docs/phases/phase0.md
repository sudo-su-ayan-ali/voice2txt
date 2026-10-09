# Phase 0: Environment & CI Setup

## Goal
Reproducible build environment on Windows and Linux with CI validation.

## Duration Estimate
0.5 days

## Tasks & Steps

### Task 0.1: Pin Dependency Versions
- [ ] Step 0.1.1: Choose whisper.cpp git tag (e.g., `v1.5.4` or latest stable)
- [ ] Step 0.1.2: Choose miniaudio version (single header, pin commit/tag)
- [ ] Step 0.1.3: Set minimum CMake version (3.20+)
- [ ] Step 0.1.4: Set C++ standard (C++20)
- [ ] Step 0.1.5: Document compiler requirements (GCC 11+, Clang 13+, MSVC 19.30+)
- [ ] Step 0.1.6: Create `dependencies.md` with all pinned versions and rationale

### Task 0.2: Create CMake Project Structure
- [ ] Step 0.2.1: Create root `CMakeLists.txt` with project definition
- [ ] Step 0.2.2: Create `cmake/` directory for helper modules (FindWhisper, FindMiniaudio, etc.)
- [ ] Step 0.2.3: Create `src/` with subdirectories: `daemon/`, `cli/`, `common/`, `audio/`, `transcribe/`, `ipc/`, `injection/`, `clipboard/`, `platform/`
- [ ] Step 0.2.4: Create `tests/` directory structure
- [ ] Step 0.2.5: Create `third_party/` for vendored dependencies (git submodules)
- [ ] Step 0.2.6: Add CMake options for: `BUILD_DAEMON`, `BUILD_CLI`, `BUILD_TESTS`, `USE_SYSTEM_WHISPER`, `USE_SYSTE_MINIAUDIO`
- [ ] Step 0.2.7: Configure platform-specific settings (Windows: static runtime, Linux: position-independent code)

### Task 0.3: Set Up GitHub Actions CI
- [ ] Step 0.3.1: Create `.github/workflows/ci.yml` with matrix: `ubuntu-latest`, `windows-latest`
- [ ] Step 0.3.2: Install dependencies on Ubuntu: `build-essential`, `cmake`, `ninja-build`, `libasound2-dev`, `libx11-dev`, `libxtst-dev`, `libwayland-dev`, `pkg-config`
- [ ] Step 0.3.3: Install dependencies on Windows: `cmake`, `ninja`, `Visual Studio Build Tools` (via `microsoft/setup-msbuild`)
- [ ] Step 0.3.4: Configure build steps: configure → build → test (when tests exist)
- [ ] Step 0.3.5: Add artifact upload for binaries (optional, for later phases)
- [ ] Step 0.3.6: Add caching for `third_party/` and CMake build directories

### Task 0.4: Verify Empty Project Builds in CI
- [ ] Step 0.4.1: Push initial commit to trigger CI
- [ ] Step 0.4.2: Verify both Ubuntu and Windows builds pass (configure + build)
- [ ] Step 0.4.3: Fix any platform-specific CMake issues
- [ ] Step 0.4.4: Document any CI-specific quirks in `docs/ci-notes.md`

## Exit Gate
✅ Empty CMake project builds successfully on GitHub Actions Windows and Ubuntu runners

## Dependencies
- None (starting phase)

## Deliverables
- `CMakeLists.txt` (root)
- `cmake/` helper modules
- `src/` directory structure
- `.github/workflows/ci.yml`
- `dependencies.md`
- `docs/ci-notes.md`

## Notes
- Keep CMake modern (target-based, no global include directories)
- Use `FetchContent` or git submodules for whisper.cpp and miniaudio
- Windows: prefer static linking for runtime to avoid DLL hell
- Linux: ensure Wayland/X11 development headers available in CI