# Changelog

All notable changes to the JOCKY VS Code Extension are documented in this file.

## [0.1.0] - 2026-09-29

### Added
- Initial release of JOCKY Language Support extension
- Syntax highlighting with comprehensive TextMate grammar
- IntelliSense with keyword, type, and built-in function completions
- Hover provider showing type documentation and syntax examples
- Code snippets for common patterns (functions, structs, enums, control flow)
- Diagnostics provider with bracket matching and type annotation checking
- Language configuration for auto-closing pairs and indentation rules
- Professional documentation (README.md, INSTALL.md)
- Support for .jky file extension
- Dark and light theme support
- Block and line comment support
- Symbol extraction and completion from open files

### Features
- **Completion Provider**
  - Keywords: fn, struct, enum, let, if, else, while, for, return, match, etc.
  - Built-in types: i8, i32, i64, bool, void, string
  - Built-in functions: printf, strlen, sizeof, nameof, jocky_* functions
  - Symbol completion from document

- **Hover Provider**
  - Keyword documentation with syntax examples
  - Type definitions and descriptions
  - Built-in function signatures

- **Snippets** (13 templates)
  - Function and main definitions
  - Struct and enum definitions
  - Control flow (if, while, for, match)
  - Variable bindings and returns
  - FFI declarations and lambdas

- **Diagnostics**
  - Bracket matching validation
  - Unclosed brace detection
  - Basic type annotation checking

- **Language Features**
  - Smart indentation
  - Auto-closing brackets and quotes
  - Bracket pair matching
  - Code folding for comments

### Technical
- TypeScript implementation for maintainability
- Full type safety with strict tsconfig
- Modular provider architecture
- Comprehensive TextMate grammar with all JOCKY tokens
- Proper scope classification for syntax highlighting

## Future Releases

### Planned for [0.2.0]
- Language Server Protocol (LSP) integration for advanced features
- Go to definition and find references
- Rename refactoring
- Symbol outline/document symbols
- Integration with JOCKY compiler for real-time error reporting

### Planned for [0.3.0]
- Debug adapter protocol (DAP) support
- Breakpoint management
- Stack trace inspection
- Variable inspection

### Planned for [0.4.0]
- Test runner integration
- Test code lens
- Coverage reporting

---

Contributions and feedback welcome!
