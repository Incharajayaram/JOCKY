# Instructions

- Following Playwright test failed.
- Explain why, be concise, respect Playwright best practices.
- Provide a snippet of code with the fix, if possible.

# Test info

- Name: app.spec.ts >> JOCKY Frontend >> page loads with dark theme and branding
- Location: tests/app.spec.ts:9:3

# Error details

```
Error: expect(locator).toBeVisible() failed

Locator: locator('text=JOCKY')
Expected: visible
Error: strict mode violation: locator('text=JOCKY') resolved to 6 elements:
    1) <span>JOCKY</span> aka getByText('JOCKY', { exact: true })
    2) <span class="mtk7">// JOCKY Windows Research Chain - Full Capability </span> aka getByText('// JOCKY Windows Research')
    3) <span class="mtk1"> jocky.runtime</span> aka getByText('jocky.runtime')
    4) <span class="mtk1"> jocky.fs</span> aka getByText('jocky.fs')
    5) <span class="mtk1"> jocky.crypto</span> aka getByText('jocky.crypto')
    6) <span class="mtk1"> jocky.net</span> aka getByText('jocky.net')

Call log:
  - Expect "toBeVisible" locator('text=JOCKY') with timeout 5000ms
  - waiting for locator('text=JOCKY')

```

# Page snapshot

