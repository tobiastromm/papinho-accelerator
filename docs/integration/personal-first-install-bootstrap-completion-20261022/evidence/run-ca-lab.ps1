$ErrorActionPreference='Stop'
$exe='C:\Projetos\PapinhoAccelerator\build\personal-bootstrap-completion-20261022\Selected\Release\papacc_personal_target_component_test.exe'
$expected='610a083df2ee0d29da3f36adf08de23b4a083ccf0c529eba9d8793fcdf5a9006'
if((Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash.ToLowerInvariant() -cne $expected){throw 'STOP executable hash mismatch'}
$parent='C:\Projetos\PapinhoAccelerator\build\personal-bootstrap-completion-20261022\effects\ca-lab'
if(Test-Path -LiteralPath $parent){throw 'STOP parent exists; no retry or reuse'}
New-Item -ItemType Directory -Path $parent | Out-Null
$output=& $exe $parent 'ca-lab' 2>&1
$code=$LASTEXITCODE
$output | Set-Content -LiteralPath 'C:\Projetos\PapinhoAccelerator\build\personal-bootstrap-completion-20261022\effects\ca-lab-result.log'
'LAB_CA_EXIT='+$code
$output
if($code -ne 0){throw 'STOP LAB CA initialization/cleanup not qualified; no retry'}
