export interface BasicBlock {
  id: string;
  label: string;
  lines: string[];
  startLine: number;
  endLine: number;
  type: 'entry' | 'return' | 'condition' | 'regular';
}

export interface CFGEdge {
  id: string;
  source: string;
  target: string;
  type: 'branch' | 'flow';
  label?: string;
}

export interface CFGFunction {
  name: string;
  blocks: BasicBlock[];
  edges: CFGEdge[];
}

export interface CFGData {
  functions: CFGFunction[];
}

const CONDITION_RE = /^\s*(if|else\s+if|while|for|do|switch)\s*[({]/;
const ELSE_RE = /^\s*else\s*[{]/;
const RETURN_RE = /^\s*return\b/;
const GOTO_RE = /^\s*(goto|break|continue)\b/;
const FUNC_RE = /^[\w\s*]+\s+(\w+)\s*\([^)]*\)\s*\{/;
const BRACE_OPEN_RE = /\{/g;
const BRACE_CLOSE_RE = /\}/g;

function countBraces(line: string): number {
  return (line.match(BRACE_OPEN_RE) || []).length - (line.match(BRACE_CLOSE_RE) || []).length;
}

function extractFunctions(code: string): { name: string; lines: string[] }[] {
  const allLines = code.split('\n');
  const functions: { name: string; lines: string[] }[] = [];
  let i = 0;

  while (i < allLines.length) {
    const line = allLines[i];
    const match = line.match(FUNC_RE);
    if (match && !line.trim().startsWith('//') && !line.trim().startsWith('*')) {
      const name = match[1];
      const funcLines: string[] = [line];
      let depth = countBraces(line);
      i++;
      while (i < allLines.length && (depth > 0 || funcLines.length === 1)) {
        funcLines.push(allLines[i]);
        depth += countBraces(allLines[i]);
        i++;
        if (depth <= 0 && funcLines.length > 1) break;
      }
      if (funcLines.length > 2) {
        functions.push({ name, lines: funcLines });
      }
    } else {
      i++;
    }
  }
  return functions;
}

function blockType(lines: string[]): BasicBlock['type'] {
  if (lines.some(l => RETURN_RE.test(l))) return 'return';
  if (lines.some(l => CONDITION_RE.test(l))) return 'condition';
  return 'regular';
}

function buildBlocks(lines: string[], funcName: string): BasicBlock[] {
  const blocks: BasicBlock[] = [];
  let current: string[] = [];
  let startLine = 0;
  let blockIdx = 0;

  const flush = (endLine: number) => {
    const trimmed = current.filter(l => l.trim());
    if (trimmed.length === 0) return;
    const id = `${funcName}_b${blockIdx++}`;
    const label = trimmed.slice(0, 3).map(l => l.trim()).join('\n');
    blocks.push({
      id,
      label,
      lines: trimmed,
      startLine,
      endLine,
      type: blockIdx === 1 ? 'entry' : blockType(trimmed),
    });
    current = [];
  };

  lines.forEach((line, idx) => {
    if (CONDITION_RE.test(line) || ELSE_RE.test(line)) {
      flush(idx);
      startLine = idx;
      current.push(line);
      flush(idx + 1);
      startLine = idx + 1;
    } else if (RETURN_RE.test(line) || GOTO_RE.test(line)) {
      current.push(line);
      flush(idx + 1);
      startLine = idx + 1;
    } else {
      current.push(line);
    }
  });
  flush(lines.length);

  // Mark first block as entry
  if (blocks.length > 0) blocks[0].type = 'entry';
  return blocks;
}

function buildEdges(blocks: BasicBlock[], funcName: string): CFGEdge[] {
  const edges: CFGEdge[] = [];
  let edgeIdx = 0;

  blocks.forEach((block, i) => {
    const next = blocks[i + 1];
    if (!next) return;

    const hasReturn = block.lines.some(l => RETURN_RE.test(l));
    const hasGoto = block.lines.some(l => GOTO_RE.test(l));
    if (hasReturn || hasGoto) return;

    const isCondition = block.type === 'condition';

    edges.push({
      id: `${funcName}_e${edgeIdx++}`,
      source: block.id,
      target: next.id,
      type: isCondition ? 'branch' : 'flow',
      label: isCondition ? 'true' : undefined,
    });

    // For conditions, also add a false edge to the block after next (else branch)
    if (isCondition && blocks[i + 2]) {
      edges.push({
        id: `${funcName}_e${edgeIdx++}`,
        source: block.id,
        target: blocks[i + 2].id,
        type: 'branch',
        label: 'false',
      });
    }
  });

  return edges;
}

export function parseApproximateCFG(code: string): CFGData {
  const functions = extractFunctions(code);
  return {
    functions: functions.map(({ name, lines }) => {
      const blocks = buildBlocks(lines, name);
      const edges = buildEdges(blocks, name);
      return { name, blocks, edges };
    }).filter(f => f.blocks.length > 0),
  };
}
