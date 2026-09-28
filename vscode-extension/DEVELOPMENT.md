# Extension Development Guide

This guide covers how to develop and extend the JOCKY VS Code extension.

## Architecture

The extension follows a modular provider-based architecture:

```
extension.ts (Main entry point)
├── completionProvider.ts (IntelliSense)
├── hoverProvider.ts (Hover documentation)
├── diagnosticsProvider.ts (Error reporting)
└── jocky.tmLanguage.json (Syntax highlighting)
```

### Extension Lifecycle

1. **Activation** - Extension activates when a `.jky` file is opened
2. **Providers Register** - Completion, hover, and diagnostic providers register
3. **Event Listeners** - Document change/open events trigger diagnostics
4. **Deactivation** - Cleanup on VS Code shutdown

## Extending the Extension

### Adding New Completions

Edit `src/providers/completionProvider.ts`:

1. Add to `keywords`, `types`, or `builtins` object
2. Provide display text and documentation
3. Completions filter by word prefix automatically

Example:
```typescript
private builtins: Record<string, [string, string]> = {
  'my_function': ['fn my_function() -> i32', 'Does something cool'],
};
```

### Adding Hover Documentation

Edit `src/providers/hoverProvider.ts`:

1. Add entry to `documentation` object
2. Use Markdown for formatting
3. Include code examples in triple backticks

Example:
```typescript
private documentation: Record<string, string> = {
  'my_keyword': '**my_keyword** - Does X\n\n```jocky\n...\n```\n\nExplanation.',
};
```

### Adding Code Snippets

Edit `snippets/jocky.json`:

1. Add JSON entry with prefix, body, and description
2. Use `${1:name}` for tab stops
3. Use `${0}` for final cursor position

Example:
```json
{
  "My Snippet": {
    "prefix": "mysnip",
    "body": "fn ${1:name}() -> void { ${0} }",
    "description": "Generate a function stub"
  }
}
```

### Updating Syntax Highlighting

Edit `syntaxes/jocky.tmLanguage.json`:

1. Add patterns to `repository` for new token types
2. Use regex patterns with capturing groups
3. Assign `name` for VS Code theme colors
4. Include patterns in main `patterns` array

TextMate syntax reference: https://macromates.com/manual/en/language_grammars

Example pattern:
```json
{
  "name": "keyword.custom.jocky",
  "match": "\\b(myKeyword|anotherOne)\\b"
}
```

### Adding Diagnostics

Edit `src/providers/diagnosticsProvider.ts`:

1. Implement check function in `provideDiagnostics()`
2. Create `vscode.Diagnostic` objects with range and message
3. Push to diagnostics array
4. Set diagnostics collection: `this.diagnosticCollection.set(uri, diagnostics)`

Example:
```typescript
private checkCustomRule(lines: string[], diagnostics: vscode.Diagnostic[]): void {
  lines.forEach((line, lineNum) => {
    if (myCondition(line)) {
      const range = new vscode.Range(lineNum, 0, lineNum, line.length);
      diagnostics.push(new vscode.Diagnostic(
        range,
        'Error message',
        vscode.DiagnosticSeverity.Error
      ));
    }
  });
}
```

### Adding Commands

Edit `src/extension.ts`, add to `registerCommands()`:

```typescript
context.subscriptions.push(
  vscode.commands.registerCommand('jocky.myCommand', async () => {
    // Command implementation
  })
);
```

Register in `package.json`:
```json
{
  "contributes": {
    "commands": [
      {
        "command": "jocky.myCommand",
        "title": "JOCKY: My Command"
      }
    ]
  }
}
```

## Debug Workflow

1. Make changes to TypeScript files
2. Run `npm run compile` (or watch mode: `npm run watch`)
3. Press `F5` to start debug session
4. New VS Code window opens with extension loaded
5. Test changes in debug window
6. Press `Ctrl+Shift+P` → "Developer: Reload Window" to reload
7. Check debug console for logs

## Testing

### Manual Testing Checklist

