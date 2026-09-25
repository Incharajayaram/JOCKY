import pytest
from jocky.language.errors import (
    ParseError, TypeError, SourceRange, SourceLocation,
    ErrorFormatter, JockyError
)


class TestSourceLocation:
    def test_source_location_str(self):
        loc = SourceLocation("test.jky", 5, 10)
        assert str(loc) == "test.jky:5:10"


class TestSourceRange:
    def test_source_range_at(self):
        rng = SourceRange.at(5, 10)
        assert rng.start_line == 5
        assert rng.start_col == 10
        assert rng.end_line == 5
        assert rng.end_col == 10

    def test_source_range_str_single_line(self):
        rng = SourceRange(5, 10, 5, 15)
        assert "line 5" in str(rng)
        assert "10-15" in str(rng)

    def test_source_range_str_multiple_lines(self):
        rng = SourceRange(5, 10, 8, 15)
        assert "5-8" in str(rng)


class TestParseError:
    def test_parse_error_without_range(self):
        err = ParseError("Unexpected token")
        assert "Unexpected token" in str(err)

    def test_parse_error_with_range(self):
        rng = SourceRange.at(3, 5)
        err = ParseError("Expected semicolon", rng, "test.jky")
        assert "test.jky:3:5" in str(err)
        assert "Expected semicolon" in str(err)

    def test_parse_error_code(self):
        err = ParseError("Unexpected token")
        assert err.code == "E0004"


class TestTypeError:
    def test_type_error_with_code(self):
        err = TypeError("Type mismatch", error_code="E0002")
        assert err.code == "E0002"
        assert "Type mismatch" in str(err)


class TestErrorFormatter:
    def test_format_error_without_range(self):
        err = ParseError("Simple error")
        formatter = ErrorFormatter()
        result = formatter.format_error(err)
        assert "error" in result
        assert "Simple error" in result

    def test_format_error_with_range_and_context(self):
        file_lines = [
            "let x: i32 = true;",
            "let y = x;",
            "return y;"
        ]
        rng = SourceRange(1, 14, 1, 18)
        err = TypeError("Type mismatch: expected i32, got bool", rng, "test.jky", "E0002")
        formatter = ErrorFormatter(file_lines)
        result = formatter.format_error(err)

        assert "E0002" in result
        assert "test.jky:1:14" in result
        assert "let x: i32 = true;" in result
        assert "^" in result or "^" in result

    def test_format_error_with_multiline_range(self):
        file_lines = [
            "fn test() {",
            "  let x: i32 = 1;",
            "  let y: i32 = \"string\";",
            "}"
        ]
        rng = SourceRange(3, 18, 3, 26)
        err = TypeError("Type mismatch", rng, "test.jky", "E0002")
        formatter = ErrorFormatter(file_lines)
        result = formatter.format_error(err)

        assert "test.jky:3:18" in result
        assert "string" in result

    def test_format_multiple_errors(self):
        file_lines = ["let x = y;"]
        err1 = ParseError("Undefined variable", SourceRange.at(1, 9), "test.jky")
        err2 = ParseError("Missing semicolon", SourceRange.at(1, 9), "test.jky")

        formatter = ErrorFormatter(file_lines)
        result = formatter.format_errors([err1, err2])

        assert "Undefined variable" in result
        assert "Missing semicolon" in result


class TestJockyError:
    def test_jocky_error_inheritance(self):
        rng = SourceRange.at(1, 1)
        err = JockyError("Test error", rng, "test.jky", "E0000")
        assert err.message == "Test error"
        assert err.source_range == rng
        assert err.file == "test.jky"
        assert err.code == "E0000"

    def test_error_message_formatting(self):
        rng = SourceRange.at(5, 10)
        err = JockyError("Error message", rng, "file.jky", "E0001")
        formatted = str(err)
        assert "file.jky:5:10" in formatted
        assert "Error message" in formatted


class TestErrorCodes:
    def test_parse_error_code(self):
        assert ParseError.ERROR_CODE == "E0004"

    def test_codegen_error_code(self):
        from jocky.language.errors import CodeGenError
        assert CodeGenError.ERROR_CODE == "E0010"


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
