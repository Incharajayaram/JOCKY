import logging
import sys
from typing import Optional

_loggers: dict = {}

def get_logger(name: str = "jocky", level: Optional[int] = None) -> logging.Logger:
    """Return a named logger, creating it with a stderr handler on first call."""
    if name in _loggers:
        return _loggers[name]
    logger = logging.getLogger(name)
    if not logger.handlers:
        handler = logging.StreamHandler(sys.stderr)
        handler.setFormatter(logging.Formatter("[%(name)s] %(levelname)s: %(message)s"))
        logger.addHandler(handler)
    logger.setLevel(level or logging.WARNING)
    _loggers[name] = logger
    return logger
