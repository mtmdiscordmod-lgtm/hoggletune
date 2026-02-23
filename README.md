# Project Starter

**Bootstraps a new project with the Claude Code development system from day one.**

Start right. This sets up the full structure — CLAUDE.md, SystemDesign.md, Task.md, learnings, rules — customized to your new project through an interactive interview or design document ingestion.

---

## What Gets Created

| File/Folder | Purpose |
|-------------|---------|
| `CLAUDE.md` | Project identity, development philosophy, constraints, epistemological matrix, session protocol |
| `SystemDesign.md` | Architecture docs — components, status tracker, routing guidance for scaling |
| `Task.md` | Current task tracking — what's being built right now |
| `OlderTasks.md` | Task archive — completed tasks move here to keep Task.md lean |
| `learnings/essential.md` | Institutional memory — starts sparse, grows as you work |
| `.claude/rules/` | Cross-cutting rules (security, coding standards, etc.) |
| `.gitignore` | Sensible defaults for your tech stack |
| `.env.example` | Template for environment variables (actual `.env` is gitignored) |

---

## How to Use

### Step 1: Create Your Project Folder

```bash
mkdir my-new-project
cd my-new-project
git init
```

### Step 2: Copy Starter Contents

Copy `CLAUDE.md` and the `templates/` folder from this `Project-Starter/` folder into your new project root (don't copy this README — it's documentation for you, not a project file):

```
my-new-project/
├── CLAUDE.md              <-- the bootstrap file (gets replaced)
├── templates/             <-- used during setup, deleted after
│   ├── CLAUDE-template.md
│   ├── SystemDesign-template.md
│   ├── Task-template.md
│   ├── OlderTasks-template.md
│   ├── learnings-essential-template.md
│   ├── claude-rules-template.md
│   ├── gitignore-template
│   └── env-example-template
└── (empty — your code goes here)
```

### Step 3: Open Claude Code

Open Claude Code in your new project directory.

### Step 4: Tell Claude Code to Bootstrap

Say this (or something like it):

> Set up this project. Read CLAUDE.md for instructions.

Claude Code will:
1. Read the bootstrap CLAUDE.md (which contains setup instructions, not project instructions)
2. **Ask if you have an existing design document to ingest** — if yes, it extracts everything from that instead of running the interview
3. If no design doc: **Interview you** about your project — name, purpose, tech stack, constraints, preferences
4. Generate all project files customized to your answers
5. **Replace the bootstrap CLAUDE.md** with your real project CLAUDE.md
6. Walk you through everything that was created

### Step 5: Clean Up

After setup is complete:

```bash
rm -rf templates/
```

The templates are no longer needed — your project has real, customized files.

### Step 6: Start Building

Your project is now set up with the full Claude Code development system. Every future session, Claude Code will:
- Read your Task.md to know what to work on
- Check learnings/essential.md to avoid past mistakes
- Follow your CLAUDE.md constraints and session protocol
- Update docs as it works
- Use the epistemological matrix to know when code vs. docs is the source of truth

---

## Choosing the Right Tool

```
Has existing code?
├─ No  → Project-Starter (this tool)
└─ Yes → Has memory system files (CLAUDE.md, SystemDesign.md)?
         ├─ No  → Project-Optimizer
         └─ Yes → On latest version?
                  ├─ No  → Project-Updater
                  └─ Yes → Project-HealthCheck
```

---

## Tips

- **Answer the interview questions thoroughly** (or provide a detailed design document). The more Claude Code knows upfront, the better the generated files will be.
- **You can always edit the generated files.** They're starting points, not sacred documents.
- **The learnings file will be mostly empty.** That's expected — it fills up as you work together.
- **Add your first task to Task.md immediately.** Give Claude Code something to work on.
- **Commit the generated files before writing any code.** Clean starting point.
