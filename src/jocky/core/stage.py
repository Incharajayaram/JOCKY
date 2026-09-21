from abc import ABC, abstractmethod
from .context import BuildContext

class Stage(ABC):
    """
    The contract: preflight(), run(), postflight(). Every stage implements it.
    """
    @property
    @abstractmethod
    def name(self) -> str:
        """Name of the stage."""
        pass

    def preflight(self, ctx: BuildContext) -> None:
        """Validate inputs, toolchain availability, config sanity."""
        pass

    @abstractmethod
    def run(self, ctx: BuildContext) -> BuildContext:
        """Perform the transformation. Return updated context."""
        pass

    def postflight(self, ctx: BuildContext) -> None:
        """Verify outputs, clean temp files, log metrics."""
        pass
