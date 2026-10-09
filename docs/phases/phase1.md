# Phase 1: Capture & Transcribe Prototype

## Goal
Working audio capture → transcription pipeline with measured latency/accuracy.

## Duration Estimate
2-3 days

## Tasks & Steps

### Task 1.1: Integrate miniaudio for 16 kHz Mono Float PCM Capture
- [ ] Step 1.1.1: Add miniaudio as git submodule in `third_party/miniaudio/`
- [ ] Step 1.1.2: Create `src/audio/capture.h/cpp` wrapper around miniaudio
- [ ] Step 1.1.3: Implement `AudioCapture` class with:
  - Constructor: device ID (default), sample rate (16000), channels (1), format (float32)
  - `start(callback)` - begins capture, calls callback with float* frames, frame_count
  - `stop()` - stops capture, drains buffer
  - RAII: destructor calls `stop()`
- [ ] Step 1.1.4: Add platform-specific device enumeration (Windows: WASAPI, Linux: PulseAudio/PipeWire/ALSA)
- [ ] Step 1.1.5: Unit test: capture 2 seconds, verify 16kHz mono float output, write WAV for inspection

### Task 1.2: Integrate whisper.cpp for File Transcription
- [ ] Step 1.2.1: Add whisper.cpp as git submodule in `third_party/whisper.cpp/`
- [ ] Step 1.2.2: Build whisper.cpp as static library via CMake (disable examples, enable tools)
- [ ] Step 1.2.3: Create `src/transcribe/whisper_wrapper.h/cpp`:
  - `WhisperContext` RAII wrapper (load model, free on destruction)
  - `transcribe_file(wav_path, params)` → `TranscriptResult { text, language, segments[], timing_ms }`
  - `transcribe_buffer(float* pcm, n_samples, params)` → same result
- [ ] Step 1.2.4: Download `base.en` Q5_1 quantized model (ggml-base.en-q5_1.bin) to `models/`
- [ ] Step 1.2.5: Unit test: transcribe known WAV, verify output matches expected

### Task 1.3: Implement Live Buffer Transcription
- [ ] Step 1.3.1: Design ring buffer for accumulating audio during recording
- [ ] Step 1.3.2: Implement `LiveTranscriber` class:
  - `start()` - begins new session, clears buffer
  - `push_audio(float* pcm, n_samples)` - appends to ring buffer
  - `finish()` - runs full transcription on accumulated audio, returns result
- [ ] Step 1.3.3: Handle memory bounds (max 5 minutes = ~4.8M samples = ~19 MB float)
- [ ] Step 1.3.4: Benchmark: measure transcription time vs audio duration

### Task 1.4: Build CLI `voicetype transcribe <wav>` for Testing
- [ ] Step 1.4.1: Create `src/cli/main.cpp` with argument parsing (simple, no heavy deps)
- [ ] Step 1.4.2: Implement `transcribe` subcommand:
  - Load model (path from arg or default `models/ggml-base.en-q5_1.bin`)
  - Load WAV file (verify 16kHz mono)
  - Run transcription, print JSON result to stdout
  - Exit codes: 0=success, 1=model load fail, 2=audio load fail, 3=transcription fail
- [ ] Step 1.4.3: Add `--benchmark` flag to print timing breakdown

### Task 1.5: Record 20-Utterance Test Corpus
- [ ] Step 1.5.1: Create `test_corpus/` directory structure:
  - `test_corpus/clean/` - 5 quiet room utterances
  - `test_corpus/fast/` - 5 fast speech utterances
  - `test_corpus/noisy/` - 5 with background noise (fan, typing, TV)
  - `test_corpus/silence/` - 3 silence-only (1s, 3s, 5s)
  - `test_corpus/music/` - 2 music clips
- [ ] Step 1.5.2: Record WAV files (16kHz mono, 16-bit PCM)
- [ ] Step 1.5.3: Create `test_corpus/expected.json` with:
  ```json
  {
    "clean/utt1.wav": { "text": "expected transcript", "max_wer": 0.15 },
    "silence/sil_1s.wav": { "text": "", "max_wer": 0.0 }
  }
  ```
- [ ] Step 1.5.4: Commit corpus to repo (Git LFS if >100MB total)

### Task 1.6: Benchmark Latency and Accuracy
- [ ] Step 1.6.1: Create `scripts/benchmark_corpus.py` (or C++ tool):
  - Runs `voicetype transcribe` on each corpus file
  - Computes WER against expected transcripts
  - Measures wall-clock latency (stop-to-text)
  - Outputs summary table + JSON for CI
- [ ] Step 1.6.2: Run on target hardware, record results in `benchmark_results.md`
- [ ] Step 1.6.3: If WER >15% or latency >1.5s: try `small.en` Q5, document trade-off

## Exit Gate
✅ 20 test utterances transcribe with WER ≤15% on clean/fast/noisy, zero output on silence/music, and latency <1.5s for 10s utterance on target hardware

## Dependencies
- Phase 0 complete

## Deliverables
- `src/audio/capture.h/cpp`
- `src/transcribe/whisper_wrapper.h/cpp`
- `src/transcribe/live_transcriber.h/cpp`
- `src/cli/main.cpp` (with `transcribe` command)
- `models/ggml-base.en-q5_1.bin` (downloaded, not committed)
- `test_corpus/` (20 WAVs + expected.json)
- `scripts/benchmark_corpus.py`
- `benchmark_results.md`

## Notes
- whisper.cpp build: `-DWHISPER_SDL2=OFF -DBUILD_SHARED_LIBS=OFF`
- Model download: use `whisper.cpp/models/download-ggml-model.sh base.en` then quantize to Q5_1
- Ring buffer: use `std::vector<float>` with head/tail indices, pre-allocate max size
- WER calculation: use `jiwer` Python package or implement Levenshtein in C++