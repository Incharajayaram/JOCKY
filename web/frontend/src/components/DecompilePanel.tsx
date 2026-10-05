import { useCallback, useMemo, useState, useRef } from 'react';
import type { CSSProperties, DragEvent } from 'react';
import { X, Upload, RefreshCw, Code, GitBranch, AlertTriangle, CheckCircle, Clock, AlertCircle, ChevronDown } from 'lucide-react';
import Editor from '@monaco-editor/react';
import {
  ReactFlow,
  Background,
  Controls,
  MiniMap,
  type Node,
  type Edge,
} from '@xyflow/react';
import '@xyflow/react/dist/style.css';

import { useDogbolt } from '../hooks/useDogbolt';
import type { DecompilerResult } from '../hooks/useDogbolt';
import { parseApproximateCFG } from '../utils/cfgParser';
import type { CFGFunction } from '../utils/cfgParser';

// ─── styles ──────────────────────────────────────────────────────────────────

const overlay: CSSProperties = {
  position: 'fixed',
  inset: 0,
  zIndex: 2000,
  background: 'rgba(0,0,0,0.85)',
  backdropFilter: 'blur(6px)',
  display: 'flex',
  flexDirection: 'column',
};

const panelStyle: CSSProperties = {
  flex: 1,
  display: 'flex',
  flexDirection: 'column',
  background: 'var(--bg-primary)',
  margin: '24px',
  border: '1px solid var(--border)',
  overflow: 'hidden',
  clipPath: 'polygon(0 0, calc(100% - 24px) 0, 100% 24px, 100% 100%, 24px 100%, 0 calc(100% - 24px))',
};

const headerStyle: CSSProperties = {
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'space-between',
  padding: '14px 20px',
  borderBottom: '1px solid var(--border)',
  background: 'var(--bg-secondary)',
  flexShrink: 0,
};

const monoFont = "'JetBrains Mono', 'Fira Code', monospace";

function pill(color: string, bg?: string): CSSProperties {
  return {
    display: 'inline-flex',
    alignItems: 'center',
    gap: 4,
    padding: '3px 10px',
    fontSize: 10,
    fontFamily: monoFont,
    border: `1px solid ${color}`,
    color,
    background: bg ?? 'transparent',
    letterSpacing: 0.5,
    clipPath: 'polygon(6% 0%, 100% 0%, 94% 100%, 0% 100%)',
  };
}

function btnStyle(active: boolean, color = 'var(--accent-orange)'): CSSProperties {
  return {
    display: 'flex',
    alignItems: 'center',
    gap: 6,
    padding: '6px 14px',
    fontSize: 11,
    fontFamily: monoFont,
    border: `1px solid ${active ? color : 'var(--border)'}`,
    background: active ? `${color}22` : 'transparent',
    color: active ? color : 'var(--text-secondary)',
    cursor: 'pointer',
    transition: 'all 0.15s',
    clipPath: 'polygon(5% 0%, 100% 0%, 95% 100%, 0% 100%)',
  };
}

// ─── upload zone ─────────────────────────────────────────────────────────────

