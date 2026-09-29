import Editor from '@monaco-editor/react';
import type { OnMount } from '@monaco-editor/react';
import { forwardRef, useImperativeHandle, useRef } from 'react';
import type { CSSProperties } from 'react';
import type * as Monaco from 'monaco-editor';

const styles: Record<string, CSSProperties> = {
  container: {
    flex: 1,
    minWidth: 0,
    borderRight: '1px solid var(--border)',
    display: 'flex',
    flexDirection: 'column',
  },
  header: {
    padding: '8px 16px',
    fontSize: 11,
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
}

interface CodeEditorProps {
  value: string;
  onChange: (value: string) => void;
  filename: string;
}

const CodeEditor = forwardRef<CodeEditorHandle, CodeEditorProps>(
  ({ value, onChange, filename }, ref) => {
    const editorRef = useRef<Monaco.editor.IStandaloneCodeEditor | null>(null);

    const handleMount: OnMount = (editor) => {
      editorRef.current = editor;
    };

    useImperativeHandle(ref, () => ({
      insertAtCursor(text: string) {
        const editor = editorRef.current;
        if (!editor) return;
        const position = editor.getPosition();
        if (!position) return;
        const range = new (window as any).monaco.Range(
          position.lineNumber,
          position.column,
          position.lineNumber,
          position.column,
        );
        editor.executeEdits('insert-snippet', [
          { range, text: '\n' + text + '\n', forceMoveMarkers: true },
        ]);
        editor.focus();
      },
    }));

    return (
      <div style={styles.container}>
        <div style={styles.header}>{filename}</div>
        <div style={styles.editor}>
          <Editor
            height="100%"
            defaultLanguage="rust"
            theme="vs-dark"
            value={value}
            onChange={(v) => onChange(v ?? '')}
            onMount={handleMount}
            options={{
              fontSize: 14,
              fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
              minimap: { enabled: false },
              scrollBeyondLastLine: false,
              padding: { top: 12, bottom: 12 },
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
