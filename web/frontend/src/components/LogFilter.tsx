import { Filter, X } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import type { LogEntry } from '../types';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    alignItems: 'center',
    gap: 8,
    padding: '6px 16px',
    borderBottom: '1px solid var(--border)',
    background: 'var(--bg-secondary)',
    fontSize: 11,
  },
  label: {
    color: 'var(--text-secondary)',
    fontWeight: 600,
  },
  chip: {
    display: 'inline-flex',
    alignItems: 'center',
    gap: 4,
    padding: '2px 8px',
    borderRadius: 3,
    background: 'var(--bg-primary)',
    border: '1px solid var(--border)',
    cursor: 'pointer',
    transition: 'all 0.15s ease',
  },
  chipActive: {
    background: 'var(--accent-blue)',
    border: '1px solid var(--accent-blue)',
    color: 'white',
  },
  clearBtn: {
    background: 'none',
    border: 'none',
    color: 'var(--text-secondary)',
    cursor: 'pointer',
    padding: 2,
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