function DropZone({ onFile, uploading }: { onFile: (f: File) => void; uploading: boolean }) {
  const [dragging, setDragging] = useState(false);
  const inputRef = useRef<HTMLInputElement>(null);

  const handleDrop = (e: DragEvent<HTMLDivElement>) => {
    e.preventDefault();
    setDragging(false);
    const file = e.dataTransfer.files[0];
    if (file) onFile(file);
  };

  return (
    <div style={{ flex: 1, display: 'flex', alignItems: 'center', justifyContent: 'center', padding: 40 }}>
      <div
        onClick={() => !uploading && inputRef.current?.click()}
        onDragOver={e => { e.preventDefault(); setDragging(true); }}
        onDragLeave={() => setDragging(false)}
        onDrop={handleDrop}
        style={{
          width: '100%',
          maxWidth: 480,
          border: `2px dashed ${dragging ? 'var(--accent-orange)' : 'var(--border)'}`,
          background: dragging ? 'rgba(255,149,0,0.05)' : 'var(--bg-secondary)',
          borderRadius: 2,
          padding: '60px 40px',
          display: 'flex',
          flexDirection: 'column',
          alignItems: 'center',
          gap: 16,
          cursor: uploading ? 'default' : 'pointer',
          transition: 'all 0.2s',
        }}
      >
        {uploading ? (
          <RefreshCw size={32} color="var(--accent-orange)" style={{ animation: 'spin 1s linear infinite' }} />
        ) : (
          <Upload size={32} color={dragging ? 'var(--accent-orange)' : 'var(--text-secondary)'} />
        )}
        <div style={{ textAlign: 'center' }}>
          <div style={{ fontFamily: monoFont, fontSize: 14, color: 'var(--text-primary)', marginBottom: 6 }}>
            {uploading ? 'Uploading to Dogbolt…' : 'Drop binary here or click to browse'}
          </div>
          <div style={{ fontSize: 12, color: 'var(--text-secondary)' }}>
            Max 2 MB · PE / ELF / Mach-O
          </div>
        </div>
        <input ref={inputRef} type="file" style={{ display: 'none' }} onChange={e => e.target.files?.[0] && onFile(e.target.files[0])} />
      </div>
    </div>
  );
}

// ─── status icon ─────────────────────────────────────────────────────────────

function StatusIcon({ status }: { status: string }) {
  if (status === 'Complete') return <CheckCircle size={12} color="var(--accent-green)" />;
  if (status === 'TimedOut' || status === 'Failed') return <AlertTriangle size={12} color="var(--accent-red)" />;
  if (status === 'N/A') return <AlertCircle size={12} color="var(--text-secondary)" />;
  return <RefreshCw size={12} color="var(--accent-orange)" style={{ animation: 'spin 1s linear infinite' }} />;
}

// ─── polling grid ─────────────────────────────────────────────────────────────

