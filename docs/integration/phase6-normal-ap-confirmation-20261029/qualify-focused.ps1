$ErrorActionPreference='Stop'
$repo=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
if((Get-Location).Path -ne $repo){throw 'REPOSITORY_CWD_REQUIRED'}
$base=[IO.Path]::GetFullPath((Join-Path $repo 'build/phase6-normal-ap-confirmation-20261029/final'))
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
  foreach($mode in @('catalog-live-port','catalog-live-port-unknown','catalog-live-port-native-conflict','catalog-live-port-stop','catalog-live-port-early-stop','catalog-live-port-proof','catalog-live-port-readback')){
   $path=[IO.Path]::GetFullPath((Join-Path $dir "$mode.bin"))
   if(!$path.StartsWith($base+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase) -or (Test-Path -LiteralPath $path)){throw 'EXCLUSIVE_FIXTURE_DENY'}
   $log="$profile-$config-$mode.log";& $exe $mode $path > (Join-Path $PSScriptRoot $log) 2>&1
   $code=$LASTEXITCODE;$results+=@{profile=$profile;config=$config;mode=$mode;exit=$code;path=$path;log=$log;scope='NATIVE_METADATA_FIXTURE_NO_NORMAL_AUTHORITY_NO_REAL_CERTIFICATE'}
   $results|ConvertTo-Json -Depth 5|Set-Content (Join-Path $PSScriptRoot 'FOCUSED-NATIVE-RESULTS.json') -Encoding utf8
   if($code){Get-Content (Join-Path $PSScriptRoot $log);throw 'NATIVE_FIXTURE_FAILED'}
  }
  $codec=Join-Path $repo "build/phase6-functional-20261028/$profile/$config/papacc_issuer_root_store_test.exe"
  $path=Join-Path $dir 'codec-only-not-created.bin';$log="$profile-$config-codec-only.log"
  & $codec --codec-only $path > (Join-Path $PSScriptRoot $log) 2>&1
  $code=$LASTEXITCODE;$results+=@{profile=$profile;config=$config;mode='codec-only';exit=$code;path=$path;log=$log;scope='DATA_NATIVE_HASH_ONLY_NO_FILE_CREATED_NO_ISSUANCE'}
  $results|ConvertTo-Json -Depth 5|Set-Content (Join-Path $PSScriptRoot 'FOCUSED-NATIVE-RESULTS.json') -Encoding utf8
  if($code -or (Test-Path -LiteralPath $path)){Get-Content (Join-Path $PSScriptRoot $log);throw 'CODEC_FAILED'}
 }}
} finally {$sha.Dispose();[Environment]::SetEnvironmentVariable('PAPACC_ALLOWED_CLIENT_CERT_SHA256',$previous)}
"PASS focused_runs=$($results.Count); historical_unchanged_campaigns_not_repeated"
