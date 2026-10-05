import { useCallback, useEffect, useRef, useState } from 'react';
import type { CSSProperties, ReactNode } from 'react';
import { X, Shield, Cpu, FileText, RefreshCw, ChevronDown, ChevronRight } from 'lucide-react';

interface PassInfo {
  id: string;
  name: string;
  description: string;
}

interface ApiInfo {
  name: string;
  description: string;
  category: string;
  platform: string;
}

interface ReportData {
  job_id: string;
  platform: string;
  status: string;
  binary_name: string | null;
  binary_size: number | null;
  runtime_apis: ApiInfo[];
  obfuscation_passes: { mlir: PassInfo[]; llvm: PassInfo[] };
  behavior_summary: string;
}

interface ReportPanelProps {
  jobId: string;
  onClose: () => void;
}

const overlay: CSSProperties = {
  position: 'fixed',
  inset: 0,
  background: 'rgba(0,0,0,0.75)',
  zIndex: 1000,
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'center',
  backdropFilter: 'blur(4px)',
};

const modal: CSSProperties = {
  background: 'var(--bg-secondary)',
  border: '1px solid var(--border)',
  width: 'min(900px, 95vw)',
  maxHeight: '88vh',
  display: 'flex',
  flexDirection: 'column',
  fontFamily: "'JetBrains Mono', monospace",
  clipPath: 'polygon(0 0, calc(100% - 20px) 0, 100% 20px, 100% 100%, 20px 100%, 0 calc(100% - 20px))',
};

const modalHeader: CSSProperties = {
  display: 'flex',
  alignItems: 'center',
  justifyContent: 'space-between',
  padding: '14px 20px',
  borderBottom: '1px solid var(--border)',
  background: 'rgba(0,0,0,0.3)',
  flexShrink: 0,
};

const sectionHeader: CSSProperties = {
  display: 'flex',
  alignItems: 'center',
  gap: 8,
  padding: '10px 16px',
  background: 'rgba(255,255,255,0.03)',
  borderBottom: '1px solid var(--border)',
  fontSize: 12,
  fontWeight: 600,
  color: 'var(--accent-orange)',
  letterSpacing: 0.5,
  cursor: 'pointer',
  userSelect: 'none' as const,
};

const pill = (color: string): CSSProperties => ({
  display: 'inline-block',
  padding: '2px 8px',
  fontSize: 10,
  borderRadius: 0,
  border: `1px solid ${color}`,
  color: color,
  letterSpacing: 0.5,
  clipPath: 'polygon(6% 0%, 100% 0%, 94% 100%, 0% 100%)',
});

