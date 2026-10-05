import { Filter } from 'lucide-react';
import type { CSSProperties } from 'react';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    alignItems: 'center',
    gap: 10,
    padding: '8px 16px',
    borderBottom: '1px solid var(--border)',
    background: 'var(--bg-secondary)',
    fontSize: 12,
    flexWrap: 'wrap' as const,
  },
  label: {
    color: 'var(--text-secondary)',
    fontWeight: 600,
    fontFamily: "'JetBrains Mono', monospace",
  },
  chip: {
    display: 'inline-flex',
    alignItems: 'center',
    gap: 6,
    padding: '4px 12px',
    borderRadius: 0,
    background: 'var(--bg-primary)',
    border: '1px solid var(--border)',
    cursor: 'pointer',
    transition: 'all 0.2s ease',
    clipPath: 'polygon(8% 0%, 100% 0%, 92% 100%, 0% 100%)',
    color: 'var(--text-secondary)',
    fontFamily: "'JetBrains Mono', monospace",
    fontSize: 11,
  },
  chipActive: {
    border: '1px solid var(--accent-orange)',
    color: 'var(--accent-orange)',
    background: 'rgba(255, 149, 0, 0.1)',
  },
};

const LEVEL_COLORS: Record<string, string> = {
  info: 'var(--text-secondary)',
  warn: '#f59e0b',
  error: 'var(--accent-red)',
  success: 'var(--accent-green, #22c55e)',
};

interface LogFilterProps {
  activeFilters: Set<string>;
  onToggle: (level: string) => void;
}

export default function LogFilter({ activeFilters, onToggle }: LogFilterProps) {
  const levels = ['info', 'warn', 'error', 'success'] as const;

  return (
    <div style={styles.container}>
      <Filter size={11} color="var(--text-secondary)" />
      <span style={styles.label}>Filter:</span>
      {levels.map(level => {
        const active = activeFilters.has(level);
        return (
          <button
            key={level}
            style={{
              ...styles.chip,
              ...(active ? styles.chipActive : {}),
              ...(active ? { color: LEVEL_COLORS[level] } : {}),
            }}
            onClick={() => onToggle(level)}
          >
            {level}
          </button>
        );
      })}
    </div>
  );
}
