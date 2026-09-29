import * as vscode from 'vscode';

export class JockyHoverProvider implements vscode.HoverProvider {
  private documentation: Record<string, string> = {
    'fn': '**fn** - Function declaration\n\n```jocky\nfn name(params) -> return_type { body }\n```\n\nDefines a named function with typed parameters and return type.',
    'struct': '**struct** - Structure definition\n\n```jocky\nstruct Name {\n  field: type,\n  ...\n}\n```\n\nDefines a composite data type with named fields.',
    'enum': '**enum** - Enumeration definition\n\n```jocky\nenum Name {\n  Variant1 = 0,\n  Variant2 = 1,\n}\n```\n\nDefines a sum type with named variants.',
    'let': '**let** - Variable binding\n\n```jocky\nlet name: type = value;\n```\n\nBinds an immutable variable with explicit type annotation.',
    'if': '**if** - Conditional expression\n\n```jocky\nif condition {\n  then_body\n} else {\n  else_body\n}\n```\n\nExecutes code conditionally.',
    'else': '**else** - Alternative branch in if expression\n\nExecutes when the if condition is false.',
    'while': '**while** - Loop expression\n\n```jocky\nwhile condition {\n  body\n}\n```\n\nExecutes body repeatedly while condition is true.',
    'for': '**for** - Iterator loop\n\n```jocky\nfor i = 0; i < 10; i = i + 1 {\n  body\n}\n```\n\nExecutes body for each iteration.',
    'match': '**match** - Pattern matching expression\n\n```jocky\nmatch expr {\n  pattern => body,\n  ...\n}\n```\n\nMatches expression against patterns.',
    'return': '**return** - Exit function with value\n\n```jocky\nreturn value;\n```\n\nReturns from function with specified value.',
    'break': '**break** - Exit loop\n\nExits the innermost enclosing loop.',
    'continue': '**continue** - Continue loop\n\nSkips to next iteration of the enclosing loop.',
    'type': '**type** - Type alias declaration\n\n```jocky\ntype Alias = TargetType;\n```\n\nCreates an alias for an existing type.',
    'mod': '**mod** - Module declaration\n\n```jocky\nmod name { ... }\n```\n\nDefines a module namespace.',
    'use': '**use** - Import declaration\n\n```jocky\nuse path::to::symbol;\n```\n\nImports a symbol from a module.',
    'ffi': '**ffi** - Foreign function interface\n\n```jocky\nffi fn external() -> type;\n```\n\nDeclares an external C function.',
    'lambda': '**lambda** - Anonymous function\n\n```jocky\nlambda (params) -> type { body }\n```\n\nDefines an inline anonymous function.',
    'i8': '**i8** - 8-bit signed integer type',
    'i32': '**i32** - 32-bit signed integer type',
    'i64': '**i64** - 64-bit signed integer type',
    'bool': '**bool** - Boolean type (true/false)',
    'void': '**void** - Empty/no-return type',
    'string': '**string** - String type',
    'true': '**true** - Boolean true value',
    'false': '**false** - Boolean false value',
    'null': '**null** - Null pointer value',
    'printf': '**printf**(format: string, ...) -> i32\n\nPrints formatted output to standard output. Returns number of characters printed.',
    'strlen': '**strlen**(s: string) -> i64\n\nReturns the length of a string.',
    'sizeof': '**sizeof**(type) -> i64\n\nReturns the size in bytes of a type.',
    'nameof': '**nameof**(type) -> string\n\nReturns the name of a type as a string.',
    'jocky_check_analysis_environment': '**jocky_check_analysis_environment**() -> i32\n\nChecks if running inside debugger or sandbox. Returns non-zero if detected.',
    'jocky_unhook_ntdll': '**jocky_unhook_ntdll**() -> void\n\nRemoves EDR hooks from ntdll to bypass interception.',
    'jocky_cleanup_all': '**jocky_cleanup_all**() -> void\n\nPerforms comprehensive anti-forensics cleanup before exit.',
    'jocky_self_delete': '**jocky_self_delete**() -> void\n\nDeletes the executable file from disk.',
  };

  provideHover(
    document: vscode.TextDocument,
    position: vscode.Position,
    token: vscode.CancellationToken
  ): vscode.Hover | null {
    const wordRange = document.getWordRangeAtPosition(position, /[a-zA-Z_][a-zA-Z0-9_]*/);
    if (!wordRange) {
      return null;
    }

    const word = document.getText(wordRange);
    const doc = this.documentation[word];

    if (doc) {
      return new vscode.Hover(new vscode.MarkdownString(doc));
    }

    return null;
  }
}
