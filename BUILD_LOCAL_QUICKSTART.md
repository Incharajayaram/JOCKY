# JOCKY Local Build - Quick Start

## One Command to Build Everything

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform linux
```

That's it! Binary is ready in `build/local/research_chain_linux_production`.

## What Happens Automatically

✅ Docker services start (C2, Model Repo, CDN)
✅ Environment variables set to localhost URLs
✅ Binary compiled with embedded configuration
✅ Ready to run immediately

## Try Running Your Binary

```bash
# Execute
./build/local/research_chain_linux_production

# View logs
docker-compose -f docker-compose.dev.yml logs c2-server

# Check exfiltrated data
ls -lah build/uploads/
```

## Build for Windows

```bash
./scripts/build_local.sh examples/research_chain_linux_production.jky build/local --platform windows
```

## Stop Services

```bash
docker-compose -f docker-compose.dev.yml down
```

## For Details

See [`LOCAL_DEVELOPMENT.md`](LOCAL_DEVELOPMENT.md)
