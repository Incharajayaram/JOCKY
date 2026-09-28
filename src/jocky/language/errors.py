from dataclasses import dataclass
from typing import Optional, List
from enum import Enum


class ErrorLevel(Enum):
    ERROR = "error"
    WARNING = "warning"
    NOTE = "note"


@dataclass
class SourceLocation:
    file: str
    line: int
    column: int

    def __str__(self):
        return f"{self.file}:{self.line}:{self.column}"


@dataclass
class SourceRange:
    start_line: int
    start_col: int
    end_line: int
    end_col: int

    @staticmethod
    def at(line: int, col: int):
        return SourceRange(line, col, line, col)

    def __str__(self):
        if self.start_line == self.end_line:
            return f"line {self.start_line}, columns {self.start_col}-{self.end_col}"
        return f"lines {self.start_line}-{self.end_line}"


class JockyError(Exception):
    def __init__(self, message: str, source_range: Optional[SourceRange] = None, file: str = "<input>", code: str = ""):
        self.message = message
        self.source_range = source_range
        self.file = file
        self.code = code
        super().__init__(self.format_message())

    def format_message(self) -> str:
        if self.source_range:
            loc = f"{self.file}:{self.source_range.start_line}:{self.source_range.start_col}"
            return f"{loc}: {self.message}"
        return self.message


class ParseError(JockyError):
    ERROR_CODE = "E0004"

    def __init__(self, message: str, source_range: Optional[SourceRange] = None, file: str = "<input>"):
        super().__init__(message, source_range, file, self.ERROR_CODE)


class TypeError(JockyError):
    ERROR_CODE_BASE = "E000"

    def __init__(self, message: str, source_range: Optional[SourceRange] = None, file: str = "<input>", error_code: str = "E0002"):
        super().__init__(message, source_range, file, error_code)


class CodeGenError(JockyError):
    ERROR_CODE = "E0010"

    def __init__(self, message: str, source_range: Optional[SourceRange] = None, file: str = "<input>"):
        super().__init__(message, source_range, file, self.ERROR_CODE)


class ErrorFormatter:
    CONTEXT_LINES = 2

    def __init__(self, file_lines: Optional[List[str]] = None):
        self.file_lines = file_lines or []

    def _format_code_context(self, line_num: int, indicator_col: int = -1) -> List[str]:
        """Format code context with surrounding lines."""
        context = []
        start_line = max(1, line_num - self.CONTEXT_LINES)
        end_line = min(len(self.file_lines), line_num + self.CONTEXT_LINES)

        for i in range(start_line, end_line + 1):
            prefix = ">>>" if i == line_num else "   "
            line_content = self.file_lines[i - 1] if i <= len(self.file_lines) else ""
            context.append(f"  {prefix} {i:4d} | {line_content}")

            # Add indicator under the error location
            if i == line_num and indicator_col >= 0:
                col_indicator = " " * (indicator_col - 1) + "^"
                context.append(f"       |{col_indicator}")

        return context

    def format_error(self, error: JockyError, color: bool = False) -> str:
        lines = []

        code_prefix = f"[{error.code}]" if error.code else ""
        error_type = "error"
        header = f"{error_type}{code_prefix}: {error.message}"
        lines.append(header)

        if error.source_range and self.file_lines:
            range_ = error.source_range
            loc = f"  ├─ {error.file}:{range_.start_line}:{range_.start_col}"
            lines.append(loc)
            lines.append("  ├─")

            # Add code context for error
            for line_num in range(range_.start_line, range_.end_line + 1):
                if 1 <= line_num <= len(self.file_lines):
                    context = self._format_code_context(line_num, range_.start_col if line_num == range_.start_line else -1)
                    lines.extend(context)

        return "\n".join(lines)

    def format_errors(self, errors: List[JockyError], color: bool = False) -> str:
        return "\n\n".join(self.format_error(e, color) for e in errors)
