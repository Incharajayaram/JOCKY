import { X } from 'lucide-react';
import type { CSSProperties } from 'react';

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
    maxWidth: 600,
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
    fontSize: 20,
    fontWeight: 600,
    color: 'var(--text-primary)',
    marginBottom: 16,
    paddingRight: 32,
  },
  section: {
    marginBottom: 24,
  },
  sectionTitle: {
    fontSize: 14,
    fontWeight: 600,
    color: 'var(--text-primary)',
    marginBottom: 8,
    borderBottom: '1px solid var(--border)',
    paddingBottom: 4,
  },
  sectionContent: {
    fontSize: 12,
    lineHeight: 1.6,
    color: 'var(--text-secondary)',
  },
  code: {
    background: 'var(--bg-secondary)',
    padding: '4px 8px',
    borderRadius: 3,
    fontFamily: "'JetBrains Mono', monospace",
    fontSize: 11,
    display: 'inline-block',
  },
  list: {
    marginLeft: 16,
    lineHeight: 1.8,
  },
};

interface HelpPanelProps {
  isOpen: boolean;
  onClose: () => void;
}

export default function HelpPanel({ isOpen, onClose }: HelpPanelProps) {
  if (!isOpen) return null;

  return (
    <div style={styles.overlay} onClick={onClose}>
      <div style={styles.modal} onClick={(e) => e.stopPropagation()}>
        <button style={styles.closeBtn} onClick={onClose}>
          <X size={20} />
        </button>

        <div style={styles.title}>JOCKY Web Platform Help</div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Getting Started</div>
          <div style={styles.sectionContent}>
            <ol style={styles.list}>
              <li>Select a platform (Windows or Linux) using the tabs</li>
              <li>Write or paste your JOCKY source code in the editor</li>
              <li>Configure obfuscation passes on the right panel</li>
              <li>Click "Compile" to build your binary</li>
              <li>Download the compiled artifact when complete</li>
            </ol>
          </div>
        </div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Obfuscation Presets</div>
          <div style={styles.sectionContent}>
            <ul style={styles.list}>
              <li><strong>None:</strong> No obfuscation applied</li>
              <li><strong>Light:</strong> Basic string encryption and symbol obfuscation</li>
              <li><strong>Standard:</strong> Balanced obfuscation with code flattening</li>
              <li><strong>Aggressive:</strong> Maximum obfuscation with all passes enabled</li>
            </ul>
          </div>
        </div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Platform Differences</div>
          <div style={styles.sectionContent}>
            <p style={{ marginBottom: 8 }}>
              <strong>Windows:</strong> Produces .exe binaries with WinAPI runtime support
            </p>
            <p>
              <strong>Linux:</strong> Produces ELF binaries with POSIX runtime support
            </p>
          </div>
        </div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Keyboard Shortcuts</div>
          <div style={styles.sectionContent}>
            <ul style={styles.list}>
              <li><span style={styles.code}>Ctrl+Enter</span> - Start compilation</li>
              <li><span style={styles.code}>Ctrl+K</span> - Clear logs</li>
              <li><span style={styles.code}>Ctrl+L</span> - Load example</li>
            </ul>
          </div>
        </div>

        <div style={styles.section}>
          <div style={styles.sectionTitle}>Documentation</div>
          <div style={styles.sectionContent}>
            For detailed information, visit the API documentation at{' '}
            <span style={styles.code}>/docs</span>
          </div>
        </div>
      </div>
    </div>
  );
}
