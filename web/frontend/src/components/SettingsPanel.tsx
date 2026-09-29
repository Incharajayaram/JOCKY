import { Settings, X } from 'lucide-react';
import { useState } from 'react';
import type { CSSProperties } from 'react';
import { useConfig } from '../hooks/useConfig';

const styles: Record<string, CSSProperties> = {
  overlay: {
    position: 'fixed',
    top: 0,
    left: 0,
    right: 0,
    bottom: 0,
    background: 'rgba(0, 0, 0, 0.5)',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    zIndex: 1000,
  },
  modal: {
    background: 'var(--bg-primary)',
    border: '1px solid var(--border)',
    borderRadius: 8,
    maxWidth: 500,
    maxHeight: '80vh',
    overflow: 'auto',
    padding: 24,
    position: 'relative',
  },
  closeBtn: {
    position: 'absolute',
    top: 16,
    right: 16,
    background: 'none',
    border: 'none',
    color: 'var(--text-secondary)',
    cursor: 'pointer',
    padding: 4,
    display: 'flex',
  },
  title: {
    fontSize: 18,
    fontWeight: 600,
    color: 'var(--text-primary)',
    marginBottom: 24,
    paddingRight: 32,
    display: 'flex',
    alignItems: 'center',
    gap: 8,
  },
  section: {
    marginBottom: 24,
  },
  sectionTitle: {
    fontSize: 12,
    fontWeight: 600,
    color: 'var(--text-primary)',
    textTransform: 'uppercase',
    marginBottom: 12,
    letterSpacing: 0.5,
  },
  item: {
    display: 'flex',
    justifyContent: 'space-between',
    alignItems: 'center',
    padding: '8px 0',
    borderBottom: '1px solid var(--border)',
    fontSize: 12,
  },
  label: {
    color: 'var(--text-secondary)',
  },
  value: {
    color: 'var(--text-primary)',
    fontFamily: "'JetBrains Mono', monospace",
    fontSize: 11,
  },
  badge: {
    display: 'inline-block',
    padding: '2px 8px',
    borderRadius: 3,
    fontSize: 10,
    fontWeight: 600,
  },
};

interface SettingsPanelProps {
  isOpen: boolean;
  onClose: () => void;
}

export default function SettingsPanel({ isOpen, onClose }: SettingsPanelProps) {
  const { config } = useConfig();

  if (!isOpen || !config) return null;

  return (
    <div style={styles.overlay} onClick={onClose}>
      <div style={styles.modal} onClick={(e) => e.stopPropagation()}>
        <button style={styles.closeBtn} onClick={onClose}>
          <X size={20} />
        </button>

        <div style={styles.title}>
          <Settings size={18} />
          Settings & Configuration
        </div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Backend Information</div>
          <div style={styles.item}>
            <span style={styles.label}>Environment</span>
            <span style={{...styles.badge, background: config.environment === 'production' ? 'var(--accent-red)' : 'var(--accent-green)', color: 'white'}}>
              {config.environment}
            </span>
          </div>
          <div style={styles.item}>
            <span style={styles.label}>API Version</span>
            <span style={styles.value}>{config.api_version}</span>
          </div>
          <div style={styles.item}>
            <span style={styles.label}>Backend Host</span>
            <span style={styles.value}>{config.backend_host}:{config.backend_port}</span>
          </div>
          <div style={styles.item}>
            <span style={styles.label}>Max Compilation Time</span>
            <span style={styles.value}>{config.max_compilation_time}s</span>
          </div>
        </div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Platform Support</div>
          <div style={styles.item}>
            <span style={styles.label}>Supported Platforms</span>
            <span style={styles.value}>{config.supported_platforms.join(', ')}</span>
          </div>
          <div style={styles.item}>
            <span style={styles.label}>Obfuscation Presets</span>
            <span style={styles.value}>{config.obfuscation_presets.join(', ')}</span>
          </div>
        </div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Resources</div>
          <div style={styles.item}>
            <span style={styles.label}>API Documentation</span>
            <a href={config.docs_url} target="_blank" rel="noopener noreferrer" style={{color: 'var(--accent-blue)', textDecoration: 'none', fontSize: 11}}>
              {config.docs_url}
            </a>
          </div>
        </div>
      </div>
    </div>
  );
}
