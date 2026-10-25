#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Check','Launch','Elevated','ModelTest')][string]$Mode='Check',
 [ValidatePattern('^$|^[0-9a-f]{64}$')][string]$ExpectedScriptSha256='',
 [Parameter(ValueFromRemainingArguments=$true)][string[]]$ExtraArguments=@())
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$Root='C:\Projetos\PapinhoAccelerator'
$ScriptPath=$Root+'\tools\qualification\scm-lab-parent-acl-uac.ps1'
$Target=$Root+'\build\scm-protected-installation-binding-20261022\Selected\Release'
$OperatorSid='S-1-5-21-785699550-1440458426-1682014700-1108'
$Before='O:S-1-5-21-896877160-3262526087-2538948522-1003G:S-1-5-21-896877160-3262526087-2538948522-513D:AI(A;OICIID;0x1301bf;;;S-1-5-21-896877160-3262526087-2538948522-1002)(A;OICIID;0x1301bf;;;S-1-5-21-1212450354-1333653649-3686394325-941509878)(A;OICIID;FA;;;BA)(A;OICIID;FA;;;SY)(A;OICIID;0x1200a9;;;BU)(A;ID;0x1301bf;;;AU)(A;OICIIOID;SDGXGWGR;;;AU)'
$ReceiptRoot=$Root+'\docs\integration\scm-lab-parent-acl-correction-20261023'
$PowerShellExe='C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
$ExeHash='3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb'
$ConfHash='a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44'
Add-Type -TypeDefinition @"
using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using Microsoft.Win32.SafeHandles;
public static class PaccParentAcl {
 [StructLayout(LayoutKind.Sequential)] public struct Info {
  public uint attributes; public System.Runtime.InteropServices.ComTypes.FILETIME creation,access,write;
  public uint volume,sizeHigh,sizeLow,links,indexHigh,indexLow;
 }
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)]
 static extern IntPtr CreateFileW(string p,uint access,uint share,IntPtr sa,uint disposition,uint flags,IntPtr template);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetFileInformationByHandle(IntPtr h,out Info i);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool CloseHandle(IntPtr h);
 [DllImport("kernel32.dll",SetLastError=true)] static extern bool GetFileInformationByHandleEx(IntPtr h,int cls,out uint flags,uint size);
 [DllImport("advapi32.dll")] static extern uint GetSecurityInfo(IntPtr h,int type,uint info,out IntPtr owner,out IntPtr group,out IntPtr dacl,out IntPtr sacl,out IntPtr sd);
 [DllImport("advapi32.dll")] static extern uint GetSecurityDescriptorLength(IntPtr sd);
 [DllImport("advapi32.dll")] static extern uint SetSecurityInfo(IntPtr h,int type,uint info,IntPtr owner,IntPtr group,IntPtr dacl,IntPtr sacl);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool GetSecurityDescriptorOwner(IntPtr sd,out IntPtr owner,out bool def);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool GetSecurityDescriptorDacl(IntPtr sd,out bool present,out IntPtr dacl,out bool def);
 [DllImport("kernel32.dll")] static extern IntPtr LocalFree(IntPtr h);
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int returned);
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll")] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 public static IntPtr Open(string path,bool effect) {
  // MAXIMUM_ALLOWED suppresses child ACE propagation per SetSecurityInfo documentation.
  // Share zero also rejects concurrent conflicting opens and blocks rename/delete sharing.
  IntPtr h=CreateFileW(path,effect?0x02000000u:0x00020080u,effect?0u:7u,IntPtr.Zero,3,0x02200000,IntPtr.Zero);
  if(h==new IntPtr(-1))throw new Win32Exception(Marshal.GetLastWin32Error());
  try { Identity(h); return h; } catch {if(!CloseHandle(h))throw new Exception("OPEN_VALIDATION_AND_CLOSE_FAILED");throw;}
 }
 public static string Identity(IntPtr h) {
  Info i;if(!GetFileInformationByHandle(h,out i))throw new Win32Exception(Marshal.GetLastWin32Error());
  if(i.volume!=417614100u||i.indexHigh!=1966080u||i.indexLow!=79426u||(i.attributes&0x410u)!=0x10u)throw new Exception("OBJECT_ID_OR_REPARSE_MISMATCH");
  return i.volume+":"+i.indexHigh+":"+i.indexLow;
 }
 public static string Read(IntPtr h) {
  Identity(h);IntPtr o,g,d,s,sd;uint e=GetSecurityInfo(h,1,7,out o,out g,out d,out s,out sd);
  if(e!=0)throw new Win32Exception((int)e);
  try {uint n=GetSecurityDescriptorLength(sd);if(n==0||n>65536)throw new Exception("SD_LENGTH");byte[] b=new byte[n];Marshal.Copy(sd,b,0,(int)n);return new RawSecurityDescriptor(b,0).GetSddlForm(AccessControlSections.Owner|AccessControlSections.Group|AccessControlSections.Access);}
  finally {if(LocalFree(sd)!=IntPtr.Zero)throw new Exception("SD_FREE_UNCERTAIN_STOP");}
 }
 public static void Apply(IntPtr h,string expected,string after) {
  if(Read(h)!=expected)throw new Exception("PREIMAGE_CHANGED");
  var sd=new RawSecurityDescriptor(after);byte[] b=new byte[sd.BinaryLength];sd.GetBinaryForm(b,0);
  var pin=GCHandle.Alloc(b,GCHandleType.Pinned);
  try {IntPtr o,d;bool def,present;
   if(!GetSecurityDescriptorOwner(pin.AddrOfPinnedObject(),out o,out def)||!GetSecurityDescriptorDacl(pin.AddrOfPinnedObject(),out present,out d,out def)||!present)throw new Exception("POST_DESCRIPTOR_INVALID");
   // OWNER + DACL + PROTECTED_DACL only. Group/SACL/integrity not written.
   uint e=SetSecurityInfo(h,1,0x80000005u,o,IntPtr.Zero,d,IntPtr.Zero);
   if(e!=0)throw new Win32Exception((int)e);
  } finally {pin.Free();}
 }
}
"@
function Build-After([string]$Sddl) {
 if($Sddl -cne $Before){throw 'DESCRIPTOR_PREIMAGE_MISMATCH'}
 $sd=[Security.AccessControl.RawSecurityDescriptor]::new($Sddl)
 if($sd.DiscretionaryAcl.Count -ne 7){throw 'ACE_COUNT'}
 # Preserve ACE order, types, trustees and inheritance scope; convert inherited to explicit
 # ONLY on this pinned parent, to stop the unsafe upstream ACEs being reintroduced.
 foreach($ace in $sd.DiscretionaryAcl){$ace.AceFlags=[Security.AccessControl.AceFlags]([int]$ace.AceFlags -band (-bnot 16))}
 foreach($i in @(0,1,5)){
  if($sd.DiscretionaryAcl[$i].AccessMask -ne 0x1301bf){throw 'MASK_PREIMAGE'}
  $sd.DiscretionaryAcl[$i].AccessMask=0x1200a9
 }
 $sd.Owner=[Security.Principal.SecurityIdentifier]::new($OperatorSid)
 $flags=([int]$sd.ControlFlags -bor 0x1000) -band (-bnot 0x500)
 $sd.SetFlags([Security.AccessControl.ControlFlags]$flags)
 return $sd.GetSddlForm([Security.AccessControl.AccessControlSections]::Owner -bor [Security.AccessControl.AccessControlSections]::Group -bor [Security.AccessControl.AccessControlSections]::Access)
}
function Test-Parent([string]$Sddl) {
 $sd=[Security.AccessControl.RawSecurityDescriptor]::new($Sddl)
 $allowed=@($OperatorSid,'S-1-5-18','S-1-5-32-544')
 if($allowed -cnotcontains $sd.Owner.Value -or $null -eq $sd.DiscretionaryAcl){return $false}
 $write=[uint32]0x500d0156
 foreach($ace in $sd.DiscretionaryAcl){
  if(([int]$ace.AceFlags -band 8) -ne 0){continue}
  if($ace.AceType -ne [Security.AccessControl.AceType]::AccessAllowed -and $ace.AceType -ne [Security.AccessControl.AceType]::AccessDenied){return $false}
  if($ace.AceType -eq [Security.AccessControl.AceType]::AccessAllowed -and ($ace.AccessMask -band $write) -ne 0 -and $allowed -cnotcontains $ace.SecurityIdentifier.Value){return $false}
 }
 return $true
}
function Assert-Plain([string]$Path) {
 $p=$Path
 while($p){if(([IO.File]::GetAttributes($p) -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_ANCESTOR'};$p=[IO.Path]::GetDirectoryName($p)}
}
function Open-Pin([string]$Path,[string]$Hash) {
 Assert-Plain $Path
 $s=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 try {$sha=[Security.Cryptography.SHA256]::Create();try {$v=([BitConverter]::ToString($sha.ComputeHash($s))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()};if($v -cne $Hash){throw 'HASH_MISMATCH'};return $s}
 catch {$s.Dispose();throw}
}
. ($Root+'\tools\qualification\scm-lab-acl-postimage.ps1')
$result=2;$h=[IntPtr]::Zero;$pins=@();$pause=$false;$intent=$false;$effect=$false;$post=$null
try {
 if($ExtraArguments.Count -ne 0 -or $PSCommandPath -cne $ScriptPath){throw 'UNCONTROLLED_ARGUMENT_OR_PATH'}
 $after=Build-After $Before
 if(-not (Test-Parent $after)){throw 'PROPOSED_PARENT_PREDICATE_FAILED'}
 if($Mode -eq 'ModelTest') {
  if(Test-Parent $Before){throw 'MODEL_ACCEPTED_UNSAFE_PREIMAGE'}
  $bad=$false;try {Build-After ($Before+' ') | Out-Null}catch{$bad=$true};if(-not $bad){throw 'MODEL_ACCEPTED_CHANGED_PREIMAGE'}
  $model=[Security.AccessControl.RawSecurityDescriptor]::new($after)
  foreach($i in @(0,1,5)){if($model.DiscretionaryAcl[$i].AccessMask -ne 0x1200a9){throw 'MODEL_MASK'}}
  if($model.DiscretionaryAcl[4].AccessMask -ne 0x1200a9 -or $model.DiscretionaryAcl[6].AccessMask -ne -536805376){throw 'MODEL_PRESERVATION'}
  Write-Output ('MODEL_PASS; NO_NATIVE_OPEN; NO_MUTATION; AFTER='+$after);$result=0
 } else {
  Assert-Plain $ScriptPath;Assert-Plain $Target
  $selfHash=(Get-FileHash -LiteralPath $ScriptPath -Algorithm SHA256).Hash.ToLowerInvariant()
  if($Mode -eq 'Elevated' -and ($ExpectedScriptSha256 -eq '' -or $ExpectedScriptSha256 -cne $selfHash)){throw 'SCRIPT_PIN_MISMATCH'}
  $pins+=Open-Pin $ScriptPath $selfHash
  $pins+=Open-Pin ($Target+'\papacc_server.exe') $ExeHash
  $pins+=Open-Pin ($Target+'\papacc.conf') $ConfHash
  # No target open or mutation in launcher; actual object revalidated in elevated child.
  $cm=0;$console=[PaccParentAcl]::GetConsoleMode([PaccParentAcl]::GetStdHandle(-10),[ref]$cm) -and [PaccParentAcl]::GetConsoleMode([PaccParentAcl]::GetStdHandle(-11),[ref]$cm)
  if($Mode -eq 'Launch') {
   if(-not $console){throw 'CONSOLE_REQUIRED'}
   $a='-NoProfile -ExecutionPolicy Bypass -File "'+$ScriptPath+'" -Mode Elevated -ExpectedScriptSha256 '+$selfHash
   $child=Start-Process -FilePath $PowerShellExe -Verb RunAs -ArgumentList $a -WorkingDirectory $Root -WindowStyle Normal -Wait -PassThru
   $result=$child.ExitCode;Write-Output ('LAB_PARENT_ACL_EXIT='+$result)
  } else {
   $h=[PaccParentAcl]::Open($Target,($Mode -eq 'Elevated'))
   $observed=[PaccParentAcl]::Read($h)
   if($observed -cne $Before){throw 'PREIMAGE_DIVERGED_STOP_NO_RETRY'}
   Write-Output ('OBJECT='+[PaccParentAcl]::Identity($h));Write-Output ('BEFORE='+$observed);Write-Output ('PROPOSED='+$after)
   if($Mode -eq 'Check') {Write-Output 'CHECK_ONLY; MUTATIONS=NONE';$result=0}
   else {
    $id=[Security.Principal.WindowsIdentity]::GetCurrent()
    try {$admin=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator);$sid=$id.User.Value; $e=0;$et=0;$n=0;$eq=[PaccParentAcl]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$n);$tq=[PaccParentAcl]::GetTokenInformation($id.Token,18,[ref]$et,4,[ref]$n)}finally{$id.Dispose()}
    $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true);if($thread){$thread.Dispose();throw 'IMPERSONATION_DENY'}
    if(-not $console -or -not $admin -or -not $eq -or -not $tq -or $e -ne 1 -or $et -eq 3 -or $sid -cne $OperatorSid){throw 'EFFECTIVE_OPERATOR_OR_CONSOLE_DENY'}
    $pause=$true
    Write-Output 'LAB parent ONLY: owner becomes displayed operator; ACE0/1/5 Modify becomes ReadAndExecute. Other ACE masks preserved.'
    Write-Output 'Parent inheritance becomes protected; existing inherited ACEs copied explicitly. No child ACL propagation (MAXIMUM_ALLOWED handle).'
    Write-Output 'Build tooling may lose write access. No child remediation, service creation or operational authority. No automatic rollback/retry.'
    $nonce=[Guid]::NewGuid().ToString('N');$confirm='CORRECT_LAB_PARENT '+$nonce
    Write-Output $confirm
    if([Console]::ReadLine() -cne $confirm){throw 'CONFIRMATION_DENIED'}
    if([PaccParentAcl]::Read($h) -cne $Before){throw 'PRE_EFFECT_CHANGED'}
    # Durable one-shot intent before the effect: failure blocks retry even if descriptor looks unchanged.
    $intentPath=$ReceiptRoot+'\manual-intent.json'
    $stream=[IO.File]::Open($intentPath,[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
    try {$b=[Text.Encoding]::UTF8.GetBytes((@{operator=$sid;attempt=$nonce;before=$Before;after=$after;script=$selfHash;effect='PREPARED_DISPOSITION_UNKNOWN_UNTIL_RESULT'}|ConvertTo-Json));$stream.Write($b,0,$b.Length);$stream.Flush($true)}finally{$stream.Dispose()}
    $intent=$true;$effect=$true
    [PaccParentAcl]::Apply($h,$Before,$after)
    $post=[PaccParentAcl]::Read($h)
    if(-not (Test-PaccLabAclPostimage $after $post '417614100:1966080:79426' ([PaccParentAcl]::Identity($h))) -or -not (Test-Parent $post)){throw 'POST_READBACK_MISMATCH_STOP'}
    Write-Output 'PARENT_DESCRIPTOR_READBACK_PASS; SCM_EFFECTS=NONE; FULL_BINDING=NOT_YET_RUN'
    $result=0
   }
  }
 }
} catch {Write-Output ('STOP: '+$_.Exception.Message);$result=2}
finally {
 if($h -ne [IntPtr]::Zero){
  if($effect -and $null -eq $post){try{$post=[PaccParentAcl]::Read($h)}catch{Write-Output 'POST_DESCRIPTOR=UNKNOWN'}}
  if(-not [PaccParentAcl]::CloseHandle($h)){Write-Output 'HANDLE_CLOSE_UNCERTAIN_STOP';$result=2}
 }
 foreach($pin in $pins){$pin.Dispose()}
 if($intent){
  try {$receipt=@{exit=$result;after_observed=$post;effect_attempted=$effect;rollback='NOT_CLAIMED';retry='DENY';scm_effects='NONE';ed11='PARTIAL'}|ConvertTo-Json
   $f=[IO.File]::Open($ReceiptRoot+'\manual-result.json',[IO.FileMode]::CreateNew,[IO.FileAccess]::Write,[IO.FileShare]::None)
   try{$b=[Text.Encoding]::UTF8.GetBytes($receipt);$f.Write($b,0,$b.Length);$f.Flush($true)}finally{$f.Dispose()}
  }catch{Write-Output 'RECEIPT_FAILED_STOP';$result=2}
 }
}
if($pause){Write-Output 'Resultado preservado. F10 fecha. Nunca repetir automaticamente.';try{while([Console]::ReadKey($true).Key -ne [ConsoleKey]::F10){}}catch{$result=2}}
exit $result
