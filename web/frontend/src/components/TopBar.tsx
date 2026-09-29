import { Terminal } from 'lucide-react';
import type { CSSProperties } from 'react';
import CompileButton from './CompileButton';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    padding: '12px 24px',
    background: 'var(--bg-secondary)',
    borderBottom: '1px solid var(--border)',
    height: 56,
    flexShrink: 0,
  },
  logo: {
    display: 'flex',
    alignItems: 'center',
    gap: 10,
  },
  title: {
    fontSize: 20,
    fontWeight: 700,
    letterSpacing: 2,
    background: 'linear-gradient(135deg, var(--accent-blue), var(--accent-purple))',
    WebkitBackgroundClip: 'text',
    WebkitTextFillColor: 'transparent',
    fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
  },
  version: {
    fontSize: 11,
    color: 'var(--text-secondary)',
    marginLeft: 4,
    fontFamily: "'JetBrains Mono', monospace",
  },
};

interface TopBarProps {
  onCompile: () => void;
  compiling: boolean;
}

export default function TopBar({ onCompile, compiling }: TopBarProps) {
  return (
    <div style={styles.container}>
      <div style={styles.logo}>
        <Terminal size={22} color="var(--accent-blue)" />
        <span style={styles.title}>JOCKY</span>
        <span style={styles.version}>v1.0</span>
      </div>
      <CompileButton onClick={onCompile} loading={compiling} />
    </div>
  );
}
