# Instructions

- Following Playwright test failed.
- Explain why, be concise, respect Playwright best practices.
- Provide a snippet of code with the fix, if possible.

# Test info

- Name: app.spec.ts >> JOCKY Frontend >> page works at mobile width
- Location: tests/app.spec.ts:196:3

# Error details

```
Error: expect(locator).toBeVisible() failed

Locator: locator('text=JOCKY')
Expected: visible
Error: strict mode violation: locator('text=JOCKY') resolved to 2 elements:
    1) <span>JOCKY</span> aka getByText('JOCKY', { exact: true })
    2) <div data-mprt="8" aria-hidden="true" role="presentation" class="view-lines monaco-mouse-cursor-text">…</div> aka getByText('// JOCKY Windows Researc')

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
            - generic [ref=e32]: "1"
            - generic [aria-hidden] [ref=e130]:
              - generic [ref=e131]: /
              - generic [ref=e133]: /
              - generic [ref=e137]: J
              - generic [ref=e139]: O
              - generic [ref=e141]: C
              - generic [ref=e143]: K
              - generic [ref=e145]: "Y"
              - generic [ref=e149]: W
              - generic [ref=e151]: i
              - generic [ref=e153]: "n"
              - generic [ref=e155]: d
              - generic [ref=e157]: o
              - generic [ref=e159]: w
              - generic [ref=e161]: s
              - generic [ref=e165]: R
              - generic [ref=e167]: e
              - generic [ref=e169]: s
              - generic [ref=e171]: e
              - generic [ref=e173]: a
              - generic [ref=e175]: r
              - generic [ref=e177]: c
      - generic [ref=e180]:
        - generic [ref=e181]:
          - generic [ref=e182]: Runtime APIs
          - button "Anti-Analysis 5" [ref=e186] [cursor=pointer]:
            - text: Anti-Analysis
            - generic [ref=e189]: "5"
          - button "Crypto 2" [ref=e191] [cursor=pointer]:
            - text: Crypto
            - generic [ref=e194]: "2"
          - button "Exfiltration 3" [ref=e196] [cursor=pointer]:
            - text: Exfiltration
            - generic [ref=e199]: "3"
          - button "AI/ML 3" [ref=e201] [cursor=pointer]:
            - text: AI/ML
            - generic [ref=e204]: "3"
          - button "Sandbox 5" [ref=e206] [cursor=pointer]:
            - text: Sandbox
            - generic [ref=e209]: "5"
          - button "Plugin 2" [ref=e211] [cursor=pointer]:
            - text: Plugin
            - generic [ref=e214]: "2"
          - button "Audit 4" [ref=e216] [cursor=pointer]:
            - text: Audit
            - generic [ref=e219]: "4"
          - button "BYOVD 4" [ref=e221] [cursor=pointer]:
            - text: BYOVD
            - generic [ref=e224]: "4"
          - button "Evasion 2" [ref=e226] [cursor=pointer]:
            - text: Evasion
            - generic [ref=e229]: "2"
          - button "Registry 3" [ref=e231] [cursor=pointer]:
            - text: Registry
            - generic [ref=e234]: "3"
          - button "Forensics (Win) 7" [ref=e236] [cursor=pointer]:
            - text: Forensics (Win)
            - generic [ref=e239]: "7"
          - button "Exploitation 2" [ref=e241] [cursor=pointer]:
            - text: Exploitation
            - generic [ref=e244]: "2"
        - generic [ref=e245]:
          - generic [ref=e246]: Obfuscation Passes
          - generic [ref=e249]: MLIR PASSES
          - generic [ref=e250]:
            - generic [ref=e251]:
              - generic [ref=e252]: String Encrypt
              - generic [ref=e253]: Encrypts string constants in MLIR
            - generic [ref=e254] [cursor=pointer]
          - generic [ref=e256]:
            - generic [ref=e257]:
              - generic [ref=e258]: Constant Obfuscate
              - generic [ref=e259]: Obfuscates numeric constants
            - generic [ref=e260] [cursor=pointer]
          - generic [ref=e262]:
            - generic [ref=e263]:
              - generic [ref=e264]: Symbol Obfuscate
              - generic [ref=e265]: Renames symbols to random identifiers
            - generic [ref=e266] [cursor=pointer]
          - generic [ref=e268]: LLVM PASSES
          - generic [ref=e269]:
            - generic [ref=e270]:
              - generic [ref=e271]: Bogus Control Flow
              - generic [ref=e272]: Inserts fake control flow paths
            - generic [ref=e273] [cursor=pointer]
          - generic [ref=e275]:
            - generic [ref=e276]:
              - generic [ref=e277]: Control Flow Flattening
              - generic [ref=e278]: Flattens function control flow into switch-based dispatch
            - generic [ref=e279] [cursor=pointer]
          - generic [ref=e281]:
            - generic [ref=e282]:
              - generic [ref=e283]: Instruction Substitution
              - generic [ref=e284]: Replaces instructions with equivalent complex sequences
            - generic [ref=e285] [cursor=pointer]
          - generic [ref=e287]:
            - generic [ref=e288]:
              - generic [ref=e289]: Basic Block Splitting
              - generic [ref=e290]: Splits basic blocks into smaller fragments
            - generic [ref=e291] [cursor=pointer]
          - generic [ref=e293]:
            - generic [ref=e294]:
              - generic [ref=e295]: Indirect Calls
              - generic [ref=e296]: Replaces direct calls with indirect function pointers
            - generic [ref=e297] [cursor=pointer]
          - generic [ref=e299]:
            - generic [ref=e300]:
              - generic [ref=e301]: Strip Signatures
              - generic [ref=e302]: Removes function signature metadata
            - generic [ref=e303] [cursor=pointer]
    - generic [ref=e305]:
      - generic [ref=e306] [cursor=pointer]: Build Output
      - generic [ref=e313]: No build output yet. Click Compile to start.
  - generic [ref=e315]:
    - alert
    - alert
```

# Test source

```ts
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
  161 |     await expect(page.locator('text=Build Output')).toBeVisible();
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
> 200 |     await expect(page.locator('text=JOCKY')).toBeVisible();
      |                                              ^ Error: expect(locator).toBeVisible() failed
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