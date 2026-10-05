import { ChevronDown, ChevronUp, Terminal, Download, X, Copy, CheckCircle, Circle, Layers, AlignLeft, FileText } from 'lucide-react';
import { useEffect, useRef, useState, useMemo } from 'react';
import type { CSSProperties } from 'react';
import type { LogEntry } from '../types';
import LogFilter from './LogFilter';
import ReportPanel from './ReportPanel';

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
    gap: 8,
  },
  logArea: {
    fontFamily: "'JetBrains Mono', monospace",
    fontSize: 12,
    lineHeight: 1.7,
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
  warn: '#f59e0b',
  error: 'var(--accent-red, #ef4444)',
  success: 'var(--accent-green, #22c55e)',
};

const PIPELINE_STAGES = ['Parse', 'CodeGen', 'MLIR Obf', 'LLVM Obf', 'Compile', 'Link'];

function detectCurrentStage(logs: LogEntry[]) {
  const stages = PIPELINE_STAGES.map((name, i) => ({
    name,
    completed: logs.some((log) => log.text.includes(`Stage ${i + 1}/6`)),
  }));
  return stages;
}

const SIMPLIFIED_KEYWORDS = [
  'Starting', 'Job queued', 'Stage', 'Connected', 'complete', 'finished',
  'success', 'failed', 'error', 'Building', 'Linking', 'Compiling',
];

function isKeyInfoLog(log: LogEntry): boolean {
  if (log.level !== 'info') return true;
  const text = log.text;
  return SIMPLIFIED_KEYWORDS.some(kw => text.toLowerCase().includes(kw.toLowerCase()));
}

function iconBtn(hovered: boolean, danger = false): CSSProperties {
  return {
    background: 'none',
    border: 'none',
    color: hovered ? (danger ? 'var(--accent-red, #ef4444)' : 'var(--text-primary)') : 'var(--text-secondary)',
    cursor: 'pointer',
    padding: 4,
    display: 'flex',
    transition: 'color 0.15s ease',
    borderRadius: 2,
  };
}

function viewToggleStyle(active: boolean): CSSProperties {
  return {
    display: 'flex',
    alignItems: 'center',
    gap: 4,
    padding: '4px 10px',
    border: `1px solid ${active ? 'var(--accent-orange)' : 'var(--border)'}`,
    background: active ? 'rgba(255, 149, 0, 0.12)' : 'transparent',
    color: active ? 'var(--accent-orange)' : 'var(--text-secondary)',
    fontSize: 11,
    fontFamily: "'JetBrains Mono', monospace",
    cursor: 'pointer',
    borderRadius: 0,
    clipPath: 'polygon(5% 0%, 100% 0%, 95% 100%, 0% 100%)',
    transition: 'all 0.2s ease',
  };
}

function reportBtnStyle(hovered: boolean): CSSProperties {
  return {
    display: 'flex',
    alignItems: 'center',
    gap: 6,
    padding: '6px 14px',
    borderRadius: 0,
    border: '1px solid #60a5fa',
    background: hovered ? 'rgba(96, 165, 250, 0.15)' : 'transparent',
    color: '#60a5fa',
    fontSize: 11,
    fontFamily: "'JetBrains Mono', monospace",
    cursor: 'pointer',
    transition: 'all 0.2s ease',
    clipPath: 'polygon(6% 0%, 100% 0%, 94% 100%, 0% 100%)',
  };
}

