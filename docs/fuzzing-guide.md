# Fuzzing Guide for JOCKY

## Overview

JOCKY uses fuzzing to ensure robustness of the lexer and parser. Two fuzzing strategies are employed:

1. **Fuzz Targets**: Direct fuzzer scripts that can be run with arbitrary input data
2. **Property-Based Tests**: Hypothesis-driven property testing with randomized inputs

## Running Fuzz Tests Locally

### Prerequisites

```bash
pip install hypothesis
```

### Property-Based Tests (Recommended for Local Development)

Run the property-based tests for the lexer and parser:

```bash
# Lexer property tests
pytest tests/unit/test_lexer_properties.py -v

# Parser property tests
pytest tests/unit/test_parser_properties.py -v

# All fuzz-related tests
pytest tests/unit/test_*_properties.py -v
```

### Fuzz Targets with Corpus

Test the fuzzer scripts against the corpus files:

```bash
# Fuzz lexer with all corpus files
for f in tests/fuzz/corpus/*.jky; do
  python tests/fuzz/fuzz_lexer.py < "$f"
done

# Fuzz parser with all corpus files
for f in tests/fuzz/corpus/*.jky; do
  python tests/fuzz/fuzz_parser.py < "$f"
done
```

### Manual Fuzzing

Pipe arbitrary input directly to the fuzz targets:

```bash
# Fuzz lexer with random data
echo "arbitrary random data @#$%" | python tests/fuzz/fuzz_lexer.py

# Fuzz parser with random data
echo "let let let" | python tests/fuzz/fuzz_parser.py

# Fuzz lexer with Unicode
echo "你好世界 мир 🎉" | python tests/fuzz/fuzz_lexer.py
```

## CI/CD Integration

Fuzzing runs automatically in the CI/CD pipeline:

- **Property tests** run on every push and PR
- **Corpus fuzzing** runs as part of the `fuzz` job
- Results are uploaded as artifacts for inspection

View fuzzing results in GitHub Actions under the "Fuzz" job.

## Corpus Files

The corpus directory contains seed inputs for fuzzing:

- `empty.jky` - Empty file (edge case)
- `comments.jky` - Comments only (whitespace handling)
- `valid_function.jky` - Simple valid function
- `valid_struct.jky` - Struct definition
- `valid_enum.jky` - Enum definition

### Adding New Corpus Files

To improve fuzzing coverage, add new corpus files to `tests/fuzz/corpus/`:

```bash
cat > tests/fuzz/corpus/my_feature.jky << 'EOF'
fn my_feature() -> i32 {
    return 0;
}
EOF
```

Good corpus files should:
- Cover different code paths in lexer/parser
- Exercise edge cases and boundaries
- Be minimal (simple, not complex)
- Represent valid or near-valid syntax

## Understanding Results

### Successful Fuzzing

Property-based tests pass when:
- No crashes occur on random input
- Lexer produces valid token lists
- Parser produces valid AST or rejects with ParseError
- All invariants hold

Example passing output:
```
test_lexer_never_crashes PASSED [100%]
test_tokenize_produces_valid_list PASSED [100%]
```

### Failures

If a fuzzing test fails:

1. Note the failing input (Hypothesis prints it)
2. Create a minimal reproduction file
3. Add to corpus for regression testing
4. Fix the underlying bug in lexer/parser
5. Re-run to confirm fix

Example failure:
```
Falsifying example: test_lexer_never_crashes(
    code='\x00\x01\x02\x03'
)
```

## What Gets Tested

### Lexer Fuzzing

- **Tokenization invariants**: All tokens have type, value, line, column
- **Position tracking**: Line and column numbers are valid and consistent
- **Error handling**: Lexer rejects invalid input gracefully (no crashes)
- **Edge cases**: Empty input, whitespace only, comments only
- **Unicode handling**: UTF-8 and invalid encodings

### Parser Fuzzing

- **AST invariants**: Parser returns Program or None, never invalid types
- **Error handling**: Parser rejects invalid syntax gracefully (no crashes)
- **Token sequences**: Handles all combinations of tokens from lexer
- **Nested structures**: Deep nesting of brackets, braces, parens
- **Edge cases**: Empty programs, incomplete declarations

## Performance Considerations

Property-based tests use settings to balance thoroughness and speed:

- **Health checks**: Disabled for slow operations
- **Deadlines**: Set to None to avoid timeouts
- **Iteration count**: Configurable via Hypothesis settings

To increase fuzzing intensity:

```bash
# Run with more examples (default is 100)
HYPOTHESIS_PROFILE=dev pytest tests/unit/test_lexer_properties.py -v
```

## Reproducing Failures

If a fuzzing test fails, Hypothesis generates a minimal input that triggers the bug.

To reproduce:

1. Copy the failing input from the test output
2. Create a test case:

```python
def test_regression_issue_123():
    code = '<failing input from hypothesis>'
    # Should not crash
    lexer = Lexer(code)
    tokens = lexer.tokenize()
```

3. Fix the bug
4. Verify test passes
5. Keep test in suite for regression prevention

## Integration with Development Workflow

1. Run property tests locally before committing:
   ```bash
   pytest tests/unit/test_*_properties.py -v
   ```

2. Add corpus files for new features:
   ```bash
   # New syntax feature gets a corpus file
   echo "fn new_syntax() { }" > tests/fuzz/corpus/new_feature.jky
   ```

3. Fuzzing runs automatically on CI
   - No action needed, results appear in GitHub Actions
   - Artifacts are available for inspection

## References

- [Hypothesis Documentation](https://hypothesis.readthedocs.io/)
- [libFuzzer](https://llvm.org/docs/LibFuzzer/)
- [Fuzzing Best Practices](https://github.com/google/fuzzing/tree/master/docs)