- [ ] Open `.jky` file - syntax highlighting appears correct
- [ ] Type `fn` and press `Ctrl+Space` - completion shows
- [ ] Hover over keyword - hover documentation appears
- [ ] Create syntax error - diagnostic appears in Problems panel
- [ ] Try each snippet - inserts correct code
- [ ] Test bracket matching - colored brackets pair correctly
- [ ] Test auto-closing - quotes/brackets close automatically

### Test File

Use `examples/demo.jky` as test file. It covers:
- All keyword types
- Function definitions
- Struct and enum definitions
- Control flow structures
- Built-in function calls
- Comments (line and block)
- Strings with escape sequences
- Operations (arithmetic, logical, bitwise)

## Performance Considerations

### Completion Provider
- Word extraction runs on every keystroke
- Symbol extraction happens once per keystroke
- Limit regex complexity for large files
- Cache compiled regexes if needed

### Hover Provider
- Hover only triggers on mouse movement
- Documentation lookup is O(1) dictionary access
- No performance concerns for current implementation

### Diagnostics Provider
- Runs on every document change
- Limit rule complexity for files with 1000+ lines
- Consider debouncing if needed
- Current rules are O(n) where n = lines in file

## Common Patterns

### Extracting Text at Position
```typescript
const wordRange = document.getWordRangeAtPosition(position, /[a-zA-Z_][a-zA-Z0-9_]*/);
const word = document.getText(wordRange);
```

### Creating a Completion Item
```typescript
const item = new vscode.CompletionItem(label, kind);
item.detail = 'Additional info';
item.documentation = new vscode.MarkdownString('**Bold** text with `code`');
item.sortText = '1';  // Sort order
return item;
```

### Iterating Document Text
```typescript
const lines = document.getText().split('\n');
lines.forEach((line, lineNum) => {
  // Process line
  line.forEach((char, colNum) => {
    // Process character at (lineNum, colNum)
  });
});
```

### Working with Ranges
```typescript
// Create range for entire line
new vscode.Range(lineNum, 0, lineNum, line.length)

// Create range for word
new vscode.Range(position.translate(0, -word.length), position)
```

## Type Safety

The extension uses strict TypeScript:
```json
{
  "compilerOptions": {
    "strict": true,
    "strictNullChecks": true,
    "noImplicitAny": true,
    "noImplicitThis": true
  }
}
```

All functions should have explicit return types and parameter types.

## Dependencies

### Current
- `vscode` - VS Code API (types)
- `@types/vscode` - Type definitions
- `typescript` - Language and compiler

### Future (Optional)
- `vscode-languageclient` - For Language Server Protocol
- `vscode-test` - For integration testing
- `@vscode/vsce` - For packaging

## Versioning

Follow semantic versioning:
- **MAJOR**: Breaking changes to extension behavior
- **MINOR**: New features (backward compatible)
- **PATCH**: Bug fixes

Update `package.json` version before publishing.

## Publishing

### Local Testing
```bash
npm run compile
npx vsce package
# Install .vsix file in VS Code
```

### Publishing to Marketplace
```bash
npm run compile
vsce publish [major|minor|patch]
```

Requires VS Code Publisher token configured.

## Troubleshooting

### TypeScript errors
- Check `out/` folder exists and has compiled JS
- Run `npm run compile` explicitly
- Delete `out/` and rebuild if issues persist

### Extension not loading
- Check browser console (F12) for errors
- Verify `activationEvents` in package.json
- Ensure file has `.jky` extension
- Check `out/extension.js` exists

### Completion not working
- Verify trigger characters: `'.'` and `'_'`
- Check `registerCompletionItemProvider` called
- Confirm language selector is `jocky`

### Hover not working
- Verify `registerHoverProvider` called
- Check documentation dictionary has entry
- Ensure word regex matches correctly

## Resources

- [VS Code Extension API](https://code.visualstudio.com/api)
- [Language Grammars](https://macromates.com/manual/en/language_grammars)
- [TextMate Syntax](https://www.sublimetext.com/docs/syntax)
- [VS Code Snippets](https://code.visualstudio.com/docs/editor/userdefinedsnippets)

---

Happy developing!
