export interface ParsedError {
  file: string;
  line: number;
  column: number;
  message: string;
  level: 'error' | 'warning' | 'info';
}

const ERROR_PATTERNS = [
  /(?:error|Error)\s*(?:E\d+)?\s*:\s*(.+?)(?:\s+at\s+(\w+):(\d+):(\d+))?/gi,
  /^(?:Error|ERROR):\s*(.+?)\s+\((\w+):(\d+):(\d+)\)/gm,
  /(.+?):(\d+):(\d+):\s*(?:error|Error)\s*:\s*(.+)/gm,
];

export function parseErrors(text: string): ParsedError[] {
  const errors: ParsedError[] = [];

  for (const pattern of ERROR_PATTERNS) {
    let match;
    while ((match = pattern.exec(text)) !== null) {
      if (match.length >= 4) {
        const error: ParsedError = {
          file: match[2] || 'unknown',
          line: parseInt(match[3] || '0', 10) || 1,
          column: parseInt(match[4] || '0', 10) || 1,
          message: match[1] || match[4] || 'Unknown error',
          level: 'error',
        };
        if (!errors.some(e => e.line === error.line && e.column === error.column)) {
          errors.push(error);
        }
      }
    }
  }

  return errors;
}

export function groupErrorsByFile(errors: ParsedError[]): Record<string, ParsedError[]> {
  return errors.reduce((acc, error) => {
    if (!acc[error.file]) acc[error.file] = [];
    acc[error.file].push(error);
    return acc;
  }, {} as Record<string, ParsedError[]>);
}
