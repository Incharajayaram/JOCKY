# JOCKY Language Support for VS Code

Professional-grade VS Code extension for the JOCKY programming language with syntax highlighting, IntelliSense, and code snippets.

## Features

### Syntax Highlighting
- **Professional TextMate grammar** with comprehensive token classification
- Support for all JOCKY language constructs (functions, structs, enums, patterns)
- Proper handling of strings, comments, numbers, and operators
- Dark and light theme support

### IntelliSense (Code Completion)
- **Keyword completion** for control flow and declarations
- **Type completion** for built-in types (i8, i32, i64, bool, void, string)
- **Built-in function suggestions** with signatures and documentation
- **Symbol completion** from the current document
- Real-time completion as you type

### Hover Information
- **Type documentation** on hover
- **Function signatures** with parameter and return types
- **Keyword explanations** with syntax examples
- **Built-in function documentation**

### Code Snippets
- Function definitions (`fn`)
- Main entry point (`main`)
- Struct definitions (`struct`)
- Enum definitions (`enum`)
- Control flow (if, while, for, match)
- Variable bindings (`let`)
- FFI declarations (`ffi`)
- Type aliases (`type`)
- Lambda expressions (`lambda`)

### Diagnostics
- Bracket matching validation
- Struct/enum closing brace detection
- Type annotation checking
- Error and warning reporting

### Language Features
- Auto-closing brackets and quotes
- Smart indentation
- Code folding support
- Block and line comments

## Installation

### From Source
1. Clone or download this extension directory
2. Run `npm install` to install dependencies
3. Run `npm run compile` to compile TypeScript
4. In VS Code, press `Ctrl+Shift+D` and select "Run Extension"
5. Or package with `vsce package` and install the `.vsix` file

### From VS Code Marketplace
Search for "JOCKY Language Support" in the VS Code Extensions marketplace and click Install.

## Usage

### File Association
JOCKY files use the `.jky` extension and will automatically be recognized.

### Keyboard Shortcuts
- **Ctrl+Space** - Trigger completion
- **Ctrl+K Ctrl+I** - Show hover information
- **Ctrl+Shift+M** - Toggle problems panel

### Snippets
Type the snippet prefix and press Tab:
- `fn` → Function definition
- `main` → Main function
- `struct` → Struct definition
- `enum` → Enum definition
- `if` → If-else block
- `while` → While loop
- `for` → For loop
- `match` → Match expression
- `let` → Let binding
- `ret` → Return statement
- `lambda` → Lambda function

### Example Code
```jocky
// Basic JOCKY example
fn add(a: i32, b: i32) -> i32 {
  return a + b;
}

struct Point {
  x: i32;
  y: i32;
};

enum Status {
  OK = 0,
  ERROR = 1,
};

fn main() -> void {
  let result: i32 = add(10, 20);
  printf("Result: %d\n", result);
}
```

## Configuration

Add to your VS Code `settings.json` for JOCKY-specific configuration:

```json
{
  "[jocky]": {
    "editor.tabSize": 2,
    "editor.formatOnSave": true,
    "editor.detectIndentation": false
  }
}
```

## Development

### Build
```bash
npm install
npm run compile
```

### Watch Mode
```bash
npm run watch
```

### Testing
```bash
npm run test
```

## Project Structure

```
vscode-extension/
├── src/
│   ├── extension.ts              # Extension entry point
│   └── providers/
│       ├── completionProvider.ts # IntelliSense
│       ├── hoverProvider.ts      # Hover documentation
│       └── diagnosticsProvider.ts # Syntax checking
├── syntaxes/
│   └── jocky.tmLanguage.json    # TextMate grammar
├── snippets/
│   └── jocky.json               # Code snippets
├── language-configuration.json   # Language rules
├── package.json                  # Extension metadata
├── tsconfig.json                 # TypeScript config
└── README.md                     # This file
```

## Roadmap

- [ ] Language Server Protocol (LSP) integration
- [ ] Go to definition support
- [ ] Find all references
- [ ] Symbol outline
- [ ] Rename refactoring
- [ ] Debugging support
- [ ] Integrated compiler error reporting
- [ ] Test runner integration

## Contributing

Contributions are welcome! Please ensure code follows the project standards:
- No redundant code or unnecessary comments
- Clear, self-documenting naming
- One logical change per commit
- Tests for new features

## License

MIT License - See LICENSE file for details

## Support

For issues and feature requests, visit the GitHub repository.

---

**Built with ❤ for the JOCKY Language**
