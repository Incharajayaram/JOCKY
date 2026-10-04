import { ChevronDown, ChevronUp, Terminal, Download, X, Copy, CheckCircle, Circle } from 'lucide-react';
import { useEffect, useRef, useState, useMemo } from 'react';
import type { CSSProperties } from 'react';
import type { LogEntry } from '../types';
import LogFilter from './LogFilter';

const styles: Record<string, CSSProperties> = {
  container: {
    borderTop: '1px solid var(--border)',
    background: 'var(--bg-secondary)',
    display: 'flex',
    flexDirection: 'column',
    flexShrink: 0,
  },
  header: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    padding: '10px 20px',
    cursor: 'pointer',
    borderBottom: '1px solid var(--border)',
    userSelect: 'none' as const,
  },
  headerLeft: {
    display: 'flex',
    alignItems: 'center',
    gap: 10,
    fontSize: 13,
    fontWeight: 600,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-secondary)',
    letterSpacing: 0.5,
  },
  headerRight: {
    display: 'flex',
    alignItems: 'center',
    gap: 10,
  },
  logArea: {
    fontFamily: "'JetBrains Mono', monospace",
    fontSize: 13,
    lineHeight: 1.6,
    padding: '12px 20px',
    overflow: 'auto',
    background: '#06060a',
  },
  logLine: {
    whiteSpace: 'pre-wrap' as const,
    wordBreak: 'break-all' as const,
  },
  emptyState: {
    color: 'var(--text-secondary)',
    fontSize: 13,
    fontFamily: "'JetBrains Mono', monospace",
    padding: '20px',
    textAlign: 'center' as const,
    opacity: 0.6,
  },
};

const logColors: Record<string, string> = {
  info: 'var(--text-primary)',
  warn: 'var(--accent-orange)',
  error: 'var(--accent-red)',
  success: 'var(--accent-orange)',
};

function downloadButtonStyle(hovered: boolean): CSSProperties {
  return {
    display: 'flex',
    alignItems: 'center',
    gap: 6,
    padding: '8px 16px',
    borderRadius: 0,
    border: '1px solid var(--accent-orange)',
    background: hovered ? 'rgba(255, 149, 0, 0.15)' : 'transparent',
    color: 'var(--accent-orange)',
    fontSize: 12,
    fontFamily: "'JetBrains Mono', monospace",
    cursor: 'pointer',
    transition: 'all 0.25s cubic-bezier(0.4, 0, 0.2, 1)',
    clipPath: 'polygon(6% 0%, 100% 0%, 94% 100%, 0% 100%)',
    pointerEvents: 'auto',
  };
}

interface BuildOutputProps {
  logs: LogEntry[];
  jobId: string | null;
  buildDone: boolean;
  onClear: () => void;
}

const PIPELINE_STAGES = ['Parse', 'CodeGen', 'MLIR Obf', 'LLVM Obf', 'Compile', 'Link'];

function detectCurrentStage(logs: LogEntry[]): { current: number; stages: { name: string; completed: boolean }[] } {
  const stages = PIPELINE_STAGES.map((name) => ({
    name,
    completed: logs.some((log) => log.text.includes(`Stage ${PIPELINE_STAGES.indexOf(name) + 1}/6`)),
  }));

  let current = 0;
  for (let i = 0; i < stages.length; i++) {
    if (stages[i].completed) current = i + 1;
  }

  return { current: Math.min(current, PIPELINE_STAGES.length - 1), stages };
}

