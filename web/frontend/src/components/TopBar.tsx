import { HelpCircle, Settings } from 'lucide-react';
import type { CSSProperties } from 'react';
import { useState } from 'react';
import CompileButton from './CompileButton';
import HelpPanel from './HelpPanel';
import SettingsPanel from './SettingsPanel';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    padding: '14px 28px',
    background: 'var(--bg-secondary)',
    borderBottom: '1px solid var(--border)',
    height: 64,
    flexShrink: 0,
  },
  logo: {
    display: 'flex',
    alignItems: 'center',
    gap: 12,
  },
  title: {
    fontSize: 26,
    fontWeight: 900,
    letterSpacing: 0.5,
    color: 'var(--accent-orange)',
    fontFamily: "'Courier New', 'Courier', monospace",
    textTransform: 'uppercase',
  },
  version: {
    fontSize: 12,
    color: 'var(--text-secondary)',
    marginLeft: 8,
    fontFamily: "'Segoe UI', -apple-system, BlinkMacSystemFont, sans-serif",
    fontWeight: 500,
  },
  rightSection: {
    display: 'flex',
    alignItems: 'center',
    gap: 14,
  },
  iconButton: {
    background: 'none',
    border: 'none',
    color: 'var(--text-secondary)',
    cursor: 'pointer',
    padding: 6,
    display: 'flex',
    transition: 'color 0.15s ease',
  },
};

interface TopBarProps {
  onCompile: () => void;
  compiling: boolean;
}

export default function TopBar({ onCompile, compiling }: TopBarProps) {
  const [helpOpen, setHelpOpen] = useState(false);
  const [settingsOpen, setSettingsOpen] = useState(false);

  return (
    <>
      <div style={styles.container}>
        <div style={styles.logo}>
          <span style={styles.title}>JOCKY</span>
          <span style={styles.version}>v1.0</span>
        </div>
        <div style={styles.rightSection}>
          <button
            style={styles.iconButton}
            onClick={() => setHelpOpen(true)}
            title="Help (?)">
            <HelpCircle size={18} />
          </button>
          <button
            style={styles.iconButton}
            onClick={() => setSettingsOpen(true)}
            title="Settings">
            <Settings size={18} />
          </button>
          <CompileButton onClick={onCompile} loading={compiling} />
        </div>
      </div>
      <HelpPanel isOpen={helpOpen} onClose={() => setHelpOpen(false)} />
      <SettingsPanel isOpen={settingsOpen} onClose={() => setSettingsOpen(false)} />
    </>
  );
}
