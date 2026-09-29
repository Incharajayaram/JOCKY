# JOCKY Web Platform

A comprehensive web-based interface for compiling JOCKY source code into obfuscated binaries for Windows and Linux.

## Architecture Overview

### Backend (FastAPI)
- **Language:** Python 3.11+
- **Framework:** FastAPI with WebSocket support
- **Features:**
  - RESTful API endpoints for compilation management
  - Real-time WebSocket streaming for compilation logs
  - Job persistence with SQLAlchemy (SQLite/PostgreSQL)
  - Metrics tracking and analytics
  - Pydantic validation for all requests
  - Health checks and error reporting

### Frontend (React)
- **Language:** TypeScript
- **Framework:** React 19 + Vite
- **Editor:** Monaco Editor with JOCKY language support
- **Features:**
  - Real-time code editing with syntax highlighting
  - Live compilation progress tracking
  - Job history and artifact management
  - Obfuscation configuration UI
  - Example projects and templates
  - Help and settings panels
  - Log filtering and export

### Compiler Service (Docker)
- **Language:** C++/LLVM
- **Architecture:** JOCKY compilation pipeline
- **Targets:** Windows (.exe), Linux (ELF)

## Quick Start

### Development Setup

```bash
# Backend
cd web/backend
pip install -r requirements.txt
python -m uvicorn app:app --reload

# Frontend (in another terminal)
cd web/frontend
npm install
npm run dev
```

Then open `http://localhost:5173` in your browser.

### Docker Compose

```bash
# Development
docker-compose up

# Production
docker-compose --profile production up
```

The frontend will be available at `http://localhost:3000` (production) or `http://localhost:5173` (development).

## API Endpoints

### Compilation
- `POST /api/compile` - Start compilation job
- `GET /api/status/{job_id}` - Get job status
- `GET /api/download/{job_id}` - Download compiled binary
- `WS /ws/logs/{job_id}` - WebSocket stream of compilation logs

### Configuration
- `GET /api/config` - Get backend configuration
- `POST /api/validate-source` - Validate source code

### Runtime APIs
- `GET /api/runtime-apis` - List available runtime APIs
- `GET /api/obfuscation-passes` - List obfuscation passes

### Job Management
- `GET /api/jobs/history` - Compilation history
- `GET /api/jobs/{job_id}/metrics` - Compilation metrics
- `GET /api/jobs/{job_id}/artifact` - Artifact information

### Examples
- `GET /api/examples` - List example projects
- `GET /api/examples/{example_id}` - Get specific example

## Configuration

### Environment Variables

**Backend**
- `ENVIRONMENT` - `development` or `production` (default: development)
- `BACKEND_HOST` - Server host (default: 0.0.0.0)
- `BACKEND_PORT` - Server port (default: 8000)
- `CORS_ORIGINS` - Comma-separated CORS origins
- `DATABASE_URL` - Database connection string (default: sqlite:///./jocky_jobs.db)
- `MAX_COMPILATION_TIME` - Max seconds for compilation (default: 600)

**Frontend**
- `VITE_API_BASE` - Backend API URL (default: http://localhost:8000)
- `NODE_ENV` - `development` or `production`

## Features by Phase

### Phase 1: Foundation ✅
- Environment configuration loading
- Pydantic validation models
- JOCKY language syntax highlighting
- Error validation API with structured messages
- ConfigResponse with backend info

### Phase 2: Job Management ✅
- Database persistence (SQLAlchemy)
- Job history tracking
- Compilation metrics collection
- Artifact information API
- Example projects management

### Phase 3: Containerization ✅
- Enhanced Docker setup with health checks
- Multi-stage builds
- Environment variable support
- docker-compose orchestration
- Production-ready configuration

### Phase 4: Advanced UI ✅
- Job history component
- Examples panel with platform filtering
- Log filtering by level
- Copy logs to clipboard
- Help panel with documentation
- Settings panel with backend info

### Phase 5: Testing & Documentation ✅
- Pytest-based API tests
- Pydantic validation tests
- Comprehensive help documentation
- Settings and configuration UI
- Security considerations

## Testing

```bash
# Backend tests
cd web/backend
pytest test_api.py -v

# Frontend build
cd web/frontend
npm run build
npm run lint
```

## Security Considerations

1. **Input Validation:** All requests validated with Pydantic
2. **CORS:** Configured for development and production origins
3. **File Size Limits:** Source code limited to 1MB
4. **Timeout Protection:** Compilation timeout configurable
5. **Error Handling:** Structured error responses without sensitive info
6. **WebSocket Security:** Job ID verification on connection

## Performance Optimization

- Frontend: Vite build with code splitting
- Backend: Async compilation with progress streaming
- Docker: Multi-stage builds for minimal images
- Caching: Monaco editor language definitions cached
- Database: SQLite for dev, PostgreSQL for production

## Production Deployment

1. Build Docker images:
   ```bash
   docker build -f Dockerfile.frontend -t jocky-frontend .
   docker build -f Dockerfile.backend -t jocky-backend .
   docker build -f Dockerfile -t jocky-compiler .
   ```

2. Use docker-compose with production profile:
   ```bash
   docker-compose --profile production up -d
   ```

3. Configure reverse proxy (Caddy) with SSL

## Troubleshooting

### Backend Won't Start
- Check port 8000 is available
- Verify Python dependencies: `pip install -r requirements.txt`
- Check database connectivity if using PostgreSQL

### Frontend Build Fails
- Clear node_modules: `rm -rf node_modules && npm install`
- Check Node version: `node -v` (requires v18+)

### Compilation Fails
- Ensure compiler Docker image is built
- Check Docker daemon is running
- Verify source code is valid JOCKY syntax

## Development Workflow

1. **Backend Changes:** Modify Python files in `web/backend/`, restart uvicorn
2. **Frontend Changes:** Modify TypeScript/React files in `web/frontend/`, Vite auto-reloads
3. **New API Endpoints:** Add route to `web/backend/app.py`, update frontend hook
4. **New UI Components:** Create component in `web/frontend/src/components/`, use in App.tsx

## Contributing

When adding features:
1. Follow existing code style and patterns
2. Add Pydantic validation for API inputs
3. Include TypeScript types for frontend
4. Test both backend API and frontend integration
5. Update documentation

## References

- [FastAPI Documentation](https://fastapi.tiangolo.com/)
- [React 19 Documentation](https://react.dev/)
- [Monaco Editor Guide](https://microsoft.github.io/monaco-editor/)
- [JOCKY Language Reference](../../README.md)
