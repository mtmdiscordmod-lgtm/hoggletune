<!-- hoggle-memory-system: v1.0 -->
# CLAUDE.md

## Project Identity

[PROJECT_NAME] — [ONE_LINE_DESCRIPTION]

[2-3 sentences about what this project does, who it's for, and why it exists. This is the first thing Claude Code reads every session — make it count.]

**Core values:** [What matters most? Examples: correctness over speed, user privacy, backwards compatibility, clean APIs, test coverage. Pick 2-4 that are real, not aspirational.]

---

## Development Philosophy

- **No shortcuts.** Modular, scalable code from day one. Don't build it fragile and "optimize later" — build it right the first time.
- **Human hands guide, AI hands build.** The developer is a project manager and architect. They decide direction, make design calls, and approve changes. The AI writes, tests, and maintains the code.
- **AI-optimized code.** Write code that AI agents can efficiently read, modify, and extend. Clear structure, clear naming, clear separation of concerns. The next reader is always an AI.
- **Bring me into decisions.** Never silently choose an approach. When there's a design fork, present the options with trade-offs and let the developer decide.
- **Modular by default.** Every component should be independently understandable, testable, and replaceable. If something can't be explained in isolation, it's too coupled.

---

## Critical Constraints (NON-NEGOTIABLE)

[List the hard rules that can NEVER be broken. These are the things that, if violated, would cause real damage. Not aspirational goals — actual constraints.]

[Examples — delete ones that don't apply, add ones that do:]
- **Security:** [e.g., No secrets in git. API keys in .env only. Input validation on all user-facing endpoints.]
- **Privacy:** [e.g., No PII in logs. User data encrypted at rest. GDPR compliance required.]
- **Performance:** [e.g., Page load under 3 seconds. API responses under 500ms. Bundle size under 200KB.]
- **Compatibility:** [e.g., Must support Node 18+. Must work in Chrome, Firefox, Safari. No breaking API changes without major version bump.]

---

## Before You Write Any Code

1. Read `Task.md` — understand the current task
2. Read `learnings/essential.md` — learn from past mistakes
3. Read `SystemDesign.md` OR `docs/architecture/overview.md` — whichever exists — to understand the system shape and status tracker
4. If using routed architecture (`docs/architecture/`), read ONLY the component docs relevant to your current task (see `INDEX.md` for the map)
5. Check "Critical Constraints" above — rules that cannot be broken
6. If unsure how something works → **READ THE SOURCE FILES before guessing**

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

[Help Claude Code communicate effectively with you.]

**Strengths:** [e.g., Backend architecture, system design, Python]
**Challenges:** [e.g., CSS, frontend build tools, regex]
**Preferences:** [e.g., Explain commands before running them. Prefers explicit over implicit. Wants to understand WHY, not just HOW.]

---

## Scoped Instructions

[If subdirectories have their own conventions, list them here. Claude Code loads subdirectory CLAUDE.md files automatically.]

Rules that apply everywhere live in **`.claude/rules/`**:
- [List your rules files and what they cover]

---

## Task Completion Checklist

**STOP before saying "done." Every time.**

> **Scope:** For micro-tasks (quick bug fixes, small tweaks), steps 1 (Update Task.md) and 4-5 (Walk through + Commit) are sufficient. Save the full checklist — especially architecture and learnings updates — for substantial work. Use the **Phase Wrap-Up** command (see Session Protocol) to batch heavy maintenance.

1. **Update `Task.md`** — mark completed items, note any follow-up work. If all current tasks are done, archive completed sections to `OlderTasks.md` (timestamped).
2. **Update Status Tracker** — if any component moved from PLANNED to COMPLETE (or a new component was added), update the status tracker in `docs/architecture/overview.md` (or `SystemDesign.md`). Be specific — don't just say "update docs."
3. **Update and PRUNE `learnings/essential.md`** — add new patterns, mistakes, or decisions. Remove or archive outdated entries. Keep it lean — this file is read every session. If over ~80 lines, distill aggressively.
4. **Walk the user through it** — plain language, what changed, what to test
5. **Commit and push** — clean commit message, correct branch

---

## Session Protocol

**Starting:** Read Task.md → state your understanding of the task → **WAIT for approval before writing code.** Do not charge ahead on assumptions.

**Ending:** Tell the user what changed, what to test, and whether anything needs follow-up.

**Phase Wrap-Up (on request):** When the developer says "run wrap-up" or signals a phase is complete, run the full maintenance pass: archive completed tasks to OlderTasks.md, audit Status Tracker entries against the codebase, distill learnings/essential.md, and recommend a HealthCheck. This batches the heavy clerical work so it doesn't tax every micro-task.
