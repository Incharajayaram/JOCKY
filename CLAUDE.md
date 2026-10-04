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

## Backend & Compilation Testing

### 1. Backend Testing Strategy
- **Use dev launch sh only** — No Docker for debugging backend services
  - Docker setup and debugging takes too long; dev mode is faster iteration
  - Test backend services via `./dev_launch.sh` or equivalent dev runner
  - Verify both Windows and Linux script compilation through main pipeline

### 2. Runtime API Deduplication Strategy
- **Central Reference:** See `docs/RUNTIME_API_STRUCTURE.md` for complete organization
- **Windows & Linux APIs:** Do NOT duplicate implementations across platforms
  - Shared APIs (crypto, io, network, etc.) live in `src/runtime/common/`
  - Platform-specific APIs (win_* in `windows/`, linux_* in `linux/`)
  - Search `src/runtime` thoroughly before implementing new APIs
- **Before adding ANY runtime API:** Check `src/runtime/RUNTIME_API.md` and `docs/RUNTIME_API_STRUCTURE.md`

### 3. Forensic Pipeline Integration
- **Status:** Kamimi's forensic pipeline integrated halfway; needs full integration
- **Scope:** Main pipeline → Backend → Both research chain files
- **See:** `docs/FORENSIC_INTEGRATION_STATUS.md` for detailed checklist
- **Action Items:**
  - Integrate forensic analysis APIs into main compilation pipeline
  - Add forensic APIs to `research_chain_windows_production.jky` and `research_chain_linux_production.jky`
  - Link forensic APIs into prelude, backend, and all compilation stages
  - Ensure compiled code with forensic APIs compiles without errors

### 4. Research Chain File Guidelines
- **Files affected:** 
  - `examples/research_chain_windows_production.jky`
  - `examples/research_chain_linux_production.jky`
  - `examples/authorized_research_full_chain.jky`
- **Before ANY edit:** Read the entire file first to verify what exists
  - Prevents duplicate API implementations and variable declarations
  - Check for existing forensic analysis APIs before adding new ones
- **Forensic Analysis APIs Missing:** Must add and test these (see forensic integration doc)

### 5. Compiler & Toolchain Issues
- **Issue Tracking:** See `docs/CODEGEN_COMPILER_ISSUES.md`
- **Scope:** Codegen errors, LLVM issues, MLIR problems, obfuscation pass failures
- **When Issues Found:** Document with:
  - Error message (full output)
  - Reproduction steps
  - Affected file(s) and line numbers
  - Temporary workaround if any
  - Priority level (critical/high/medium/low)

### 6. End-to-End Testing Requirements (MANDATORY BEFORE PUSH)
- **Golden Rule:** NO COMMITS TO MAIN WITHOUT FULL END-TO-END TESTING
- **Pipeline must compile BOTH:**
  - ✅ Windows production script (`examples/production_windows_complete.jky`)
  - ✅ Linux production script (`examples/production_linux_complete.jky`)
  - ✅ Research chains (`research_chain_windows_production.jky`, `research_chain_linux_production.jky`)
- **Testing procedure:**
  1. Run: `./dev_launch.sh` (dev launcher, NOT Docker)
  2. Attempt compilation of both Windows and Linux production scripts
  3. Verify no linker errors (undefined symbols)
  4. Verify no type checking errors
  5. Verify no codegen errors
  6. Verify compiled binaries are generated
  7. Document all test results before pushing

### 7. Code Quality Gates (Pre-Push Verification)
- ❌ **NO STUBS** — Only real implementations allowed
  - Verify all APIs are real implementations, not placeholders
  - NO stub files in repository (e.g., `*_stubs.c`, `*_placeholder.c`)
  - If stub files exist, they MUST be deleted before pushing
  - Any file containing only `return -1;` or `return 0;` without logic is a stub — DELETE IT
  - Check: `find src/runtime -name "*stub*.c" -o -name "*placeholder*.c"` (should be empty)
  - All function implementations must have real logic, not just error returns
- ❌ **NO PROGRESS FILES** — Only committed code allowed
  - No "work in progress" markers
  - No "TODO" tracking files
  - No "partial implementation" markers
  - Only complete, tested code
- ❌ **NO UNRELATED FILES**
  - No test output files
  - No build artifacts (CMakeFiles, Makefile, *.o, etc.)
  - No debug scripts or temporary files
  - Only source code and configuration
- ✅ **MEANINGFUL CHANGES ONLY**
  - Every file change must serve a purpose
  - No placeholder changes or refactoring without reason
  - Each commit must have clear, reviewable changes

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
- ❌ Commit Docker/infrastructure code without testing first
  - **Rule:** Test Docker images, build scripts, and CI/CD changes before committing
  - **Why:** Broken infrastructure affects all developers and wastes time on rebuilds
  - **How:** Build Docker image locally, test compilation pipeline, verify outputs work
- ❌ Add stub implementations without checking for real implementations first
  - **Rule:** Use real runtime API implementations before writing any stub
  - **If real API doesn't exist:** Ask user if this particular API should be implemented from scratch
  - **Why:** Duplicates waste code, wrong signatures cause linker errors, stubs hide real bugs
  - **How:** `grep -r "function_name" src/runtime --include="*.c" --include="*.h"` to verify if real impl exists, check `src/runtime/RUNTIME_API.md`
- ❌ Duplicate runtime APIs between Windows and Linux platforms
  - **Rule:** Check `docs/RUNTIME_API_STRUCTURE.md` for existing implementations
  - **Why:** Duplicates cause linker conflicts, maintenance burden, and inconsistent behavior
  - **How:** Place shared APIs in `src/runtime/common/`, platform-specific in `windows/` or `linux/`
- ❌ Add forensic analysis APIs without full pipeline integration
  - **Rule:** Forensic APIs must be integrated into main pipeline → backend → research chains
  - **Why:** Incomplete integration causes compilation errors and unused code
  - **How:** Follow checklist in `docs/FORENSIC_INTEGRATION_STATUS.md`
- ❌ Edit research chain files without reading them fully first
  - **Rule:** Read entire file before making changes to avoid duplicates
  - **Why:** Prevents duplicate API declarations and variable redefinitions
  - **Files:** `research_chain_windows_production.jky`, `research_chain_linux_production.jky`, `authorized_research_full_chain.jky`
- ❌ Use Docker for backend debugging
  - **Rule:** Use dev launch script only for backend testing
  - **Why:** Docker setup and debugging takes significantly longer
  - **How:** `./dev_launch.sh` or equivalent dev runner
- ❌ Delete code that has missing dependencies
  - **Rule:** Implement the missing dependencies instead of removing code
  - **Why:** Removing code loses functionality; implementing headers/stubs preserves it
  - **How:** Create missing header files, add forward declarations, implement missing functions
  - **Example:** If file X needs `helper.h`, create `helper.h` with proper definitions, don't delete file X
  - Every implementation file must compile — if it depends on something missing, provide it

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
- ✅ Test infrastructure changes before committing
  - **Docker/Build scripts:** Build image and test compilation pipeline
  - **CMakeLists.txt:** Verify build succeeds with all source files
  - **Runtime APIs:** Compile test suite and verify all tests pass
  - **CLI changes:** Test `jocky build` with sample JOCKY scripts

---

## Questions?

Refer to this document first. When in doubt, err on the side of clean, simple code over complex features.
