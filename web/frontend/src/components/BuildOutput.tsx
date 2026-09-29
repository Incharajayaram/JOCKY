import { ChevronDown, ChevronUp, Terminal, Download, X, Copy } from 'lucide-react';
import { useEffect, useRef, useState } from 'react';
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
    padding: '6px 16px',
    cursor: 'pointer',
    borderBottom: '1px solid var(--border)',
    userSelect: 'none' as const,
  },
  headerLeft: {
    display: 'flex',
    alignItems: 'center',
    gap: 8,
    fontSize: 11,
    fontWeight: 600,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-secondary)',
    letterSpacing: 0.5,
  },
  headerRight: {
    display: 'flex',
    alignItems: 'center',
    gap: 8,
  },
  logArea: {
    fontFamily: "'JetBrains Mono', monospace",
    fontSize: 12,
    lineHeight: 1.6,
    padding: '8px 16px',
    overflow: 'auto',
    background: '#06060a',
  },
  logLine: {
    whiteSpace: 'pre-wrap' as const,
    wordBreak: 'break-all' as const,
  },
  emptyState: {
    color: 'var(--text-secondary)',
    fontSize: 12,
    fontFamily: "'JetBrains Mono', monospace",
    padding: '16px',
    textAlign: 'center' as const,
    opacity: 0.6,
  },
};

const logColors: Record<string, string> = {
  info: 'var(--text-primary)',
  warn: 'var(--accent-orange)',
  error: 'var(--accent-red)',
  success: 'var(--accent-green)',
};

function downloadButtonStyle(hovered: boolean): CSSProperties {
  return {
    display: 'flex',
    alignItems: 'center',
    gap: 4,
    padding: '3px 10px',
    borderRadius: 4,
    border: '1px solid var(--accent-green)',
    background: hovered ? 'rgba(34, 197, 94, 0.15)' : 'transparent',
    color: 'var(--accent-green)',
    fontSize: 11,
    fontFamily: "'JetBrains Mono', monospace",
    cursor: 'pointer',
    transition: 'all 0.15s ease',
  };
}

interface BuildOutputProps {
  logs: LogEntry[];
  jobId: string | null;
  buildDone: boolean;
  onClear: () => void;
}

export default function BuildOutput({ logs, jobId, buildDone, onClear }: BuildOutputProps) {
  const [expanded, setExpanded] = useState(true);
  const [dlHovered, setDlHovered] = useState(false);
  const [filteredLogs, setFilteredLogs] = useState<LogEntry[]>(logs);
  const scrollRef = useRef<HTMLDivElement>(null);

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
            <span style={{ color: 'var(--accent-blue)', fontWeight: 400 }}>
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
      {expanded && logs.length > 0 && <LogFilter logs={logs} onFilterChange={setFilteredLogs} />}
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
