import { useEffect, useState, useCallback } from 'react';

export interface Example {
  id: string;
  platform: string;
  name: string;
  description: string;
  source: string;
}

const API_BASE = 'http://localhost:8000';

export function useExamples() {
  const [examples, setExamples] = useState<Example[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);

  useEffect(() => {
    const loadExamples = async () => {
      try {
        const res = await fetch(`${API_BASE}/api/examples`);
        if (!res.ok) throw new Error('Failed to load examples');
        const data = await res.json();
        setExamples(data.examples || []);
      } catch (err) {
        setError(err instanceof Error ? err.message : 'Unknown error');
        setExamples([]);
      } finally {
        setLoading(false);
      }
    };

    loadExamples();
  }, []);

  const getExample = useCallback((exampleId: string) => {
    return examples.find(ex => ex.id === exampleId);
  }, [examples]);

  const getExamplesByPlatform = useCallback((platform: string) => {
    return examples.filter(ex => ex.platform === platform);
  }, [examples]);

  return { examples, loading, error, getExample, getExamplesByPlatform };
}
