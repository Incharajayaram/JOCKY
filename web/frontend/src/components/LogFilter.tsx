import { Filter, X } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import type { LogEntry } from '../types';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    alignItems: 'center',
    gap: 10,
    padding: '10px 20px',
    borderBottom: '1px solid var(--border)',
    background: 'var(--bg-secondary)',
    fontSize: 13,
  },
  label: {
    color: 'var(--text-secondary)',
    fontWeight: 600,
  },
  chip: {
    display: 'inline-flex',
    alignItems: 'center',
    gap: 6,
    padding: '6px 14px',
    borderRadius: 0,
    background: 'var(--bg-primary)',
    border: '1px solid var(--border)',
    cursor: 'pointer',
    transition: 'all 0.25s cubic-bezier(0.4, 0, 0.2, 1)',
    clipPath: 'polygon(8% 0%, 100% 0%, 92% 100%, 0% 100%)',
    pointerEvents: 'auto',
    color: 'var(--text-primary)',
  },
  chipActive: {
    background: 'var(--accent-orange)',
    border: '1px solid var(--accent-orange)',
    color: '#fff',
  },
  clearBtn: {
    background: 'none',
    border: 'none',
    color: 'var(--text-secondary)',
    cursor: 'pointer',
    padding: 4,
    display: 'flex',
    marginLeft: 'auto',
  },
};

interface LogFilterProps {
  logs: LogEntry[];
  onFilterChange: (filtered: LogEntry[]) => void;
}

export default function LogFilter({ logs, onFilterChange }: LogFilterProps) {
  const [activeFilters, setActiveFilters] = useState<Set<string>>(new Set());

  const levels = ['info', 'warn', 'error', 'success'] as const;

  const toggleFilter = (level: string) => {
    const newFilters = new Set(activeFilters);
    if (newFilters.has(level)) {
      newFilters.delete(level);
    } else {
      newFilters.add(level);
    }
    setActiveFilters(newFilters);

    if (newFilters.size === 0) {
      onFilterChange(logs);
    } else {
      onFilterChange(logs.filter(log => newFilters.has(log.level)));
    }
  };

  const clearFilters = () => {
    setActiveFilters(new Set());
    onFilterChange(logs);
  };

  return (
    <div style={styles.container}>
      <Filter size={12} color="var(--text-secondary)" />
      <span style={styles.label}>Filter:</span>
      {levels.map(level => (
        <button
          key={level}
          style={{
            ...styles.chip,
            ...(activeFilters.has(level) ? styles.chipActive : {}),
          }}
          onClick={() => toggleFilter(level)}
        >
          {level}
        </button>
      ))}
      {activeFilters.size > 0 && (
        <button style={styles.clearBtn} onClick={clearFilters} title="Clear filters">
          <X size={12} />
        </button>
      )}
    </div>
  );
}
