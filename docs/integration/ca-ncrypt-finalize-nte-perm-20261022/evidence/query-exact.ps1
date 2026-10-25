$ErrorActionPreference='Stop'
$exe='C:\Projetos\PapinhoAccelerator\build\ca-finalize-readonly-diagnosis-20261022\readonly_diagnostic.exe'
if((Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant() -cne '8cf1e4a6057033594bc761e115b0e8d27344f1ac5f3814a337eb609d325c401b'){throw 'STOP hash mismatch'}
$identity=[Security.Principal.WindowsIdentity]::GetCurrent()
if($identity.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108'){throw 'STOP operator mismatch'}
python 'C:\Users\tobias\AppData\Local\Temp\pacc-ca-finalize-diagnosis-20261022\probe-token.py' | Tee-Object -FilePath 'C:\Users\tobias\AppData\Local\Temp\pacc-ca-finalize-diagnosis-20261022\official-token.json'
if($LASTEXITCODE -ne 0){throw 'STOP token probe failed'}
$output=& $exe '--observe-existing' 'cb87632463bb48dd8a34fa8ea6f3911b' '030fa34dd44f41e8b143b9da09532cf2' 'PapinhoAccelerator.CA.cb87632463bb48dd8a34fa8ea6f3911b.030fa34dd44f41e8b143b9da09532cf2' 'S-1-5-21-785699550-1440458426-1682014700-1108' 'S-1-5-19' 2>&1
$code=$LASTEXITCODE
$output | Tee-Object -FilePath 'C:\Users\tobias\AppData\Local\Temp\pacc-ca-finalize-diagnosis-20261022\exact-query.log'
'READONLY_QUERY_EXIT='+$code
