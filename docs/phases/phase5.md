# Phase 5: Accuracy Hardening

## Goal
Eliminate phantom text, improve real-world accuracy.

## Duration Estimate
3-4 days

## Tasks & Steps

### Task 5.1: Enable Silero VAD (Built into whisper.cpp)
- [ ] Step 5.1.1: Verify whisper.cpp build includes VAD (`-DWHISPER_VAD=ON`)
- [ ] Step 5.1.2: In `LiveTranscriber`, enable VAD via `whisper_full_params`:
  - `params.vad_threshold = 0.6` (tunable)
  - `params.no_speech_threshold = 0.6`
- [ ] Step 5.1.3: Test: VAD should trim leading/trailing silence from transcription segments

### Task 5.2: Minimum Speech Duration Gate
- [ ] Step 5.2.1: After transcription, compute total speech duration from VAD segments
- [ ] Step 5.2.2: If `total_speech_ms < 300` (0.3s): discard result, return empty transcript
- [ ] Step 5.2.3: Log discarded attempts (debug level) with audio energy stats

### Task 5.3: Hallucination Filter
- [ ] Step 5.3.1: Create `src/accuracy/hallucination_filter.h/cpp`:
  - Maintain versioned list in `data/hallucinations.json`:
    ```json
    { "patterns": ["^thank you\\.?$", "^\\[music\\]$", "^you$", "^the$"], "min_energy_db": -40 }
    ```
  - `filter(transcript, avg_energy_db)` → `filtered_transcript` (empty if matched)
  - Case-insensitive, regex matching
  - Only apply when `avg_energy_db < min_energy_db` (low energy = likely silence)
- [ ] Step 5.3.2: Compute average audio energy (RMS dB) during recording
- [ ] Step 5.3.3: Unit test: silence file → empty output; quiet "thank you" → filtered; loud "thank you" → kept

### Task 5.4: Custom Vocabulary via Initial Prompt
- [ ] Step 5.4.1: Add config `initial_prompt` (string, default empty)
- [ ] Step 5.4.2: Pass to `whisper_full_params.prompt_tokens` (whisper.cpp tokenizes automatically)
- [ ] Step 5.4.3: Document: use for domain terms, names, technical vocabulary
- [ ] Step 5.4.4: CLI command `voicetype config set initial_prompt "..."`

### Task 5.5: User Replacement List (Post-Processing)
- [ ] Step 5.5.1: Add config `replacements` (array of `{ "from": "...", "to": "..." }`)
- [ ] Step 5.5.2: Implement `PostProcessor`:
  - Apply replacements in order (first match wins per position)
  - Support regex or literal (config per entry)
  - Run after hallucination filter, before clipboard/injection
- [ ] Step 5.5.3: Example: `{ "from": "voicetype", "to": "VoiceType" }`

### Task 5.6: Silence/Noise Test Corpus in CI
- [ ] Step 5.6.1: Ensure `test_corpus/silence/` and `test_corpus/noisy/` have expected transcripts (empty for silence)
- [ ] Step 5.6.2: Extend `scripts/benchmark_corpus.py`:
  - Fail CI if ANY silence file produces non-empty output
  - Fail CI if WER on clean set regresses >2% from baseline
  - Output JSON for CI artifact
- [ ] Step 5.6.3: Add GitHub Actions step to run accuracy tests on every push

## Exit Gate
✅ Silence and noise test files produce zero phantom words; WER on clean speech maintains or improves vs Phase 1 baseline

## Dependencies
- Phase 4 complete

## Deliverables
- `src/accuracy/hallucination_filter.h/cpp`
- `src/accuracy/post_processor.h/cpp`
- `data/hallucinations.json` (versioned, committed)
- Updated `src/transcribe/live_transcriber.h/cpp` (VAD integration, energy computation)
- Updated `src/daemon/daemon.h/cpp` (post-processing pipeline)
- Extended `scripts/benchmark_corpus.py` (CI integration)
- CI workflow update (accuracy test step)
- Config schema additions (`initial_prompt`, `replacements`)

## Notes
- Hallucination list should be curated from real-world testing, not just theoretical
- Energy threshold: calibrate on your microphone; -40 dB is a starting point
- VAD in whisper.cpp runs on CPU; negligible overhead
- Replacement list: keep simple for v0.1; regex support optional
- All accuracy features must be configurable and disableable