# Instructions

- Following Playwright test failed.
- Explain why, be concise, respect Playwright best practices.
- Provide a snippet of code with the fix, if possible.

# Test info

- Name: app.spec.ts >> JOCKY Frontend >> switching tabs preserves editor state independently
- Location: tests/app.spec.ts:165:3

# Error details

```
Error: page.waitForLoadState: Target page, context or browser has been closed
```

# Test source

```ts
  1   | import { test, expect } from '@playwright/test';
  2   | 
  3   | test.describe('JOCKY Frontend', () => {
  4   |   test.beforeEach(async ({ page }) => {
  5   |     await page.goto('/');
> 6   |     await page.waitForLoadState('networkidle');
      |                ^ Error: page.waitForLoadState: Target page, context or browser has been closed
  7   |   });
  8   | 
  9   |   test('page loads with dark theme and branding', async ({ page }) => {
  10  |     await expect(page.locator('text=JOCKY')).toBeVisible();
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
```