#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Launch','Elevated')][string]$Mode='Check',
 [ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$ScriptPath=$Root+'\tools\qualification\scm-protected-binding-readonly-uac.ps1'
$PowerShellExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$ExpectedSid='S-1-5-21-785699550-1440458426-1682014700-1108'
$ReceiptRoot=$Root+'\docs\integration\scm-protected-binding-positive-20261023'
function Assert-Plain([string]$Path){
 if([IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'}
 $p=$Path;while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$p=[IO.Path]::GetDirectoryName($p)}
}
function Open-Pinned([string]$Path,[string]$Hash){
 Assert-Plain $Path
 $f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try {$sha=[Security.Cryptography.SHA256]::Create();try{$v=([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()};if($v -cne $Hash){throw 'HASH_PIN_MISMATCH'};return $f}catch{$f.Dispose();throw}
}
Add-Type -TypeDefinition @"
using System;using System.Runtime.InteropServices;
public static class PaccScmReadonlyContext {
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll")] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int returned);
}
"@
function Observe-Context {
 $id=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true);$impersonating=$null -ne $thread;if($thread){$thread.Dispose()}
  $e=0;$t=0;$n=0;$im=0;$om=0
  $eq=[PaccScmReadonlyContext]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$n)
  $tq=[PaccScmReadonlyContext]::GetTokenInformation($id.Token,18,[ref]$t,4,[ref]$n)
  $console=[PaccScmReadonlyContext]::GetConsoleMode([PaccScmReadonlyContext]::GetStdHandle(-10),[ref]$im) -and [PaccScmReadonlyContext]::GetConsoleMode([PaccScmReadonlyContext]::GetStdHandle(-11),[ref]$om)
  $admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
  return [pscustomobject]@{sid=$id.User.Value;query_ok=($eq -and $tq);elevated=($e -eq 1);elevation_type=$t;administrators_enabled=$admin;console=$console;impersonating=$impersonating}
 }finally{$id.Dispose()}
}
$pins=@();$pause=$false;$result=2
try {
 if($PSCommandPath -cne $ScriptPath){throw 'SCRIPT_PATH_MISMATCH'}
 Assert-Plain $ScriptPath
 $selfHash=(Get-FileHash -LiteralPath $ScriptPath -Algorithm SHA256).Hash.ToLowerInvariant()
 if($Mode -eq 'Elevated' -and ($ExpectedScriptSha256 -eq '' -or $ExpectedScriptSha256 -cne $selfHash)){throw 'SCRIPT_HASH_MISMATCH'}
 $pins+=Open-Pinned $ScriptPath $selfHash
  $pins+=Open-Pinned ($Root+'\tools\qualification\scm-protected-binding-readonly.ps1') 'da6464af8f022bd2e9667a7032cb288dea82f31232b1c9e0ddcb3872a8d884fd'
  $pins+=Open-Pinned ($Root+'\tools\qualification\scm-lab-acl-postapply-readonly.ps1') '166158e31048c81f9e49a1c31d9d237cdf8e5e1b95806e1b1f5df6ebf98076bc'
  $pins+=Open-Pinned ($Root+'\tools\qualification\scm-lab-acl-postimage.ps1') '7558c7140c381693e486b56fe81e4f74c25617a0624ef3338b8f7232dbd7198e'
  $pins+=Open-Pinned ($Root+'\docs\integration\scm-lab-parent-acl-correction-20261023\manual-intent.json') '505ee46bd8b6dc43e531b44a73ca3c340807d81464e619deba233da8a9df48c4'
  $pins+=Open-Pinned ($Root+'\docs\integration\scm-lab-parent-acl-correction-20261023\manual-result.json') 'a3831f677bcadddae96bf01176f09ce6895005c898160bad27270bd32914cb9f'
  $pins+=Open-Pinned ($Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server.exe') '3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb'
  $pins+=Open-Pinned ($Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc.conf') 'a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44'
 $operator=Observe-Context
 if($Mode -eq 'Check') {
  $operator|ConvertTo-Json
  Write-Output 'CHECK_ONLY; NATIVE_BINDING=NOT_RUN; ACL_MUTATIONS=NONE; SCM_EFFECTS=NONE'
  if($operator.query_ok -and $operator.elevated -and $operator.elevation_type -ne 3 -and $operator.administrators_enabled -and $operator.console -and -not $operator.impersonating -and $operator.sid -ceq $ExpectedSid){$result=0}
 }elseif($Mode -eq 'Launch'){
  if(-not $operator.console){throw 'INTERACTIVE_CONSOLE_REQUIRED'}
  $a='-NoProfile -ExecutionPolicy Bypass -File "'+$ScriptPath+'" -Mode Elevated -ExpectedScriptSha256 '+$selfHash
  try{$child=Start-Process -FilePath $PowerShellExe -Verb RunAs -ArgumentList $a -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru}catch{throw 'UAC_CANCELLED_OR_LAUNCH_DENIED'}
  $result=$child.ExitCode;Write-Output ('SCM_READONLY_UAC_EXIT='+$result)
 }else{
  $pause=$operator.console
  if(-not $operator.query_ok -or -not $operator.elevated -or $operator.elevation_type -eq 3 -or -not $operator.administrators_enabled -or -not $operator.console -or $operator.impersonating -or $operator.sid -cne $ExpectedSid){throw 'ELEVATED_OPERATOR_OR_CONSOLE_DENY'}
  Write-Output ('READ-ONLY SCM LAB; effective operator='+$operator.sid)
  Write-Output 'No ACL remediation, service/account/key creation, bootstrap, CA or CURRENT. No retry.'
  $parent=@(& $PowerShellExe -NoProfile -ExecutionPolicy Bypass -File ($Root+'\tools\qualification\scm-lab-acl-postapply-readonly.ps1') 2>&1|ForEach-Object{$_.ToString()})
  $parentExit=$LASTEXITCODE;$parent|ForEach-Object{Write-Output $_}
  if($parentExit -ne 0){throw 'READONLY_PARENT_REVALIDATION_DENY'}
  # Same canonical executable and exact read-only CLI; no new worker/authority.
  $exe=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server.exe'
  $target=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release'
  $savedPreference=$ErrorActionPreference
  try {$ErrorActionPreference='Continue';$lines=@(& $exe service install --prepare-lab --installation $target --configuration papacc.conf 2>&1|ForEach-Object{$_.ToString()});$nativeExit=$LASTEXITCODE}finally{$ErrorActionPreference=$savedPreference}
  $lines|ForEach-Object{Write-Output $_}
  $sid=$null;$sidError=$null
  # Configured local virtual-account resolution only, NOT actual service-token proof.
  try{$sid=([Security.Principal.NTAccount]::new('NT SERVICE','PapinhoAccelerator')).Translate([Security.Principal.SecurityIdentifier]).Value}catch{$sidError=$_.Exception.Message}
  $bound=@($lines|Where-Object{$_ -match '^SCM_LAB_PREPARE=BOUND_READ_ONLY;' -and $_ -match 'OPERATOR=S-1-5-21-785699550-1440458426-1682014700-1108;' -and $_ -match 'INSTALLATION=417614100:1966080:79426;' -and $_ -match 'ATTEMPT=[0-9a-f]{64}; SERVICE=PapinhoAccelerator; ACCOUNT=NT SERVICE\\PapinhoAccelerator;' -and $_ -match 'IMAGE_SHA256=3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb; CONFIGURATION_SHA256=a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44;'  -and $_ -match 'AUTHORIZATION=NOT_ACQUIRED; CREATE_SERVICE=DENY; SCOPE=LAB_ONLY$'}).Count -eq 1
  $closed=@($lines|Where-Object{$_ -ceq 'SCM_LAB_BINDING=PREPARED_NOT_AUTHORIZED; STAGE=REVALIDATION; RESOURCES=CLOSED; NO_SCM_EFFECT; CA_CURRENT_AUTHORITY=DENY'}).Count -eq 1
  $sidQualified=$null -ne $sid -and $sid -match '^S-1-5-80(-[0-9]+){5}$'
  $dataPass=$nativeExit -eq 0 -and $bound -and $closed
  $pass=$dataPass -and $sidQualified
  $sidState='PENDING_FACTUAL_POST_REGISTRATION_READBACK';if($sidQualified){$sidState='NAME_RESOLVED_TOKEN_NOT_QUALIFIED'}
  $result=2;if($pass){$result=0}
  Assert-Plain $ReceiptRoot
  $receipt=[ordered]@{purpose='SCM_PROTECTED_BINDING_READ_ONLY';operator=$operator;script_sha256=$selfHash;parent_output=$parent;native_output=$lines;native_exit=$nativeExit;native_bound_closed=($bound -and $closed);virtual_account='NT SERVICE\PapinhoAccelerator';resolved_sid=$sid;sid_error=$sidError;actual_service_token='NOT_QUALIFIED';positive_qualification=$pass;read_only_data_pass=$dataPass;sid_name_resolution=$sidQualified;sid_resolution_state=$sidState;acl_mutations='NONE';scm_effects='NONE';current='DENY';ed11='PARTIAL';exit=$result}
  $path=$ReceiptRoot+'\manual-'+[Guid]::NewGuid().ToString('N')+'.json'
  $f=[IO.File]::Open($path,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
  try{$b=[Text.Encoding]::UTF8.GetBytes(($receipt|ConvertTo-Json -Depth 6));$f.Write($b,0,$b.Length);$f.Flush($true)}finally{$f.Dispose()}
  Write-Output ('RECEIPT='+$path+'; POSITIVE_READONLY_PASS='+$pass+'; NATIVE_EXIT='+$nativeExit+'; RESOLVED_SID='+$sid)
  Write-Output ('READONLY_DATA_PASS='+$dataPass+'; SID_STATE='+$sidState+'; SID_ERROR='+$sidError)
  Write-Output 'CreateService, operational service token, CA/CURRENT/signature remain DENY. Never retry automatically.'
 }
}catch{Write-Output ('DENY: '+$_.Exception.Message);$result=2}
finally{foreach($pin in $pins){$pin.Dispose()}}
if($pause){Write-Output 'Resultado preservado. Pressione F10 para fechar.';try{while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}}catch{$result=2}}
exit $result
