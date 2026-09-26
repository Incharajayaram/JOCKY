# JOCKY Git Branching Strategy

## Overview
Each feature gets its own branch to enable parallel development, independent testing, and clean pull requests.

## Branch Naming Convention
- `feature/<feature-name>` - New features
- `fix/<bug-name>` - Bug fixes
- `refactor/<area>` - Refactoring work
- `docs/<topic>` - Documentation only

## Current Feature Branches

### COMPLETED FEATURES (Ready for PR)
- `feature/module-system-basics` (17 commits)
  - ✅ Module system implementation
  - ✅ Variant construction
  - ✅ Error infrastructure
  - ✅ Build system performance
  - ✅ Manifest generation tool
  - ✅ Tier 1 Runtime APIs (File I/O, VirtualAlloc, Registry)
  - Status: Ready for PR → main

### IN PROGRESS FEATURES
- `feature/closures-lambdas` (TBD)
  - Anonymous functions with capture
  - Closure type inference
  - LLVM codegen for captures
  
- `feature/attributes-decorators` (TBD)
  - Function attributes (#[inline], #[no_mangle])
  - Type attributes (#[packed], #[repr(C)])
  - LLVM metadata emission

### PLANNED FEATURES (Next)
- `feature/thread-pool`
  - Worker thread management
  - Task queue implementation
  
- `feature/network-primitives`
  - TCP/UDP sockets
  - Windows + Linux support
  
- `feature/crypto-library`
  - AES, RSA, ECDH
  - Beyond RC4/XOR
  
- `feature/amsi-bypass`
  - Windows Defender evasion
  - AmsiScanBuffer hooking

## Workflow

### Starting a Feature
```bash
# 1. Ensure main is up to date
git checkout main
git pull origin main

# 2. Create feature branch
git checkout -b feature/your-feature

# 3. Make commits (one per logical change)
git add <files>
git commit -m "Brief description"

# 4. Push when ready
git push -u origin feature/your-feature
```

### Before Creating PR
```bash
# 1. Rebase on latest main
git fetch origin
git rebase origin/main

# 2. Run tests
pytest tests/

# 3. Force push (safe for feature branches)
git push -f origin feature/your-feature
```

### PR Process
1. Create PR on GitHub
2. Title: Match first commit message
3. Description: Summary of changes, testing notes
4. Request review
5. Address feedback in new commits
6. Once approved: Rebase, squash if needed, merge to main

## Branch Cleanup
After merging to main:
```bash
# Delete local branch
git branch -d feature/your-feature

# Delete remote branch
git push origin --delete feature/your-feature
```

## Status Dashboard

| Feature | Branch | Status | Commits | LOC |
|---------|--------|--------|---------|-----|
| Module System + Manifest | `feature/module-system-basics` | Ready for PR | 17 | 4,500+ |
| Closures/Lambdas | TBD | Ready to start | 0 | 0 |
| Attributes | TBD | Ready to start | 0 | 0 |
| Thread Pool | TBD | Backlog | 0 | 0 |
| Network API | TBD | Backlog | 0 | 0 |
| AMSI Bypass | TBD | Backlog | 0 | 0 |

## Parallel Development

Multiple features can be developed in parallel:
- Each developer works on their own feature branch
- No conflicts as long as different files/areas
- Easy to test individually
- Clean history in main branch

## CI/CD Integration

Each PR should:
- ✅ Pass all tests (pytest tests/)
- ✅ Have no compilation warnings
- ✅ Include documentation
- ✅ Follow code style guidelines
- ✅ Be rebased on latest main

