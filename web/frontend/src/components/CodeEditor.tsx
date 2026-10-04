import Editor from '@monaco-editor/react';
import type { OnMount } from '@monaco-editor/react';
import { forwardRef, useImperativeHandle, useRef, useState } from 'react';
import type { CSSProperties } from 'react';
import type * as Monaco from 'monaco-editor';
import { registerJockyLanguage } from '../utils/jockyLanguage';

const styles: Record<string, CSSProperties> = {
  container: {
    flex: 1,
    minWidth: 0,
    borderRight: '1px solid var(--border)',
    display: 'flex',
    flexDirection: 'column',
  },
  header: {
    padding: '12px 20px',
    fontSize: 13,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-secondary)',
    background: 'var(--bg-secondary)',
    borderBottom: '1px solid var(--border)',
    letterSpacing: 0.5,
  },
  editor: {
    flex: 1,
  },
};

export interface CodeEditorHandle {
  insertAtCursor: (text: string) => void;
  insertAtEnd: (text: string) => void;
}

interface CodeEditorProps {
  value: string;
  onChange: (value: string) => void;
  filename: string;
}

const CodeEditor = forwardRef<CodeEditorHandle, CodeEditorProps>(
  ({ value, onChange, filename }, ref) => {
    const editorRef = useRef<Monaco.editor.IStandaloneCodeEditor | null>(null);
    const [languageRegistered, setLanguageRegistered] = useState(false);

    const handleMount: OnMount = (editor, monaco) => {
      editorRef.current = editor;
      if (!languageRegistered) {
        registerJockyLanguage(monaco);
        setLanguageRegistered(true);
        editor.setModel(monaco.editor.createModel(value, 'jocky'));
      }

      // Move cursor to end of file by default
      const model = editor.getModel();
      if (model) {
        const lineCount = model.getLineCount();
        const lastLine = model.getLineContent(lineCount);
        const endColumn = lastLine ? lastLine.length + 1 : 1;
        editor.setPosition(new (window as any).monaco.Position(lineCount, endColumn));
      }
    };

    useImperativeHandle(ref, () => ({
      insertAtCursor(text: string) {
        const editor = editorRef.current;
        if (!editor) return;
        const position = editor.getPosition();
        if (!position) return;

        const startLine = position.lineNumber;
        const range = new (window as any).monaco.Range(
          startLine,
          position.column,
          startLine,
          position.column,
        );

        editor.executeEdits('insert-snippet', [
          { range, text: '\n' + text + '\n', forceMoveMarkers: true },
        ]);

        // Scroll to show inserted code
        setTimeout(() => {
          editor.revealLine(startLine + 1);
        }, 100);

        // Highlight the inserted text temporarily
        const textLines = text.split('\n').length;
        const decorations = editor.deltaDecorations([], [
          {
            range: new (window as any).monaco.Range(
              startLine + 1,
              1,
              startLine + textLines,
              1,
            ),
            options: {
              isWholeLine: true,
              className: 'inserted-code-highlight',
              glyphMarginClassName: 'myGlyphMarginClass',
            },
          },
        ]);

        // Remove highlight after 2 seconds
        setTimeout(() => {
          editor.deltaDecorations(decorations, []);
        }, 2000);

        editor.focus();
      },
      insertAtEnd(text: string) {
        const editor = editorRef.current;
        if (!editor) return;
        const model = editor.getModel();
        if (!model) return;

        // Get end of file
        const lineCount = model.getLineCount();
        const lastLine = model.getLineContent(lineCount);
        const endColumn = lastLine ? lastLine.length + 1 : 1;

        // Insert at end with newlines
        const range = new (window as any).monaco.Range(
          lineCount,
          endColumn,
          lineCount,
          endColumn,
        );

        editor.executeEdits('insert-at-end', [
          { range, text: '\n' + text + '\n', forceMoveMarkers: true },
        ]);

        // Scroll to show inserted code
        setTimeout(() => {
          editor.revealLine(lineCount + 1);
        }, 100);

        // Highlight the inserted text temporarily
        const decorations = editor.deltaDecorations([], [
          {
            range: new (window as any).monaco.Range(
              lineCount + 1,
              1,
              lineCount + text.split('\n').length,
              1,
            ),
            options: {
              isWholeLine: true,
              className: 'inserted-code-highlight',
              glyphMarginClassName: 'myGlyphMarginClass',
            },
          },
        ]);

        // Remove highlight after 2 seconds
        setTimeout(() => {
          editor.deltaDecorations(decorations, []);
        }, 2000);

        editor.focus();
      },
    }));

    return (
      <div style={styles.container}>
        <div style={styles.header}>{filename}</div>
        <div style={styles.editor}>
          <Editor
            height="100%"
            defaultLanguage="jocky"
            theme="vs-dark"
            value={value}
            onChange={(v) => onChange(v ?? '')}
            onMount={handleMount}
            options={{
              fontSize: 15,
              fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
              minimap: { enabled: false },
              scrollBeyondLastLine: false,
              padding: { top: 16, bottom: 16 },
              lineNumbers: 'on',
              renderLineHighlight: 'line',
              cursorBlinking: 'smooth',
              smoothScrolling: true,
              tabSize: 4,
              wordWrap: 'on',
              automaticLayout: true,
            }}
          />
        </div>
      </div>
    );
  },
);

export default CodeEditor;
