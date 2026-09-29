import { Play, Loader2 } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';

const baseStyle: CSSProperties = {
  display: 'flex',
  alignItems: 'center',
  gap: 8,
  padding: '8px 20px',
  borderRadius: 6,
  border: '1px solid var(--accent-blue)',
  background: 'rgba(59, 130, 246, 0.1)',
  color: 'var(--accent-blue)',
  fontSize: 14,
  fontWeight: 600,
  fontFamily: "'JetBrains Mono', monospace",
  cursor: 'pointer',
  transition: 'all 0.2s ease',
  letterSpacing: 0.5,
};

interface CompileButtonProps {
  onClick: () => void;
  loading: boolean;
}

export default function CompileButton({ onClick, loading }: CompileButtonProps) {
  const [hovered, setHovered] = useState(false);

  const style: CSSProperties = {
    ...baseStyle,
    ...(hovered && !loading
      ? {
          background: 'rgba(59, 130, 246, 0.2)',
          boxShadow: 'var(--glow-blue)',
        }
      : {}),
    ...(loading
      ? {
          opacity: 0.7,
          cursor: 'not-allowed',
          borderColor: 'var(--text-secondary)',
          color: 'var(--text-secondary)',
        }
      : {}),
  };

  return (
    <button
      style={style}
      onClick={loading ? undefined : onClick}
      onMouseEnter={() => setHovered(true)}
      onMouseLeave={() => setHovered(false)}
    >
      {loading ? (
        <Loader2 size={16} style={{ animation: 'spin 1s linear infinite' }} />
      ) : (
        <Play size={16} />
      )}
      {loading ? 'Compiling...' : 'Compile'}
    </button>
  );
}
