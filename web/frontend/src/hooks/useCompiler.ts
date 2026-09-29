import { useCallback, useRef, useState } from 'react';
import type { CompileRequest, LogEntry, ObfuscationState } from '../types';

function parseLogLevel(text: string): LogEntry['level'] {
  if (text.startsWith('[ERROR]') || text.startsWith('error:')) return 'error';
  if (text.startsWith('[WARN]') || text.startsWith('warning:')) return 'warn';
  if (text.startsWith('[OK]') || text.includes('success') || text.includes('complete')) return 'success';
  return 'info';
}

export function useCompiler() {
  const [compiling, setCompiling] = useState(false);
  const [logs, setLogs] = useState<LogEntry[]>([]);
  const [jobId, setJobId] = useState<string | null>(null);
  const [buildDone, setBuildDone] = useState(false);
  const wsRef = useRef<WebSocket | null>(null);
  const wsRetryRef = useRef<number>(0);

  const addLog = useCallback((text: string, level?: LogEntry['level']) => {
    setLogs((prev) => [...prev, {
      text,
      level: level ?? parseLogLevel(text),
      timestamp: Date.now(),
    }]);
  }, []);

  const clearLogs = useCallback(() => {
    setLogs([]);
    setJobId(null);
    setBuildDone(false);
  }, []);

  const compile = useCallback(async (
    source: string,
    platform: 'windows' | 'linux',
    obfuscation: ObfuscationState,
  ) => {
    setCompiling(true);
    setBuildDone(false);
    setLogs([]);

    const mlirMap: Record<string, boolean> = {};
    const llvmMap: Record<string, boolean> = {};
    obfuscation.mlir.forEach((p) => {
      mlirMap[p.id] = p.enabled;
    });
    obfuscation.llvm.forEach((p) => {
      llvmMap[p.id] = p.enabled;
    });

    const request: CompileRequest = {
      source,
      platform,
      obfuscation: { mlir: mlirMap, llvm: llvmMap },
      preset: 'standard',
    };

    const enabledPasses = [
      ...obfuscation.mlir.filter((p) => p.enabled).map((p) => p.id),
      ...obfuscation.llvm.filter((p) => p.enabled).map((p) => p.id),
    ];
    addLog(`[INFO] Starting ${platform} compilation...`, 'info');
    addLog(`[INFO] Obfuscation passes: ${enabledPasses.join(', ') || 'none'}`, 'info');

    try {
      const res = await fetch('/api/compile', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(request),
      });

      if (!res.ok) {
        const errText = await res.text().catch(() => 'Unknown error');
        addLog(`[ERROR] Compilation request failed: ${errText}`, 'error');
        setCompiling(false);
        return;
      }

      const data = await res.json();
      const id = data.job_id;
      setJobId(id);
      addLog(`[INFO] Job queued: ${id}`, 'info');

      if (wsRef.current) {
        wsRef.current.close();
      }

      const connectWebSocket = () => {
        const wsProtocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const ws = new WebSocket(`${wsProtocol}//${window.location.host}/ws/logs/${id}`);
        wsRef.current = ws;

        ws.onopen = () => {
          wsRetryRef.current = 0;
          addLog('[INFO] Connected to live log stream', 'info');
        };

        ws.onmessage = (event) => {
          const msg = event.data;
          addLog(msg);
        };

        ws.onerror = () => {
          addLog('[WARN] WebSocket connection lost', 'warn');
        };

        ws.onclose = () => {
          if (wsRetryRef.current < 3 && !buildDone) {
            const delay = Math.min(1000 * Math.pow(2, wsRetryRef.current), 5000);
            wsRetryRef.current += 1;
            addLog(`[INFO] Reconnecting in ${delay}ms (attempt ${wsRetryRef.current}/3)...`, 'info');
            setTimeout(connectWebSocket, delay);
          } else if (!buildDone) {
            addLog('[ERROR] WebSocket disconnected, switching to polling', 'error');
            pollStatus(id);
          } else {
            setCompiling(false);
            setBuildDone(true);
            addLog('[OK] Build process finished', 'success');
          }
        };
      };

      connectWebSocket();
    } catch {
      addLog('[ERROR] Cannot reach compilation server at localhost:8000', 'error');
      addLog('[INFO] Ensure the backend is running: cd web/backend && python main.py', 'info');
      setCompiling(false);
    }
  }, [addLog]);

  async function pollStatus(id: string) {
    try {
      const res = await fetch(`/api/status/${id}`);
      if (!res.ok) return;
      const data = await res.json();
      if (data.logs) {
        data.logs.forEach((line: string) => addLog(line));
      }
      if (data.status === 'done') {
        setBuildDone(true);
        setCompiling(false);
        addLog('[OK] Build complete', 'success');
      } else if (data.status === 'error') {
        setCompiling(false);
        addLog('[ERROR] Build failed', 'error');
      } else {
        setTimeout(() => pollStatus(id), 2000);
      }
    } catch {
      setCompiling(false);
    }
  }

  return { compiling, logs, jobId, buildDone, compile, clearLogs };
}
