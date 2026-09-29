$ErrorActionPreference = 'Stop'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"
$packageName = "jocky"

# Uninstall binaries
Uninstall-BinFile -Name "jockyc" -Path "$toolsDir\bin\jockyc.exe" | Out-Null
Uninstall-BinFile -Name "jocky" | Out-Null

Write-Host "JOCKY compiler uninstalled successfully." -ForegroundColor Green
