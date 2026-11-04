$ErrorActionPreference='Stop'
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if((Get-Location).Path -ne $repo){throw 'REPOSITORY_CWD_REQUIRED'}
$base=Join-Path $repo 'build/phase6-ca-initialize-native-20261029/final-phase-qualified'
if(Test-Path -LiteralPath $base){throw 'NO_OVERWRITE_FIXTURE_DIRECTORY'}
[IO.Directory]::CreateDirectory($base)|Out-Null
$sha=[Security.Cryptography.SHA256]::Create()
$previous=[Environment]::GetEnvironmentVariable('PAPACC_ALLOWED_CLIENT_CERT_SHA256')
$results=@()
try {
 $der=[Convert]::FromBase64String((Get-Content tests/fixtures/security/client-a.der.b64 -Raw))
 $env:PAPACC_ALLOWED_CLIENT_CERT_SHA256=([BitConverter]::ToString($sha.ComputeHash($der))).Replace('-','').ToLowerInvariant()
 foreach($profile in @('Selected','Legacy')){foreach($config in @('Debug','Release')){
  $dir=Join-Path $base "$profile/$config";[IO.Directory]::CreateDirectory($dir)|Out-Null
  $exe=Join-Path $repo "build/phase6-functional-20261028/$profile/$config/papacc_canonical_root_transaction_test.exe"
  foreach($mode in @('catalog-live-port','catalog-live-port-v2')){
   $path=[IO.Path]::GetFullPath((Join-Path $dir "$mode.bin"))
   if(!$path.StartsWith($base+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase) -or (Test-Path -LiteralPath $path)){throw 'EXCLUSIVE_FIXTURE_DENY'}
   $log="$profile-$config-$mode.log";& $exe $mode $path > (Join-Path $PSScriptRoot $log) 2>&1
   $code=$LASTEXITCODE;$results+=@{profile=$profile;config=$config;mode=$mode;exit=$code;path=$path;log=$log;scope='ACTUAL_ROOT_WRAPPER_NEGATIVE_NO_ORIGINAL_CA; METADATA_FIXTURE_CURRENT_ONLY'}
   $results|ConvertTo-Json -Depth 5|Set-Content (Join-Path $PSScriptRoot 'NATIVE-RESULTS.json') -Encoding utf8
   if($code){Get-Content (Join-Path $PSScriptRoot $log);throw 'NATIVE_FIXTURE_FAILED'}
  }
 }}
} finally {$sha.Dispose();[Environment]::SetEnvironmentVariable('PAPACC_ALLOWED_CLIENT_CERT_SHA256',$previous)}
"PASS native_runs=$($results.Count); unchanged CAS fault campaign reused; no original CA or signature"
