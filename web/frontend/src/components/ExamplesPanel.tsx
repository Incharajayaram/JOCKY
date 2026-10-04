import { BookOpen, ChevronDown } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import { useExamples } from '../hooks/useExamples';
import type { Platform } from '../types';

const styles: Record<string, CSSProperties> = {
  container: {
    background: 'var(--bg-secondary)',
    border: '1px solid var(--border)',
    borderRadius: 0,
    marginBottom: 10,
    clipPath: 'polygon(2% 0%, 100% 0%, 98% 100%, 0% 100%)',
  },
  header: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    padding: '10px 14px',
    cursor: 'pointer',
    userSelect: 'none',
    fontSize: 13,
    fontWeight: 600,
    color: 'var(--text-secondary)',
  },
  content: {
    maxHeight: 200,
    overflow: 'auto',
    borderTop: '1px solid var(--border)',
  },
  example: {
    padding: '10px 14px',
    borderBottom: '1px solid var(--border)',
    cursor: 'pointer',
    transition: 'background 0.15s ease',
    fontSize: 13,
  },
  exampleHovered: {
    background: 'rgba(255, 255, 255, 0.05)',
  },
  exampleName: {
    color: 'var(--text-primary)',
    fontWeight: 500,
    marginBottom: 3,
  },
  exampleDesc: {
    color: 'var(--text-secondary)',
    fontSize: 12,
  },
};

interface ExamplesPanelProps {
  platform: Platform;
  onLoadExample: (source: string) => void;
}

export default function ExamplesPanel({ platform, onLoadExample }: ExamplesPanelProps) {
  const { examples, loading } = useExamples();
  const [expanded, setExpanded] = useState(false);
  const [hoveredId, setHoveredId] = useState<string | null>(null);

  const filtered = examples.filter(ex => ex.platform === platform);

  return (
    <div style={styles.container}>
      <div style={styles.header} onClick={() => setExpanded(!expanded)}>
        <div style={{ display: 'flex', alignItems: 'center', gap: 4 }}>
          <BookOpen size={12} />
          Examples {filtered.length > 0 && `(${filtered.length})`}
        </div>
        <ChevronDown size={12} style={{ transform: expanded ? 'rotate(180deg)' : 'none', transition: 'transform 0.2s' }} />
      </div>
      {expanded && (
        <div style={styles.content}>
          {loading ? (
            <div style={{ padding: '10px 14px', fontSize: 13, color: 'var(--text-secondary)' }}>Loading...</div>
          ) : filtered.length === 0 ? (
            <div style={{ padding: '10px 14px', fontSize: 13, color: 'var(--text-secondary)' }}>No examples for {platform}</div>
          ) : (
            filtered.map((ex) => (
              <div
                key={ex.id}
                style={{
                  ...styles.example,
                  ...(hoveredId === ex.id ? styles.exampleHovered : {}),
                }}
                onClick={() => onLoadExample(ex.source)}
                onMouseEnter={() => setHoveredId(ex.id)}
                onMouseLeave={() => setHoveredId(null)}
              >
                <div style={styles.exampleName}>{ex.name}</div>
                <div style={styles.exampleDesc}>{ex.description}</div>
              </div>
            ))
          )}
        </div>
      )}
    </div>
  );
}
