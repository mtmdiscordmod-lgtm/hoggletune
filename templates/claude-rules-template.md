<!-- hoggle-memory-system: v1.0 -->
# .claude/rules/ — Scoped Rules Guide

Place rule files in `.claude/rules/` for cross-cutting concerns that apply to ALL work in the project. Claude Code loads these automatically.

**Create one `.md` file per concern.** Each file should be short (under 30 lines) and focused on a single topic. Don't combine unrelated rules in one file — separate concerns make rules easier to maintain and update. The examples below show the format for each category — pick only the ones that apply to your project.

---

## When to Create a Rule File

Create a rule file when:
- A constraint applies everywhere (not just one directory)
- You've made the same mistake twice and want to prevent a third
- There's a security or compliance requirement that must never be violated
- A team convention needs enforcement

## Suggested Rule Files

Choose the ones that apply to your project. Don't create rules you don't need.

### coding-standards.md
```markdown
# Coding Standards

- [Language-specific conventions: naming, formatting, patterns]
- [Import ordering rules]
- [Error handling patterns]
- [Logging conventions]
```

### testing-rules.md
```markdown
# Testing Rules

- [Test framework and how to run tests]
- [What must be tested: all public APIs? all bug fixes?]
- [Test naming conventions]
- [Mocking policy: what can be mocked, what must be real]
```

### security-rules.md
```markdown
# Security Rules

- [Secret handling: where do secrets live, what's gitignored]
- [Input validation requirements]
- [Authentication/authorization patterns]
- [Dependency vetting: how to evaluate new libraries]
```

### deployment-rules.md
```markdown
# Deployment Rules

- [How to deploy: commands, environments, approval process]
- [What must pass before deploying: tests, linting, type checks]
- [Rollback procedure]
- [Environment-specific config handling]
```

---

## Rule File Best Practices

- **Keep rules short and specific.** "Never commit .env files" is better than a paragraph about secret management philosophy.
- **Explain WHY when it's not obvious.** "Use UTC for all timestamps (because our servers span 3 time zones)" is better than just "Use UTC."
- **Rules should be enforceable.** If Claude Code can't verify compliance, it's guidance, not a rule. Put guidance in CLAUDE.md instead.
- **Update rules when they change.** Stale rules are worse than no rules — they cause confusion.
