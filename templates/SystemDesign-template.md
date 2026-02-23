<!-- hoggle-memory-system: v1.0 -->
# SystemDesign.md

> **For small/new projects**, this single file is fine. As it grows past ~500 lines, split into `docs/architecture/` using the routed structure (see Architecture Routing below).

## Overview

[2-3 sentences: What does this system do? Who is it for? What's the core architectural idea?]

**Current state:** [Brief description of what's built, what's in progress, what's planned.]

---

## Architecture Diagram

```
[ASCII diagram of how the major components connect. Keep it simple — show data flow, not every file.]

Example structure:
┌──────────┐     ┌──────────┐     ┌──────────┐
│  Client   │────>│  Server  │────>│ Database │
└──────────┘     └──────────┘     └──────────┘
```

---

## Component Details

### 1. [Component Name]

**Purpose:** [What does this component do?]
**Key files:** [List the main files]
**Dependencies:** [What does it depend on?]

[Brief description of how it works. Include data flow if relevant.]

### 2. [Component Name]

[Same structure as above. Add as many components as needed.]

---

## Key Contracts

[Document the important interfaces between components — APIs, data formats, config schemas. These are the things that, if changed, break other components.]

### [Contract Name]
- **Between:** [Component A] and [Component B]
- **Format:** [JSON, protobuf, function signature, etc.]
- **Schema:** [Key fields and types]

---

## Known Pitfalls

[Things that have gone wrong before or are easy to get wrong. Future sessions read this to avoid repeating mistakes.]

- **[Pitfall name]:** [What goes wrong and how to avoid it]

---

## Status Tracker

Track what's built, what's in progress, and what's planned. **Scope this to high-level architectural components only** — not feature-level subtasks (those belong in Task.md).

The Status Tracker is the disambiguation key for the Epistemological Matrix in CLAUDE.md. It determines whether missing code means "not built yet" (PLANNED) or "stale docs" (COMPLETE but code removed).

| Component | Status | Notes |
|-----------|--------|-------|
| [Name] | COMPLETE / IN PROGRESS / PLANNED | [Brief note] |

---

## Configuration

[Document where config lives and what the key settings are.]

| Config File | Purpose | Sensitive? |
|-------------|---------|------------|
| `.env` | Secrets and environment-specific values | YES — gitignored |
| [other files] | [purpose] | [yes/no] |

---

## Dependencies

[Key external dependencies and why they were chosen.]

| Dependency | Purpose | Why This One |
|------------|---------|--------------|
| [name] | [what it does] | [why chosen over alternatives] |

---

## Ignored / Utility Directories

[Directories that have been reviewed and intentionally excluded from architecture documentation. List them here so the HealthCheck's Dark Matter scan doesn't repeatedly flag them.]

| Directory | Reason |
|-----------|--------|
| [e.g., `src/components/ui/`] | [e.g., Standard UI primitives — no architectural significance] |

---

## Architecture Routing

When this file grows past ~500 lines, split into a routed structure:

```
docs/
  architecture/
    INDEX.md           ← component-to-doc mapping table (always read)
    overview.md        ← this file's Overview + Diagram + Status Tracker (always read)
    [component].md     ← one per major subsystem (read only when relevant)
```

**INDEX.md format:**

| Component | Doc | Scope |
|-----------|-----|-------|
| [Name] | `[name].md` | [What this component covers] |

CLAUDE.md's "Before You Write Any Code" section routes Claude Code to read only the relevant component docs based on the current task, keeping context lean.
