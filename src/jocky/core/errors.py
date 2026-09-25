class JockyError(Exception):
    """Base class for all JOCKY compiler errors."""

class LexError(JockyError):
    def __init__(self, msg: str, line: int = 0, col: int = 0):
        super().__init__(f"{msg} at line {line}, col {col}" if line else msg)
        self.line = line
        self.col = col

class ParseError(JockyError):
    def __init__(self, msg: str, line: int = 0):
        super().__init__(f"{msg} at line {line}" if line else msg)
        self.line = line

class TypeError(JockyError):
    def __init__(self, msg: str):
        super().__init__(msg)

class CodeGenError(JockyError):
    def __init__(self, msg: str):
        super().__init__(msg)

class LinkError(JockyError):
    def __init__(self, msg: str):
        super().__init__(msg)

class ToolchainError(JockyError):
    def __init__(self, msg: str):
        super().__init__(msg)
