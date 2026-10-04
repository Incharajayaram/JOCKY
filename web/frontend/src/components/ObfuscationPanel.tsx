import { Shield } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import type { ObfuscationPass } from '../types';

const styles: Record<string, CSSProperties> = {
  container: {
    padding: '16px 20px',
    overflow: 'auto',
    height: '100%',
    display: 'flex',
    flexDirection: 'column',
  },
  sectionTitle: {
    fontSize: 13,
    fontWeight: 700,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-secondary)',
    letterSpacing: 1.5,
    textTransform: 'uppercase' as const,
    marginBottom: 14,
    display: 'flex',
    alignItems: 'center',
    gap: 8,
  },
  subTitle: {
    fontSize: 12,
    fontWeight: 700,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--accent-orange)',
    letterSpacing: 1,
    marginBottom: 8,
    marginTop: 12,
  },
  row: {
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'space-between',
    padding: '8px 10px',
    borderRadius: 0,
    marginBottom: 3,
    transition: 'background 0.15s ease',
  },
  label: {
    fontSize: 14,
    fontFamily: "'JetBrains Mono', monospace",
    color: 'var(--text-primary)',
  },
  description: {
    fontSize: 12,
    color: 'var(--text-secondary)',
    marginTop: 2,
  },
};

function switchTrackStyle(enabled: boolean): CSSProperties {
  return {
    width: 44,
    height: 24,
    borderRadius: 0,
    background: enabled ? 'var(--accent-orange)' : 'var(--bg-hover)',
    border: `1px solid ${enabled ? 'var(--accent-orange)' : 'var(--border)'}`,
    position: 'relative' as const,
    cursor: 'pointer',
    transition: 'all 0.2s ease',
    flexShrink: 0,
    boxShadow: enabled ? 'var(--glow-blue)' : 'none',
  };
}

function switchKnobStyle(enabled: boolean): CSSProperties {
  return {
    width: 18,
    height: 18,
    borderRadius: 0,
    background: enabled ? '#fff' : 'var(--text-secondary)',
    position: 'absolute' as const,
    top: 2,
    left: enabled ? 23 : 2,
    transition: 'all 0.2s ease',
  };
}

const passImpact: Record<string, { level: 'Light' | 'Medium' | 'High'; cost: string; color: string }> = {
  // MLIR passes
  'string-encrypt': { level: 'Light', cost: '+10-15%', color: '#ffb366' },
  'constant-obfuscate': { level: 'Medium', cost: '+20-30%', color: '#ff9500' },
  'symbol-obfuscate': { level: 'Light', cost: '+5-10%', color: '#ffb366' },
  'crypto-hash': { level: 'High', cost: '+40-50%', color: '#ff7700' },
  'scf-obfuscate': { level: 'Medium', cost: '+25-35%', color: '#ff9500' },
  'import-obfuscate': { level: 'Light', cost: '+8-12%', color: '#ffb366' },
  // LLVM passes
  'strip-signature': { level: 'Light', cost: '+2-5%', color: '#ffb366' },
  'pdata-strip': { level: 'Light', cost: '+1-3%', color: '#ffb366' },
  'virtualize': { level: 'High', cost: '+80-120%', color: '#ff7700' },
  'opaque-pred': { level: 'Medium', cost: '+30-40%', color: '#ff9500' },
  'substitution': { level: 'Medium', cost: '+35-50%', color: '#ff9500' },
  'boguscf': { level: 'High', cost: '+50-70%', color: '#ff7700' },
  'flattening': { level: 'High', cost: '+60-80%', color: '#ff7700' },
  'linear-mba': { level: 'High', cost: '+70-100%', color: '#ff7700' },
  'anti-debug': { level: 'Medium', cost: '+15-25%', color: '#ff9500' },
  'indirect-call': { level: 'Medium', cost: '+20-30%', color: '#ff9500' },
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