function PollingView({
  results,
  secondsLeft,
  binaryId,
  onRerun,
  onSelect,
  selectedKey,
}: {
  results: Map<string, DecompilerResult>;
  secondsLeft: number;
  binaryId: string;
  onRerun: (id: string) => void;
  onSelect: (key: string) => void;
  selectedKey: string | null;
}) {
  const pending = Array.from(results.values()).filter(r => r.status !== 'Complete' && r.status !== 'TimedOut' && r.status !== 'Failed' && r.status !== 'N/A').length;

  return (
    <div style={{ flex: 1, display: 'flex', flexDirection: 'column', overflow: 'hidden' }}>
      {/* Status bar */}
      <div style={{ padding: '10px 20px', borderBottom: '1px solid var(--border)', background: 'var(--bg-secondary)', display: 'flex', alignItems: 'center', gap: 16, flexShrink: 0 }}>
        <div style={{ display: 'flex', alignItems: 'center', gap: 6, fontFamily: monoFont, fontSize: 12 }}>
          <Clock size={12} color="var(--accent-orange)" />
          <span style={{ color: 'var(--accent-orange)' }}>{secondsLeft}s</span>
          <span style={{ color: 'var(--text-secondary)' }}>remaining</span>
        </div>
        <div style={{ fontSize: 11, color: 'var(--text-secondary)', fontFamily: monoFont }}>
          {pending > 0 ? `${pending} decompiler${pending > 1 ? 's' : ''} running` : 'All finished'}
        </div>
        {binaryId && (
          <div style={{ fontSize: 11, color: 'var(--text-secondary)', fontFamily: monoFont, marginLeft: 'auto' }}>
            ID: <span style={{ color: 'var(--text-primary)' }}>{binaryId.slice(0, 8)}…</span>
          </div>
        )}
      </div>

      {/* Grid of decompiler cards */}
      <div style={{ flex: 1, overflow: 'auto', padding: '16px 20px', display: 'grid', gridTemplateColumns: 'repeat(auto-fill, minmax(220px, 1fr))', gap: 10, alignContent: 'start' }}>
        {results.size === 0 && (
          <div style={{ gridColumn: '1/-1', color: 'var(--text-secondary)', fontFamily: monoFont, fontSize: 12, padding: 20 }}>
            Waiting for decompilers to start…
          </div>
        )}
        {Array.from(results.entries()).map(([key, r]) => {
          const isSelected = selectedKey === key;
          const hasCode = !!r.code;
          return (
            <div
              key={key}
              onClick={() => hasCode && onSelect(key)}
              style={{
                border: `1px solid ${isSelected ? 'var(--accent-orange)' : 'var(--border)'}`,
                background: isSelected ? 'rgba(255,149,0,0.07)' : 'var(--bg-secondary)',
                padding: '12px 14px',
                cursor: hasCode ? 'pointer' : 'default',
                transition: 'all 0.15s',
              }}
            >
              <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between', marginBottom: 6 }}>
                <span style={{ fontFamily: monoFont, fontSize: 12, fontWeight: 700, color: 'var(--text-primary)' }}>
                  {r.decompiler.name}
                </span>
                <StatusIcon status={r.status} />
              </div>
              <div style={{ fontSize: 10, color: 'var(--text-secondary)', fontFamily: monoFont, marginBottom: 8 }}>
                v{r.decompiler.version}
              </div>
              <div style={{ display: 'flex', alignItems: 'center', justifyContent: 'space-between' }}>
                <span style={{ fontSize: 10, color: r.status === 'Complete' ? 'var(--accent-green)' : r.status === 'TimedOut' ? 'var(--accent-red)' : 'var(--text-secondary)', fontFamily: monoFont }}>
                  {r.status}
                </span>
                {(r.status === 'TimedOut' || r.status === 'Failed') && (
                  <button
                    onClick={e => { e.stopPropagation(); onRerun(r.id); }}
                    style={{ background: 'none', border: '1px solid var(--border)', color: 'var(--text-secondary)', cursor: 'pointer', fontSize: 9, padding: '2px 6px', fontFamily: monoFont }}
                  >
                    Rerun
                  </button>
                )}
                {hasCode && (
                  <span style={{ fontSize: 9, color: 'var(--accent-orange)', fontFamily: monoFont }}>
                    {isSelected ? '▶ SELECTED' : 'Click to view'}
                  </span>
                )}
              </div>
            </div>
          );
        })}
      </div>
    </div>
  );
}

// ─── CFG view using React Flow ────────────────────────────────────────────────

const NODE_WIDTH = 220;
const NODE_HEIGHT_BASE = 60;
const LEVEL_GAP = 120;
const H_GAP = 260;

function buildFlowGraph(fn: CFGFunction): { nodes: Node[]; edges: Edge[] } {
  // Simple layered layout
  const nodes: Node[] = fn.blocks.map((block, i) => {
    const col = i % 3;
    const row = Math.floor(i / 3);
    const bgColor =
      block.type === 'entry' ? '#1a3a1a' :
      block.type === 'return' ? '#3a1a1a' :
      block.type === 'condition' ? '#1a1a3a' : '#1a1a1a';
    const borderColor =
      block.type === 'entry' ? '#22c55e' :
      block.type === 'return' ? '#ef4444' :
      block.type === 'condition' ? '#60a5fa' : '#333';

    return {
      id: block.id,
      position: { x: col * H_GAP, y: row * LEVEL_GAP },
      style: {
        background: bgColor,
        border: `1px solid ${borderColor}`,
        borderRadius: 0,
        padding: '8px 10px',
        fontFamily: monoFont,
        fontSize: 10,
        color: '#e0e0e8',
        width: NODE_WIDTH,
        minHeight: NODE_HEIGHT_BASE,
        whiteSpace: 'pre-wrap' as const,
      },
      data: { label: block.label || block.id },
    };
  });

  const edges: Edge[] = fn.edges.map(e => ({
    id: e.id,
    source: e.source,
    target: e.target,
    label: e.label,
    type: 'smoothstep',
    style: { stroke: e.type === 'branch' ? '#60a5fa' : '#555', strokeWidth: 1.5 },
    labelStyle: { fontFamily: monoFont, fontSize: 9, fill: '#8888a0' },
    animated: e.type === 'flow',
  }));

  return { nodes, edges };
}

