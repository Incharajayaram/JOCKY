import { AlertCircle } from 'lucide-react';
import React from 'react';
import type { CSSProperties } from 'react';

interface Props {
  children: React.ReactNode;
}

interface State {
  hasError: boolean;
  error?: Error;
}

export default class ErrorBoundary extends React.Component<Props, State> {
  constructor(props: Props) {
    super(props);
    this.state = { hasError: false };
  }

  static getDerivedStateFromError(error: Error): State {
    return { hasError: true, error };
  }

  componentDidCatch(error: Error) {
    console.error('[ErrorBoundary] Caught error:', error);
  }

  render() {
    if (this.state.hasError) {
      const containerStyle: CSSProperties = {
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        height: '100%',
        padding: '20px',
        textAlign: 'center',
        flexDirection: 'column',
        gap: 12,
      };

      const messageStyle: CSSProperties = {
        fontSize: 14,
        color: 'var(--accent-red)',
        fontFamily: "'JetBrains Mono', monospace",
      };

      const detailStyle: CSSProperties = {
        fontSize: 12,
        color: 'var(--text-secondary)',
        maxWidth: 400,
        fontFamily: "'JetBrains Mono', monospace",
        wordBreak: 'break-word',
      };

      const buttonStyle: CSSProperties = {
        padding: '8px 16px',
        borderRadius: 4,
        border: '1px solid var(--accent-red)',
        background: 'rgba(239, 68, 68, 0.1)',
        color: 'var(--accent-red)',
        cursor: 'pointer',
        fontSize: 12,
        fontFamily: "'JetBrains Mono', monospace",
        marginTop: 12,
      };

      return (
        <div style={containerStyle}>
          <AlertCircle size={32} color="var(--accent-red)" />
          <div style={messageStyle}>Something went wrong</div>
          {this.state.error && (
            <div style={detailStyle}>{this.state.error.message}</div>
          )}
          <button
            style={buttonStyle}
            onClick={() => window.location.reload()}
          >
            Reload Page
          </button>
        </div>
      );
    }

    return this.props.children;
  }
}
