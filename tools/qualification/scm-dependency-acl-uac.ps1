#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Launch','Elevated','Verify','ModelTest')][string]$Mode='Check',[ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$Script=$Root+'\tools\qualification\scm-dependency-acl-uac.ps1'
$Helper=$Root+'\tools\qualification\scm-file-acl-native.ps1'
$ReceiptRoot=$Root+'\docs\integration\scm-lab-dependency-acl-correction-20261023'
$PlanPath=$ReceiptRoot+'\exact-plan.json'
$Target=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release'
$Validator=$Root+'\build\windows-service-protected-runtime-20261023\Selected\Release\papacc_scm_dependency_acl_readonly.exe'
$ValidatorHash='d956daa9297ee95d1b9f9b64ae7779a9427aaab7e4db427f5952e37c18308b52'
$ExpectedSid='S-1-5-21-785699550-1440458426-1682014700-1108'
$PSExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
function Assert-Plain([string]$Path){
 if([IO.Path]::GetFullPath($Path) -cne $Path){throw 'NONCANONICAL_PATH'}
 $p=$Path;while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$p=[IO.Path]::GetDirectoryName($p)}
}
function Pin([string]$Path,[string]$Hash){
 Assert-Plain $Path;$f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try{$sha=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()};if($actual -cne $Hash){throw 'PIN_MISMATCH'};return $f}catch{$f.Dispose();throw}
}
function Path-Hash([string]$Path){
 $f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try{$sha=[Security.Cryptography.SHA256]::Create();try{return ([BitConverter]::ToString($sha.ComputeHash($f))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}}finally{$f.Dispose()}
}
function Context {
 $id=[Security.Principal.WindowsIdentity]::GetCurrent()
 try{$e=0;$ty=0;$n=0;$im=0;$om=0;$eq=[PaccFileAcl]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$n);$tq=[PaccFileAcl]::GetTokenInformation($id.Token,18,[ref]$ty,4,[ref]$n);$admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator);$sid=$id.User.Value}finally{$id.Dispose()}
 $th=[Security.Principal.WindowsIdentity]::GetCurrent($true);$imp=$null -ne $th;if($th){$th.Dispose()}
 $console=[PaccFileAcl]::GetConsoleMode([PaccFileAcl]::GetStdHandle(-10),[ref]$im) -and [PaccFileAcl]::GetConsoleMode([PaccFileAcl]::GetStdHandle(-11),[ref]$om)
 return [pscustomobject]@{sid=$sid;elevated=($eq -and $tq -and $e -eq 1 -and $ty -ne 3);administrators=$admin;console=$console;impersonating=$imp}
}
function Assert-Operator {
 $c=Context
 if(-not $c.elevated -or -not $c.administrators -or -not $c.console -or $c.impersonating -or $c.sid -cne $ExpectedSid){throw 'OPERATOR_OR_CONSOLE_DENY'}
 return $c
}
function Assert-File($f){
 $n=$f.Plan.NativeIdentity;[PaccFileAcl]::Identity($f.Handle,$n.indexHigh,$n.indexLow,$n.size,$n.writeHigh,$n.writeLow,$false)
 if([PaccFileAcl]::FileDigest($f.Handle) -cne $f.Plan.Sha256){throw 'FILE_HASH_MISMATCH'}
}
function Assert-Preserved {
 [PaccFileAcl]::Identity($parent,1966080,79426,0,0,0,$true)
 if(-not [PaccFileAcl]::Equivalent([PaccFileAcl]::Encode($plan.Parent.sddl),[PaccFileAcl]::Read($parent),7)){throw 'PARENT_CHANGED'}
 foreach($f in $unchanged){Assert-File $f;if([PaccFileAcl]::Digest([PaccFileAcl]::Read($f.Handle)) -cne $f.Plan.BeforeNativeSdSha256){throw 'PRESERVED_FILE_SD_CHANGED'}}
}
function Run-Validator([int]$ExpectedExit){
 $saved=$ErrorActionPreference
 try{$ErrorActionPreference='Continue';$lines=@(& $Validator 2>&1|ForEach-Object{$_.ToString()});$nativeExit=$LASTEXITCODE}finally{$ErrorActionPreference=$saved}
 $lines|ForEach-Object{Write-Host $_}
 $tag='SCM_DEPENDENCY_VALIDATOR='+$(if($ExpectedExit -eq 0){'PASS'}else{'DENY'})+'; ACL_MUTATIONS=NONE'
 if($nativeExit -ne $ExpectedExit -or @($lines|Where-Object{$_ -ceq $tag}).Count -ne 1 -or @($lines|Where-Object{$_ -ceq 'CHECKED_CLOSE_ERRORS=0; MUTATIONS=NONE'}).Count -ne 1){throw 'NATIVE_DESCRIPTOR_VALIDATOR_MISMATCH'}
}
$pins=@();$files=@();$unchanged=@();$parent=[IntPtr]::Zero;$intent=$false;$pause=$false;$effects=0;$code=2;$post=@();$attempt=$null;$parentClosed=$false
try{
 if($PSCommandPath -cne $Script){throw 'SCRIPT_PATH'};Assert-Plain $Script
 $self=Path-Hash $Script
 if($Mode -eq 'Elevated' -and ($ExpectedScriptSha256 -eq '' -or $self -cne $ExpectedScriptSha256)){throw 'SCRIPT_PIN'}
 $pins+=Pin $Script $self;$pins+=Pin $Helper 'f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';$pins+=Pin $PlanPath 'bcb1d3d0b53c71ef6438a506336aa30c8e66d809d2f76eaa7c407515ae0bdd38';$pins+=Pin $Validator $ValidatorHash
 . $Helper
 $plan=Get-Content -LiteralPath $PlanPath -Raw|ConvertFrom-Json
 if($plan.Task -cne 'PACC-PHASE6-SCM-LAB-DEPENDENCY-ACL-CORRECTION-1' -or $plan.Files.Count -ne 2 -or $plan.Preserve.Count -ne 2){throw 'PLAN_SCOPE'}
 if($Mode -eq 'ModelTest'){
  $tests=0
  foreach($p in $plan.Files){
   $b=[PaccFileAcl]::Encode($p.AfterSddl)
   if(-not [PaccFileAcl]::Equivalent($b,$b,6)){throw 'POST_MODEL'};$tests++
   $d=[Security.AccessControl.RawSecurityDescriptor]::new($b,0);$d.SetFlags([Security.AccessControl.ControlFlags]([int]$d.ControlFlags -bor 0x400));$v=[PaccFileAcl]::Encode($d.GetSddlForm([Security.AccessControl.AccessControlSections]14));if(-not [PaccFileAcl]::Equivalent($b,$v,6)){throw 'PAI_MODEL'};$tests++
   foreach($kind in @('OWNER','GROUP','WRITER','UNPROTECTED','EXTRA','FLAGS','PRINCIPAL','ORDER')){
    $d=[Security.AccessControl.RawSecurityDescriptor]::new($b,0)
    switch($kind){
     OWNER{$d.Owner=[Security.Principal.SecurityIdentifier]::new('S-1-5-18')}
     GROUP{$d.Group=[Security.Principal.SecurityIdentifier]::new('S-1-5-18')}
     WRITER{$d.DiscretionaryAcl[0].AccessMask=0x1f01ff}
     UNPROTECTED{$d.SetFlags([Security.AccessControl.ControlFlags]([int]$d.ControlFlags -band (-bnot 0x1000)))}
     EXTRA{$d.DiscretionaryAcl.InsertAce(6,$d.DiscretionaryAcl[0])}
     FLAGS{$d.DiscretionaryAcl[0].AceFlags=[Security.AccessControl.AceFlags]8}
     PRINCIPAL{$d.DiscretionaryAcl[0].SecurityIdentifier=[Security.Principal.SecurityIdentifier]::new('S-1-1-0')}
     ORDER{$ace=$d.DiscretionaryAcl[0];$d.DiscretionaryAcl.RemoveAce(0);$d.DiscretionaryAcl.InsertAce(1,$ace)}
    }
    if([PaccFileAcl]::Equivalent($b,[PaccFileAcl]::Encode($d.GetSddlForm([Security.AccessControl.AccessControlSections]14)),6)){throw ('UNSAFE_MODEL_'+$kind)};$tests++
   }
  };Write-Output ('MODEL_PASS; CASES='+$tests+'; NO_NATIVE_TARGET_OPEN; NO_ACL_MUTATION');$code=0
 }elseif($Mode -eq 'Launch'){
  $ctx=Context;if(-not $ctx.console){throw 'CONSOLE_REQUIRED'}
  if(Test-Path -LiteralPath ($ReceiptRoot+'\manual-intent.json')){throw 'PREVIOUS_ATTEMPT_NO_RETRY'}
  $a='-NoProfile -ExecutionPolicy Bypass -File "'+$Script+'" -Mode Elevated -ExpectedScriptSha256 '+$self
  $child=Start-Process -FilePath $PSExe -Verb RunAs -ArgumentList $a -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru
  $code=$child.ExitCode;Write-Output ('SCM_DLL_ACL_EXIT='+$code)
 }else{
  if($Mode -eq 'Elevated'){$ctx=Assert-Operator;$pause=$true;if(Test-Path -LiteralPath ($ReceiptRoot+'\manual-intent.json')){throw 'PREVIOUS_ATTEMPT_NO_RETRY'}}
  Assert-Plain $Target;$parent=[PaccFileAcl]::Open($Target,$false,$true)
  for($j=0;$j -lt 2;$j++){
   $p=$plan.Preserve[$j];if($p.Path -cne ($Target+'\'+@('papacc.conf','papacc_server.exe')[$j])){throw 'PRESERVE_PATH'}
   Assert-Plain $p.Path;$f=[pscustomobject]@{Handle=[PaccFileAcl]::Open($p.Path,$false,$false);Plan=$p;Closed=$false};$unchanged+=$f
  };Assert-Preserved
  for($j=0;$j -lt 2;$j++){
   $p=$plan.Files[$j];if($p.Path -cne ($Target+'\'+@('libcrypto-3-x64.dll','libssl-3-x64.dll')[$j])){throw 'PLAN_PATH'}
   Assert-Plain $p.Path;$f=[pscustomobject]@{Handle=[PaccFileAcl]::Open($p.Path,($Mode -eq 'Elevated'),$false);Plan=$p;Closed=$false};$files+=$f;Assert-File $f
   $sd=[PaccFileAcl]::Read($f.Handle)
   if($Mode -eq 'Verify'){if(-not [PaccFileAcl]::Equivalent([PaccFileAcl]::Encode($p.AfterSddl),$sd,6)){throw 'POSTIMAGE_MISMATCH'}}
   elseif([PaccFileAcl]::Digest($sd) -cne $p.BeforeNativeSdSha256){throw 'PREIMAGE_MISMATCH'}
   Write-Output ('DLL='+$p.Path+'; SHA256='+$p.Sha256+'; SD='+[PaccFileAcl]::Sddl($sd))
  }
  if($Mode -eq 'Elevated'){
   Run-Validator 2
   $ctx|ConvertTo-Json
   Write-Output 'ONLY THESE TWO LAB DLLs. Owner becomes displayed approved operator; group unchanged.'
   Write-Output 'Copy six inherited ACEs to explicit; protect each DLL DACL. ACE0/1/5 Modify -> ReadAndExecute; BA/SYSTEM FullControl and Users RX preserved.'
   Write-Output 'Parent, exe/conf, DLL bytes/hash/native identity, service, PST and CA unchanged. No SACL/integrity change. Partial failure is NOT rollback; no retry.'
   foreach($f in $files){Write-Output ('PROPOSED '+$f.Plan.Path+' '+$f.Plan.AfterSddl)}
   $attempt=[Guid]::NewGuid().ToString('N');$confirm='CORRECT_SCM_DLLS '+$attempt;Write-Output ('Type ENTIRE exact line, then Enter (visible input; no passwords):'+[Environment]::NewLine+$confirm)
   if([Console]::ReadLine() -cne $confirm){throw 'CONFIRMATION_DENY'}
   $null=Assert-Operator;Assert-Preserved
   foreach($f in $files){Assert-File $f;if([PaccFileAcl]::Digest([PaccFileAcl]::Read($f.Handle)) -cne $f.Plan.BeforeNativeSdSha256){throw 'PRE_EFFECT_CHANGED'}}
   Assert-Plain $ReceiptRoot
   $out=[IO.File]::Open($ReceiptRoot+'\manual-intent.json',[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
   try{$b=[Text.Encoding]::UTF8.GetBytes((@{attempt=$attempt;operator=$ctx;script=$self;plan_sha256='bcb1d3d0b53c71ef6438a506336aa30c8e66d809d2f76eaa7c407515ae0bdd38';disposition='EFFECT_NOT_YET_CONFIRMED';scope='TWO_EXACT_LAB_DLLS';retry='DENY'}|ConvertTo-Json -Depth 6));$out.Write($b,0,$b.Length);$out.Flush($true)}finally{$out.Dispose()};$intent=$true
   foreach($f in $files){
    $null=Assert-Operator;Assert-Preserved;Assert-File $f
    if([PaccFileAcl]::Digest([PaccFileAcl]::Read($f.Handle)) -cne $f.Plan.BeforeNativeSdSha256){throw 'DLL_CHANGED_BEFORE_EFFECT'}
    $effects++;[PaccFileAcl]::Apply($f.Handle,[PaccFileAcl]::Encode($f.Plan.AfterSddl),$true)
    Assert-File $f;if(-not [PaccFileAcl]::Equivalent([PaccFileAcl]::Encode($f.Plan.AfterSddl),[PaccFileAcl]::Read($f.Handle),6)){throw 'POSTIMAGE_MISMATCH'}
   }
   Assert-Preserved;Run-Validator 0
   Write-Output 'TWO_DLL_POST_READBACK_PASS; SAME_NATIVE_IDS_HASHES; PARENT_EXE_CONFIG_PRESERVED; SCM_EFFECTS=NONE'
  }elseif($Mode -eq 'Verify'){Assert-Preserved;Run-Validator 0;Write-Output 'VERIFY_PASS; BOTH_DLL_POSTIMAGES_AND_NATIVE_VALIDATOR_PASS; PRESERVED_FILES_PASS; ACL_MUTATIONS=NONE'}
  else{Run-Validator 2;Assert-Preserved;Write-Output 'CHECK_PASS; EXACT_PREIMAGES_MATCH; CURRENT_DLL_ACL=DENY_EXPECTED; ACL_MUTATIONS=NONE'}
  $code=0
 }
}catch{Write-Output ('STOP: '+$_.Exception.Message);$code=2}
finally{
 if($intent){foreach($f in $files){try{$post+=@{path=$f.Plan.Path;sddl=[PaccFileAcl]::Sddl([PaccFileAcl]::Read($f.Handle));sha256=[PaccFileAcl]::FileDigest($f.Handle)}}catch{$post+=@{path=$f.Plan.Path;state='UNKNOWN'};$code=2}}}
 foreach($f in @($files)+@($unchanged)){$f.Closed=[PaccFileAcl]::CloseHandle($f.Handle);if(-not $f.Closed){Write-Output ('FILE_CLOSE_UNCERTAIN '+$f.Plan.Path);$code=2}}
 if($parent -ne [IntPtr]::Zero){$parentClosed=[PaccFileAcl]::CloseHandle($parent);if(-not $parentClosed){Write-Output 'PARENT_CLOSE_UNCERTAIN';$code=2}}
 if($intent){try{$receipt=@{attempt=$attempt;effect_attempts=$effects;observed=$post;files=@(@($files)+@($unchanged)|ForEach-Object{@{path=$_.Plan.Path;closed=$_.Closed}});parent_closed=$parentClosed;descriptor_retained=[PaccFileAcl]::DescriptorRetained;exit=$code;rollback='NOT_CLAIMED';retry='DENY';acl_scope='TWO_EXACT_LAB_DLLS';scm_effects='NONE';ed11='PARTIAL'}|ConvertTo-Json -Depth 8;$o=[IO.File]::Open($ReceiptRoot+'\manual-result.json',[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None);try{$b=[Text.Encoding]::UTF8.GetBytes($receipt);$o.Write($b,0,$b.Length);$o.Flush($true)}finally{$o.Dispose()}}catch{Write-Output 'RECEIPT_FAILURE';$code=2}}
 foreach($pin in $pins){try{$pin.Dispose()}catch{Write-Output 'PIN_CLOSE_UNCERTAIN';$code=2}}
}
if($pause){Write-Output 'Resultado preservado. F10 fecha. Nunca repetir automaticamente.';try{while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}}catch{$code=2}}
exit $code