function CFGView({ code }: { code: string }) {
  const cfg = useMemo(() => parseApproximateCFG(code), [code]);
  const [selectedFn, setSelectedFn] = useState(0);
  const [open, setOpen] = useState(false);

  const fn = cfg.functions[selectedFn];
  if (!fn) {
    return (
      <div style={{ flex: 1, display: 'flex', alignItems: 'center', justifyContent: 'center', color: 'var(--text-secondary)', fontFamily: monoFont, fontSize: 13 }}>
        No parseable functions found in decompiled output.
      </div>
    );
  }

  const { nodes, edges } = buildFlowGraph(fn);

  return (
    <div style={{ flex: 1, display: 'flex', flexDirection: 'column', overflow: 'hidden' }}>
      {/* Function selector */}
      <div style={{ padding: '8px 16px', borderBottom: '1px solid var(--border)', background: 'var(--bg-secondary)', display: 'flex', alignItems: 'center', gap: 12, flexShrink: 0 }}>
        <div style={{ fontSize: 10, color: 'var(--text-secondary)', fontFamily: monoFont, letterSpacing: 1 }}>FUNCTION</div>
        <div style={{ position: 'relative' }}>
          <button
            onClick={() => setOpen(o => !o)}
            style={{ display: 'flex', alignItems: 'center', gap: 8, background: 'var(--bg-primary)', border: '1px solid var(--border)', color: 'var(--text-primary)', padding: '4px 10px', fontFamily: monoFont, fontSize: 11, cursor: 'pointer' }}
          >
            {fn.name} ({fn.blocks.length} blocks)
            <ChevronDown size={10} />
          </button>
          {open && (
            <div style={{ position: 'absolute', top: '100%', left: 0, zIndex: 10, background: 'var(--bg-secondary)', border: '1px solid var(--border)', minWidth: 200, maxHeight: 200, overflowY: 'auto' }}>
              {cfg.functions.map((f, i) => (
                <div
                  key={f.name}
                  onClick={() => { setSelectedFn(i); setOpen(false); }}
                  style={{ padding: '6px 12px', fontSize: 11, fontFamily: monoFont, cursor: 'pointer', background: i === selectedFn ? 'rgba(255,149,0,0.1)' : 'transparent', color: i === selectedFn ? 'var(--accent-orange)' : 'var(--text-primary)' }}
                >
                  {f.name}
                </div>
              ))}
            </div>
          )}
        </div>
        <div style={pill('#60a5fa')}>Approximate CFG (from pseudocode)</div>
      </div>

      {/* React Flow */}
      <div style={{ flex: 1 }}>
        <ReactFlow
          nodes={nodes}
          edges={edges}
          fitView
          minZoom={0.2}
          maxZoom={3}
          colorMode="dark"
        >
          <Background color="#222" gap={20} />
          <Controls style={{ background: 'var(--bg-secondary)', border: '1px solid var(--border)' }} />
          <MiniMap
            style={{ background: 'var(--bg-secondary)', border: '1px solid var(--border)' }}
            nodeColor="#555"
          />
        </ReactFlow>
      </div>
    </div>
  );
}

// ─── IR (Monaco) view ─────────────────────────────────────────────────────────

function IRView({ code }: { code: string }) {
  return (
    <div style={{ flex: 1, overflow: 'hidden' }}>
      <Editor
        defaultLanguage="c"
        value={code}
        theme="vs-dark"
        options={{
          readOnly: true,
          minimap: { enabled: true },
          fontSize: 12,
          fontFamily: monoFont,
          wordWrap: 'on',
          scrollBeyondLastLine: false,
          lineNumbers: 'on',
        }}
        height="100%"
      />
    </div>
  );
}

// ─── result view ─────────────────────────────────────────────────────────────

