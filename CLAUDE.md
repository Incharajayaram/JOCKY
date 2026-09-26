# JOCKY Development Guidelines

## Project Status
**Open Source Project** — All contributions should follow high-quality standards. No redundant code, no unnecessary comments, clean git history.

---

## Code Quality Standards

### 1. No Redundant Code
- Write code once. Use abstractions only when needed (3+ similar instances).
- No copy-paste. Refactor repeated logic into helper functions.
- Delete unused code completely. No commented-out code.
- No stub implementations or half-finished features.

### 2. Comments & Documentation
- **No unnecessary comments.** Code should be self-documenting via clear naming.
- Only add comments when:
  - **WHY** is non-obvious (hidden constraint, workaround, subtle invariant)
  - Implementation uses a surprising technique
  - Cross-file or non-local behavior needs explanation
- Never comment WHAT the code does — that's what readable code does.
- No multi-line docstrings. Use single-line comments max.

### 3. Type & Style
- Follow Python/C++ conventions for the codebase.
- Use meaningful variable/function names. Avoid abbreviations.
- Keep functions focused. Single responsibility.
- Prefer explicit over implicit.

---

## Git & Branching Workflow

### 1. Branching Strategy
- **Main branch:** Production-ready code only.
- **Feature branches:** One feature per branch.
- Branch naming: `feature/descriptor` or `fix/descriptor`
  - Examples: `feature/type-aliases`, `fix/pattern-matching-codegen`

### 2. Before Every Push
```bash
# 1. Fetch latest main
git fetch origin main

# 2. Rebase your branch on main
git rebase origin/main

# 3. Resolve conflicts if any
# 4. Test locally (run test suite)
# 5. Force push to feature branch (safe since it's your own branch)
git push -f origin your-feature-branch
```

### 3. Commits
- **One logical change per commit** — easy to review and revert if needed.
- Commit message format:
  ```
  Brief description of change (under 70 chars)
  
  Longer explanation if needed. Explain the "why", not the "what".
  Reference issue numbers if applicable: fixes #123
  ```
- **NO Co-Author lines.** Every commit is from Claude Haiku 4.5.
- Keep commits atomic. Don't mix refactoring with feature work.

### 4. Pull Requests
- **One PR per feature.** Don't batch multiple features.
- PR title: Short, descriptive (same as commit message first line)
- PR description:
  ```markdown
  ## Summary
  - What this feature does
  - Why it was needed
  
  ## Changes
  - List of files modified
  - Key implementation details
  
  ## Testing
  - How to test this feature
  - Tests added/modified
  ```
- Wait for review before merging.
- Squash or rebase before merge if needed to keep history clean.

---

## Development Workflow

### Starting a Feature
```bash
# 1. Update main
git checkout main
git pull origin main

# 2. Create feature branch
git checkout -b feature/my-feature

# 3. Develop & commit regularly
# 4. Push to trigger CI
git push -u origin feature/my-feature
```

### Finishing a Feature
```bash
# 1. Ensure main is latest
git fetch origin main

# 2. Rebase on main
git rebase origin/main

# 3. Run full test suite locally
pytest tests/

# 4. Force push
git push -f origin feature/my-feature

# 5. Open PR on GitHub
# 6. Wait for review/CI
# 7. Merge to main
```

---

## Testing Requirements

### Before Pushing
- [ ] All existing tests pass: `pytest tests/`
- [ ] New feature has unit tests
- [ ] Integration tests added if relevant
- [ ] No regressions (run full suite)

### Test Coverage
- Unit tests for parser, type checker, codegen
- Integration tests for end-to-end pipeline
- Example programs that demonstrate the feature
- Edge cases and error paths tested

---

## Code Review Checklist

Before opening PR, self-review:
- [ ] No commented-out code
- [ ] No debug print statements (unless intentional)
- [ ] No unused variables or imports
- [ ] Clear commit history (squash if needed)
- [ ] Rebased on latest main
- [ ] Tests pass locally
- [ ] Documentation updated
- [ ] No breaking changes (or documented)

---

## File Organization

```
JOCKY/
├── src/jocky/
│   ├── language/          # Parser, type checker, codegen
│   ├── cli.py            # Command-line interface
│   ├── core/             # Core compilation pipeline
│   ├── passes/           # Obfuscation passes
│   └── stdlib/           # Standard library
├── compiler/             # C++ compiler components
├── src/runtime/          # C runtime library
├── examples/             # Example programs
├── tests/                # Test suite
│   ├── unit/            # Unit tests
│   └── integration/      # Integration tests
├── docs/                 # Documentation
└── README.md            # Project overview
```

---

## Example: Feature Branch Workflow

### Step 1: Create Branch
```bash
git checkout -b feature/pattern-matching
```

### Step 2: Implement Feature
- Add parser support for patterns
- Update type checker for validation
- Implement codegen for MatchExpr
- Add tests
- Update docs

### Step 3: Commit (one per logical step)
```bash
git add src/jocky/language/parser.py
git commit -m "Add pattern parsing for match expressions"

git add src/jocky/language/checker.py
git commit -m "Type check match expressions and patterns"

git add src/jocky/language/codegen.py
git commit -m "Codegen for match expressions with switch fallback"

git add tests/unit/test_pattern_matching.py
git commit -m "Add comprehensive pattern matching tests"
```

### Step 4: Rebase & Push
```bash
git fetch origin main
git rebase origin/main
pytest tests/  # Verify all tests pass
git push -f origin feature/pattern-matching
```

### Step 5: Open PR
- Title: "Add pattern matching support for enums"
- Description: Explain what patterns do, test coverage, docs
- Request review

### Step 6: Address Feedback & Merge
- Make requested changes on the same branch
- Rebase again before pushing fixes
- Once approved, merge to main

---

## DO NOT

- ❌ Commit directly to main
- ❌ Add Co-Author lines to commits
- ❌ Push without rebasing
- ❌ Include commented-out code
- ❌ Add unnecessary comments or docstrings
- ❌ Mix multiple features in one PR
- ❌ Force push to main branch
- ❌ Merge without CI passing
- ❌ Leave dead code or TODOs without context
- ❌ Use vague commit messages

---

## DO

- ✅ Create feature branch per feature
- ✅ Rebase before every push
- ✅ Write clear, single-purpose commits
- ✅ Test locally before pushing
- ✅ Use meaningful variable/function names
- ✅ Keep commits atomic and reviewable
- ✅ Update docs when changing behavior
- ✅ Run full test suite before PR
- ✅ Squash fixup commits into logical units
- ✅ Provide detailed PR descriptions

---

## Questions?

Refer to this document first. When in doubt, err on the side of clean, simple code over complex features.
