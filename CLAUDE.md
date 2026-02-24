<!-- hoggle-memory-system: v1.0 -->
# CLAUDE.md

## Project Identity

**HoggleTune** — A professional autotune/harmonizer audio plugin (AU + VST3 + AAX) built with JUCE.

HoggleTune is a pitch correction and harmonizer plugin for musicians and producers. It supports chromatic and diatonic (all 7 modes) pitch correction with adjustable speed, up to 4 harmony voices at user-selectable intervals, and built-in effects (distortion, reverb, delay). The UI uses a skeuomorphic vintage hardware aesthetic.

**Core values:** Correctness (pitch detection and correction must be accurate), simplicity (beginner-friendly code with clear comments), clean audio (low-artifact pitch shifting).

---

## Development Philosophy

- **No shortcuts.** Modular, scalable code from day one. Don't build it fragile and "optimize later" — build it right the first time.
- **Human hands guide, AI hands build.** The developer is a project manager and architect. They decide direction, make design calls, and approve changes. The AI writes, tests, and maintains the code.
- **AI-optimized code.** Write code that AI agents can efficiently read, modify, and extend. Clear structure, clear naming, clear separation of concerns. The next reader is always an AI.
- **Bring me into decisions.** Never silently choose an approach. When there's a design fork, present the options with trade-offs and let the developer decide.
- **Modular by default.** Every component should be independently understandable, testable, and replaceable. If something can't be explained in isolation, it's too coupled.

---

## Critical Constraints (NON-NEGOTIABLE)

- **Audio quality:** Pitch correction artifacts must be minimized. Distortion under 10%, reverb under 10%, delay under 6% wet by default.
- **Real-time safe:** No memory allocation in the audio callback. No blocking operations. No exceptions in the audio thread.
- **Cross-platform:** Must compile for AU (macOS), VST3 (all platforms), and AAX (Pro Tools). Use only cross-platform JUCE APIs.
- **No secrets in git.** Any API keys or license keys go in `.env` only.

---

## Before You Write Any Code

1. Read `Task.md` — understand the current task
2. Read `learnings/essential.md` — learn from past mistakes
3. Read `SystemDesign.md` — understand the system shape and status tracker
4. Check "Critical Constraints" above — rules that cannot be broken
5. If unsure how something works → **READ THE SOURCE FILES before guessing**

> **Note:** Most projects start with a single `SystemDesign.md`. As it grows past ~500 lines, split into the routed `docs/architecture/` structure (see Architecture Routing in SystemDesign.md for guidance).

---

## When You're Stuck

- About to guess how something works? **STOP.** Read the source file.
- About to modify a pattern you don't understand? **STOP.** Ask the user.
- Fix requires changing more than 3 files? **STOP.** Describe plan first, get approval.
- Same error repeatedly? **STOP.** Re-read architecture docs and learnings/essential.md.

---

## Epistemological Matrix — Code vs. Docs

When the codebase and architecture docs disagree, use the Status Tracker to determine which is truth:

| Status | Code Exists | Code Missing |
|--------|------------|--------------|
| **COMPLETE** | Healthy | **Broken** — docs are stale. Update docs to match code reality. |
| **IN PROGRESS** | Healthy — work underway | Healthy — build hasn't started or code not pushed yet |
| **PLANNED** | **Drifting** — code outpaced docs. Update status to COMPLETE. | Healthy — this is architectural intent, not stale docs. |

**Rule:** "Code is truth" applies ONLY to components marked COMPLETE. For PLANNED components, the docs describe the target architecture. For IN PROGRESS components, partial or missing code is expected.

---

## About the Developer

**Strengths:** Music production, creative direction, product vision
**Challenges:** C++ and audio DSP (beginner level) — needs clear explanations and well-commented code
**Preferences:** Explain what the code does and why. Break complex DSP concepts into plain English. Show the big picture before diving into details.

---

## Scoped Instructions

Rules that apply everywhere live in **`.claude/rules/`**:
- `coding-standards.md` — C++/JUCE coding conventions for this project

---

## Task Completion Checklist

**STOP before saying "done." Every time.**

1. **Update `Task.md`** — mark completed items, note any follow-up work.
2. **Update Status Tracker** — if any component moved from PLANNED to COMPLETE, update `SystemDesign.md`.
3. **Update and PRUNE `learnings/essential.md`** — add new patterns, mistakes, or decisions.
4. **Walk the user through it** — plain language, what changed, what to test
5. **Commit and push** — clean commit message, correct branch

---

## Session Protocol

**Starting:** Read Task.md → state your understanding of the task → **WAIT for approval before writing code.**

**Ending:** Tell the user what changed, what to test, and whether anything needs follow-up.

**Phase Wrap-Up (on request):** When the developer says "run wrap-up" or signals a phase is complete, run the full maintenance pass: archive completed tasks to OlderTasks.md, audit Status Tracker entries, distill learnings/essential.md.