function ResultView({
  results,
  secondsLeft,
  binaryId,
  onRerun,
}: {
  results: Map<string, DecompilerResult>;
  secondsLeft: number;
  binaryId: string;
  onRerun: (id: string) => void;
}) {
  const [view, setView] = useState<'code' | 'cfg'>('code');
  const [selectedKey, setSelectedKey] = useState<string | null>(() => {
    // Default to Ghidra if available
    for (const [key, r] of results) {
      if (r.decompiler.name === 'Ghidra' && r.code) return key;
    }
    for (const [key, r] of results) {
      if (r.code) return key;
    }
    return null;
  });

  // Auto-select Ghidra when its code arrives
  const selectedResult = selectedKey ? results.get(selectedKey) : null;
  const ghidraResult = Array.from(results.values()).find(r => r.decompiler.name === 'Ghidra' && r.code);

  const active = selectedResult ?? ghidraResult ?? Array.from(results.values()).find(r => r.code) ?? null;
  const activeKey = active ? `${active.decompiler.name}@${active.decompiler.version}` : null;

  return (
    <div style={{ flex: 1, display: 'flex', flexDirection: 'column', overflow: 'hidden' }}>
      {/* Toolbar */}
      <div style={{ display: 'flex', alignItems: 'center', gap: 8, padding: '8px 16px', borderBottom: '1px solid var(--border)', background: 'var(--bg-secondary)', flexShrink: 0, flexWrap: 'wrap' as const }}>
        <button style={btnStyle(view === 'code')} onClick={() => setView('code')}>
          <Code size={11} /> IR / Code
        </button>
        <button style={btnStyle(view === 'cfg', '#60a5fa')} onClick={() => setView('cfg')}>
          <GitBranch size={11} /> CFG
        </button>
        <div style={{ marginLeft: 8, display: 'flex', gap: 6, flexWrap: 'wrap' as const }}>
          {Array.from(results.entries()).map(([key, r]) => r.code && (
            <button
              key={key}
              onClick={() => setSelectedKey(key)}
              style={btnStyle(activeKey === key)}
            >
              {r.decompiler.name} v{r.decompiler.version}
            </button>
          ))}
        </div>
        <div style={{ marginLeft: 'auto', fontSize: 10, color: 'var(--text-secondary)', fontFamily: monoFont }}>
          {binaryId?.slice(0, 8)}…
        </div>

        {/* Decompilers that timed out */}
        {Array.from(results.values()).filter(r => r.status === 'TimedOut' || r.status === 'Failed').map(r => (
          <div key={r.id} style={{ display: 'flex', alignItems: 'center', gap: 6, fontSize: 10, color: 'var(--accent-red)', fontFamily: monoFont }}>
            <AlertTriangle size={10} />
            {r.decompiler.name} timed out
            <button onClick={() => onRerun(r.id)} style={{ background: 'none', border: '1px solid var(--border)', color: 'var(--text-secondary)', cursor: 'pointer', fontSize: 9, padding: '1px 5px', fontFamily: monoFont }}>
              Rerun
            </button>
          </div>
        ))}
      </div>

      {/* Pending decompilers strip */}
      {Array.from(results.values()).filter(r => !r.code && r.status !== 'TimedOut' && r.status !== 'Failed' && r.status !== 'N/A').length > 0 && (
        <div style={{ padding: '6px 16px', borderBottom: '1px solid var(--border)', background: 'rgba(255,149,0,0.05)', display: 'flex', alignItems: 'center', gap: 8, flexShrink: 0 }}>
          <RefreshCw size={10} color="var(--accent-orange)" style={{ animation: 'spin 1s linear infinite' }} />
          <span style={{ fontSize: 11, color: 'var(--text-secondary)', fontFamily: monoFont }}>
            Still running: {Array.from(results.values()).filter(r => !r.code && r.status !== 'TimedOut' && r.status !== 'Failed' && r.status !== 'N/A').map(r => r.decompiler.name).join(', ')} · {secondsLeft}s left
          </span>
        </div>
      )}

      {/* Content */}
      {!active ? (
        <div style={{ flex: 1, display: 'flex', alignItems: 'center', justifyContent: 'center', color: 'var(--text-secondary)', fontFamily: monoFont, fontSize: 13 }}>
          <RefreshCw size={14} style={{ marginRight: 8, animation: 'spin 1s linear infinite' }} />
          Waiting for decompilation results…
        </div>
      ) : view === 'code' ? (
        <IRView code={active.code!} />
      ) : (
        <CFGView code={active.code!} />
      )}
    </div>
  );
}

