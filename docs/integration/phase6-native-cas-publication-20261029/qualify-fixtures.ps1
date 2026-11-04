param([ValidateSet('qualified','final','integrated')][string]$Run = 'integrated')
$ErrorActionPreference = 'Stop'
# Offline native file fixtures only. Never point at a deployed root/CURRENT.
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if ((Get-Location).Path -ne $repo) { throw 'REPOSITORY_CWD_REQUIRED' }
$fixtureBase = [IO.Path]::GetFullPath((Join-Path $repo "build/phase6-native-cas-20261029/$Run"))
if (Test-Path -LiteralPath $fixtureBase) { throw 'FIXTURE_DIRECTORY_EXISTS_NO_OVERWRITE' }
[IO.Directory]::CreateDirectory($fixtureBase) | Out-Null
$sha = [Security.Cryptography.SHA256]::Create()
$oldFingerprint = [Environment]::GetEnvironmentVariable('PAPACC_ALLOWED_CLIENT_CERT_SHA256')
$results = @()
try {
    $der = [Convert]::FromBase64String((Get-Content -LiteralPath 'tests/fixtures/security/client-a.der.b64' -Raw))
    $env:PAPACC_ALLOWED_CLIENT_CERT_SHA256 = ([BitConverter]::ToString($sha.ComputeHash($der))).Replace('-', '').ToLowerInvariant()
    foreach ($profile in @('Selected', 'Legacy')) {
        foreach ($config in @('Debug', 'Release')) {
            $exe = Join-Path $repo "build/phase6-functional-20261028/$profile/$config/papacc_canonical_root_transaction_test.exe"
            $dir = Join-Path $fixtureBase "$profile/$config"
            [IO.Directory]::CreateDirectory($dir) | Out-Null
            foreach ($mode in @('catalog-live-port', 'catalog-live-port-unknown', 'catalog-live-port-native-conflict', 'catalog-live-port-stop', 'catalog-live-port-early-stop', 'catalog-live-port-proof', 'catalog-live-port-readback', 'catalog-live', 'catalog-live-conflict', 'catalog-live-unknown', 'catalog-live-late-stop', 'catalog-live-proof-change', 'catalog-issuer06')) {
                $path = [IO.Path]::GetFullPath((Join-Path $dir "$mode.bin"))
                if (!$path.StartsWith($fixtureBase + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) { throw 'FIXTURE_PATH_DENY' }
                if (Test-Path -LiteralPath $path) { throw 'EXCLUSIVE_FIXTURE_REQUIRED' }
                $log = Join-Path $PSScriptRoot "$profile-$config-$mode-$Run.log"
                & $exe $mode $path > $log 2>&1
                $code = $LASTEXITCODE
                $results += [ordered]@{profile=$profile;config=$config;mode=$mode;exit=$code;fixture=$path;log=[IO.Path]::GetFileName($log);scope='NATIVE_OFFLINE_FIXTURE_ONLY_NO_PRODUCTIVE_ROOT_NO_SIGNING_NO_NETWORK'}
                $results | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $PSScriptRoot "NATIVE-FIXTURE-RESULTS-$Run.json") -Encoding utf8
                if ($code -ne 0) { Get-Content -LiteralPath $log; throw "FIXTURE_FAILED $profile/$config/$mode" }
            }
            $store = Join-Path $repo "build/phase6-functional-20261028/$profile/$config/papacc_issuer_root_store_test.exe"
            foreach ($mode in @('pending-success','pending-conflict','pending-unknown','pending-stop')) {
                $path = Join-Path $dir "$mode.bin"
                if (Test-Path -LiteralPath $path) { throw 'EXCLUSIVE_FIXTURE_REQUIRED' }
                $log = Join-Path $PSScriptRoot "$profile-$config-$mode-$Run.log"
                & $store "--$mode" $path > $log 2>&1
                $code = $LASTEXITCODE
                $results += [ordered]@{profile=$profile;config=$config;mode=$mode;exit=$code;fixture=$path;log=[IO.Path]::GetFileName($log);scope='NATIVE_COMPONENT_FIXTURE_SYNTHETIC_UNKNOWN_INJECTION_NO_PUBLICATION'}
                $results | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath (Join-Path $PSScriptRoot "NATIVE-FIXTURE-RESULTS-$Run.json") -Encoding utf8
                if ($code -ne 0) { Get-Content -LiteralPath $log; throw "STORE_FAILED $profile/$config/$mode" }
            }
        }
    }
} finally {
    $sha.Dispose()
    [Environment]::SetEnvironmentVariable('PAPACC_ALLOWED_CLIENT_CERT_SHA256', $oldFingerprint)
}
"PASS native_fixture_runs=$($results.Count)"
