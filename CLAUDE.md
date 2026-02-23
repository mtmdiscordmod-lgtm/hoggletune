# CLAUDE.md — Bootstrap Mode

> **THIS IS A TEMPORARY BOOTSTRAP FILE.** It tells Claude Code how to set up a new project. After setup, this file gets replaced with the real project CLAUDE.md.

---

## What You (Claude Code) Should Do

You've been opened on a brand new project. The developer wants you to set up the full Claude Code development system. Follow these steps in order.

---

## Step 0: Check for Existing Design Documents (Ingest Mode)

**Before starting the interview, ask the developer:**

> "Do you have an existing design document, system design, or architecture spec to ingest? (If you already designed this project with another AI or wrote it up yourself, I can extract everything I need from that instead of running the full interview.)"

**WAIT for the developer's answer before proceeding.**

**If YES (Ingest Mode):**
1. Read the provided document(s) thoroughly
2. Extract: project name, purpose, audience, tech stack, architecture, constraints, coding conventions, developer preferences
3. Proceed to Step 2 (Generate Project Files) — use the extracted information instead of interview answers. All the generation rules in Step 2 still apply (version tags, exceptions, customization rules).
4. Then continue to Step 3 (Walk Through Results)

**If NO:** Proceed to Step 1 (the interview).

---

## Step 1: Interview the Developer

Ask these questions. Use the answers to customize all generated files. Ask them conversationally — don't dump all questions at once. Group them naturally.

### Project Basics
1. **What's the project name?**
2. **What does it do?** (One sentence is fine. Two is great.)
3. **Who is it for?** (Personal tool? Team project? End users? Open source?)

### Technical
4. **What's the tech stack?** (Languages, frameworks, databases, hosting)
5. **How is the code organized?** (Monorepo? Separate frontend/backend? Microservices? Simple script?)
6. **Are there existing tests?** (If so, how do you run them?)
7. **How do you deploy?** (If applicable — local-only projects skip this)

### Constraints & Values
8. **What are the non-negotiable rules?** (Things that must NEVER be violated. Security requirements, compliance, performance budgets, etc. "None yet" is a valid answer.)
9. **What matters most to you?** (Pick 2-3: correctness, speed, clean code, test coverage, simplicity, documentation, backwards compatibility, user privacy, etc.)

### Working Style
10. **What are your strengths?** (So Claude Code knows what to explain vs. what to skip)
11. **What do you want help with?** (So Claude Code knows where to be more detailed)
12. **Any preferences for how Claude Code should work?** (e.g., "Always explain before doing," "Just do it and show me," "Ask before modifying more than 2 files")

### Optional
13. **Any secrets or API keys the project will need?** (Just the names — "OPENAI_API_KEY", "DATABASE_URL" — not the values. These go in .env.example.)
14. **Anything that should be gitignored?** (Beyond the usual suspects for your tech stack)

---

## Step 2: Generate Project Files

Use the templates in the `templates/` folder as starting points. **Customize every file** based on the interview answers (or ingested design document). Never leave placeholder text.

**All generated files must include the version metadata tag** — each template already has `<!-- hoggle-memory-system: v1.0 -->` at the top. Preserve this in the generated files. **Exception:** `.gitignore` and `.env.example` use `#` for comments, not HTML — do NOT add the HTML version tag to these files.

### Files to generate:

1. **`CLAUDE.md`** (replaces THIS file) — from `templates/CLAUDE-template.md`
   - Fill in project identity, constraints, developer profile, session protocol
   - The Development Philosophy section and Epistemological Matrix ship as-is (don't customize these — they're universal)
   - **Resolve the "OR" in "Before You Write Any Code":** The template says `SystemDesign.md OR docs/architecture/overview.md`. Replace this with whichever path actually exists in this project (almost always `SystemDesign.md` for new projects).
   - This is the real CLAUDE.md that future sessions will read

2. **`SystemDesign.md`** — from `templates/SystemDesign-template.md`
   - For a brand new project, the architecture section will be sparse — that's fine
   - Document what's PLANNED, not just what exists
   - Include the tech stack decisions from the interview
   - Keep the Architecture Routing section — it tells future sessions how to split this file when it grows

3. **`Task.md`** — from `templates/Task-template.md`
   - Ask the developer: "What's the first thing you want to build?"
   - Create the initial task list from their answer

4. **`OlderTasks.md`** — from `templates/OlderTasks-template.md`
   - Generate as-is (mostly empty). This is the archive for completed tasks.

5. **`learnings/essential.md`** — from `templates/learnings-essential-template.md`
   - Start with the developer profile and any stated preferences
   - The "mistakes to avoid" and "proven patterns" sections start empty — that's correct
   - The "outdated lessons" section starts empty — that's correct

6. **`.claude/rules/`** — from `templates/claude-rules-template.md`
   - Only create rules that apply based on the interview answers
   - Don't create rules for things the project doesn't do
   - At minimum, create one file for coding standards if the developer mentioned any preferences

7. **`.gitignore`** — from `templates/gitignore-template`
   - Customize for the stated tech stack
   - Always include `.env` and OS files

8. **`.env.example`** — from `templates/env-example-template`
   - Include any secret names mentioned in the interview
   - Add common ones for the tech stack (DATABASE_URL, PORT, etc.)

---

## Step 3: Walk Through Results

After generating all files:

1. **List every file created** with a one-line explanation
2. **Read back the CLAUDE.md constraints** — make sure the developer agrees
3. **Confirm the first task** in Task.md
4. **Remind them to:**
   - Review and edit the generated files
   - Delete the `templates/` folder
   - Run `git add . && git commit -m "Initial project setup with Claude Code memory system"`
   - Copy `.env.example` to `.env` and fill in real values

---

## Important Notes

- **Don't rush the interview.** Good answers = good files. Bad answers = files that need rewriting.
- **Don't over-engineer.** A new project needs a lightweight setup. Heavy documentation comes later as the project grows.
- **The SystemDesign.md will be sparse.** That's fine for a new project. It grows as architecture is built.
- **Ask "What's the first thing you want to build?" last.** That becomes the first Task.md entry, and gives the developer momentum to start immediately after setup.
- **Preserve version tags.** Every generated file must keep the `<!-- hoggle-memory-system: v1.0 -->` comment at the top.
