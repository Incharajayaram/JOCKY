import { useEffect, useState } from 'react';

export interface Config {
  api_version: string;
  supported_platforms: string[];
  obfuscation_presets: string[];
  docs_url: string;
  environment: string;
  backend_host: string;
  backend_port: number;
  max_compilation_time: number;
}

const API_BASE = typeof window !== 'undefined' && window.location.hostname === 'localhost'
  ? 'http://localhost:8000'
  : `http://${window.location.hostname}:8000`;

export function useConfig() {
  const [config, setConfig] = useState<Config | null>(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const loadConfig = async () => {
      try {
        const res = await fetch(`${API_BASE}/api/config`);
        if (!res.ok) throw new Error(`Failed to load config: ${res.statusText}`);
        const data = await res.json();
        setConfig(data);
      } catch (err) {
        setError(err instanceof Error ? err.message : 'Unknown error');
        setConfig({
          api_version: '1.0.0',
          supported_platforms: ['windows', 'linux'],
          obfuscation_presets: ['none', 'light', 'standard', 'aggressive'],
          docs_url: '/docs',
          environment: 'unknown',
          backend_host: 'localhost',
          backend_port: 8000,
          max_compilation_time: 600,
        });
      } finally {
        setLoading(false);
      }
    };

    loadConfig();
  }, []);

  return { config, loading, error };
}
