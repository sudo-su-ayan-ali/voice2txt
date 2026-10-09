# Phase 8: AI Layer (Post v0.1)

## Goal
Optional post-processing hook for transcript enhancement.

## Duration Estimate
TBD (after v0.1 ships)

## Tasks & Steps

### Task 8.1: Define Post-Processing Hook Interface
- [ ] Step 8.1.1: Design hook protocol (stdin/stdout JSON):
  - Daemon writes: `{"text": "raw transcript", "timestamp": "ISO8601", "audio_duration_ms": 1234}`
  - Hook reads stdin, writes stdout: `{"text": "enhanced transcript", "changes": [{"from": "...", "to": "..."}]}`
  - Timeout: 2 seconds max (configurable)
  - Hook process: long-running (daemon spawns on startup) or per-transcription (spawn each time)
- [ ] Step 8.1.2: Alternative: local HTTP endpoint (`POST /enhance` with JSON body)
- [ ] Step 8.1.3: Config: `ai_hook = { "enabled": false, "type": "stdio|http", "command": [...], "url": "...", "timeout_ms": 2000 }`

### Task 8.2: Implement Timeout Guard
- [ ] Step 8.2.1: In daemon transcription pipeline:
  - After local processing (VAD, hallucination filter, replacements)
  - If `ai_hook.enabled`: send to hook, wait for response with timeout
  - On timeout: log warning, use locally-processed transcript, continue
  - On hook error: log, use local transcript
- [ ] Step 8.2.2: Never block typing path - injection uses whatever text is ready at timeout

### Task 8.3: Default Disabled + Opt-In
- [ ] Step 8.3.1: Config default: `enabled: false`
- [ ] Step 8.3.2: Settings UI: "AI Enhancement (Experimental)" toggle + command/URL config
- [ ] Step 8.3.3: Documentation: example hooks (local LLM via Ollama, cloud API with privacy warning)

### Task 8.4: Verify Identical Behavior When Disabled
- [ ] Step 8.4.1: Integration test: run full pipeline with `enabled: false` → same output as no hook code
- [ ] Step 8.4.2: Benchmark: latency with hook disabled = baseline (no overhead)
- [ ] Step 8.4.3: CI test: verify hook disabled path has zero network calls, zero extra processes

## Exit Gate
✅ App works identically with AI hook disabled; hook timeout enforced; no blocking on typing path

## Dependencies
- Phase 7 complete (v0.1 released)

## Deliverables
- `src/ai/hook_interface.h/cpp`
- `src/ai/stdio_hook.h/cpp`
- `src/ai/http_hook.h/cpp`
- Config schema additions (`ai_hook` section)
- Settings UI additions
- Example hook scripts: `examples/ollama_hook.py`, `examples/openai_hook.py`
- Documentation: `docs/ai-hooks.md`
- Integration tests for hook disabled/enabled paths

## Notes
- Hook runs AFTER all local processing (clipboard gets enhanced text if hook succeeds)
- Hook must be sandboxed: no network unless explicitly configured (document risks)
- Consider: hook could run on separate machine via HTTP (local network)
- Timeout must be aggressive (2s) to not degrade perceived latency
- v0.1 ships WITHOUT this feature; architecture supports it for future