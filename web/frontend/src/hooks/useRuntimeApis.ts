import { useEffect, useState } from 'react';
import type { ApiCategory } from '../types';
import { runtimeApis } from '../data';

const API_BASE = 'http://localhost:8000';

export function useRuntimeApis() {
  const [categories, setCategories] = useState<ApiCategory[]>(runtimeApis);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    let cancelled = false;

    async function fetchApis() {
      try {
        const res = await fetch(`${API_BASE}/api/runtime-apis`);
        if (!res.ok) throw new Error('API unavailable');
        const data = await res.json();
        if (!cancelled) {
          // Handle both array and {categories: array} formats
          const cats = Array.isArray(data) ? data : data.categories;
          if (cats && Array.isArray(cats)) {
            setCategories(cats);
          }
        }
      } catch {
        // Backend down -- use bundled fallback data
      } finally {
        if (!cancelled) setLoading(false);
      }
    }

    fetchApis();
    return () => { cancelled = true; };
  }, []);

  return { categories, loading };
}