function downloadBtnStyle(hovered: boolean): CSSProperties {
  return {
    display: 'flex',
    alignItems: 'center',
    gap: 6,
    padding: '6px 14px',
    borderRadius: 0,
    border: '1px solid var(--accent-orange)',
    background: hovered ? 'rgba(255, 149, 0, 0.15)' : 'transparent',
    color: 'var(--accent-orange)',
    fontSize: 11,
    fontFamily: "'JetBrains Mono', monospace",
    cursor: 'pointer',
    transition: 'all 0.2s ease',
    clipPath: 'polygon(6% 0%, 100% 0%, 94% 100%, 0% 100%)',
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
  const [simplified, setSimplified] = useState(false);
  const [activeFilters, setActiveFilters] = useState<Set<string>>(new Set());
  const [showReport, setShowReport] = useState(false);
  const [dlHovered, setDlHovered] = useState(false);
  const [reportHovered, setReportHovered] = useState(false);
  const [copyHovered, setCopyHovered] = useState(false);
  const [clearHovered, setClearHovered] = useState(false);
  const scrollRef = useRef<HTMLDivElement>(null);

  const stages = useMemo(() => detectCurrentStage(logs), [logs]);

  // Reset filters when a new compile starts (logs cleared then repopulated)
  useEffect(() => {
    if (logs.length === 0) {
      setActiveFilters(new Set());
      setSimplified(false);
    }
  }, [logs.length]);

  const displayedLogs = useMemo(() => {
    let result = logs;

    if (simplified) {
      result = result.filter(isKeyInfoLog);
    }

    if (activeFilters.size > 0) {
      result = result.filter(log => activeFilters.has(log.level));
    }

    return result;
  }, [logs, simplified, activeFilters]);

  useEffect(() => {
    if (scrollRef.current) {
      scrollRef.current.scrollTop = scrollRef.current.scrollHeight;
    }
  }, [displayedLogs]);

  useEffect(() => {
    if (logs.length > 0) setExpanded(true);
  }, [logs.length]);

  const toggleFilter = (level: string) => {
    setActiveFilters(prev => {
      if (prev.has(level)) return new Set();
      return new Set([level]);
    });
  };

  const copyLogs = () => {
    const text = displayedLogs.map(log => log.text).join('\n');
    navigator.clipboard.writeText(text).catch(() => {});
  };

  const height = expanded ? 220 : 0;

  return (
    <div style={styles.container}>
      <div style={styles.header} onClick={() => setExpanded(e => !e)}>
        <div style={styles.headerLeft}>
          <Terminal size={13} />
          Build Output
          {logs.length > 0 && (
            <span style={{ color: 'var(--accent-orange)', fontWeight: 400, fontSize: 11 }}>
              ({displayedLogs.length}/{logs.length})
            </span>
          )}
        </div>

        <div style={styles.headerRight} onClick={e => e.stopPropagation()}>
          {/* Simplified / Full toggle */}
          {logs.length > 0 && (
            <>
              <button
                style={viewToggleStyle(!simplified)}
                onClick={() => setSimplified(false)}
                title="Show all log lines"
              >
                <Layers size={10} />
                Full
              </button>
              <button
                style={viewToggleStyle(simplified)}
                onClick={() => setSimplified(true)}
                title="Show only key events"
              >
                <AlignLeft size={10} />
                Simple
              </button>
            </>
          )}

          {buildDone && jobId && (
            <>
              <button
                style={reportBtnStyle(reportHovered)}
                onClick={() => setShowReport(true)}
                onMouseEnter={() => setReportHovered(true)}
                onMouseLeave={() => setReportHovered(false)}
              >
                <FileText size={11} />
                View Report
              </button>
              <button
                style={downloadBtnStyle(dlHovered)}
                onClick={() => window.open(`/api/download/${jobId}`, '_blank')}
                onMouseEnter={() => setDlHovered(true)}
                onMouseLeave={() => setDlHovered(false)}
              >
                <Download size={11} />
                Download Binary
              </button>
            </>
          )}

          {logs.length > 0 && (
            <button
              style={iconBtn(copyHovered)}
              onClick={copyLogs}
              onMouseEnter={() => setCopyHovered(true)}
              onMouseLeave={() => setCopyHovered(false)}
              title="Copy visible logs"
            >
              <Copy size={13} />
            </button>
          )}

          {logs.length > 0 && (
            <button
              style={iconBtn(clearHovered, true)}
              onClick={onClear}
              onMouseEnter={() => setClearHovered(true)}
              onMouseLeave={() => setClearHovered(false)}
              title="Clear logs"
            >
              <X size={13} />
            </button>
          )}

          {expanded
            ? <ChevronDown size={14} color="var(--text-secondary)" />
            : <ChevronUp size={14} color="var(--text-secondary)" />
          }
        </div>
      </div>

      {expanded && logs.length > 0 && (
        <>
          {/* Pipeline stage progress */}
          <div
            style={{
              display: 'flex',
              gap: 6,
              padding: '8px 16px',
              borderBottom: '1px solid var(--border)',
              background: 'rgba(0,0,0,0.2)',
              alignItems: 'center',
              fontSize: 11,
              fontFamily: "'JetBrains Mono', monospace",
              flexWrap: 'wrap' as const,
            }}
          >
            {stages.map((stage, i) => (
              <div key={stage.name} style={{ display: 'flex', alignItems: 'center', gap: 4 }}>
                {stage.completed
                  ? <CheckCircle size={11} color="var(--accent-green, #22c55e)" />
                  : <Circle size={11} color="var(--text-secondary)" />
                }
                <span style={{ color: stage.completed ? 'var(--accent-green, #22c55e)' : 'var(--text-secondary)', fontWeight: stage.completed ? 600 : 400 }}>
                  {stage.name}
                </span>
                {i < stages.length - 1 && (
                  <span style={{ color: 'var(--border)', marginLeft: 2 }}>›</span>
                )}
              </div>
            ))}
          </div>

          <LogFilter
            activeFilters={activeFilters}
            onToggle={toggleFilter}
          />
        </>
      )}

      <div
        ref={scrollRef}
        style={{
          ...styles.logArea,
          height,
          transition: 'height 0.2s ease',
          overflow: expanded ? 'auto' : 'hidden',
        }}
      >
        {logs.length === 0 ? (
          <div style={styles.emptyState}>No build output yet. Click Compile to start.</div>
        ) : displayedLogs.length === 0 ? (
          <div style={styles.emptyState}>No logs match the current filter.</div>
        ) : (
          displayedLogs.map((log, i) => (
            <div key={i} style={{ ...styles.logLine, color: logColors[log.level] }}>
              {log.text}
            </div>
          ))
        )}
      </div>

      {showReport && jobId && (
        <ReportPanel jobId={jobId} onClose={() => setShowReport(false)} />
      )}
    </div>
  );
}
