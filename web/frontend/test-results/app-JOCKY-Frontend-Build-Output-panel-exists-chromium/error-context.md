# Instructions

- Following Playwright test failed.
- Explain why, be concise, respect Playwright best practices.
- Provide a snippet of code with the fix, if possible.

# Test info

- Name: app.spec.ts >> JOCKY Frontend >> Build Output panel exists
- Location: tests/app.spec.ts:160:3

# Error details

```
Error: expect(locator).toBeVisible() failed

Locator: locator('text=Build Output')
Expected: visible
Error: strict mode violation: locator('text=Build Output') resolved to 2 elements:
    1) <div>…</div> aka getByText('Build Output', { exact: true })
    2) <div>No build output yet. Click Compile to start.</div> aka getByText('No build output yet. Click')

Call log:
  - Expect "toBeVisible" locator('text=Build Output') with timeout 5000ms
  - waiting for locator('text=Build Output')

```

# Page snapshot

```yaml
- generic [active] [ref=e1]:
  - generic [ref=e3]:
    - generic [ref=e4]:
      - generic [ref=e5]:
        - generic [ref=e8]: JOCKY
        - generic [ref=e9]: v1.0
      - generic [ref=e10]:
        - button "Help (?)" [ref=e11] [cursor=pointer]
        - button "Settings" [ref=e15] [cursor=pointer]
        - button "Compile" [ref=e19] [cursor=pointer]
    - generic [ref=e22]:
      - button "Windows" [ref=e23] [cursor=pointer]
      - button "Linux" [ref=e26] [cursor=pointer]
    - generic [ref=e29]:
      - generic [ref=e30]:
        - generic [ref=e31]: payload_win.jky
        - code [ref=e35]:
          - generic [ref=e36]:
            - textbox "Editor content" [ref=e37]
            - textbox [aria-hidden] [ref=e38]
            - generic [aria-hidden] [ref=e40]:
              - generic [ref=e41]: "1"
              - generic [ref=e44]: "2"
              - generic [ref=e46]: "3"
              - generic [ref=e48]: "4"
              - generic [ref=e50]: "5"
              - generic [ref=e52]: "6"
              - generic [ref=e54]: "7"
              - generic [ref=e56]: "8"
              - generic [ref=e58]: "9"
              - generic [ref=e60]: "10"
              - generic [ref=e62]:
                - generic [ref=e63] [cursor=pointer]: 
                - generic [ref=e64]: "11"
              - generic [ref=e65]: "12"
              - generic [ref=e67]: "13"
              - generic [ref=e69]: "14"
              - generic [ref=e71]: "15"
              - generic [ref=e73]: "16"
              - generic [ref=e75]: "17"
              - generic [ref=e77]: "18"
              - generic [ref=e79]: "19"
            - generic [aria-hidden] [ref=e110]:
              - generic [ref=e111]: // JOCKY Windows Research Chain - Full Capability Demo
              - generic [ref=e113]: // BYOVD + Exploitation + Persistence + Multi-Channel Exfiltration
              - generic [ref=e115]: "// Authorized: Red Hat + IIT Bombay Cyber Security Team"
              - generic [ref=e117]: "// Purpose: Defense research, detection validation, authorized testing"
              - generic [ref=e120]: use jocky.runtime
              - generic [ref=e122]: use jocky.fs
              - generic [ref=e124]: use jocky.crypto
              - generic [ref=e126]: use jocky.net
              - generic [ref=e129]: const DRIVER_CHAIN = [
              - generic [ref=e131]: ("rtkiow10x64.sys", "\\\\.\\RTCore64", 0x82000000),
              - generic [ref=e133]: ("rtkiow8x64.sys", "\\\\.\\RTCore64", 0x82000000),
              - generic [ref=e135]: ("AMDRyzenMasterDriver.sys", "\\\\.\\AMDRyzenMasterDriver", 0x81000000),
              - generic [ref=e137]: ("nvflsh64.sys", "\\\\.\\nvflsh64", 0x80002000),
              - generic [ref=e139]: ("speedfan.sys", "\\\\.\\speedfan", 0x80002000),
              - generic [ref=e141]: ("ene.sys", "\\\\.\\EneIo", 0x85000000),
              - generic [ref=e143]: ("iQVW64.SYS", "\\\\.\\Nal", 0x80802000),
              - generic [ref=e145]: ("UCOREW64.SYS", "\\\\.\\Global\\", 0x88000000),
      - generic [ref=e148]:
        - generic [ref=e150]:
          - generic [ref=e151]: Runtime APIs
          - textbox "Search APIs..." [ref=e158]
          - generic [ref=e159]:
            - button "Anti-Analysis 5" [ref=e161] [cursor=pointer]:
              - text: Anti-Analysis
              - generic [ref=e164]: "5"
            - button "BYOVD 4" [ref=e166] [cursor=pointer]:
              - text: BYOVD
              - generic [ref=e169]: "4"
            - button "Evasion 2" [ref=e171] [cursor=pointer]:
              - text: Evasion
              - generic [ref=e174]: "2"
            - button "Crypto 2" [ref=e176] [cursor=pointer]:
              - text: Crypto
              - generic [ref=e179]: "2"
            - button "Exfiltration 3" [ref=e181] [cursor=pointer]:
              - text: Exfiltration
              - generic [ref=e184]: "3"
            - button "Forensics 7" [ref=e186] [cursor=pointer]:
              - text: Forensics
              - generic [ref=e189]: "7"
            - button "AI/ML Runtime 3" [ref=e191] [cursor=pointer]:
              - text: AI/ML Runtime
              - generic [ref=e194]: "3"
            - button "Sandbox 5" [ref=e196] [cursor=pointer]:
              - text: Sandbox
              - generic [ref=e199]: "5"
            - button "Plugin 2" [ref=e201] [cursor=pointer]:
              - text: Plugin
              - generic [ref=e204]: "2"
            - button "Audit 4" [ref=e206] [cursor=pointer]:
              - text: Audit
              - generic [ref=e209]: "4"
            - button "Registry 3" [ref=e211] [cursor=pointer]:
              - text: Registry
              - generic [ref=e214]: "3"
            - button "Exploitation 2" [ref=e216] [cursor=pointer]:
              - text: Exploitation
              - generic [ref=e219]: "2"
        - generic [ref=e221]:
          - generic [ref=e222]: Obfuscation Passes
          - generic [ref=e225]: MLIR PASSES
          - generic [ref=e226]:
            - generic [ref=e227]:
              - generic [ref=e228]:
                - generic [ref=e229]: String Encryption
                - generic [ref=e230]: Medium
                - generic [ref=e231]: +25%
              - generic [ref=e232]: Encrypts all string literals with XOR/RC4
            - generic [ref=e233] [cursor=pointer]
          - generic [ref=e235]:
            - generic [ref=e236]:
              - generic [ref=e237]:
                - generic [ref=e238]: Constant Obfuscation
                - generic [ref=e239]: Medium
                - generic [ref=e240]: +25%
              - generic [ref=e241]: Replaces numeric constants with opaque expressions
            - generic [ref=e242] [cursor=pointer]
          - generic [ref=e244]:
            - generic [ref=e245]:
              - generic [ref=e246]:
                - generic [ref=e247]: Symbol Obfuscation
                - generic [ref=e248]: Medium
                - generic [ref=e249]: +25%
              - generic [ref=e250]: Renames internal symbols to randomized identifiers
            - generic [ref=e251] [cursor=pointer]
          - generic [ref=e253]:
            - generic [ref=e254]:
              - generic [ref=e255]:
                - generic [ref=e256]: Cryptographic Hashing
                - generic [ref=e257]: Medium
                - generic [ref=e258]: +25%
              - generic [ref=e259]: Uses crypto hashing for symbol obfuscation verification
            - generic [ref=e260] [cursor=pointer]
          - generic [ref=e262]:
            - generic [ref=e263]:
              - generic [ref=e264]:
                - generic [ref=e265]: SCF Region Obfuscation
                - generic [ref=e266]: Medium
                - generic [ref=e267]: +25%
              - generic [ref=e268]: Applies opaque predicates to structured control flow
            - generic [ref=e269] [cursor=pointer]
          - generic [ref=e271]:
            - generic [ref=e272]:
              - generic [ref=e273]:
                - generic [ref=e274]: Import Obfuscation
                - generic [ref=e275]: Medium
                - generic [ref=e276]: +25%
              - generic [ref=e277]: Hides imports behind wrapper functions
            - generic [ref=e278] [cursor=pointer]
          - generic [ref=e280]: LLVM PASSES
          - generic [ref=e281]:
            - generic [ref=e282]:
              - generic [ref=e283]:
                - generic [ref=e284]: Strip Signatures
                - generic [ref=e285]: Medium
                - generic [ref=e286]: +25%
              - generic [ref=e287]: Removes debug metadata and function signature info
            - generic [ref=e288] [cursor=pointer]
          - generic [ref=e290]:
            - generic [ref=e291]:
              - generic [ref=e292]:
                - generic [ref=e293]: PDATA Strip
                - generic [ref=e294]: Medium
                - generic [ref=e295]: +25%
              - generic [ref=e296]: Removes exception handling and unwinding metadata
            - generic [ref=e297] [cursor=pointer]
          - generic [ref=e299]:
            - generic [ref=e300]:
              - generic [ref=e301]:
                - generic [ref=e302]: Function Virtualization
                - generic [ref=e303]: High
                - generic [ref=e304]: +80-120%
              - generic [ref=e305]: Converts functions to virtualized bytecode dispatchers
            - generic [ref=e306] [cursor=pointer]
          - generic [ref=e308]:
            - generic [ref=e309]:
              - generic [ref=e310]:
                - generic [ref=e311]: Opaque Predicates
                - generic [ref=e312]: Medium
                - generic [ref=e313]: +25%
              - generic [ref=e314]: Injects opaque predicates to obfuscate control flow
            - generic [ref=e315] [cursor=pointer]
          - generic [ref=e317]:
            - generic [ref=e318]:
              - generic [ref=e319]:
                - generic [ref=e320]: Instruction Substitution
                - generic [ref=e321]: Medium
                - generic [ref=e322]: +35-50%
              - generic [ref=e323]: Replaces standard instructions with complex sequences
            - generic [ref=e324] [cursor=pointer]
          - generic [ref=e326]:
            - generic [ref=e327]:
              - generic [ref=e328]:
                - generic [ref=e329]: Bogus Control Flow
                - generic [ref=e330]: High
                - generic [ref=e331]: +50-70%
              - generic [ref=e332]: Inserts opaque predicates and dead code branches
            - generic [ref=e333] [cursor=pointer]
          - generic [ref=e335]:
            - generic [ref=e336]:
              - generic [ref=e337]:
                - generic [ref=e338]: Control Flow Flattening
                - generic [ref=e339]: High
                - generic [ref=e340]: +60-80%
              - generic [ref=e341]: Converts structured control flow to switch dispatcher
            - generic [ref=e342] [cursor=pointer]
          - generic [ref=e344]:
            - generic [ref=e345]:
              - generic [ref=e346]:
                - generic [ref=e347]: Linear MBA
                - generic [ref=e348]: Medium
                - generic [ref=e349]: +25%
              - generic [ref=e350]: Converts arithmetic operations to mixed-boolean arithmetic
            - generic [ref=e351] [cursor=pointer]
          - generic [ref=e353]:
            - generic [ref=e354]:
              - generic [ref=e355]:
                - generic [ref=e356]: Anti-Debug Protection
                - generic [ref=e357]: Medium
                - generic [ref=e358]: +25%
              - generic [ref=e359]: Injects debugger detection and anti-debugging techniques
            - generic [ref=e360] [cursor=pointer]
          - generic [ref=e362]:
            - generic [ref=e363]:
              - generic [ref=e364]:
                - generic [ref=e365]: Indirect Calls
                - generic [ref=e366]: Medium
                - generic [ref=e367]: +25%
              - generic [ref=e368]: Converts direct calls to indirect via function pointers
            - generic [ref=e369] [cursor=pointer]
    - generic [ref=e371]:
      - generic [ref=e372] [cursor=pointer]: Build Output
      - generic [ref=e379]: No build output yet. Click Compile to start.
  - generic [ref=e381]:
    - alert
    - alert
```