export default function BuildOutput({ logs, jobId, buildDone, onClear }: BuildOutputProps) {
  const [expanded, setExpanded] = useState(true);
  const [dlHovered, setDlHovered] = useState(false);
  const [filteredLogs, setFilteredLogs] = useState<LogEntry[]>(logs);
  const scrollRef = useRef<HTMLDivElement>(null);
  const pipeline = useMemo(() => detectCurrentStage(logs), [logs]);

  useEffect(() => {
    if (scrollRef.current) {
      scrollRef.current.scrollTop = scrollRef.current.scrollHeight;
    }
  }, [filteredLogs]);

  useEffect(() => {
    setFilteredLogs(logs);
  }, [logs]);

  useEffect(() => {
    if (logs.length > 0) setExpanded(true);
  }, [logs.length]);

  const copyLogs = () => {
    const text = filteredLogs.map(log => log.text).join('\n');
    navigator.clipboard.writeText(text);
  };

  const height = expanded ? 200 : 0;

  return (
    <div style={styles.container}>
      <div style={styles.header} onClick={() => setExpanded(!expanded)}>
        <div style={styles.headerLeft}>
          <Terminal size={13} />
          Build Output
          {logs.length > 0 && (
            <span style={{ color: 'var(--accent-orange)', fontWeight: 400 }}>
              ({filteredLogs.length}/{logs.length} lines)
            </span>
          )}
        </div>
        <div style={styles.headerRight}>
          {buildDone && jobId && (
            <button
              style={downloadButtonStyle(dlHovered)}
              onClick={(e) => {
                e.stopPropagation();
                window.open(`/api/download/${jobId}`, '_blank');
              }}
              onMouseEnter={() => setDlHovered(true)}
              onMouseLeave={() => setDlHovered(false)}
            >
              <Download size={12} />
              Download Binary
            </button>
          )}
          {logs.length > 0 && (
            <button
              style={{
                background: 'none',
                border: 'none',
                color: 'var(--text-secondary)',
                cursor: 'pointer',
                padding: 2,
                display: 'flex',
              }}
              onClick={(e) => {
                e.stopPropagation();
                copyLogs();
              }}
              title="Copy logs"
            >
              <Copy size={14} />
            </button>
          )}
          {logs.length > 0 && (
            <button
              style={{
                background: 'none',
                border: 'none',
                color: 'var(--text-secondary)',
                cursor: 'pointer',
                padding: 2,
                display: 'flex',
              }}
              onClick={(e) => {
                e.stopPropagation();
                onClear();
              }}
              title="Clear logs"
            >
              <X size={14} />
            </button>
          )}
          {expanded ? <ChevronDown size={14} color="var(--text-secondary)" /> : <ChevronUp size={14} color="var(--text-secondary)" />}
        </div>
      </div>
      {expanded && logs.length > 0 && (
        <>
          <div
            style={{
              display: 'flex',
              gap: 8,
              padding: '8px 16px',
              borderBottom: '1px solid var(--border)',
              background: 'rgba(0,0,0,0.2)',
              alignItems: 'center',
              fontSize: 11,
              fontFamily: "'JetBrains Mono', monospace",
            }}
          >
            {pipeline.stages.map((stage, i) => (
              <div key={stage.name} style={{ display: 'flex', alignItems: 'center', gap: 4 }}>
                {stage.completed ? (
                  <CheckCircle size={12} color="var(--accent-green)" />
                ) : (
                  <Circle size={12} color="var(--text-secondary)" />
                )}
                <span
                  style={{
                    color: stage.completed ? 'var(--accent-green)' : 'var(--text-secondary)',
                    fontWeight: stage.completed ? 600 : 400,
                  }}
                >
                  {stage.name}
                </span>
                {i < pipeline.stages.length - 1 && (
                  <div style={{ color: 'var(--text-secondary)', marginLeft: 4 }}>→</div>
                )}
              </div>
            ))}
          </div>
          <LogFilter logs={logs} onFilterChange={setFilteredLogs} />
        </>
      )}
      <div
        ref={scrollRef}
        style={{ ...styles.logArea, height, transition: 'height 0.2s ease', overflow: expanded ? 'auto' : 'hidden' }}
      >
        {logs.length === 0 ? (
          <div style={styles.emptyState}>
            No build output yet. Click Compile to start.
          </div>
        ) : filteredLogs.length === 0 ? (
          <div style={styles.emptyState}>
            No logs match the current filter.
          </div>
        ) : (
          filteredLogs.map((log, i) => (
            <div key={i} style={{ ...styles.logLine, color: logColors[log.level] }}>
              {log.text}
            </div>
          ))
        )}
      </div>
    </div>
  );
}
