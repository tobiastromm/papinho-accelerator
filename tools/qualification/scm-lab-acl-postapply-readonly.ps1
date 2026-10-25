#requires -Version 5.1
param([switch]$SelfTest)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator'
. ($root+'\tools\qualification\scm-lab-acl-postimage.ps1')
$i=Get-Content -LiteralPath ($root+'\docs\integration\scm-lab-parent-acl-correction-20261023\manual-intent.json') -Raw | ConvertFrom-Json
$r=Get-Content -LiteralPath ($root+'\docs\integration\scm-lab-parent-acl-correction-20261023\manual-result.json') -Raw | ConvertFrom-Json
$expected=$i.after;$identity='417614100:1966080:79426'
function Assert-Test([bool]$v,[string]$name){if(-not $v){throw ('FAIL '+$name)};Write-Output ('PASS '+$name)}
if($SelfTest){
 Assert-Test (Test-PaccLabAclPostimage $expected $expected $identity $identity) 'PROTECTED_P'
 Assert-Test (Test-PaccLabAclPostimage $expected $r.after_observed $identity $identity) 'PROTECTED_PAI'
 foreach($case in @('UNPROTECTED','OWNER','EXTRA_ACE','RIGHTS','PRINCIPAL','ORDER','FLAGS','OTHER_CONTROL')){
  $d=[Security.AccessControl.RawSecurityDescriptor]::new($expected)
  switch($case){
   UNPROTECTED {$d.SetFlags([Security.AccessControl.ControlFlags]([int]$d.ControlFlags -band (-bnot 0x1000)))}
   OWNER {$d.Owner=[Security.Principal.SecurityIdentifier]::new('S-1-5-18')}
   EXTRA_ACE {$d.DiscretionaryAcl.InsertAce(7,$d.DiscretionaryAcl[0])}
   RIGHTS {$d.DiscretionaryAcl[0].AccessMask=0x1f01ff}
   PRINCIPAL {$d.DiscretionaryAcl[0].SecurityIdentifier=[Security.Principal.SecurityIdentifier]::new('S-1-1-0')}
   ORDER {$x=$d.DiscretionaryAcl[0];$d.DiscretionaryAcl.RemoveAce(0);$d.DiscretionaryAcl.InsertAce(1,$x)}
   FLAGS {$d.DiscretionaryAcl[0].AceFlags=[Security.AccessControl.AceFlags]8}
   OTHER_CONTROL {$d.SetFlags([Security.AccessControl.ControlFlags]([int]$d.ControlFlags -bor 0x100))}
  }
  $v=$d.GetSddlForm([Security.AccessControl.AccessControlSections]7)
  Assert-Test (-not (Test-PaccLabAclPostimage $expected $v $identity $identity)) $case
 }
 Assert-Test (-not (Test-PaccLabAclPostimage $expected $expected $identity '417614100:1966080:79427')) 'SUBSTITUTED_OBJECT'
 Write-Output 'SELFTEST_PASS; ACL_MUTATIONS=NONE';exit 0
}
Add-Type -TypeDefinition @"

using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Security.AccessControl;
using Microsoft.Win32.SafeHandles;
public static class PaccAclReadOnly {
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
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool GetSecurityDescriptorOwner(IntPtr sd,out IntPtr owner,out bool def);
 [DllImport("advapi32.dll",SetLastError=true)] static extern bool GetSecurityDescriptorDacl(IntPtr sd,out bool present,out IntPtr dacl,out bool def);
 [DllImport("kernel32.dll")] static extern IntPtr LocalFree(IntPtr h);
 [DllImport("advapi32.dll",SetLastError=true)] public static extern bool GetTokenInformation(IntPtr h,int cls,out int value,int size,out int returned);
 [DllImport("kernel32.dll")] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll")] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 public static IntPtr Open(string path) {
  IntPtr h=CreateFileW(path,0x00020080u,1u,IntPtr.Zero,3,0x02200000,IntPtr.Zero);
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
}
"@
$h=[IntPtr]::Zero;$code=2
try {
 $h=[PaccAclReadOnly]::Open($root+'\build\scm-protected-installation-binding-20261022\Selected\Release')
 $id=[PaccAclReadOnly]::Identity($h);$sd=[PaccAclReadOnly]::Read($h)
 if(-not (Test-PaccLabAclPostimage $expected $sd $identity $id)){throw 'POSTIMAGE_REJECTED'}
 Write-Output ('OBJECT='+$id);Write-Output ('OBSERVED='+$sd);Write-Output 'ACL_POSTAPPLY_VERIFIER=PASS; ACL_MUTATIONS=NONE; SCM_EFFECTS=NONE';$code=0
}finally{if($h -ne [IntPtr]::Zero -and -not [PaccAclReadOnly]::CloseHandle($h)){$code=2;Write-Output 'CLOSE_UNCERTAIN'}}
exit $code