function formatBytes(bytes: number): string {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(1)} KB`;
  return `${(bytes / (1024 * 1024)).toFixed(2)} MB`;
}

function groupByCategory(apis: ApiInfo[]): Record<string, ApiInfo[]> {
  return apis.reduce((acc, api) => {
    if (!acc[api.category]) acc[api.category] = [];
    acc[api.category].push(api);
    return acc;
  }, {} as Record<string, ApiInfo[]>);
}

function CollapsibleSection({ title, icon, children, defaultOpen = true }: {
  title: string;
  icon: ReactNode;
  children: ReactNode;
  defaultOpen?: boolean;
}) {
  const [open, setOpen] = useState(defaultOpen);
  return (
    <div>
      <div style={sectionHeader} onClick={() => setOpen(o => !o)}>
        {icon}
        {title}
        <span style={{ marginLeft: 'auto' }}>
          {open ? <ChevronDown size={12} /> : <ChevronRight size={12} />}
        </span>
      </div>
      {open && children}
    </div>
  );
}

export default function ReportPanel({ jobId, onClose }: ReportPanelProps) {
  const [report, setReport] = useState<ReportData | null>(null);
  const [error, setError] = useState<string | null>(null);
  const pollRef = useRef<ReturnType<typeof setTimeout> | null>(null);

  const fetchReport = useCallback(async () => {
    try {
      const res = await fetch(`/api/jobs/${jobId}/report`);
      if (!res.ok) { setError('Failed to load report'); return; }
      const data: ReportData = await res.json();
      setReport(data);
    } catch {
      setError('Failed to load report');
    }
  }, [jobId]);

  useEffect(() => {
    fetchReport();
    return () => { if (pollRef.current) clearTimeout(pollRef.current); };
  }, [fetchReport]);

  const apisByCategory = report ? groupByCategory(report.runtime_apis) : {};
  const totalPasses = report
    ? report.obfuscation_passes.mlir.length + report.obfuscation_passes.llvm.length
    : 0;

  return (
    <div style={overlay} onClick={e => { if (e.target === e.currentTarget) onClose(); }}>
      <div style={modal}>
        {/* Header */}
        <div style={modalHeader}>
          <div style={{ display: 'flex', alignItems: 'center', gap: 10, fontSize: 13, fontWeight: 700, color: 'var(--text-primary)' }}>
            <FileText size={14} color="var(--accent-orange)" />
            Compilation Report
            {report && (
              <span style={{ fontSize: 11, fontWeight: 400, color: 'var(--text-secondary)' }}>
                — {report.binary_name ?? 'unknown'}{report.binary_size != null ? ` (${formatBytes(report.binary_size)})` : ''}
              </span>
            )}
          </div>
          <button
            onClick={onClose}
            style={{ background: 'none', border: 'none', color: 'var(--text-secondary)', cursor: 'pointer', padding: 4, display: 'flex' }}
          >
            <X size={16} />
          </button>
        </div>

        {/* Body */}
        <div style={{ overflowY: 'auto', flex: 1 }}>
          {error && (
            <div style={{ padding: 24, color: 'var(--accent-red, #ef4444)', fontSize: 13 }}>{error}</div>
          )}

          {!report && !error && (
            <div style={{ padding: 24, color: 'var(--text-secondary)', fontSize: 13, display: 'flex', alignItems: 'center', gap: 8 }}>
              <RefreshCw size={13} style={{ animation: 'spin 1s linear infinite' }} />
              Loading report...
            </div>
          )}

          {report && (
            <>
              {/* Summary stats */}
              <div style={{ display: 'flex', gap: 12, padding: '12px 16px', borderBottom: '1px solid var(--border)', flexWrap: 'wrap' as const }}>
                <div style={pill('#60a5fa')}>{report.platform.toUpperCase()}</div>
                <div style={pill('var(--accent-orange)')}>{totalPasses} obfuscation passes</div>
                <div style={pill('#22c55e')}>{report.runtime_apis.length} runtime APIs</div>
                <div style={pill(Object.keys(apisByCategory).length > 0 ? '#a78bfa' : 'var(--text-secondary)')}>
                  {Object.keys(apisByCategory).length} capability {Object.keys(apisByCategory).length === 1 ? 'category' : 'categories'}
                </div>
              </div>

              {/* Behavior Summary */}
              <CollapsibleSection title="BEHAVIOR SUMMARY" icon={<Shield size={12} />}>
                <div style={{ padding: '14px 20px', fontSize: 12, lineHeight: 1.8, color: 'var(--text-primary)', whiteSpace: 'pre-wrap' as const }}>
                  {report.behavior_summary || 'No behavior data available.'}
                </div>
              </CollapsibleSection>

              {/* Obfuscation Passes */}
              <CollapsibleSection title="OBFUSCATION PASSES" icon={<Shield size={12} />}>
                <div style={{ padding: '10px 16px 16px' }}>
                  {report.obfuscation_passes.mlir.length > 0 && (
                    <>
                      <div style={{ fontSize: 10, color: 'var(--text-secondary)', letterSpacing: 1, marginBottom: 8, marginTop: 4 }}>MLIR PASSES</div>
                      <div style={{ display: 'flex', flexDirection: 'column', gap: 4 }}>
                        {report.obfuscation_passes.mlir.map(p => (
                          <div key={p.id} style={{ display: 'flex', gap: 10, fontSize: 12, alignItems: 'flex-start' }}>
                            <span style={{ color: 'var(--accent-orange)', minWidth: 160 }}>{p.name}</span>
                            <span style={{ color: 'var(--text-secondary)' }}>{p.description}</span>
                          </div>
                        ))}
                      </div>
                    </>
                  )}
                  {report.obfuscation_passes.llvm.length > 0 && (
                    <>
                      <div style={{ fontSize: 10, color: 'var(--text-secondary)', letterSpacing: 1, margin: '12px 0 8px' }}>LLVM PASSES</div>
                      <div style={{ display: 'flex', flexDirection: 'column', gap: 4 }}>
                        {report.obfuscation_passes.llvm.map(p => (
                          <div key={p.id} style={{ display: 'flex', gap: 10, fontSize: 12, alignItems: 'flex-start' }}>
                            <span style={{ color: '#60a5fa', minWidth: 160 }}>{p.name}</span>
                            <span style={{ color: 'var(--text-secondary)' }}>{p.description}</span>
                          </div>
                        ))}
                      </div>
                    </>
                  )}
                  {totalPasses === 0 && (
                    <div style={{ color: 'var(--text-secondary)', fontSize: 12 }}>No obfuscation passes enabled.</div>
                  )}
                </div>
              </CollapsibleSection>

              {/* Runtime APIs */}
              <CollapsibleSection title={`RUNTIME APIs (${report.runtime_apis.length})`} icon={<Cpu size={12} />}>
                <div style={{ padding: '10px 16px 16px' }}>
                  {report.runtime_apis.length === 0 ? (
                    <div style={{ color: 'var(--text-secondary)', fontSize: 12 }}>No runtime APIs detected in source.</div>
                  ) : (
                    Object.entries(apisByCategory).map(([cat, apis]) => (
                      <div key={cat} style={{ marginBottom: 14 }}>
                        <div style={{ fontSize: 10, color: '#a78bfa', letterSpacing: 1, marginBottom: 6 }}>{cat.toUpperCase()}</div>
                        <div style={{ display: 'flex', flexDirection: 'column', gap: 4 }}>
                          {apis.map(api => (
                            <div key={api.name} style={{ display: 'flex', gap: 10, fontSize: 12, alignItems: 'flex-start' }}>
                              <span style={{ color: '#22c55e', minWidth: 250, fontFamily: 'monospace' }}>{api.name}</span>
                              <span style={{ color: 'var(--text-secondary)' }}>{api.description}</span>
                            </div>
                          ))}
                        </div>
                      </div>
                    ))
                  )}
                </div>
              </CollapsibleSection>

            </>
          )}
        </div>
      </div>

      <style>{`@keyframes spin { from { transform: rotate(0deg); } to { transform: rotate(360deg); } }`}</style>
    </div>
  );
}
