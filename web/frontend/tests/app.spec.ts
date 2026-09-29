import { test, expect } from '@playwright/test';

test.describe('JOCKY Frontend', () => {
  test.beforeEach(async ({ page }) => {
    await page.goto('/');
    await page.waitForLoadState('networkidle');
  });

  test('page loads with dark theme and branding', async ({ page }) => {
    await expect(page.locator('text=JOCKY')).toBeVisible();
    await expect(page.locator('text=v1.0')).toBeVisible();
    const bg = await page.evaluate(() => getComputedStyle(document.body).backgroundColor);
    expect(bg).not.toBe('rgb(255, 255, 255)');
  });

  test('two platform tabs exist', async ({ page }) => {
    const windowsTab = page.locator('button', { hasText: 'Windows' });
    const linuxTab = page.locator('button', { hasText: 'Linux' });
    await expect(windowsTab).toBeVisible();
    await expect(linuxTab).toBeVisible();
  });

  test('default tab is Windows with demo script loaded', async ({ page }) => {
    const windowsTab = page.locator('button', { hasText: 'Windows' });
    const borderBottom = await windowsTab.evaluate((el) => getComputedStyle(el).borderBottomColor);
    expect(borderBottom).not.toBe('rgba(0, 0, 0, 0)');

    await expect(page.locator('text=payload_win.jky')).toBeVisible();
  });

  test('Windows demo script is comprehensive (not a stub)', async ({ page }) => {
    await page.waitForTimeout(2000);
    const editorContent = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      if (editors && editors.length > 0) {
        return editors[0].getValue();
      }
      return '';
    });
    expect(editorContent.length).toBeGreaterThan(5000);
    expect(editorContent).toContain('phase_init');
    expect(editorContent).toContain('phase_byovd');
    expect(editorContent).toContain('DRIVER_CHAIN');
    expect(editorContent).toContain('phase_forensic_cleanup');
    expect(editorContent).toContain('phase_exfiltration');
  });

  test('switching to Linux tab shows Linux demo script', async ({ page }) => {
    await page.locator('button', { hasText: 'Linux' }).click();
    await expect(page.locator('text=payload_linux.jky')).toBeVisible();

    await page.waitForTimeout(1000);
    const editorContent = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      if (editors && editors.length > 0) {
        return editors[0].getValue();
      }
      return '';
    });
    expect(editorContent).toContain('LKM_PATHS');
    expect(editorContent).toContain('EBPF_PROGRAMS');
    expect(editorContent).toContain('phase_lkm_loading');
    expect(editorContent).toContain('phase_process_hollowing');
  });

  test('Linux demo script is comprehensive (not a stub)', async ({ page }) => {
    await page.locator('button', { hasText: 'Linux' }).click();
    await page.waitForTimeout(1000);
    const editorContent = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      if (editors && editors.length > 0) {
        return editors[0].getValue();
      }
      return '';
    });
    expect(editorContent.length).toBeGreaterThan(5000);
    expect(editorContent).toContain('phase_ebpf_loading');
    expect(editorContent).toContain('phase_threat_assessment');
    expect(editorContent).toContain('phase_self_delete');
  });

  test('Monaco editor is present and functional', async ({ page }) => {
    await page.waitForTimeout(2000);
    const monacoPresent = await page.evaluate(() => {
      return !!(window as any).monaco?.editor?.getEditors?.()?.length;
    });
    expect(monacoPresent).toBe(true);

    const editorContainer = page.locator('.monaco-editor');
    await expect(editorContainer.first()).toBeVisible();
  });

  test('Runtime API panel exists with categorized buttons', async ({ page }) => {
    await expect(page.locator('text=Runtime APIs')).toBeVisible();
    await expect(page.locator('text=Anti-Analysis')).toBeVisible();
  });

  test('clicking a runtime API category reveals API buttons', async ({ page }) => {
    const category = page.locator('button', { hasText: 'Anti-Analysis' });
    await category.click();

    await expect(page.locator('button', { hasText: 'check_analysis_environment' })).toBeVisible();
    await expect(page.locator('button', { hasText: 'is_debugger_present' })).toBeVisible();
  });

  test('clicking a runtime API button inserts code into editor', async ({ page }) => {
    await page.waitForTimeout(2000);

    const beforeContent = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      return editors?.[0]?.getValue() || '';
    });

    const category = page.locator('button', { hasText: 'Anti-Analysis' });
    await category.click();
    await page.waitForTimeout(300);

    const apiButton = page.locator('button', { hasText: 'is_vm' });
    await apiButton.click();
    await page.waitForTimeout(500);

    const afterContent = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      return editors?.[0]?.getValue() || '';
    });

    expect(afterContent.length).toBeGreaterThan(beforeContent.length);
  });

  test('Obfuscation panel exists with MLIR and LLVM sections', async ({ page }) => {
    await expect(page.locator('text=Obfuscation Passes')).toBeVisible();
    await expect(page.locator('text=MLIR PASSES')).toBeVisible();
    await expect(page.locator('text=LLVM PASSES')).toBeVisible();
  });

  test('obfuscation passes are listed with toggle switches', async ({ page }) => {
    await expect(page.locator('text=String Encrypt')).toBeVisible();
    await expect(page.locator('text=Bogus Control Flow')).toBeVisible();
    await expect(page.locator('text=Control Flow Flattening')).toBeVisible();
  });

  test('toggling obfuscation pass changes visual state', async ({ page }) => {
    const stringEncryptRow = page.locator('div').filter({ hasText: /^String Encrypt/ }).first();
    const toggle = stringEncryptRow.locator('div[style*="cursor: pointer"]').first();

    const bgBefore = await toggle.evaluate((el) => getComputedStyle(el).backgroundColor);

    await toggle.click();
    await page.waitForTimeout(300);

    const bgAfter = await toggle.evaluate((el) => getComputedStyle(el).backgroundColor);
    expect(bgAfter).not.toBe(bgBefore);
  });

  test('Compile button exists', async ({ page }) => {
    const compileBtn = page.locator('button', { hasText: 'Compile' });
    await expect(compileBtn).toBeVisible();
  });

  test('Build Output panel exists', async ({ page }) => {
    await expect(page.locator('text=Build Output')).toBeVisible();
    await expect(page.locator('text=No build output yet')).toBeVisible();
  });

  test('switching tabs preserves editor state independently', async ({ page }) => {
    await page.waitForTimeout(2000);

    const winContent = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      return editors?.[0]?.getValue() || '';
    });

    await page.locator('button', { hasText: 'Linux' }).click();
    await page.waitForTimeout(1000);

    const linuxContent = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      return editors?.[0]?.getValue() || '';
    });

    expect(winContent).not.toBe(linuxContent);
    expect(winContent).toContain('DRIVER_CHAIN');
    expect(linuxContent).toContain('LKM_PATHS');

    await page.locator('button', { hasText: 'Windows' }).click();
    await page.waitForTimeout(1000);

    const winContentAgain = await page.evaluate(() => {
      const editors = (window as any).monaco?.editor?.getEditors?.();
      return editors?.[0]?.getValue() || '';
    });

    expect(winContentAgain).toBe(winContent);
  });

  test('page works at mobile width', async ({ page }) => {
    await page.setViewportSize({ width: 375, height: 812 });
    await page.waitForTimeout(500);

    await expect(page.locator('text=JOCKY')).toBeVisible();
    await expect(page.locator('button', { hasText: 'Windows' })).toBeVisible();
    await expect(page.locator('button', { hasText: 'Compile' })).toBeVisible();

    const body = page.locator('body');
    const scrollWidth = await body.evaluate((el) => el.scrollWidth);
    const clientWidth = await body.evaluate((el) => el.clientWidth);
    expect(scrollWidth).toBeLessThanOrEqual(clientWidth + 5);
  });
});
