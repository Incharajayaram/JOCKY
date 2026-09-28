# JOCKY Tools Guide

## Interactive REPL

### Overview

The JOCKY REPL is an interactive command-line interface for experimenting with the JOCKY language and compiler.

### Usage

Start the REPL:
```bash
jocky repl
```

Load a file at startup:
```bash
jocky repl -f my_program.jky
```

### Commands

#### Code Evaluation

Simply type JOCKY code to evaluate it:

```jocky
jocky> let x = 5;
jocky> let y = x * 2;
jocky> printf("%d\n", y);
```

Multiline code (ends with `\`):

```jocky
jocky> let add = lambda(a, b) \
...> { a + b };
jocky> add(3, 4)
```

#### Built-in Commands

**`vars`** - Show all variables:
```
jocky> vars
┌──────────────────┐
│   Variables      │
├──────────────────┤
│ x     i32    5   │
│ y     i32    10  │
└──────────────────┘
```

**`funcs`** - Show all functions:
```
jocky> funcs
┌──────────────────┐
│   Functions      │
├──────────────────┤
│ add    fn(i32,i32)->i32 │
└──────────────────┘
```

**`history`** - Show command history:
```
jocky> history
1 let x = 5;
2 let y = x * 2;
3 printf("%d\n", y);
```

**`load <file>`** - Load JOCKY file:
```
jocky> load my_code.jky
✓ Loaded my_code.jky
```

**`compile <output>`** - Compile to executable:
```
jocky> compile output/app
✓ Compiled to output/app
```

**`clear`** - Clear all state:
```
jocky> clear
State cleared
```

**`help`** - Show help:
```
jocky> help
JOCKY REPL Commands:
...
```

**`exit`** - Exit REPL (Ctrl-D also works):
```
jocky> exit
Goodbye!
```

### Examples

#### Experiment with Functions

```jocky
jocky> fn factorial(n: i32) -> i32 {
...>   if n <= 1 { 1 } else { n * factorial(n - 1) }
...> }
jocky> factorial(5)
AST: FunctionDef(...)
LLVM IR: ...
```

#### Test Pattern Matching

```jocky
jocky> enum Result { Ok(value: i32), Err }
jocky> match Result::Ok(42) {
...>   Ok(v) => printf("Success: %d\n", v),
...>   Err => printf("Failed\n")
...> }
```

---

## Performance Profiler

### Overview

The profiler measures JOCKY compiler performance across different obfuscation profiles.

### Usage

Profile all profiles:
```bash
python tools/profiler.py profile examples/hello-world/main.jky
```

Profile specific profile:
```bash
python tools/profiler.py profile examples/hello-world/main.jky -p aggressive -i 5
```

Save results:
```bash
python tools/profiler.py profile examples/hello-world/main.jky -o results.json
```

Compare results:
```bash
python tools/profiler.py compare before.json after.json
```

### Output

```
╭─────────────────────────────────────╮
│  JOCKY Compiler Performance Report  │
├─────────────────────────────────────┤
│ Profile    │ Avg (s) │ Min  │ Max │ Overhead │
├────────────┼─────────┼──────┼─────┼──────────┤
│ none       │ 0.234   │0.220 │0.250│ —        │
│ light      │ 0.312   │0.305 │0.320│ +33.3%   │
│ standard   │ 0.876   │0.850 │0.900│ +274%    │
│ aggressive │ 2.134   │2.100 │2.200│ +812%    │
│ paranoid   │ 3.456   │3.400 │3.520│ +1376%   │
╰─────────────────────────────────────╯
```

### Interpretation

- **Overhead** shows the slowdown vs. `none` profile
- **Min/Max** show performance variance (consistency indicator)
- Use for optimization decisions and benchmarking

---

## Docker Setup

### Build Image

```bash
docker build -t jocky:latest .
```

### Run Container

Interactive shell:
```bash
docker run -it jocky:latest bash
```

Compile file:
```bash
docker run -v $(pwd):/workspace jocky:latest build /workspace/main.jky -o /workspace/output
```

### Dockerfile Stages

1. **Builder stage**: Compiles C++ components
2. **Runtime stage**: Minimal image with dependencies

---

## CI/CD Pipeline (GitHub Actions)

### Overview

Automated testing and building on every push and PR.

### Workflows

Located in `.github/workflows/ci.yml`:

- **Lint** - Code quality checks (flake8, black, mypy)
- **Test** - Python unit tests (Python 3.9, 3.10, 3.11)
- **Build** - C++ compiler compilation
- **Integration** - End-to-end tests
- **Security** - Security scanning with bandit
- **Release** - Create releases from git tags

### Running Locally

Install dependencies:
```bash
pip install -r requirements.txt
pip install pytest pytest-cov flake8 black mypy bandit
```

Run tests:
```bash
pytest tests/ -v --cov=src/jocky
```

Run linter:
```bash
flake8 src/jocky
black --check src/jocky
mypy src/jocky --ignore-missing-imports
```

### Artifacts

CI/CD produces:
- `jockyc-linux` - Compiled C++ compiler
- `security-report` - Bandit security scan results
- Code coverage reports → codecov.io

---

## Coverage Reports

### Setup

Configuration in `.coveragerc`:
- Source tracking: `src/jocky`
- Branch coverage enabled
- Excludes test files and abstract methods

### Generate Report

Command line:
```bash
pytest tests/ --cov=src/jocky --cov-report=html --cov-report=xml
```

View HTML report:
```bash
open htmlcov/index.html
```

CI/CD uploads to codecov.io automatically.

### Target Coverage

- **Minimum**: 80% line coverage
- **Target**: 90% line coverage
- **Stretch**: 85% branch coverage

---

## Best Practices

### REPL
- Use for quick experiments and testing
- Load full files for integration testing
- Use `compile` to verify code works end-to-end

### Profiler
- Profile before optimization claims
- Run multiple iterations for stability
- Compare before/after to measure impact
- Track trends over time in CI/CD

### Docker
- Use for consistent development environment
- Build images in CI/CD
- Mount volumes for local file access
- Use `.dockerignore` to reduce image size

### CI/CD
- Keep test suite fast (<5 min)
- Run linter/type check first (catches issues early)
- Cache LLVM to speed up builds
- Archive security reports for review

---

See also: [README](../README.md), [Build System](./pipeline.md)
