export type Platform = 'windows' | 'linux';

export interface RuntimeApi {
  name: string;
  description: string;
  snippet: string;
}

export interface ApiCategory {
  name: string;
  platform: 'both' | 'windows' | 'linux';
  apis: RuntimeApi[];
}

export interface ObfuscationPass {
  id: string;
  name: string;
  description: string;
  enabled: boolean;
}

export interface ObfuscationState {
  mlir: ObfuscationPass[];
  llvm: ObfuscationPass[];
}

export interface CompileRequest {
  source: string;
  platform: Platform;
  obfuscation: Record<string, boolean>;
}

export interface CompileResponse {
  job_id: string;
}

export interface JobStatus {
  status: 'queued' | 'compiling' | 'done' | 'error';
  logs: string[];
  progress: number;
}

export interface LogEntry {
  text: string;
  level: 'info' | 'warn' | 'error' | 'success';
  timestamp: number;
}
