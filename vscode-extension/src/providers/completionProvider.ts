import * as vscode from 'vscode';

export class JockyCompletionProvider implements vscode.CompletionItemProvider {
  private keywords: Record<string, vscode.CompletionItemKind> = {
    'fn': vscode.CompletionItemKind.Keyword,
    'struct': vscode.CompletionItemKind.Keyword,
    'enum': vscode.CompletionItemKind.Keyword,
    'let': vscode.CompletionItemKind.Keyword,
    'if': vscode.CompletionItemKind.Keyword,
    'else': vscode.CompletionItemKind.Keyword,
    'while': vscode.CompletionItemKind.Keyword,
    'for': vscode.CompletionItemKind.Keyword,
    'return': vscode.CompletionItemKind.Keyword,
    'match': vscode.CompletionItemKind.Keyword,
    'break': vscode.CompletionItemKind.Keyword,
    'continue': vscode.CompletionItemKind.Keyword,
    'type': vscode.CompletionItemKind.Keyword,
    'mod': vscode.CompletionItemKind.Keyword,
    'use': vscode.CompletionItemKind.Keyword,
    'ffi': vscode.CompletionItemKind.Keyword,
    'lambda': vscode.CompletionItemKind.Keyword,
  };

  private types: Record<string, vscode.CompletionItemKind> = {
    'i8': vscode.CompletionItemKind.TypeParameter,
    'i32': vscode.CompletionItemKind.TypeParameter,
    'i64': vscode.CompletionItemKind.TypeParameter,
    'bool': vscode.CompletionItemKind.TypeParameter,
    'void': vscode.CompletionItemKind.TypeParameter,
    'string': vscode.CompletionItemKind.TypeParameter,
  };

  private builtins: Record<string, [string, string]> = {
    'printf': ['fn printf(format: string, ...) -> i32', 'Print formatted output'],
    'strlen': ['fn strlen(s: string) -> i64', 'Get string length'],
    'sizeof': ['fn sizeof(T) -> i64', 'Get size of type'],
    'nameof': ['fn nameof(T) -> string', 'Get name of type'],
    'jocky_check_analysis_environment': ['fn jocky_check_analysis_environment() -> i32', 'Check for analysis/debugging'],
    'jocky_unhook_ntdll': ['fn jocky_unhook_ntdll() -> void', 'Unhook EDR hooks from ntdll'],
    'jocky_cleanup_all': ['fn jocky_cleanup_all() -> void', 'Clean up anti-forensics'],
    'jocky_self_delete': ['fn jocky_self_delete() -> void', 'Delete executable from disk'],
  };

  provideCompletionItems(
    document: vscode.TextDocument,
    position: vscode.Position,
    token: vscode.CancellationToken,
    context: vscode.CompletionContext
  ): vscode.CompletionItem[] {
    const line = document.lineAt(position).text;
    const word = this.getWordAt(line, position.character);
    const items: vscode.CompletionItem[] = [];

    if (!word) {
      return items;
    }

    this.addCompletions(items, this.keywords, word, (kind) => {
      const item = new vscode.CompletionItem(word, kind);
      item.range = new vscode.Range(
        position.translate(0, -word.length),
        position
      );
      return item;
    });

    this.addCompletions(items, this.types, word, (kind) => {
      const item = new vscode.CompletionItem(word, kind);
      item.range = new vscode.Range(
        position.translate(0, -word.length),
        position
      );
      return item;
    });

    for (const [name, [signature, doc]] of Object.entries(this.builtins)) {
      if (name.startsWith(word)) {
        const item = new vscode.CompletionItem(name, vscode.CompletionItemKind.Function);
        item.detail = signature;
        item.documentation = doc;
        item.range = new vscode.Range(
          position.translate(0, -word.length),
          position
        );
        items.push(item);
      }
    }

    this.addSymbolsFromDocument(document, word, position, items);

    return items;
  }

  private getWordAt(line: string, column: number): string {
    let start = column - 1;
    while (start >= 0 && /[a-zA-Z0-9_]/.test(line[start])) {
      start--;
    }
    return line.substring(start + 1, column);
  }

  private addCompletions(
    items: vscode.CompletionItem[],
    source: Record<string, vscode.CompletionItemKind>,
    word: string,
    factory: (kind: vscode.CompletionItemKind) => vscode.CompletionItem
  ): void {
    for (const [key, kind] of Object.entries(source)) {
      if (key.startsWith(word)) {
        const item = new vscode.CompletionItem(key, kind);
        items.push(item);
      }
    }
  }

  private addSymbolsFromDocument(
    document: vscode.TextDocument,
    word: string,
    position: vscode.Position,
    items: vscode.CompletionItem[]
  ): void {
    const symbolRegex = /(?:fn|let|struct|enum|type)\s+([a-zA-Z_][a-zA-Z0-9_]*)/g;
    let match;

    while ((match = symbolRegex.exec(document.getText())) !== null) {
      const symbol = match[1];
      if (symbol.startsWith(word) && !items.some(item => item.label === symbol)) {
        const item = new vscode.CompletionItem(symbol, vscode.CompletionItemKind.Variable);
        items.push(item);
      }
    }
  }
}
