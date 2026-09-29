import { Monitor, Terminal } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import type { Platform } from '../types';

const styles: Record<string, CSSProperties> = {
  container: {
    display: 'flex',
    gap: 0,
    background: 'var(--bg-secondary)',
    borderBottom: '1px solid var(--border)',
    paddingLeft: 24,
    flexShrink: 0,
  },
};

function tabStyle(active: boolean, hovered: boolean): CSSProperties {
  return {
    display: 'flex',
    alignItems: 'center',
    gap: 8,
    padding: '10px 24px',
    fontSize: 13,
    fontWeight: 600,
    fontFamily: "'JetBrains Mono', monospace",
    letterSpacing: 0.5,
    cursor: 'pointer',
    border: 'none',
    borderBottom: active ? '2px solid var(--accent-blue)' : '2px solid transparent',
    background: active
      ? 'rgba(59, 130, 246, 0.08)'
      : hovered
        ? 'rgba(255, 255, 255, 0.03)'
        : 'transparent',
    color: active ? 'var(--accent-blue)' : 'var(--text-secondary)',
    transition: 'all 0.15s ease',
  };
}

interface PlatformTabsProps {
  active: Platform;
  onChange: (p: Platform) => void;
}

function Tab({ label, icon, active, onClick }: {
  label: string;
  icon: React.ReactNode;
  active: boolean;
  onClick: () => void;
}) {
  const [hovered, setHovered] = useState(false);
  return (
    <button
      style={tabStyle(active, hovered)}
      onClick={onClick}
      onMouseEnter={() => setHovered(true)}
      onMouseLeave={() => setHovered(false)}
    >
      {icon}
      {label}
    </button>
  );
}

export default function PlatformTabs({ active, onChange }: PlatformTabsProps) {
  return (
    <div style={styles.container}>
      <Tab
        label="Windows"
        icon={<Monitor size={15} />}
        active={active === 'windows'}
        onClick={() => onChange('windows')}
      />
      <Tab
        label="Linux"
        icon={<Terminal size={15} />}
        active={active === 'linux'}
        onClick={() => onChange('linux')}
      />
    </div>
  );
}