// ─── main component ───────────────────────────────────────────────────────────

interface DecompilePanelProps {
  onClose: () => void;
}

export default function DecompilePanel({ onClose }: DecompilePanelProps) {
  const { uploadState, binaryId, results, error, secondsLeft, fileName, upload, rerun, reset } = useDogbolt();
  const [selectedKey, setSelectedKey] = useState<string | null>(null);

  const handleFile = useCallback((file: File) => {
    upload(file);
  }, [upload]);

  const handleReset = () => {
    reset();
    setSelectedKey(null);
  };

  const showPollingGrid = uploadState === 'polling' && results.size > 0 && !Array.from(results.values()).some(r => r.code);
  const showResult = (uploadState === 'polling' || uploadState === 'done') && Array.from(results.values()).some(r => r.code);

  return (
    <div style={overlay} onClick={e => { if (e.target === e.currentTarget) onClose(); }}>
      <div style={panelStyle}>
        {/* Header */}
        <div style={headerStyle}>
          <div style={{ display: 'flex', alignItems: 'center', gap: 12 }}>
            <GitBranch size={14} color="var(--accent-orange)" />
            <span style={{ fontFamily: monoFont, fontSize: 13, fontWeight: 700, color: 'var(--text-primary)' }}>
              Decompiler Explorer
            </span>
            {fileName && (
              <span style={{ fontSize: 11, color: 'var(--text-secondary)', fontFamily: monoFont }}>
                — {fileName}
              </span>
            )}
            <div style={pill('#60a5fa')}>via Dogbolt · Ghidra</div>
          </div>
          <div style={{ display: 'flex', alignItems: 'center', gap: 8 }}>
            {(uploadState !== 'idle') && (
              <button onClick={handleReset} style={btnStyle(false)}>
                New Upload
              </button>
            )}
            <button
              onClick={onClose}
              style={{ background: 'none', border: 'none', color: 'var(--text-secondary)', cursor: 'pointer', padding: 4, display: 'flex' }}
            >
              <X size={16} />
            </button>
          </div>
        </div>

        {/* Body */}
        {error && (
          <div style={{ padding: '12px 20px', background: 'rgba(239,68,68,0.1)', borderBottom: '1px solid #ef4444', display: 'flex', alignItems: 'center', gap: 8, fontSize: 12, color: '#ef4444', fontFamily: monoFont, flexShrink: 0 }}>
            <AlertTriangle size={12} />
            {error}
          </div>
        )}

        {uploadState === 'idle' || uploadState === 'uploading' ? (
          <DropZone onFile={handleFile} uploading={uploadState === 'uploading'} />
        ) : showPollingGrid ? (
          <PollingView
            results={results}
            secondsLeft={secondsLeft}
            binaryId={binaryId!}
            onRerun={rerun}
            onSelect={setSelectedKey}
            selectedKey={selectedKey}
          />
        ) : showResult ? (
          <ResultView
            results={results}
            secondsLeft={secondsLeft}
            binaryId={binaryId!}
            onRerun={rerun}
          />
        ) : (
          <div style={{ flex: 1, display: 'flex', alignItems: 'center', justifyContent: 'center', color: 'var(--text-secondary)', fontFamily: monoFont, fontSize: 13 }}>
            <RefreshCw size={14} style={{ marginRight: 8, animation: 'spin 1s linear infinite' }} />
            Uploading to Dogbolt…
          </div>
        )}
      </div>
    </div>
  );
}
