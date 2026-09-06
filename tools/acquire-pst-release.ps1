param(
    [string]$PinFile,
    [Parameter(Mandatory = $true)]
    [string]$OutputRoot
)

$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrWhiteSpace($PinFile)) {
    $PinFile = Join-Path $PSScriptRoot '..\dependencies\papinho-secure-transport.txt'
}

function Read-KeyValueFile([string]$Path) {
    $values = @{}
    foreach ($line in Get-Content -LiteralPath $Path) {
        if ([string]::IsNullOrWhiteSpace($line) -or $line.TrimStart().StartsWith('#')) { continue }
        $separator = $line.IndexOf('=')
        if ($separator -le 0) {
            throw "Invalid or duplicate key/value entry in $Path"
        }
        $key = $line.Substring(0, $separator)
        $value = $line.Substring($separator + 1)
        if ($values.ContainsKey($key)) {
            throw "Invalid or duplicate key/value entry in $Path"
        }
        $values.Add($key, $value)
    }
    return $values
}

function Require-Exact([hashtable]$Values, [string]$Key, [string]$Expected, [string]$Source) {
    if (-not $Values.ContainsKey($Key) -or $Values[$Key] -cne $Expected) {
        throw "$Source requires $Key=$Expected"
    }
}

$pin = Read-KeyValueFile $PinFile
$requiredKeys = @('repository', 'release_tag', 'target', 'asset', 'sha256')
foreach ($key in $requiredKeys) {
    if (-not $pin.ContainsKey($key) -or [string]::IsNullOrWhiteSpace($pin[$key])) {
        throw "Missing required key '$key' in $PinFile"
    }
}
if ($pin.Count -ne $requiredKeys.Count) { throw "Unexpected key in $PinFile" }
if ($pin['sha256'] -notmatch '^[0-9a-f]{64}$') { throw 'The pinned SHA-256 is invalid' }
if ($pin['repository'] -notmatch '^https://github\.com/[^/]+/[^/]+$') { throw 'The pinned repository URL is invalid' }

$releaseUrl = $pin['repository'] + '/releases/download/' + $pin['release_tag'] + '/' + $pin['asset']
$canonicalOutputRoot = [IO.Path]::GetFullPath($OutputRoot).TrimEnd('\')
$targetRoot = Join-Path $canonicalOutputRoot ($pin['release_tag'] + '\' + $pin['target'])
$downloadRoot = Join-Path $canonicalOutputRoot 'downloads'
$zipPath = Join-Path $downloadRoot $pin['asset']
$temporaryRoot = $targetRoot + '.extracting-' + [Guid]::NewGuid().ToString('N')
if (-not $targetRoot.StartsWith($canonicalOutputRoot + '\', [StringComparison]::OrdinalIgnoreCase) -or
    -not $temporaryRoot.StartsWith($canonicalOutputRoot + '\', [StringComparison]::OrdinalIgnoreCase)) {
    throw 'Computed staging paths escaped the requested output root'
}

New-Item -ItemType Directory -Force -Path $downloadRoot | Out-Null
Write-Output "PST_DOWNLOAD_URL=$releaseUrl"
Invoke-WebRequest -UseBasicParsing -Uri $releaseUrl -OutFile $zipPath
$actualHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $zipPath).Hash.ToLowerInvariant()
if ($actualHash -cne $pin['sha256']) { throw "External SHA-256 mismatch: $actualHash" }
Write-Output "PST_EXTERNAL_SHA256=$actualHash"

New-Item -ItemType Directory -Force -Path $temporaryRoot | Out-Null
Expand-Archive -LiteralPath $zipPath -DestinationPath $temporaryRoot

foreach ($name in @('manifest.ini', 'VERSION', 'consumer-link.ini', 'SHA256SUMS.txt')) {
    if (-not (Test-Path -LiteralPath (Join-Path $temporaryRoot $name) -PathType Leaf)) {
        throw "Required PST package file is missing: $name"
    }
}

$manifest = Read-KeyValueFile (Join-Path $temporaryRoot 'manifest.ini')
$version = Read-KeyValueFile (Join-Path $temporaryRoot 'VERSION')
$link = Read-KeyValueFile (Join-Path $temporaryRoot 'consumer-link.ini')
Require-Exact $manifest 'package_version' '0.4.0' 'manifest.ini'
Require-Exact $manifest 'library_version' '0.4.0' 'manifest.ini'
Require-Exact $manifest 'api_version' '1.3.0' 'manifest.ini'
Require-Exact $manifest 'spi_version' '2.4' 'manifest.ini'
Require-Exact $manifest 'target_id' $pin['target'] 'manifest.ini'
Require-Exact $manifest 'linkage' 'static' 'manifest.ini'
Require-Exact $version 'package_version' '0.4.0' 'VERSION'
Require-Exact $version 'library_version' '0.4.0' 'VERSION'
Require-Exact $version 'api_version' '1.3.0' 'VERSION'
Require-Exact $version 'spi_version' '2.4' 'VERSION'
Require-Exact $link 'target_id' $pin['target'] 'consumer-link.ini'
Require-Exact $link 'link_libraries' 'papinho_secure_transport.lib,libssl.lib,libcrypto.lib,ws2_32.lib,crypt32.lib' 'consumer-link.ini'
Require-Exact $link 'runtime_files' 'libssl-3-x64.dll,libcrypto-3-x64.dll' 'consumer-link.ini'

foreach ($checksumLine in Get-Content -LiteralPath (Join-Path $temporaryRoot 'SHA256SUMS.txt')) {
    if ($checksumLine -notmatch '^([0-9a-f]{64})  (.+)$') { throw 'Invalid SHA256SUMS.txt entry' }
    $expected = $Matches[1]
    $relative = $Matches[2].Replace('/', [IO.Path]::DirectorySeparatorChar)
    $file = Join-Path $temporaryRoot $relative
    if (-not (Test-Path -LiteralPath $file -PathType Leaf)) { throw "Missing hashed package file: $relative" }
    $hash = (Get-FileHash -Algorithm SHA256 -LiteralPath $file).Hash.ToLowerInvariant()
    if ($hash -cne $expected) { throw "Internal SHA-256 mismatch: $relative" }
}

if (Test-Path -LiteralPath $targetRoot) {
    Remove-Item -Recurse -Force -LiteralPath $targetRoot
}
Move-Item -LiteralPath $temporaryRoot -Destination $targetRoot
Write-Output "PST_SDK_ROOT=$targetRoot"
Write-Output 'PST_PACKAGE_VALIDATION=PASS'
