import { useEffect, useState } from 'react';
import type { ApiCategory } from '../types';
import { runtimeApis } from '../data';

export function useRuntimeApis() {
  const [categories, setCategories] = useState<ApiCategory[]>(runtimeApis);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [isUsingFallback, setIsUsingFallback] = useState(false);

  useEffect(() => {
    let cancelled = false;

    async function fetchApis() {
      try {
        const res = await fetch('/api/runtime-apis');
        if (!res.ok) throw new Error('API unavailable');
        const data = await res.json();
        if (!cancelled) {
          const cats = Array.isArray(data) ? data : data.categories;
          if (cats && Array.isArray(cats)) {
            setCategories(cats);
            setError(null);
            setIsUsingFallback(false);
          }
        }
      } catch (err) {
        if (!cancelled) {
          const message = err instanceof Error ? err.message : 'Failed to load runtime APIs';
          setError(message);
          setIsUsingFallback(true);
          console.warn('[RuntimeApis] Using fallback data:', message);
        }
      } finally {
        if (!cancelled) setLoading(false);
      }
    }

    fetchApis();
    return () => { cancelled = true; };
  }, []);

  return { categories, loading, error, isUsingFallback };
}