```yaml
- generic [active] [ref=e1]:
  - generic [ref=e3]:
    - generic [ref=e4]:
      - generic [ref=e5]:
        - generic [ref=e8]: JOCKY
        - generic [ref=e9]: v1.0
      - button "Compile" [ref=e10] [cursor=pointer]
    - generic [ref=e13]:
      - button "Windows" [ref=e14] [cursor=pointer]
      - button "Linux" [ref=e17] [cursor=pointer]
    - generic [ref=e20]:
      - generic [ref=e21]:
        - generic [ref=e22]: payload_win.jky
        - code [ref=e26]:
          - generic [ref=e27]:
            - textbox "Editor content" [ref=e28]
            - textbox [aria-hidden] [ref=e29]
            - generic [aria-hidden] [ref=e31]:
              - generic [ref=e32]: "1"
              - generic [ref=e35]: "2"
              - generic [ref=e37]: "3"
              - generic [ref=e39]: "4"
              - generic [ref=e41]: "5"
              - generic [ref=e43]: "6"
              - generic [ref=e45]: "7"
              - generic [ref=e47]: "8"
              - generic [ref=e49]: "9"
              - generic [ref=e51]: "10"
              - generic [ref=e53]:
                - generic [ref=e54] [cursor=pointer]: 
                - generic [ref=e55]: "11"
              - generic [ref=e56]: "12"
              - generic [ref=e58]: "13"
              - generic [ref=e60]: "14"
              - generic [ref=e62]: "15"
              - generic [ref=e64]: "16"
              - generic [ref=e66]: "17"
              - generic [ref=e68]: "18"
              - generic [ref=e70]: "19"
            - generic [aria-hidden] [ref=e101]:
              - generic [ref=e102]: // JOCKY Windows Research Chain - Full Capability Demo
              - generic [ref=e104]: // BYOVD + Exploitation + Persistence + Multi-Channel Exfiltration
              - generic [ref=e106]: "// Authorized: Red Hat + IIT Bombay Cyber Security Team"
              - generic [ref=e108]: "// Purpose: Defense research, detection validation, authorized testing"
              - generic [ref=e111]: use jocky.runtime
              - generic [ref=e113]: use jocky.fs
              - generic [ref=e115]: use jocky.crypto
              - generic [ref=e117]: use jocky.net
              - generic [ref=e120]: const DRIVER_CHAIN = [
              - generic [ref=e122]: ("rtkiow10x64.sys", "\\\\.\\RTCore64", 0x82000000),
              - generic [ref=e124]: ("rtkiow8x64.sys", "\\\\.\\RTCore64", 0x82000000),
              - generic [ref=e126]: ("AMDRyzenMasterDriver.sys", "\\\\.\\AMDRyzenMasterDriver", 0x81000000),
              - generic [ref=e128]: ("nvflsh64.sys", "\\\\.\\nvflsh64", 0x80002000),
              - generic [ref=e130]: ("speedfan.sys", "\\\\.\\speedfan", 0x80002000),
              - generic [ref=e132]: ("ene.sys", "\\\\.\\EneIo", 0x85000000),
              - generic [ref=e134]: ("iQVW64.SYS", "\\\\.\\Nal", 0x80802000),
              - generic [ref=e136]: ("UCOREW64.SYS", "\\\\.\\Global\\", 0x88000000),
      - generic [ref=e139]:
        - generic [ref=e140]:
          - generic [ref=e141]: Runtime APIs
          - button "Anti-Analysis 5" [ref=e145] [cursor=pointer]:
            - text: Anti-Analysis
            - generic [ref=e148]: "5"
          - button "Crypto 2" [ref=e150] [cursor=pointer]:
            - text: Crypto
            - generic [ref=e153]: "2"
          - button "Exfiltration 3" [ref=e155] [cursor=pointer]:
            - text: Exfiltration
            - generic [ref=e158]: "3"
          - button "AI/ML 3" [ref=e160] [cursor=pointer]:
            - text: AI/ML
            - generic [ref=e163]: "3"
          - button "Sandbox 5" [ref=e165] [cursor=pointer]:
            - text: Sandbox
            - generic [ref=e168]: "5"
          - button "Plugin 2" [ref=e170] [cursor=pointer]:
            - text: Plugin
            - generic [ref=e173]: "2"
          - button "Audit 4" [ref=e175] [cursor=pointer]:
            - text: Audit
            - generic [ref=e178]: "4"
          - button "BYOVD 4" [ref=e180] [cursor=pointer]:
            - text: BYOVD
            - generic [ref=e183]: "4"
          - button "Evasion 2" [ref=e185] [cursor=pointer]:
            - text: Evasion
            - generic [ref=e188]: "2"
          - button "Registry 3" [ref=e190] [cursor=pointer]:
            - text: Registry
            - generic [ref=e193]: "3"
          - button "Forensics (Win) 7" [ref=e195] [cursor=pointer]:
            - text: Forensics (Win)
            - generic [ref=e198]: "7"
          - button "Exploitation 2" [ref=e200] [cursor=pointer]:
            - text: Exploitation
            - generic [ref=e203]: "2"
        - generic [ref=e204]:
          - generic [ref=e205]: Obfuscation Passes
          - generic [ref=e208]: MLIR PASSES
          - generic [ref=e209]:
            - generic [ref=e210]:
              - generic [ref=e211]: String Encrypt
              - generic [ref=e212]: Encrypts string constants in MLIR
            - generic [ref=e213] [cursor=pointer]
          - generic [ref=e215]:
            - generic [ref=e216]:
              - generic [ref=e217]: Constant Obfuscate
              - generic [ref=e218]: Obfuscates numeric constants
            - generic [ref=e219] [cursor=pointer]
          - generic [ref=e221]:
            - generic [ref=e222]:
              - generic [ref=e223]: Symbol Obfuscate
              - generic [ref=e224]: Renames symbols to random identifiers
            - generic [ref=e225] [cursor=pointer]
          - generic [ref=e227]: LLVM PASSES
          - generic [ref=e228]:
            - generic [ref=e229]:
              - generic [ref=e230]: Bogus Control Flow
              - generic [ref=e231]: Inserts fake control flow paths
            - generic [ref=e232] [cursor=pointer]
          - generic [ref=e234]:
            - generic [ref=e235]:
              - generic [ref=e236]: Control Flow Flattening
              - generic [ref=e237]: Flattens function control flow into switch-based dispatch
            - generic [ref=e238] [cursor=pointer]
          - generic [ref=e240]:
            - generic [ref=e241]:
              - generic [ref=e242]: Instruction Substitution
              - generic [ref=e243]: Replaces instructions with equivalent complex sequences
            - generic [ref=e244] [cursor=pointer]
          - generic [ref=e246]:
            - generic [ref=e247]:
              - generic [ref=e248]: Basic Block Splitting
              - generic [ref=e249]: Splits basic blocks into smaller fragments
            - generic [ref=e250] [cursor=pointer]
          - generic [ref=e252]:
            - generic [ref=e253]:
              - generic [ref=e254]: Indirect Calls
              - generic [ref=e255]: Replaces direct calls with indirect function pointers
            - generic [ref=e256] [cursor=pointer]
          - generic [ref=e258]:
            - generic [ref=e259]:
              - generic [ref=e260]: Strip Signatures
              - generic [ref=e261]: Removes function signature metadata
            - generic [ref=e262] [cursor=pointer]
    - generic [ref=e264]:
      - generic [ref=e265] [cursor=pointer]: Build Output
      - generic [ref=e272]: No build output yet. Click Compile to start.
  - generic [ref=e274]:
    - alert
    - alert
```

# Test source

