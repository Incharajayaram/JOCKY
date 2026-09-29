# JOCKY VS Code Extension - Installation Guide

## Quick Start (End Users)

### Option 1: VS Code Marketplace
1. Open VS Code
2. Go to Extensions (Ctrl+Shift+X)
3. Search for "JOCKY Language Support"
4. Click "Install"
5. Reload VS Code when prompted

### Option 2: Manual Installation from Source
1. Clone the JOCKY repository
2. Navigate to `vscode-extension/` directory
3. Run: `npm install`
4. Run: `npm run compile`
5. Package the extension: `npx vsce package`
6. In VS Code, go to Extensions
7. Click "..." menu → "Install from VSIX..."
8. Select the generated `.vsix` file

### Option 3: Development Mode
1. Open the extension folder in VS Code
2. Run `npm install` in the terminal
3. Press `F5` to start debugging
4. A new VS Code window opens with the extension loaded
5. Create a test file with `.jky` extension to test

## Prerequisites

- Node.js 14+
- npm or yarn
- VS Code 1.60.0+

## Build Instructions

### Setup
```bash
cd vscode-extension
npm install
```

### Compile TypeScript
```bash
npm run compile
```

### Watch Mode (Auto-compile)
```bash
npm run watch
```

### Package Extension
```bash
npm install -g vsce  # One-time installation
vsce package
```

This creates a `.vsix` file you can distribute or install locally.

## Verification

### Test Syntax Highlighting
1. Create a new file: `test.jky`
2. Type the following:
```jocky
fn main() -> void {
  let x: i32 = 42;
  printf("Hello, World!\n");
}
```
3. Verify keywords are highlighted in purple, types in blue, strings in orange

### Test Completion
1. Open a `.jky` file
2. Press `Ctrl+Space` after typing `f`
3. Verify `fn` appears in completion list

### Test Hover
1. Move mouse over `fn` keyword
2. Verify hover box shows function definition syntax

## Troubleshooting

### Extension doesn't appear in Extensions list
- Ensure `.vsix` file was created successfully
- Try reinstalling: `vsce package` and install again
- Check Node.js version: `node --version` (should be 14+)

### Syntax highlighting not working
- Ensure file has `.jky` extension
- File must be associated with language: `jocky`
- Try reloading VS Code: `Ctrl+R`

### Completion not triggering
- Ensure you're typing in a `.jky` file
- Try pressing `Ctrl+Space` explicitly
- Check that IntelliSense is enabled in settings

### TypeScript compilation errors
- Delete `node_modules/` and `out/` folders
- Run `npm install` again
- Run `npm run compile`

## Configuration

### Adjust Editor Settings for JOCKY
Add to your VS Code `settings.json`:
```json
{
  "[jocky]": {
    "editor.tabSize": 2,
    "editor.insertSpaces": true,
    "editor.formatOnSave": false,
    "editor.wordBasedSuggestions": false,
    "editor.quickSuggestions": {
      "other": true,
      "comments": false,
      "strings": false
    }
  }
}
```

### Disable for Specific Workspaces
Create `.vscode/settings.json` in project root:
```json
{
  "extensions.ignoreRecommendations": ["jocky-lang.vscode-jocky"]
}
```

## Development Setup

### Contributing to the Extension

1. Clone the repository
2. Install dependencies: `npm install`
3. Make changes to TypeScript files in `src/`
4. Run `npm run compile` or use watch mode
5. Press `F5` to test in debug mode
6. Check your changes work before committing

### Adding Features

- **New completion items**: Edit `src/providers/completionProvider.ts`
- **New hover documentation**: Edit `src/providers/hoverProvider.ts`
- **Syntax rules**: Edit `syntaxes/jocky.tmLanguage.json`
- **Code snippets**: Edit `snippets/jocky.json`
- **Diagnostics**: Edit `src/providers/diagnosticsProvider.ts`

### Testing Locally

Debug mode automatically reloads changes:
1. Make code changes
2. Run `npm run compile` (or it's in watch mode)
3. Switch to debug window
4. Press `Ctrl+Shift+P` and run "Developer: Reload Window"

## Publishing

### For Maintainers

To publish a new version:

1. Update version in `package.json`
2. Update `CHANGELOG.md` with changes
3. Commit: `git commit -am "vscode: Release v0.1.0"`
4. Create tag: `git tag vscode/0.1.0`
5. Build: `npm run compile`
6. Package: `vsce package`
7. Publish: `vsce publish`

## Support

For issues or questions:
- GitHub Issues: Report bugs and feature requests
- GitHub Discussions: Ask questions and discuss
- Documentation: See README.md for feature overview

## Next Steps

After installation:
1. Open a `.jky` file in VS Code
2. Use snippets for common patterns (type `fn` + Tab)
3. Hover over keywords to learn syntax
4. Use Ctrl+Space for code completion
5. Check the Problems panel for diagnostics

---

Enjoy coding in JOCKY!
