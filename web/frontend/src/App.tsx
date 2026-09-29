import { useCallback, useRef, useState } from 'react';
import type { CSSProperties } from 'react';
import TopBar from './components/TopBar';
import PlatformTabs from './components/PlatformTabs';
import CodeEditor from './components/CodeEditor';
import type { CodeEditorHandle } from './components/CodeEditor';
import RuntimePanel from './components/RuntimePanel';
import ObfuscationPanel from './components/ObfuscationPanel';
import BuildOutput from './components/BuildOutput';
import { useRuntimeApis } from './hooks/useRuntimeApis';
import { useCompiler } from './hooks/useCompiler';
import type { Platform, ObfuscationState } from './types';
import { defaultObfuscation, windowsDemoScript, linuxDemoScript } from './data';

const styles: Record<string, CSSProperties> = {
  app: {
    display: 'flex',
    flexDirection: 'column',
    height: '100vh',
    width: '100vw',
    overflow: 'hidden',
  },
  main: {
    display: 'flex',
    flex: 1,
    minHeight: 0,
    overflow: 'hidden',
  },
  rightPanel: {
    width: '40%',
    minWidth: 300,
    maxWidth: 500,
    display: 'flex',
    flexDirection: 'column',
    background: 'var(--bg-primary)',
    overflow: 'hidden',
    minHeight: 0,
  },
  runtimePanelWrapper: {
    flex: 1,
    minHeight: 0,
    overflow: 'auto',
  },
  obfuscationPanelWrapper: {
    flex: 0,
    minHeight: 200,
    borderTop: '1px solid var(--border)',
  },
};

export default function App() {
  const [platform, setPlatform] = useState<Platform>('windows');
  const [editorState, setEditorState] = useState<Record<Platform, string>>({
    windows: windowsDemoScript,
    linux: linuxDemoScript,
  });
  const [obfuscation, setObfuscation] = useState<ObfuscationState>(defaultObfuscation);
  const [forensic, setForensic] = useState(false);
  const editorRef = useRef<CodeEditorHandle>(null);
  const { categories } = useRuntimeApis();
  const { compiling, logs, jobId, buildDone, compile, clearLogs } = useCompiler();

  const handlePlatformChange = useCallback((p: Platform) => {
    setPlatform(p);
  }, []);

  const handleEditorChange = useCallback((value: string) => {
    setEditorState((prev) => ({ ...prev, [platform]: value }));
  }, [platform]);

  const handleInsertSnippet = useCallback((snippet: string) => {
    editorRef.current?.insertAtCursor(snippet);
  }, []);

  const handleTogglePass = useCallback((type: 'mlir' | 'llvm', id: string) => {
    setObfuscation((prev) => ({
      ...prev,
      [type]: prev[type].map((p) =>
        p.id === id ? { ...p, enabled: !p.enabled } : p,
      ),
    }));
  }, []);

  const handleToggleForensic = useCallback(() => {
    setForensic((prev) => !prev);
  }, []);

  const handleCompile = useCallback(() => {
    compile(editorState[platform], platform, obfuscation, forensic);
  }, [compile, editorState, platform, obfuscation, forensic]);

  return (
    <div style={styles.app}>
      <TopBar onCompile={handleCompile} compiling={compiling} />
      <PlatformTabs active={platform} onChange={handlePlatformChange} />
      <div style={styles.main}>
        <CodeEditor
          ref={editorRef}
          value={editorState[platform]}
          onChange={handleEditorChange}
          filename={platform === 'windows' ? 'payload_win.jky' : 'payload_linux.jky'}
        />
        <div style={styles.rightPanel}>
          <div style={styles.runtimePanelWrapper}>
            <RuntimePanel
              categories={categories}
              platform={platform}
              onInsert={handleInsertSnippet}
            />
          </div>
          <div style={styles.obfuscationPanelWrapper}>
            <ObfuscationPanel
              mlir={obfuscation.mlir}
              llvm={obfuscation.llvm}
              onToggle={handleTogglePass}
              forensic={forensic}
              onToggleForensic={handleToggleForensic}
            />
          </div>
        </div>
      </div>
      <BuildOutput
        logs={logs}
        jobId={jobId}
        buildDone={buildDone}
        onClear={clearLogs}
      />
    </div>
  );
}
