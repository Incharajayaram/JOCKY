$ErrorActionPreference = 'Stop'

$toolsDir = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"
$packageName = "jocky"

# Download URL and checksum for the release
$url64 = "https://github.com/devalgupta/JOCKY/releases/download/v0.1.0/jocky-windows-x64.zip"
$url32 = "https://github.com/devalgupta/JOCKY/releases/download/v0.1.0/jocky-windows-x86.zip"

# Update these checksums when releasing
$checksum64 = "0d1c95f68cd12d75a3dd3fa5a5b7e4d3a2b8c9f1e2d3c4b5a6f7e8d9c0a1b2c"
$checksum32 = "1e2d3c4b5a6f7e8d9c0a1b2c0d1c95f68cd12d75a3dd3fa5a5b7e4d3a2b8c9"
$checksumType = "sha256"

$packageArgs = @{
    packageName   = $packageName
    unzipLocation = $toolsDir
    url64bit      = $url64
    url32bit      = $url32
    checksum64    = $checksum64
    checksum32    = $checksum32
    checksumType  = $checksumType
}

Install-ChocolateyZipPackage @packageArgs

# Install binary to PATH
$binDir = Join-Path $toolsDir "bin"
if (Test-Path "$binDir\jockyc.exe") {
    Install-BinFile -Name "jockyc" -Path "$binDir\jockyc.exe" -UseOriginalLocation
}

# Install Python wrapper if available
$pythonCliPath = Join-Path $toolsDir "jocky"
if (Test-Path "$pythonCliPath\cli.py") {
    # Create a batch wrapper for the Python CLI
    $batchWrapper = Join-Path (Split-Path -parent $toolsDir) "jocky.bat"
    @"
@echo off
python "$pythonCliPath\cli.py" %*
"@ | Set-Content $batchWrapper -Encoding ASCII

    Install-BinFile -Name "jocky" -Path $batchWrapper -UseOriginalLocation
}

Write-Host "JOCKY compiler installed successfully!" -ForegroundColor Green
Write-Host "Verify installation with: jockyc --version" -ForegroundColor Cyan
