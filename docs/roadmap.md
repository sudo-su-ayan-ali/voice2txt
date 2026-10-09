# Roadmap: voice2txt

## Phase Overview

| Phase | Focus | Est. Duration | Status |
|-------|-------|---------------|--------|
| **Phase 0** | Environment & CI Setup | 0.5 days | 🔄 Planned |
| **Phase 1** | Capture & Transcribe Prototype | 2-3 days | ⏳ Pending |
| **Phase 2** | Daemon, IPC & Hotkey | 3-4 days | ⏳ Pending |
| **Phase 3** | Output & Clipboard | 2 days | ⏳ Pending |
| **Phase 4** | Text Injection (Riskiest) | 4-6 days | ⏳ Pending |
| **Phase 5** | Accuracy Hardening | 3-4 days | ⏳ Pending |
| **Phase 6** | Settings & Tray (Qt) | 4-5 days | ⏳ Pending |
| **Phase 7** | Packaging & Release | 4-6 days | ⏳ Pending |
| **Phase 8** | AI Layer (Post v0.1) | TBD | ⏳ Future |

---

## Phase 0: Environment & CI Setup
**Goal**: Reproducible build environment on Windows and Linux with CI validation.

### Tasks
- [ ] 0.1 Pin dependency versions (whisper.cpp tag, miniaudio version, CMake, compiler)
- [ ] 0.2 Create CMake project structure with cross-platform support
- [ ] 0.3 Set up GitHub Actions workflow for Windows + Ubuntu builds
- [ ] 0.4 Verify empty project builds on both platforms in CI

### Exit Gate
✅ Empty CMake project builds successfully on GitHub Actions Windows and Ubuntu runners

### Dependencies
- None (starting phase)

---

## Phase 1: Capture & Transcribe Prototype
**Goal**: Working audio capture → transcription pipeline with measured latency/accuracy.

### Tasks
- [ ] 1.1 Integrate miniaudio for 16 kHz mono float PCM capture
- [ ] 1.2 Integrate whisper.cpp (static library) for file transcription
- [ ] 1.3 Implement live buffer transcription (streaming to whisper)
- [ ] 1.4 Build CLI `voicetype transcribe <wav>` for testing
- [ ] 1.5 Record 20-utterance test corpus (quiet, fast, noisy, silence, music)
- [ ] 1.6 Benchmark latency and accuracy on target hardware

### Exit Gate
✅ 20 test utterances transcribe with acceptable WER (target ≤15%) and latency < 1.5s for 10s utterance on target hardware

### Dependencies
- Phase 0 complete