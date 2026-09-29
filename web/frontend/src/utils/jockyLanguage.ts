import type * as Monaco from 'monaco-editor';

export function registerJockyLanguage(monaco: typeof Monaco) {
  monaco.languages.register({ id: 'jocky' });

  monaco.languages.setMonarchTokensProvider('jocky', {
    tokenizer: {
      root: [
        [/\bfn\b/, 'keyword'],
        [/\b(let|mut|const|struct|enum|impl|trait|type|pub|priv|async|await|unsafe|move)\b/, 'keyword'],
        [/\b(if|else|match|for|while|loop|break|continue|return)\b/, 'keyword'],
        [/\b(true|false|null|none)\b/, 'constant'],
        [/\/\/.*$/, 'comment'],
        [/\/\*/, 'comment', '@comment'],
        [/"([^"\\]|\\.)*"/, 'string'],
        [/'([^'\\]|\\.)*'/, 'string'],
        [/\d+\.\d+([eE][+-]?\d+)?/, 'number'],
        [/\d+/, 'number'],
        [/[a-zA-Z_][a-zA-Z0-9_]*(?=\()/, 'function'],
        [/[A-Z][a-zA-Z0-9_]*/, 'type'],
        [/[a-zA-Z_][a-zA-Z0-9_]*/, 'identifier'],
        [/[{}()[\]<>]/, 'delimiter'],
        [/[+\-*/%&|^!=<>?:]/, 'operator'],
      ],
      comment: [
        [/[^*/]+/, 'comment'],
        [/\*\//, 'comment', '@pop'],
        [/[*/]/, 'comment'],
      ],
    },
  });

  monaco.languages.setLanguageConfiguration('jocky', {
    comments: {
      lineComment: '//',
      blockComment: ['/*', '*/'],
    },
    brackets: [
      ['{', '}'],
      ['[', ']'],
      ['(', ')'],
      ['<', '>'],
    ],
    autoClosingPairs: [
      { open: '{', close: '}' },
      { open: '[', close: ']' },
      { open: '(', close: ')' },
      { open: '"', close: '"' },
      { open: "'", close: "'" },
    ],
    surroundingPairs: [
      { open: '{', close: '}' },
      { open: '[', close: ']' },
      { open: '(', close: ')' },
      { open: '"', close: '"' },
      { open: "'", close: "'" },
      { open: '<', close: '>' },
    ],
    folding: {
      markers: {
        start: new RegExp('^\\s*//\\s*#?region\\b'),
        end: new RegExp('^\\s*//\\s*#?endregion\\b'),
      },
    },
  });
}
