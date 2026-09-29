import { ChevronDown, ChevronRight, Zap, Search, X } from 'lucide-react';
import { useState, useMemo } from 'react';
import type { CSSProperties } from 'react';
import type { ApiCategory, Platform, RuntimeApi } from '../types';
import { categoryColors } from '../theme';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    flexDirection: 'column',
    height: '100%',
    padding: '12px 16px',
  },
  searchBox: {
    display: 'flex',
    alignItems: 'center',
    gap: 6,
    padding: '6px 10px',
    borderRadius: 4,
    border: '1px solid var(--border)',
    background: 'var(--bg-secondary)',
    marginBottom: 12,
  },
  searchInput: {
    flex: 1,
    background: 'transparent',
    border: 'none',
    color: 'var(--text-primary)',
    fontSize: 12,
    fontFamily: "'JetBrains Mono', monospace",
    outline: 'none',
  },
  listContainer: {
    flex: 1,
    overflow: 'auto',
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
  const [search, setSearch] = useState('');

  const filtered = useMemo(() => {
    let result = categories.filter(
      (c) => c.platform === 'both' || c.platform === platform,
    );

    if (search.trim()) {
      const query = search.toLowerCase();
      result = result
        .map((cat) => ({
          ...cat,
          apis: cat.apis.filter(
            (api) =>
              api.name.toLowerCase().includes(query) ||
              api.description.toLowerCase().includes(query),
          ),
        }))
        .filter((cat) => cat.apis.length > 0);
    }

    return result;
  }, [categories, platform, search]);

  const totalApis = filtered.reduce((sum, cat) => sum + cat.apis.length, 0);

  return (
    <div style={styles.container}>
      <div style={styles.sectionTitle}>
        <Zap size={12} />
        Runtime APIs
      </div>
      <div style={styles.searchBox}>
        <Search size={12} color="var(--text-secondary)" />
        <input
          style={styles.searchInput}
          type="text"
          placeholder="Search APIs..."
          value={search}
          onChange={(e) => setSearch(e.target.value)}
        />
        {search && (
          <button
            onClick={() => setSearch('')}
            style={{
              background: 'none',
              border: 'none',
              color: 'var(--text-secondary)',
              cursor: 'pointer',
              padding: 0,
            }}
          >
            <X size={12} />
          </button>
        )}
      </div>
      {search && (
        <div
          style={{
            fontSize: 10,
            color: 'var(--text-secondary)',
            marginBottom: 8,
            textAlign: 'center' as const,
          }}
        >
          {totalApis} API{totalApis !== 1 ? 's' : ''} found
        </div>
      )}
      <div style={styles.listContainer}>
        {filtered.length === 0 ? (
          <div
            style={{
              textAlign: 'center' as const,
              color: 'var(--text-secondary)',
              fontSize: 11,
              padding: '20px 0',
            }}
          >
            {search ? 'No APIs found' : 'No APIs for this platform'}
          </div>
        ) : (
          filtered.map((category) => (
            <Category key={category.name} category={category} onInsert={onInsert} />
          ))
        )}
      </div>
    </div>
  );
}
