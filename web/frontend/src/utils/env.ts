function getApiBase(): string {
  if (typeof window === 'undefined') {
    return 'http://localhost:8000';
  }

  const hostname = window.location.hostname;
  const protocol = window.location.protocol;

  if (hostname === 'localhost' || hostname === '127.0.0.1') {
    return 'http://localhost:8000';
  }

  if (hostname === 'frontend') {
    return 'http://backend:8000';
  }

  return `${protocol}//${hostname}:8000`;
}

export const API_BASE = getApiBase();

export const ENV = {
  API_BASE,
  DEBUG: import.meta.env.MODE === 'development',
  VERSION: '1.0.0',
};
