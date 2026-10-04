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
    gap: 10,
    padding: '12px 28px',
    fontSize: 15,
    fontWeight: 600,
    fontFamily: "'JetBrains Mono', monospace",
    letterSpacing: 0.5,
    cursor: 'pointer',
    border: 'none',
    background: active
      ? 'rgba(255, 149, 0, 0.1)'
      : hovered
        ? 'rgba(255, 255, 255, 0.05)'
        : 'transparent',
    color: active ? 'var(--accent-orange)' : 'var(--text-secondary)',
    transition: 'all 0.3s cubic-bezier(0.4, 0, 0.2, 1)',
    borderBottom: active ? '3px solid var(--accent-orange)' : '3px solid transparent',
    clipPath: 'polygon(5% 0%, 100% 0%, 95% 100%, 0% 100%)',
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
