# Static Windows file read only; no protected root, process token or admission.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$root='C:\Projetos\PapinhoAccelerator'
$tool=$root+'\build\successor-exact-binding-20261024\Selected\Release\papacc_service_n0_sd_test.exe'
$stream=[IO.File]::Open($tool,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
try {
 $sha=[Security.Cryptography.SHA256]::Create()
 try {$hash=([BitConverter]::ToString($sha.ComputeHash($stream))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($hash-cne'836a52adc86ea41b818ec8bd0f53a72345c5e50baf8cb587cb5781f833876d7c'){throw 'NATIVE_READER_PIN_DENY'}
 $lines=@(& $tool --file);$exitCode=$LASTEXITCODE
 $record=[ordered]@{status='READ_ONLY_STATIC_FILE_NOT_ADMISSION';utc=[DateTime]::UtcNow.ToString('o');tool_sha256=$hash;tool_handle_held=$true;exit=$exitCode;native_output=($lines-join"`n");signature='NOT_REQUERIED_NETWORK_NOT_REQUESTED';historical_signature_applicability='REQUIRES_EXACT_FILE_ID_CONTENT_RAW1_MATCH';protected_root='NOT_QUERIED';SCM='NOT_QUERIED';Q4='NOT_ACTIVATED'}
 $dest=$root+'\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025\static-windows-file-read.json'
 [IO.File]::WriteAllText($dest,($record|ConvertTo-Json -Depth 5),[Text.UTF8Encoding]::new($false))
 foreach($line in $lines){[Console]::Out.WriteLine($line)}
 if($exitCode-ne0){throw 'STATIC_FILE_READER_FAILED_NO_RETRY'}
} finally {$stream.Dispose()}
