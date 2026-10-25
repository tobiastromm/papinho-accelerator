$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator\build\personal-target-ca-qualification-20261022'
$effects=Join-Path $root 'effects'
if(Test-Path -LiteralPath $effects){throw 'STOP: effects root already exists; never delete or reuse'}
New-Item -ItemType Directory -Path $effects | Out-Null
$rows=@()
foreach($profile in @('Legacy','Selected')){foreach($config in @('Debug','Release')){
 $exe=Join-Path $root "$profile\$config\papacc_personal_target_component_test.exe"
 if(!(Test-Path -LiteralPath $exe)){throw 'binary absent'}
 $hash=(Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash
 foreach($case in @('new','exists-dir','exists-file','stop-before','stop-after','unknown','closefail','parent-mismatch','attempt-mismatch','replay','model-ca')){
  $parent=Join-Path $effects "$profile-$config-$case"
  New-Item -ItemType Directory -Path $parent | Out-Null
  $output=& $exe $parent $case 2>&1
  $code=$LASTEXITCODE
  $output | Set-Content -LiteralPath (Join-Path $effects "$profile-$config-$case.log")
  $rows+= [pscustomobject]@{profile=$profile;config=$config;case=$case;exit=$code;sha256=$hash;output=($output -join "`n")}
  $rows | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $effects 'results.json')
  if($code -ne 0){throw "STOP: $profile $config $case exit=$code"}
 }
}}
'LAB_NATIVE_CASES_PASS='+$rows.Count
