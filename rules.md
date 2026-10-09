# Working Rules: voice2txt

## Mandatory Workflow Rules

### Rule 1: Read Project Overview First
Before starting any work, read `project_overview.md` to understand the project vision, architecture, and current phase context.

### Rule 2: Read Work Log
Before starting any work, read `worklog.md` to understand what has been done, what issues exist, and avoid repeating work.

### Rule 3: Git Commit After Every Phase, Task, and Step
**After completing each phase, task, or step:**
1. Stage changes: `git add -A`
2. Commit with conventional message: `git commit -m "<type>(<scope>): <description>"`
   - Types: `feat`, `fix`, `docs`, `refactor`, `test`, `chore`, `build`
   - Scope: `phase0`, `phase1`, ..., `phase8`, `docs`, `build`, `ci`
3. Push to remote: `git push`

**Commit message examples:**
- `feat(phase1): add miniaudio capture with 16kHz mono output`
- `fix(phase4): handle X11 display connection failure gracefully`
- `docs(phase3): update clipboard implementation notes`
- `test(phase5): add silence corpus WAV files and expected transcripts`

### Rule 4: Phase Gates Are Mandatory
Do not start the next phase until the current phase's exit gate passes. Gates are defined in each phase file under "## Exit Gate".

### Rule 5: Update Work Log After Every Step
Append to `worklog.md` after completing each step with:
- Date
- Phase/Task identifier
- Action taken
- Outcome
- Any issues or blockers encountered

### Rule 6: Keep Documentation Current
Update relevant phase file, roadmap, and project overview when:
- Scope changes
- Decisions are made
- Blockers are resolved
- Gates pass/fail

### Rule 7: Test Before Commit
Run relevant tests (unit, integration, manual matrix) before committing. CI must pass.

### Rule 8: No Placeholder Code
All committed code must be complete and functional. No `TODO`, `FIXME`, or stub implementations unless explicitly tracked as a follow-up task with a GitHub issue reference.

---

## Development Standards

### Code Style
- C++: C++20, clang-format (config in repo root)
- CMake: 3.20+ modern syntax
- JSON: 2-space indent, trailing commas where valid

### Error Handling
- Every IPC command returns a result (success + data OR error + message)
- Daemon logs structured JSON to stdout/stderr
- CLI exits non-zero on failure with descriptive message

### Security
- No network calls except explicit model download (with SHA-256 verification)
- No transcript logging by default
- Never execute transcribed text as commands
- Clear microphone-active indicator in tray

### Performance Budgets (from project_overview.md)
| Metric | Target |
|--------|--------|
| Hotkey to recording start | < 200 ms |
| Stop to transcript (10s, base.en, CPU) | < 1.5 s |
| Idle memory | < 150 MB (excl. model) |
| Recording memory | < 300 MB with base.en |
| Silence-only input | Zero output characters |

---

## Git Branch Strategy
- `main` - protected, only via PR with passing CI
- `phase<N>-<description>` - feature branches per phase
- `hotfix/<issue>` - urgent fixes

## CI Requirements
- GitHub Actions: Windows (latest) + Ubuntu (latest) builds
- All phases must build on both platforms
- Audio corpus regression tests run on every push