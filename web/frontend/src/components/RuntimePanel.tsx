import { ChevronDown, ChevronRight, Zap } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import type { ApiCategory, Platform, RuntimeApi } from '../types';
import { categoryColors } from '../theme';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    flexDirection: 'column',
    height: '100%',
    overflow: 'auto',
    padding: '12px 16px',
  },
  sectionTitle: {
    fontSize: 11,
    fontWeight: 700,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-secondary)',
    letterSpacing: 1.5,
    textTransform: 'uppercase' as const,
    marginBottom: 8,
    display: 'flex',
    alignItems: 'center',
    gap: 6,
  },
  categoryHeader: {
    display: 'flex',
    alignItems: 'center',
    gap: 6,
    padding: '6px 8px',
    marginBottom: 2,
    borderRadius: 4,
    cursor: 'pointer',
    border: 'none',
    background: 'transparent',
    width: '100%',
    textAlign: 'left' as const,
    transition: 'background 0.15s ease',
    fontSize: 12,
    fontWeight: 600,
    fontFamily: "'JetBrains Mono', monospace",
  },
  apiGrid: {
    display: 'flex',
    flexWrap: 'wrap' as const,
    gap: 4,
    padding: '4px 0 8px 20px',
  },
};

function apiButtonStyle(color: string, hovered: boolean): CSSProperties {
  return {
    padding: '4px 10px',
    borderRadius: 4,
    border: `1px solid ${hovered ? color : 'var(--border)'}`,
    background: hovered ? `${color}15` : 'var(--bg-secondary)',
    color: hovered ? color : 'var(--text-primary)',
    fontSize: 11,
    fontFamily: "'JetBrains Mono', monospace",
    cursor: 'pointer',
    transition: 'all 0.15s ease',
    whiteSpace: 'nowrap' as const,
    boxShadow: hovered ? `0 0 12px ${color}20` : 'none',
  };
}

function ApiButton({ api, color, onInsert }: {
  api: RuntimeApi;
  color: string;
  onInsert: (snippet: string) => void;
}) {
  const [hovered, setHovered] = useState(false);
  return (
    <button
      style={apiButtonStyle(color, hovered)}
      title={api.description}
      onClick={() => onInsert(api.snippet)}
      onMouseEnter={() => setHovered(true)}
      onMouseLeave={() => setHovered(false)}
    >
      {api.name}
    </button>
  );
}

function Category({ category, onInsert }: {
  category: ApiCategory;
  onInsert: (snippet: string) => void;
}) {
  const [open, setOpen] = useState(false);
  const [hovered, setHovered] = useState(false);
  const color = categoryColors[category.name] || 'var(--accent-blue)';

  return (
    <div>
      <button
        style={{
          ...styles.categoryHeader,
          color,
          background: hovered ? 'var(--bg-hover)' : 'transparent',
        }}
        onClick={() => setOpen(!open)}
        onMouseEnter={() => setHovered(true)}
        onMouseLeave={() => setHovered(false)}
      >
        {open ? <ChevronDown size={14} /> : <ChevronRight size={14} />}
        {category.name}
        <span style={{
          marginLeft: 'auto',
          fontSize: 10,
          color: 'var(--text-secondary)',
          fontWeight: 400,
        }}>
          {category.apis.length}
        </span>
      </button>
      {open && (
        <div style={styles.apiGrid}>
          {category.apis.map((api) => (
            <ApiButton key={api.name} api={api} color={color} onInsert={onInsert} />
          ))}
        </div>
      )}
    </div>
  );
}

interface RuntimePanelProps {
  categories: ApiCategory[];
  platform: Platform;
  onInsert: (snippet: string) => void;
}

export default function RuntimePanel({ categories, platform, onInsert }: RuntimePanelProps) {
  const filtered = categories.filter(
    (c) => c.platform === 'both' || c.platform === platform,
  );

  return (
    <div style={styles.container}>
      <div style={styles.sectionTitle}>
        <Zap size={12} />
        Runtime APIs
      </div>
      {filtered.map((category) => (
        <Category key={category.name} category={category} onInsert={onInsert} />
      ))}
    </div>
  );
}