```ts
  1   | import { test, expect } from '@playwright/test';
  2   | 
  3   | test.describe('JOCKY Frontend', () => {
  4   |   test.beforeEach(async ({ page }) => {
  5   |     await page.goto('/');
  6   |     await page.waitForLoadState('networkidle');
  7   |   });
  8   | 
  9   |   test('page loads with dark theme and branding', async ({ page }) => {
> 10  |     await expect(page.locator('text=JOCKY')).toBeVisible();
      |                                              ^ Error: expect(locator).toBeVisible() failed
  11  |     await expect(page.locator('text=v1.0')).toBeVisible();
  12  |     const bg = await page.evaluate(() => getComputedStyle(document.body).backgroundColor);
  13  |     expect(bg).not.toBe('rgb(255, 255, 255)');
  14  |   });
  15  | 
  16  |   test('two platform tabs exist', async ({ page }) => {
  17  |     const windowsTab = page.locator('button', { hasText: 'Windows' });
  18  |     const linuxTab = page.locator('button', { hasText: 'Linux' });
  19  |     await expect(windowsTab).toBeVisible();
  20  |     await expect(linuxTab).toBeVisible();
  21  |   });
  22  | 
  23  |   test('default tab is Windows with demo script loaded', async ({ page }) => {
  24  |     const windowsTab = page.locator('button', { hasText: 'Windows' });
  25  |     const borderBottom = await windowsTab.evaluate((el) => getComputedStyle(el).borderBottomColor);
  26  |     expect(borderBottom).not.toBe('rgba(0, 0, 0, 0)');
  27  | 
  28  |     await expect(page.locator('text=payload_win.jky')).toBeVisible();
  29  |   });
  30  | 
  31  |   test('Windows demo script is comprehensive (not a stub)', async ({ page }) => {
  32  |     await page.waitForTimeout(2000);
  33  |     const editorContent = await page.evaluate(() => {
  34  |       const editors = (window as any).monaco?.editor?.getEditors?.();
  35  |       if (editors && editors.length > 0) {
  36  |         return editors[0].getValue();
  37  |       }
  38  |       return '';
  39  |     });
  40  |     expect(editorContent.length).toBeGreaterThan(5000);
  41  |     expect(editorContent).toContain('phase_init');
  42  |     expect(editorContent).toContain('phase_byovd');
  43  |     expect(editorContent).toContain('DRIVER_CHAIN');
  44  |     expect(editorContent).toContain('phase_forensic_cleanup');
  45  |     expect(editorContent).toContain('phase_exfiltration');
  46  |   });
  47  | 
  48  |   test('switching to Linux tab shows Linux demo script', async ({ page }) => {
  49  |     await page.locator('button', { hasText: 'Linux' }).click();
  50  |     await expect(page.locator('text=payload_linux.jky')).toBeVisible();
  51  | 
  52  |     await page.waitForTimeout(1000);
  53  |     const editorContent = await page.evaluate(() => {
  54  |       const editors = (window as any).monaco?.editor?.getEditors?.();
  55  |       if (editors && editors.length > 0) {
  56  |         return editors[0].getValue();
  57  |       }
  58  |       return '';
  59  |     });
  60  |     expect(editorContent).toContain('LKM_PATHS');
  61  |     expect(editorContent).toContain('EBPF_PROGRAMS');
  62  |     expect(editorContent).toContain('phase_lkm_loading');
  63  |     expect(editorContent).toContain('phase_process_hollowing');
  64  |   });
  65  | 
  66  |   test('Linux demo script is comprehensive (not a stub)', async ({ page }) => {
  67  |     await page.locator('button', { hasText: 'Linux' }).click();
  68  |     await page.waitForTimeout(1000);
  69  |     const editorContent = await page.evaluate(() => {
  70  |       const editors = (window as any).monaco?.editor?.getEditors?.();
  71  |       if (editors && editors.length > 0) {
  72  |         return editors[0].getValue();
  73  |       }
  74  |       return '';
  75  |     });
  76  |     expect(editorContent.length).toBeGreaterThan(5000);
  77  |     expect(editorContent).toContain('phase_ebpf_loading');
  78  |     expect(editorContent).toContain('phase_threat_assessment');
  79  |     expect(editorContent).toContain('phase_self_delete');
  80  |   });
  81  | 
  82  |   test('Monaco editor is present and functional', async ({ page }) => {
  83  |     await page.waitForTimeout(2000);
  84  |     const monacoPresent = await page.evaluate(() => {
  85  |       return !!(window as any).monaco?.editor?.getEditors?.()?.length;
  86  |     });
  87  |     expect(monacoPresent).toBe(true);
  88  | 
  89  |     const editorContainer = page.locator('.monaco-editor');
  90  |     await expect(editorContainer.first()).toBeVisible();
  91  |   });
  92  | 
  93  |   test('Runtime API panel exists with categorized buttons', async ({ page }) => {
  94  |     await expect(page.locator('text=Runtime APIs')).toBeVisible();
  95  |     await expect(page.locator('text=Anti-Analysis')).toBeVisible();
  96  |   });
  97  | 
  98  |   test('clicking a runtime API category reveals API buttons', async ({ page }) => {
  99  |     const category = page.locator('button', { hasText: 'Anti-Analysis' });
  100 |     await category.click();
  101 | 
  102 |     await expect(page.locator('button', { hasText: 'check_analysis_environment' })).toBeVisible();
  103 |     await expect(page.locator('button', { hasText: 'is_debugger_present' })).toBeVisible();
  104 |   });
  105 | 
  106 |   test('clicking a runtime API button inserts code into editor', async ({ page }) => {
  107 |     await page.waitForTimeout(2000);
  108 | 
  109 |     const beforeContent = await page.evaluate(() => {
  110 |       const editors = (window as any).monaco?.editor?.getEditors?.();
```