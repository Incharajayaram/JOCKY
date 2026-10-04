import { Play, Loader2 } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';

const baseStyle: CSSProperties = {
  display: 'flex',
  alignItems: 'center',
  gap: 10,
  padding: '10px 28px',
  borderRadius: 0,
  border: '2px solid var(--accent-orange)',
  background: 'rgba(255, 149, 0, 0.1)',
  color: 'var(--accent-orange)',
  fontSize: 15,
  fontWeight: 600,
  fontFamily: "'JetBrains Mono', monospace",
  cursor: 'pointer',
  transition: 'all 0.3s cubic-bezier(0.4, 0, 0.2, 1)',
  letterSpacing: 0.5,
  clipPath: 'polygon(8% 0%, 100% 0%, 92% 100%, 0% 100%)',
  pointerEvents: 'auto',
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
          background: 'rgba(255, 149, 0, 0.25)',
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
