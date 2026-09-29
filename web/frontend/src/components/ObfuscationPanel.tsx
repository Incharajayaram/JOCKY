import { Shield } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import type { ObfuscationPass } from '../types';

const styles: Record<string, CSSProperties> = {
  container: {
    padding: '12px 16px',
    overflow: 'auto',
    height: '100%',
    display: 'flex',
    flexDirection: 'column',
  },
  sectionTitle: {
    fontSize: 11,
    fontWeight: 700,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-secondary)',
    letterSpacing: 1.5,
    textTransform: 'uppercase' as const,
    marginBottom: 12,
    display: 'flex',
    alignItems: 'center',
    gap: 6,
  },
  subTitle: {
    fontSize: 10,
    fontWeight: 700,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--accent-purple)',
    letterSpacing: 1,
    marginBottom: 6,
    marginTop: 10,
  },
  row: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    padding: '5px 8px',
    borderRadius: 4,
    marginBottom: 2,
    transition: 'background 0.15s ease',
  },
  label: {
    fontSize: 12,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-primary)',
  },
  description: {
    fontSize: 10,
    color: 'var(--text-secondary)',
    marginTop: 1,
  },
};

function switchTrackStyle(enabled: boolean): CSSProperties {
  return {
    width: 36,
    height: 20,
    borderRadius: 10,
    background: enabled ? 'var(--accent-purple)' : 'var(--bg-hover)',
    border: `1px solid ${enabled ? 'var(--accent-purple)' : 'var(--border)'}`,
    position: 'relative' as const,
    cursor: 'pointer',
    transition: 'all 0.2s ease',
    flexShrink: 0,
    boxShadow: enabled ? 'var(--glow-purple)' : 'none',
  };
}

function switchKnobStyle(enabled: boolean): CSSProperties {
  return {
    width: 14,
    height: 14,
    borderRadius: '50%',
    background: enabled ? '#fff' : 'var(--text-secondary)',
    position: 'absolute' as const,
    top: 2,
    left: enabled ? 19 : 2,
    transition: 'all 0.2s ease',
  };
}

const passImpact: Record<string, { level: 'Light' | 'Medium' | 'High'; cost: string; color: string }> = {
  // MLIR passes
  'string-encrypt': { level: 'Light', cost: '+10-15%', color: '#10b981' },
  'constant-obfuscate': { level: 'Medium', cost: '+20-30%', color: '#f59e0b' },
  'symbol-obfuscate': { level: 'Light', cost: '+5-10%', color: '#10b981' },
  'crypto-hash': { level: 'High', cost: '+40-50%', color: '#ef4444' },
  'scf-obfuscate': { level: 'Medium', cost: '+25-35%', color: '#f59e0b' },
  'import-obfuscate': { level: 'Light', cost: '+8-12%', color: '#10b981' },
  // LLVM passes
  'strip-signature': { level: 'Light', cost: '+2-5%', color: '#10b981' },
  'pdata-strip': { level: 'Light', cost: '+1-3%', color: '#10b981' },
  'virtualize': { level: 'High', cost: '+80-120%', color: '#ef4444' },
  'opaque-pred': { level: 'Medium', cost: '+30-40%', color: '#f59e0b' },
  'substitution': { level: 'Medium', cost: '+35-50%', color: '#f59e0b' },
  'boguscf': { level: 'High', cost: '+50-70%', color: '#ef4444' },
  'flattening': { level: 'High', cost: '+60-80%', color: '#ef4444' },
  'linear-mba': { level: 'High', cost: '+70-100%', color: '#ef4444' },
  'anti-debug': { level: 'Medium', cost: '+15-25%', color: '#f59e0b' },
  'indirect-call': { level: 'Medium', cost: '+20-30%', color: '#f59e0b' },
};

function Toggle({ pass, onToggle }: {
  pass: ObfuscationPass;
  onToggle: () => void;
}) {
  const [hovered, setHovered] = useState(false);
  const impact = passImpact[pass.id] || { level: 'Medium', cost: '+25%', color: '#f59e0b' };

  return (
    <div
      style={{
        ...styles.row,
        background: hovered ? 'var(--bg-hover)' : 'transparent',
      }}
      onMouseEnter={() => setHovered(true)}
      onMouseLeave={() => setHovered(false)}
    >
      <div style={{ flex: 1 }}>
        <div style={{ display: 'flex', alignItems: 'center', gap: 8 }}>
          <div style={styles.label}>{pass.name}</div>
          <div
            style={{
              fontSize: 9,
              fontWeight: 600,
              padding: '2px 6px',
              borderRadius: 3,
              background: `${impact.color}20`,
              color: impact.color,
              fontFamily: "'JetBrains Mono', monospace",
            }}
          >
            {impact.level}
          </div>
          <div
            style={{
              fontSize: 9,
              color: 'var(--text-secondary)',
              fontFamily: "'JetBrains Mono', monospace",
            }}
          >
            {impact.cost}
          </div>
        </div>
        <div style={styles.description}>{pass.description}</div>
      </div>
      <div style={switchTrackStyle(pass.enabled)} onClick={onToggle}>
        <div style={switchKnobStyle(pass.enabled)} />
      </div>
    </div>
  );
}

interface ObfuscationPanelProps {
  mlir: ObfuscationPass[];
  llvm: ObfuscationPass[];
  onToggle: (type: 'mlir' | 'llvm', id: string) => void;
}

export default function ObfuscationPanel({ mlir, llvm, onToggle }: ObfuscationPanelProps) {
  return (
    <div style={styles.container}>
      <div style={styles.sectionTitle}>
        <Shield size={12} />
        Obfuscation Passes
      </div>
      <div style={styles.subTitle}>MLIR PASSES</div>
      {mlir.map((pass) => (
        <Toggle key={pass.id} pass={pass} onToggle={() => onToggle('mlir', pass.id)} />
      ))}
      <div style={styles.subTitle}>LLVM PASSES</div>
      {llvm.map((pass) => (
        <Toggle key={pass.id} pass={pass} onToggle={() => onToggle('llvm', pass.id)} />
      ))}
    </div>
  );
}
