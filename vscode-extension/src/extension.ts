import * as vscode from 'vscode';
import { JockyCompletionProvider } from './providers/completionProvider';
import { JockyHoverProvider } from './providers/hoverProvider';
import { JockyDiagnosticsProvider } from './providers/diagnosticsProvider';

let diagnosticsProvider: JockyDiagnosticsProvider;

export function activate(context: vscode.ExtensionContext) {
  const jockySelector = { language: 'jocky', scheme: 'file' };

  diagnosticsProvider = new JockyDiagnosticsProvider(context);

  const completionProvider = new JockyCompletionProvider();
  const hoverProvider = new JockyHoverProvider();

  context.subscriptions.push(
    vscode.languages.registerCompletionItemProvider(
      jockySelector,
      completionProvider,
      '.',
      '_'
    )
  );

  context.subscriptions.push(
    vscode.languages.registerHoverProvider(
      jockySelector,
      hoverProvider
    )
  );

  context.subscriptions.push(
    vscode.workspace.onDidChangeTextDocument((event) => {
      if (event.document.languageId === 'jocky') {
        diagnosticsProvider.provideDiagnostics(event.document);
      }
    })
  );

  context.subscriptions.push(
    vscode.workspace.onDidOpenTextDocument((document) => {
      if (document.languageId === 'jocky') {
        diagnosticsProvider.provideDiagnostics(document);
      }
    })
  );

  vscode.workspace.textDocuments.forEach((document) => {
    if (document.languageId === 'jocky') {
      diagnosticsProvider.provideDiagnostics(document);
    }
  });

  registerCommands(context);
}

function registerCommands(context: vscode.ExtensionContext): void {
  context.subscriptions.push(
    vscode.commands.registerCommand('jocky.compile', async () => {
      const editor = vscode.window.activeTextEditor;
      if (!editor) {
        vscode.window.showErrorMessage('No active editor');
        return;
      }

      const document = editor.document;
      if (document.languageId !== 'jocky') {
        vscode.window.showErrorMessage('Active file is not a JOCKY file');
        return;
      }

      vscode.window.showInformationMessage('JOCKY compilation not yet implemented');
    })
  );
}

export function deactivate() {}
