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
    def __init__(self, file_lines: Optional[List[str]] = None):
        self.file_lines = file_lines or []

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

            for line_num in range(range_.start_line, range_.end_line + 1):
                if 1 <= line_num <= len(self.file_lines):
                    source_line = self.file_lines[line_num - 1]
                    lines.append(f"  ├─ {source_line}")

                    if line_num == range_.start_line:
                        indicator = " " * (range_.start_col - 1) + "^" * max(1, range_.end_col - range_.start_col + 1)
                        lines.append(f"  │  {indicator}")

        return "\n".join(lines)

    def format_errors(self, errors: List[JockyError], color: bool = False) -> str:
        return "\n\n".join(self.format_error(e, color) for e in errors)
