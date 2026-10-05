import { useCallback, useRef, useState } from 'react';
import type { CompileRequest, LogEntry, ObfuscationState } from '../types';

function parseLogLevel(text: string): LogEntry['level'] {
  const t = text.toLowerCase();
  if (
    text.startsWith('[ERROR]') || text.startsWith('error:') ||
    text.includes('Build failed') || text.includes('ParseError') ||
    text.startsWith('Traceback') || t.includes('exception') ||
    text.startsWith('  File "')
  ) return 'error';
  if (text.startsWith('[WARN]') || text.startsWith('warning:')) return 'warn';
  if (
    text.startsWith('[OK]') || text.includes('[PIPELINE] Build succeeded') ||
    t.includes('success') || t.includes('complete')
  ) return 'success';
  return 'info';
}

interface StatusMessage {
  status?: string;
  progress?: number;
  logs?: string[];
}

export function useCompiler() {
  const [compiling, setCompiling] = useState(false);
  const [logs, setLogs] = useState<LogEntry[]>([]);
  const [jobId, setJobId] = useState<string | null>(null);
  const [buildDone, setBuildDone] = useState(false);
  const wsRef = useRef<WebSocket | null>(null);
  const wsRetryRef = useRef<number>(0);
  const buildDoneRef = useRef(false);
  const seenLogCountRef = useRef(0);

  const addLog = useCallback((text: string, level?: LogEntry['level']) => {
    setLogs((prev) => [...prev, {
      text,
      level: level ?? parseLogLevel(text),
      timestamp: Date.now(),
    }]);
  }, []);

  const addNewLines = useCallback((lines: string[]) => {
    const newLines = lines.slice(seenLogCountRef.current);
    if (newLines.length === 0) return;
    seenLogCountRef.current = lines.length;
    setLogs((prev) => [
      ...prev,
      ...newLines.map((text) => ({
        text,
        level: parseLogLevel(text),
        timestamp: Date.now(),
      })),
    ]);
  }, []);

  const handleStatusMessage = useCallback((data: StatusMessage) => {
    if (data.logs) {
      addNewLines(data.logs);
    }
    if (data.status === 'done') {
      buildDoneRef.current = true;
      setBuildDone(true);
      setCompiling(false);
    } else if (data.status === 'failed' || data.status === 'error') {
      buildDoneRef.current = true;
      setBuildDone(true);
      setCompiling(false);
    }
  }, [addNewLines]);

  const clearLogs = useCallback(() => {
    setLogs([]);
    setJobId(null);
    setBuildDone(false);
    seenLogCountRef.current = 0;
  }, []);

  const compile = useCallback(async (
    source: string,
    platform: 'windows' | 'linux',
    obfuscation: ObfuscationState,
  ) => {
    setCompiling(true);
    setBuildDone(false);
    buildDoneRef.current = false;
    seenLogCountRef.current = 0;
    setLogs([]);

    const mlirMap: Record<string, boolean> = {};
    const llvmMap: Record<string, boolean> = {};
    obfuscation.mlir.forEach((p) => { mlirMap[p.id] = p.enabled; });
    obfuscation.llvm.forEach((p) => { llvmMap[p.id] = p.enabled; });

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
          const msg: string = event.data;
          try {
            const parsed: StatusMessage = JSON.parse(msg);
            handleStatusMessage(parsed);
          } catch {
            // Plain text log line
            if (msg === '__BUILD_DONE__' || msg.includes('[DONE]')) {
              buildDoneRef.current = true;
              setBuildDone(true);
            } else {
              addLog(msg);
            }
          }
        };

        ws.onerror = () => {
          addLog('[WARN] WebSocket connection lost', 'warn');
        };

        ws.onclose = () => {
          if (buildDoneRef.current) {
            setCompiling(false);
          } else if (wsRetryRef.current < 3) {
            const delay = Math.min(1000 * Math.pow(2, wsRetryRef.current), 5000);
            wsRetryRef.current += 1;
            addLog(`[INFO] Reconnecting in ${delay}ms (attempt ${wsRetryRef.current}/3)...`, 'info');
            setTimeout(connectWebSocket, delay);
          } else {
            addLog('[WARN] Switching to polling...', 'warn');
            pollStatus(id);
          }
        };
      };

      connectWebSocket();
    } catch {
      addLog('[ERROR] Cannot reach compilation server at localhost:8000', 'error');
      addLog('[INFO] Ensure the backend is running: cd web/backend && python main.py', 'info');
      setCompiling(false);
    }
  }, [addLog, addNewLines, handleStatusMessage]);

  async function pollStatus(id: string) {
    try {
      const res = await fetch(`/api/status/${id}`);
      if (!res.ok) return;
      const data: StatusMessage = await res.json();
      if (data.logs) {
        addNewLines(data.logs);
      }
      if (data.status === 'done') {
        buildDoneRef.current = true;
        setBuildDone(true);
        setCompiling(false);
        addLog('[OK] Build complete', 'success');
      } else if (data.status === 'error' || data.status === 'failed') {
        buildDoneRef.current = true;
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
