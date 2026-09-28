import * as vscode from 'vscode';

export class JockyDiagnosticsProvider {
  private diagnosticCollection: vscode.DiagnosticCollection;

  constructor(context: vscode.ExtensionContext) {
    this.diagnosticCollection = vscode.languages.createDiagnosticCollection('jocky');
    context.subscriptions.push(this.diagnosticCollection);
  }

  provideDiagnostics(document: vscode.TextDocument): void {
    if (document.languageId !== 'jocky') {
      return;
    }

    const diagnostics: vscode.Diagnostic[] = [];
    const text = document.getText();
    const lines = text.split('\n');

    this.checkBracketMatching(lines, diagnostics, document);
    this.checkUnmatchedStructs(lines, diagnostics, document);
    this.checkTypeAnnotations(lines, diagnostics, document);

    this.diagnosticCollection.set(document.uri, diagnostics);
  }

  private checkBracketMatching(
    lines: string[],
    diagnostics: vscode.Diagnostic[],
    document: vscode.TextDocument
  ): void {
    const stack: Array<{ char: string; line: number; column: number }> = [];
    const pairs: Record<string, string> = { '(': ')', '[': ']', '{': '}' };
    const openChars = Object.keys(pairs);
    const closeChars = Object.values(pairs);

    lines.forEach((line, lineNum) => {
      for (let i = 0; i < line.length; i++) {
        const char = line[i];

        if (this.isInString(line, i)) continue;
        if (this.isInComment(line, i)) continue;

        if (openChars.includes(char)) {
          stack.push({ char, line: lineNum, column: i });
        } else if (closeChars.includes(char)) {
          const last = stack.pop();
          if (!last || pairs[last.char] !== char) {
            const range = new vscode.Range(lineNum, i, lineNum, i + 1);
            diagnostics.push(new vscode.Diagnostic(
              range,
              `Mismatched bracket: expected ${last ? pairs[last.char] : 'opening bracket'}`,
              vscode.DiagnosticSeverity.Error
            ));
          }
        }
      }
    });

    stack.forEach(({ line, column }) => {
      const range = new vscode.Range(line, column, line, column + 1);
      diagnostics.push(new vscode.Diagnostic(
        range,
        'Unclosed bracket',
        vscode.DiagnosticSeverity.Error
      ));
    });
  }

  private checkUnmatchedStructs(
    lines: string[],
    diagnostics: vscode.Diagnostic[],
    document: vscode.TextDocument
  ): void {
    const keywords = ['fn', 'struct', 'enum', 'mod', 'if', 'while', 'for', 'match'];
    const braceStack: Array<{ keyword: string; line: number; column: number }> = [];

    lines.forEach((line, lineNum) => {
      const trimmed = line.trim();

      keywords.forEach(kw => {
        if (trimmed.startsWith(kw + ' ')) {
          const openCount = (line.match(/{/g) || []).length;
          const closeCount = (line.match(/}/g) || []).length;

          for (let i = 0; i < openCount; i++) {
            braceStack.push({ keyword: kw, line: lineNum, column: line.indexOf('{') });
          }

          for (let i = 0; i < closeCount; i++) {
            braceStack.pop();
          }
        }
      });
    });

    braceStack.forEach(({ keyword, line, column }) => {
      const range = new vscode.Range(line, column, line, column + 1);
      diagnostics.push(new vscode.Diagnostic(
        range,
        `Missing closing brace for ${keyword} block`,
        vscode.DiagnosticSeverity.Error
      ));
    });
  }

  private checkTypeAnnotations(
    lines: string[],
    diagnostics: vscode.Diagnostic[],
    document: vscode.TextDocument
  ): void {
    const letRegex = /let\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*:/;
    const fnRegex = /fn\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(/;

    lines.forEach((line, lineNum) => {
      const letMatch = letRegex.exec(line);
      if (letMatch) {
        const colonIndex = line.indexOf(':', letMatch.index);
        if (colonIndex > -1) {
          const afterColon = line.substring(colonIndex + 1).trim();
          if (!this.isValidType(afterColon.split(/[;=]/)[0].trim())) {
            const range = new vscode.Range(lineNum, colonIndex + 1, lineNum, line.length);
            diagnostics.push(new vscode.Diagnostic(
              range,
              'Invalid type annotation',
              vscode.DiagnosticSeverity.Warning
            ));
          }
        }
      }
    });
  }

  private isValidType(typeStr: string): boolean {
    const validTypes = ['i8', 'i32', 'i64', 'bool', 'void', 'string'];
    const base = typeStr.replace(/[\[\]&*]/g, '').trim();
    return validTypes.includes(base) || /^[A-Z][a-zA-Z0-9_]*$/.test(base);
  }

  private isInString(line: string, position: number): boolean {
    let inString = false;
    let escapeNext = false;

    for (let i = 0; i < position && i < line.length; i++) {
      if (escapeNext) {
        escapeNext = false;
        continue;
      }

      if (line[i] === '\\') {
        escapeNext = true;
        continue;
      }

      if (line[i] === '"') {
        inString = !inString;
      }
    }

    return inString;
  }

  private isInComment(line: string, position: number): boolean {
    const commentIndex = line.indexOf('//');
    return commentIndex !== -1 && position > commentIndex;
  }
}
