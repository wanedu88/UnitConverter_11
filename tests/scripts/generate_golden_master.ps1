# Generate tests/golden_master_expected.txt from UnitConverter CLI stdout.
# Usage: .\tests\scripts\generate_golden_master.ps1 [-BuildDir build] [-ExeName UnitConverter.exe]

param(
    [string]$BuildDir = "build",
    [string]$ExeName = "UnitConverter.exe"
)

$ErrorActionPreference = "Stop"
$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$ExePath = Join-Path (Join-Path $RepoRoot $BuildDir) $ExeName
$OutFile = Join-Path $RepoRoot "tests\golden_master_expected.txt"
$Prompt = "Insert value for converting (ex: meter:2.5): "
$Scenarios = @("meter:2.5", "feet:1.0", "yard:1.0", "meter:0.0")

if (-not (Test-Path $ExePath)) {
    Write-Error "Executable not found: $ExePath (build with CMake first)"
}

function Get-ConversionLines {
    param([string]$Scenario)
    $inputFile = New-TemporaryFile
    Set-Content -Path $inputFile -Value $Scenario -NoNewline
    $actualFile = New-TemporaryFile
    Get-Content $inputFile | & $ExePath *> $actualFile.FullName
    $raw = Get-Content $actualFile.FullName -Raw
    Remove-Item $inputFile, $actualFile -Force

    $lines = @()
    foreach ($line in ($raw -split "`r?`n")) {
        if ([string]::IsNullOrWhiteSpace($line)) { continue }
        if ($line.StartsWith($Prompt)) {
            $line = $line.Substring($Prompt.Length)
        }
        if ($line -notmatch " = ") { continue }
        if ($line -match "^(.+)\s(\S+)\s=\s(.+)\s(\S+)$") {
            if ($Matches[2] -eq $Matches[4]) { continue }
        }
        $lines += $line
    }
    return $lines
}

$sb = New-Object System.Text.StringBuilder
for ($i = 0; $i -lt $Scenarios.Count; $i++) {
    $scenario = $Scenarios[$i]
    [void]$sb.AppendLine("[$scenario]")
    foreach ($line in (Get-ConversionLines $scenario)) {
        [void]$sb.AppendLine($line)
    }
    if ($i -lt $Scenarios.Count - 1) {
        [void]$sb.AppendLine("---")
    }
}

$utf8NoBom = New-Object System.Text.UTF8Encoding $false
[System.IO.File]::WriteAllText($OutFile, $sb.ToString().TrimEnd() + "`n", $utf8NoBom)
Write-Host "Wrote $OutFile"
Write-Host "Next: git add tests/golden_master_expected.txt"
