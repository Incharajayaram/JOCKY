import { Clock, Check, AlertCircle, Loader } from 'lucide-react';
import { useJobHistory } from '../hooks/useJobHistory';
import type { CSSProperties } from 'react';
import { useState } from 'react';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    flexDirection: 'column',
    height: '100%',
    background: 'var(--bg-primary)',
    borderLeft: '1px solid var(--border)',
  },
  header: {
    padding: '8px 16px',
    borderBottom: '1px solid var(--border)',
    fontSize: 11,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-secondary)',
    fontWeight: 600,
  },
  list: {
    flex: 1,
    overflow: 'auto',
    padding: '8px 0',
  },
  item: {
    padding: '8px 16px',
    borderBottom: '1px solid var(--border)',
    cursor: 'pointer',
    transition: 'background 0.15s ease',
    fontSize: 11,
    fontFamily: "'JetBrains Mono', monospace",
  },
  itemHovered: {
    background: 'var(--bg-secondary)',
  },
  itemContent: {
    display: 'flex',
    alignItems: 'center',
    gap: 8,
  },
  statusIcon: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
  },
  itemInfo: {
    flex: 1,
    minWidth: 0,
  },
  itemTitle: {
    color: 'var(--text-primary)',
    marginBottom: 2,
  },
  itemMeta: {
    fontSize: 10,
    color: 'var(--text-secondary)',
  },
};

export default function JobHistory() {
  const { history, loading } = useJobHistory();
  const [hoveredId, setHoveredId] = useState<string | null>(null);

  if (loading) {
    return (
      <div style={styles.container}>
        <div style={styles.header}>Job History (loading...)</div>
      </div>
    );
  }

  const getStatusIcon = (status: string) => {
    switch (status) {
      case 'completed':
        return <Check size={12} color="var(--accent-green)" />;
      case 'failed':
      case 'error':
        return <AlertCircle size={12} color="var(--accent-red)" />;
      case 'running':
      case 'compiling':
        return <Loader size={12} color="var(--accent-blue)" className="animate-spin" />;
      default:
        return <Clock size={12} color="var(--text-secondary)" />;
    }
  };

  return (
    <div style={styles.container}>
      <div style={styles.header}>
        <div style={{ display: 'flex', alignItems: 'center', gap: 4 }}>
          <Clock size={12} />
          Job History ({history.length})
        </div>
      </div>
      <div style={styles.list}>
        {history.length === 0 ? (
          <div style={{ padding: '16px', textAlign: 'center', color: 'var(--text-secondary)', fontSize: 11 }}>
            No compilation history yet
          </div>
        ) : (
          history.slice(0, 20).map((job) => (
            <div
              key={job.job_id}
              style={{
                ...styles.item,
                ...(hoveredId === job.job_id ? styles.itemHovered : {}),
              }}
              onMouseEnter={() => setHoveredId(job.job_id)}
              onMouseLeave={() => setHoveredId(null)}
            >
              <div style={styles.itemContent}>
                <div style={styles.statusIcon}>{getStatusIcon(job.status)}</div>
                <div style={styles.itemInfo}>
                  <div style={styles.itemTitle}>{job.platform.toUpperCase()}</div>
                  <div style={styles.itemMeta}>
                    {new Date(job.created_at).toLocaleTimeString()} · {job.status}
                  </div>
                </div>
              </div>
            </div>
          ))
        )}
      </div>
    </div>
  );
}
