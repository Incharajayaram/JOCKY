import { useEffect, useState } from 'react';

export interface JobHistoryItem {
  job_id: string;
  status: string;
  platform: string;
  created_at: string;
  compilation_time?: number;
  output_size?: number;
}

export function useJobHistory() {
  const [history, setHistory] = useState<JobHistoryItem[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const loadHistory = async (limit = 20, offset = 0) => {
    setLoading(true);
    setError(null);
    try {
      const res = await fetch(`/api/jobs/history?limit=${limit}&offset=${offset}`);
      if (!res.ok) throw new Error('Failed to load history');
      const data = await res.json();
      setHistory(data);
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Unknown error');
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    loadHistory();
  }, []);

  return { history, loading, error, loadHistory };
}
