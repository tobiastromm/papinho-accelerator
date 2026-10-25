#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Launch','Elevated','Verify','ModelTest')][string]$Mode='Check',[ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$Script=$Root+'\tools\qualification\scm-file-acl-uac.ps1'
$Helper=$Root+'\tools\qualification\scm-file-acl-native.ps1'
$PlanPath=$Root+'\docs\integration\scm-file-acl-virtual-sid-20261023\acl-correction-PROPOSED.json'
$ParentPlan=$Root+'\docs\integration\scm-lab-parent-acl-correction-20261023\manual-intent.json'
$Target=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release'
$ReceiptRoot=$Root+'\docs\integration\scm-file-acl-virtual-sid-correction-20261023'
$ExpectedSid='S-1-5-21-785699550-1440458426-1682014700-1108'
$PSExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
function Assert-Plain([string]$Path){$p=$Path;while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_PATH'};$p=[IO.Path]::GetDirectoryName($p)}}
function Pin([string]$Path,[string]$Hash){Assert-Plain $Path;$f=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read);try{if((Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash.ToLowerInvariant() -cne $Hash){throw 'PIN_MISMATCH'};return $f}catch{$f.Dispose();throw}}
function Context {
 $id=[Security.Principal.WindowsIdentity]::GetCurrent();try{$e=0;$t=0;$n=0;$im=0;$om=0;$eq=[PaccFileAcl]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$n);$tq=[PaccFileAcl]::GetTokenInformation($id.Token,18,[ref]$t,4,[ref]$n);$admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator);$sid=$id.User.Value}finally{$id.Dispose()}
 $th=[Security.Principal.WindowsIdentity]::GetCurrent($true);$imp=$null -ne $th;if($th){$th.Dispose()}
 $console=[PaccFileAcl]::GetConsoleMode([PaccFileAcl]::GetStdHandle(-10),[ref]$im) -and [PaccFileAcl]::GetConsoleMode([PaccFileAcl]::GetStdHandle(-11),[ref]$om)
 return [pscustomobject]@{sid=$sid;elevated=($eq -and $tq -and $e -eq 1 -and $t -ne 3);administrators=$admin;console=$console;impersonating=$imp}
}
function Assert-File($f){$i=$f.Plan.NativeIdentity;[PaccFileAcl]::Identity($f.Handle,$i.indexHigh,$i.indexLow,$i.size,$i.writeHigh,$i.writeLow,$false);if([PaccFileAcl]::FileDigest($f.Handle) -cne $f.Plan.Sha256){throw 'FILE_HASH_MISMATCH'}}
function Assert-Parent {
 [PaccFileAcl]::Identity($parent,1966080,79426,0,0,0,$true)
 if(-not [PaccFileAcl]::Equivalent([PaccFileAcl]::Encode($parentExpected),[PaccFileAcl]::Read($parent),7)){throw 'PARENT_CHANGED'}
}
$pins=@();$files=@();$parent=[IntPtr]::Zero;$intent=$false;$pause=$false;$effect=$false;$code=2;$post=@();$attempt=$null;$parentClosed=$false
try{
 if($PSCommandPath -cne $Script){throw 'SCRIPT_PATH'};Assert-Plain $Script
 $self=(Get-FileHash -LiteralPath $Script -Algorithm SHA256).Hash.ToLowerInvariant()
 if($Mode -eq 'Elevated' -and ($ExpectedScriptSha256 -eq '' -or $self -cne $ExpectedScriptSha256)){throw 'SCRIPT_PIN'}
 $pins+=Pin $Script $self;$pins+=Pin $Helper 'f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';$pins+=Pin $PlanPath '8426828afe787e21612f52c0f41d5cc837288dad04a4e313df8a8ea8aaf69caa';$pins+=Pin $ParentPlan '505ee46bd8b6dc43e531b44a73ca3c340807d81464e619deba233da8a9df48c4'
 . $Helper
 $plans=Get-Content -LiteralPath $PlanPath -Raw|ConvertFrom-Json;if($plans.Count -ne 2){throw 'PLAN_COUNT'}
 $parentExpected=(Get-Content -LiteralPath $ParentPlan -Raw|ConvertFrom-Json).after
 $ctx=Context
 if($Mode -eq 'ModelTest'){
  foreach($p in $plans){$b=[PaccFileAcl]::Encode($p.AfterSddl);if(-not [PaccFileAcl]::Equivalent($b,$b,6)){throw 'POST_MODEL'}
   $d=[Security.AccessControl.RawSecurityDescriptor]::new($b,0);$d.SetFlags([Security.AccessControl.ControlFlags]([int]$d.ControlFlags -bor 0x400));$v=[PaccFileAcl]::Encode($d.GetSddlForm([Security.AccessControl.AccessControlSections]14));if(-not [PaccFileAcl]::Equivalent($b,$v,6)){throw 'PAI_MODEL'}
   foreach($kind in @('OWNER','WRITER','UNPROTECTED','EXTRA','FLAGS')){
    $d=[Security.AccessControl.RawSecurityDescriptor]::new($b,0)
    switch($kind){OWNER{$d.Owner=[Security.Principal.SecurityIdentifier]::new('S-1-5-18')} WRITER{$d.DiscretionaryAcl[0].AccessMask=0x1f01ff} UNPROTECTED{$d.SetFlags([Security.AccessControl.ControlFlags]([int]$d.ControlFlags -band (-bnot 0x1000)))} EXTRA{$d.DiscretionaryAcl.InsertAce(6,$d.DiscretionaryAcl[0])} FLAGS{$d.DiscretionaryAcl[0].AceFlags=[Security.AccessControl.AceFlags]8}}
    if([PaccFileAcl]::Equivalent($b,[PaccFileAcl]::Encode($d.GetSddlForm([Security.AccessControl.AccessControlSections]14)),6)){throw ('UNSAFE_MODEL_'+$kind)}
   }
  };Write-Output 'MODEL_PASS; TWO_FILES_P_PAI_AND_10_UNSAFE_CASES; NO_NATIVE_FILE_OPEN';$code=0
 }elseif($Mode -eq 'Launch'){
  if(-not $ctx.console){throw 'CONSOLE_REQUIRED'}
  $a='-NoProfile -ExecutionPolicy Bypass -File "'+$Script+'" -Mode Elevated -ExpectedScriptSha256 '+$self
  $child=Start-Process -FilePath $PSExe -Verb RunAs -ArgumentList $a -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru
  $code=$child.ExitCode;Write-Output ('SCM_FILE_ACL_EXIT='+$code)
 }else{
  if($Mode -eq 'Elevated' -and (-not $ctx.elevated -or -not $ctx.administrators -or -not $ctx.console -or $ctx.impersonating -or $ctx.sid -cne $ExpectedSid)){throw 'OPERATOR_OR_CONSOLE_DENY'}
  $pause=$Mode -eq 'Elevated'
  Assert-Plain $Target;$parent=[PaccFileAcl]::Open($Target,$false,$true);Assert-Parent
  for($j=0;$j -lt 2;$j++){
   $p=$plans[$j];$expectedPath=$Target+'\'+@('papacc.conf','papacc_server.exe')[$j];if($p.Path -cne $expectedPath){throw 'PLAN_PATH'}
   $h=[PaccFileAcl]::Open($p.Path,($Mode -eq 'Elevated'),$false);$f=[pscustomobject]@{Handle=$h;Plan=$p;Closed=$false};$files+=$f;Assert-File $f
   $sd=[PaccFileAcl]::Read($h)
   if($Mode -eq 'Verify'){if(-not [PaccFileAcl]::Equivalent([PaccFileAcl]::Encode($p.AfterSddl),$sd,6)){throw 'POSTIMAGE_MISMATCH'}}
   elseif([PaccFileAcl]::Digest($sd) -cne $p.BeforeNativeSdSha256){throw 'PREIMAGE_MISMATCH'}
   Write-Output ('FILE='+$p.Path+'; SHA256='+$p.Sha256+'; SD='+[PaccFileAcl]::Sddl($sd))
  }
  if($Mode -eq 'Elevated'){
   $ctx|ConvertTo-Json
   Write-Output 'EXACT TWO LAB FILES ONLY; parent unchanged. Config owner unchanged; executable owner becomes approved operator.'
   Write-Output 'ACE0/1/5 Modify -> ReadAndExecute; preserve groups and other rights; protect each file DACL with copied explicit ACEs.'
   Write-Output 'No binary/config byte changes, service, CA or CURRENT. Partial effect is NOT rollback. No retry.'
   foreach($f in $files){Write-Output ('PROPOSED '+$f.Plan.Path+' '+$f.Plan.AfterSddl)}
   $attempt=[Guid]::NewGuid().ToString('N');$confirm='CORRECT_SCM_FILES '+$attempt;Write-Output $confirm
   if([Console]::ReadLine() -cne $confirm){throw 'CONFIRMATION_DENY'}
   Assert-Parent;foreach($f in $files){Assert-File $f;if([PaccFileAcl]::Digest([PaccFileAcl]::Read($f.Handle)) -cne $f.Plan.BeforeNativeSdSha256){throw 'PRE_EFFECT_CHANGED'}}
   $fout=[IO.File]::Open($ReceiptRoot+'\manual-intent.json',[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
   try{$bytes=[Text.Encoding]::UTF8.GetBytes((@{attempt=$attempt;operator=$ctx;script=$self;plans=$plans;disposition='EFFECT_NOT_YET_CONFIRMED'}|ConvertTo-Json -Depth 10));$fout.Write($bytes,0,$bytes.Length);$fout.Flush($true)}finally{$fout.Dispose()};$intent=$true
   for($j=0;$j -lt 2;$j++){
    $f=$files[$j];Assert-Parent;Assert-File $f;if([PaccFileAcl]::Digest([PaccFileAcl]::Read($f.Handle)) -cne $f.Plan.BeforeNativeSdSha256){throw 'FILE_CHANGED_BEFORE_EFFECT'}
    $effect=$true;[PaccFileAcl]::Apply($f.Handle,[PaccFileAcl]::Encode($f.Plan.AfterSddl),($j -eq 1))
    Assert-File $f;if(-not [PaccFileAcl]::Equivalent([PaccFileAcl]::Encode($f.Plan.AfterSddl),[PaccFileAcl]::Read($f.Handle),6)){throw 'POSTIMAGE_MISMATCH'}
   };Assert-Parent;Write-Output 'TWO_FILE_POST_READBACK_PASS; PARENT_PASS_PRESERVED; SCM_EFFECTS=NONE'
  }else{Write-Output ($Mode.ToUpperInvariant()+'_PASS; ACL_MUTATIONS=NONE; PARENT_PASS_PRESERVED')};$code=0
 }
}catch{Write-Output ('STOP: '+$_.Exception.Message);$code=2}
finally{
 if($intent){foreach($f in $files){try{$post+=@{path=$f.Plan.Path;sddl=[PaccFileAcl]::Sddl([PaccFileAcl]::Read($f.Handle));sha256=[PaccFileAcl]::FileDigest($f.Handle)}}catch{$post+=@{path=$f.Plan.Path;state='UNKNOWN'};$code=2}}}
 foreach($f in $files){$f.Closed=[PaccFileAcl]::CloseHandle($f.Handle);if(-not $f.Closed){Write-Output ('FILE_CLOSE_UNCERTAIN '+$f.Plan.Path);$code=2}}
 if($parent -ne [IntPtr]::Zero){$parentClosed=[PaccFileAcl]::CloseHandle($parent);if(-not $parentClosed){Write-Output 'PARENT_CLOSE_UNCERTAIN';$code=2}}
 if($intent){try{$receipt=@{attempt=$attempt;effect_attempted=$effect;observed=$post;file_closure=@($files|ForEach-Object{@{path=$_.Plan.Path;closed=$_.Closed}});parent_closed=$parentClosed;descriptor_retained=[PaccFileAcl]::DescriptorRetained;exit=$code;rollback='NOT_CLAIMED';retry='DENY';acl_scope='TWO_EXACT_LAB_FILES';parent='NOT_MODIFIED';scm_effects='NONE';ed11='PARTIAL'}|ConvertTo-Json -Depth 8;$o=[IO.File]::Open($ReceiptRoot+'\manual-result.json',[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None);try{$b=[Text.Encoding]::UTF8.GetBytes($receipt);$o.Write($b,0,$b.Length);$o.Flush($true)}finally{$o.Dispose()}}catch{Write-Output 'RECEIPT_FAILURE';$code=2}}
 foreach($pin in $pins){$pin.Dispose()}
}
if($pause){Write-Output 'Resultado preservado. F10 fecha. Nunca repetir automaticamente.';try{while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}}catch{$code=2}}
exit $code
