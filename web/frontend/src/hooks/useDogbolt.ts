import { useCallback, useRef, useState } from 'react';

export interface DecompilerInfo {
  name: string;
  version: string;
}

export interface DecompilerResult {
  id: string;
  decompiler: DecompilerInfo;
  status: string;
  error: string | null;
  analysis_time: number | null;
  download_url: string | null;
  code: string | null;
  fetchedCode: boolean;
}

export type UploadState = 'idle' | 'uploading' | 'polling' | 'done' | 'error';

// Dogbolt API has no `status` field — derive it from download_url / error presence.
function deriveStatus(r: { download_url?: string | null; error?: string | null }): string {
  if (r.download_url) return 'Complete';
  if (r.error) return 'Failed';
  return 'Pending';
}

function compareVersions(a: string, b: string): number {
  const pa = a.split('.').map(Number);
  const pb = b.split('.').map(Number);
  for (let i = 0; i < Math.max(pa.length, pb.length); i++) {
    const diff = (pa[i] || 0) - (pb[i] || 0);
    if (diff !== 0) return diff;
  }
  return 0;
}

export function useDogbolt() {
  const [uploadState, setUploadState] = useState<UploadState>('idle');
  const [binaryId, setBinaryId] = useState<string | null>(null);
  const [results, setResults] = useState<Map<string, DecompilerResult>>(new Map());
  const [error, setError] = useState<string | null>(null);
  const [secondsLeft, setSecondsLeft] = useState(300);
  const [fileName, setFileName] = useState<string | null>(null);

  const pollTimerRef = useRef<ReturnType<typeof setTimeout> | null>(null);
  const countdownRef = useRef<ReturnType<typeof setInterval> | null>(null);
  const elapsedRef = useRef(0);
  const finishedRef = useRef<Set<string>>(new Set());
  const hardTimeoutRef = useRef<ReturnType<typeof setTimeout> | null>(null);

  const clearTimers = () => {
    if (pollTimerRef.current) clearTimeout(pollTimerRef.current);
    if (countdownRef.current) clearInterval(countdownRef.current);
    if (hardTimeoutRef.current) clearTimeout(hardTimeoutRef.current);
  };

  const fetchCode = useCallback(async (id: string, result: DecompilerResult) => {
    if (result.fetchedCode || !result.download_url) return;
    try {
      const resp = await fetch(`/api/dogbolt/download/${id}/${result.id}`);
      if (!resp.ok) return;
      const data = await resp.json();
      const code: string = data.code || '';
      setResults(prev => {
        const next = new Map(prev);
        const key = `${result.decompiler.name}@${result.decompiler.version}`;
        const existing = next.get(key);
        if (existing) next.set(key, { ...existing, code, fetchedCode: true });
        return next;
      });
    } catch {
      // ignore
    }
  }, []);

  const poll = useCallback(async (id: string) => {
    try {
      const resp = await fetch(`/api/dogbolt/status/${id}`);
      if (!resp.ok) return;
      const data = await resp.json();
      const apiResults: any[] = data.results || [];

      setResults(prev => {
        const next = new Map(prev);
        for (const r of apiResults) {
          const key = `${r.decompiler?.name}@${r.decompiler?.version}`;
          const existing = next.get(key);
          // Keep best version only
          if (existing) {
            const existingVersion = existing.decompiler.version;
            if (compareVersions(r.decompiler?.version || '0', existingVersion) <= 0) continue;
          }
          next.set(key, {
            id: String(r.id),
            decompiler: r.decompiler || { name: 'Unknown', version: '?' },
            status: deriveStatus(r),
            error: r.error || null,
            analysis_time: r.analysis_time || null,
            download_url: r.download_url || null,
            code: existing?.code || null,
            fetchedCode: existing?.fetchedCode || false,
          });
        }
        return next;
      });

      // Fetch code for results that have download_url
      for (const r of apiResults) {
        const key = `${r.decompiler?.name}@${r.decompiler?.version}`;
        if (r.download_url && !finishedRef.current.has(key)) {
          finishedRef.current.add(key);
          fetchCode(id, {
            id: String(r.id),
            decompiler: r.decompiler,
            status: r.status,
            error: r.error,
            analysis_time: r.analysis_time,
            download_url: r.download_url,
            code: null,
            fetchedCode: false,
          });
        }
      }

      // All done when every result has either a download_url (Complete) or error (Failed)
      const allDone = apiResults.length > 0 && apiResults.every(r => r.download_url || r.error);
      if (allDone) {
        setUploadState('done');
        clearTimers();
        return;
      }

      const delay = elapsedRef.current < 60 ? 3000 : 5000;
      pollTimerRef.current = setTimeout(() => poll(id), delay);
    } catch {
      const delay = elapsedRef.current < 60 ? 3000 : 5000;
      pollTimerRef.current = setTimeout(() => poll(id), delay);
    }
  }, [fetchCode]);

  const upload = useCallback(async (file: File) => {
    clearTimers();
    setUploadState('uploading');
    setError(null);
    setBinaryId(null);
    setResults(new Map());
    setFileName(file.name);
    finishedRef.current = new Set();
    elapsedRef.current = 0;
    setSecondsLeft(300);

    const formData = new FormData();
    formData.append('file', file);

    try {
      const resp = await fetch('/api/dogbolt/upload', { method: 'POST', body: formData });
      if (!resp.ok) {
        const text = await resp.text().catch(() => 'Upload failed');
        setError(text);
        setUploadState('error');
        return;
      }
      const data = await resp.json();
      const id: string = data.id;
      setBinaryId(id);
      setUploadState('polling');

      // Countdown timer
      countdownRef.current = setInterval(() => {
        elapsedRef.current += 1;
        setSecondsLeft(s => Math.max(0, s - 1));
      }, 1000);

      // Hard 300s timeout
      hardTimeoutRef.current = setTimeout(() => {
        clearTimers();
        setUploadState('done');
      }, 300_000);

      poll(id);
    } catch (e: any) {
      setError(e?.message || 'Network error');
      setUploadState('error');
    }
  }, [poll]);

  const rerun = useCallback(async (decompilationId: string) => {
    if (!binaryId) return;
    try {
      await fetch(`/api/dogbolt/rerun/${binaryId}/${decompilationId}`, { method: 'POST' });
    } catch {
      // ignore
    }
  }, [binaryId]);

  const reset = useCallback(() => {
    clearTimers();
    setUploadState('idle');
    setBinaryId(null);
    setResults(new Map());
    setError(null);
    setSecondsLeft(300);
    setFileName(null);
  }, []);

  return {
    uploadState,
    binaryId,
    results,
    error,
    secondsLeft,
    fileName,
    upload,
    rerun,
    reset,
  };
}