# Test source

```ts
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
  111 |       return editors?.[0]?.getValue() || '';
  112 |     });
  113 | 
  114 |     const category = page.locator('button', { hasText: 'Anti-Analysis' });
  115 |     await category.click();
  116 |     await page.waitForTimeout(300);
  117 | 
  118 |     const apiButton = page.locator('button', { hasText: 'is_vm' });
  119 |     await apiButton.click();
  120 |     await page.waitForTimeout(500);
  121 | 
  122 |     const afterContent = await page.evaluate(() => {
  123 |       const editors = (window as any).monaco?.editor?.getEditors?.();
  124 |       return editors?.[0]?.getValue() || '';
  125 |     });
  126 | 
  127 |     expect(afterContent.length).toBeGreaterThan(beforeContent.length);
  128 |   });
  129 | 
  130 |   test('Obfuscation panel exists with MLIR and LLVM sections', async ({ page }) => {
  131 |     await expect(page.locator('text=Obfuscation Passes')).toBeVisible();
  132 |     await expect(page.locator('text=MLIR PASSES')).toBeVisible();
  133 |     await expect(page.locator('text=LLVM PASSES')).toBeVisible();
  134 |   });
  135 | 
  136 |   test('obfuscation passes are listed with toggle switches', async ({ page }) => {
  137 |     await expect(page.locator('text=String Encrypt')).toBeVisible();
  138 |     await expect(page.locator('text=Bogus Control Flow')).toBeVisible();
  139 |     await expect(page.locator('text=Control Flow Flattening')).toBeVisible();
  140 |   });
  141 | 
  142 |   test('toggling obfuscation pass changes visual state', async ({ page }) => {
  143 |     const stringEncryptRow = page.locator('div').filter({ hasText: /^String Encrypt/ }).first();
  144 |     const toggle = stringEncryptRow.locator('div[style*="cursor: pointer"]').first();
  145 | 
  146 |     const bgBefore = await toggle.evaluate((el) => getComputedStyle(el).backgroundColor);
  147 | 
  148 |     await toggle.click();
  149 |     await page.waitForTimeout(300);
  150 | 
  151 |     const bgAfter = await toggle.evaluate((el) => getComputedStyle(el).backgroundColor);
  152 |     expect(bgAfter).not.toBe(bgBefore);
  153 |   });
  154 | 
  155 |   test('Compile button exists', async ({ page }) => {
  156 |     const compileBtn = page.locator('button', { hasText: 'Compile' });
  157 |     await expect(compileBtn).toBeVisible();
  158 |   });
  159 | 
  160 |   test('Build Output panel exists', async ({ page }) => {
> 161 |     await expect(page.locator('text=Build Output')).toBeVisible();
      |                                                     ^ Error: expect(locator).toBeVisible() failed
  162 |     await expect(page.locator('text=No build output yet')).toBeVisible();
  163 |   });
  164 | 
  165 |   test('switching tabs preserves editor state independently', async ({ page }) => {
  166 |     await page.waitForTimeout(2000);
  167 | 
  168 |     const winContent = await page.evaluate(() => {
  169 |       const editors = (window as any).monaco?.editor?.getEditors?.();
  170 |       return editors?.[0]?.getValue() || '';
  171 |     });
  172 | 
  173 |     await page.locator('button', { hasText: 'Linux' }).click();
  174 |     await page.waitForTimeout(1000);
  175 | 
  176 |     const linuxContent = await page.evaluate(() => {
  177 |       const editors = (window as any).monaco?.editor?.getEditors?.();
  178 |       return editors?.[0]?.getValue() || '';
  179 |     });
  180 | 
  181 |     expect(winContent).not.toBe(linuxContent);
  182 |     expect(winContent).toContain('DRIVER_CHAIN');
  183 |     expect(linuxContent).toContain('LKM_PATHS');
  184 | 
  185 |     await page.locator('button', { hasText: 'Windows' }).click();
  186 |     await page.waitForTimeout(1000);
  187 | 
  188 |     const winContentAgain = await page.evaluate(() => {
  189 |       const editors = (window as any).monaco?.editor?.getEditors?.();
  190 |       return editors?.[0]?.getValue() || '';
  191 |     });
  192 | 
  193 |     expect(winContentAgain).toBe(winContent);
  194 |   });
  195 | 
  196 |   test('page works at mobile width', async ({ page }) => {
  197 |     await page.setViewportSize({ width: 375, height: 812 });
  198 |     await page.waitForTimeout(500);
  199 | 
  200 |     await expect(page.locator('text=JOCKY')).toBeVisible();
  201 |     await expect(page.locator('button', { hasText: 'Windows' })).toBeVisible();
  202 |     await expect(page.locator('button', { hasText: 'Compile' })).toBeVisible();
  203 | 
  204 |     const body = page.locator('body');
  205 |     const scrollWidth = await body.evaluate((el) => el.scrollWidth);
  206 |     const clientWidth = await body.evaluate((el) => el.clientWidth);
  207 |     expect(scrollWidth).toBeLessThanOrEqual(clientWidth + 5);
  208 |   });
  209 | });
  210 | 
```