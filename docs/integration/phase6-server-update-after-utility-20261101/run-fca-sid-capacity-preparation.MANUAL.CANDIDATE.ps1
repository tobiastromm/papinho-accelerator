#requires -Version 5.1
param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedOperationalRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedDecisionSha256
)
# CANDIDATE ONLY. One FCA; no D1/PDP recreation or automatic replies.
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
function New-PrepReadNative {
 if ('PapaccOneUtilityReadNative' -as [type]) { throw 'READLOCK_TYPE_EXISTS_NO_RETRY' }
 $assemblyName=New-Object Reflection.AssemblyName('PapaccOneUtilityReadNativeAssembly')
 $assembly=[AppDomain]::CurrentDomain.DefineDynamicAssembly($assemblyName,[Reflection.Emit.AssemblyBuilderAccess]::Run)
 $module=$assembly.DefineDynamicModule('ReadOnlyOriginals')
 $type=$module.DefineType('PapaccOneUtilityReadNative',[Reflection.TypeAttributes]'Public,Sealed,Abstract')
 $methods=@(
  @{Name='CreateFileW';Return=[IntPtr];Parameters=[Type[]]@([string],[uint32],[uint32],[IntPtr],[uint32],[uint32],[IntPtr])},
  @{Name='GetFileInformationByHandle';Return=[bool];Parameters=[Type[]]@([IntPtr],[IntPtr])},
  @{Name='CloseHandle';Return=[bool];Parameters=[Type[]]@([IntPtr])}
 )
 foreach($method in $methods){
  $builder=$type.DefinePInvokeMethod($method.Name,'kernel32.dll',[Reflection.MethodAttributes]'Public,Static,PinvokeImpl',
   [Reflection.CallingConventions]::Standard,$method.Return,$method.Parameters,[Runtime.InteropServices.CallingConvention]::Winapi,[Runtime.InteropServices.CharSet]::Unicode)
  $builder.SetImplementationFlags($builder.GetMethodImplementationFlags() -bor [Reflection.MethodImplAttributes]::PreserveSig)
 }
 return $type.CreateType()
}
function Get-PrepOriginalInfo([IntPtr]$Handle){
 $buffer=[Runtime.InteropServices.Marshal]::AllocHGlobal(52)
 try {
  if(-not $script:prepReadNative::GetFileInformationByHandle($Handle,$buffer)){throw 'ORIGINAL_INFO_DENY'}
  $words=New-Object 'System.Collections.Generic.List[uint32]'
  for($index=0;$index -lt 13;$index++){
   $raw=[Runtime.InteropServices.Marshal]::ReadInt32($buffer,4*$index)
   $words.Add([BitConverter]::ToUInt32([BitConverter]::GetBytes($raw),0))
  }
  return ,$words.ToArray()
 } finally {[Runtime.InteropServices.Marshal]::FreeHGlobal($buffer)}
}
function Get-PrepBorrowedHash([IntPtr]$Handle){
 $borrow=New-Object Microsoft.Win32.SafeHandles.SafeFileHandle($Handle,$false)
 $stream=$null;$hasher=$null
 try {
  $stream=New-Object IO.FileStream($borrow,[IO.FileAccess]::Read,4096,$false)
  $stream.Position=0;$hasher=[Security.Cryptography.SHA256]::Create()
  return ([BitConverter]::ToString($hasher.ComputeHash($stream))).Replace('-','').ToLowerInvariant()
 } finally {if($hasher){$hasher.Dispose()};if($stream){$stream.Dispose()};$borrow.Dispose()}
}
function Assert-PrepOriginal($Entry){
 $now=Get-PrepOriginalInfo $Entry.Handle
 # Reads can affect last-access time; exclusive child creation can affect
 # directory last-write time. Neither is a replacement of the held object.
 $fields=if($Entry.Directory){@(0,7,11,12)}else{@(0,5,6,7,8,9,10,11,12)}
 foreach($field in $fields){if($now[$field] -ne $Entry.Info[$field]){throw 'ORIGINAL_CONTINUITY_DENY'}}
}
function Add-PrepOriginal([string]$Path,[bool]$Directory,[bool]$AllowChildCreation=$false){
 if($script:prepHeld | Where-Object Path -CEQ $Path){return}
 # Only the strict protected creation-parent permits WRITE sharing for the
 # native child's FILE_ADD_FILE. Source chain/files deny WRITE and DELETE.
 $share=if($Directory -and $AllowChildCreation){[uint32]3}else{[uint32]1}
 $rights=if($Directory){[uint32]0x00120080}else{[uint32]0x00120081}
 $handle=$script:prepReadNative::CreateFileW($Path,$rights,$share,[IntPtr]::Zero,[uint32]3,[uint32]0x02200000,[IntPtr]::Zero)
 if($handle -eq [IntPtr]::Zero -or $handle -eq [IntPtr](-1)){throw 'READLOCK_OPEN_DENY'}
 $entry=[pscustomobject]@{Path=$Path;Handle=$handle;Info=$null;Directory=$Directory;CloseAttempted=$false}
 $script:prepHeld.Add($entry)
 $entry.Info=Get-PrepOriginalInfo $handle
 if(($entry.Info[0] -band 0x400) -or ((($entry.Info[0] -band 0x10) -ne 0) -ne $Directory)){throw 'TYPE_OR_REPARSE_DENY'}
}
function Add-PrepDirectoryChain([string]$Directory,[bool]$CreationParent=$false){
 $absolute=[IO.Path]::GetFullPath($Directory);$rootPath=[IO.Path]::GetPathRoot($absolute)
 if($rootPath -cne 'C:\' -or $absolute.Contains('..')){throw 'DIRECTORY_ROOT_DENY'}
 Add-PrepOriginal $rootPath $true
 $running=$rootPath
 foreach($component in $absolute.Substring($rootPath.Length).Split('\')){
  if(-not $component){continue};$running=Join-Path $running $component
  Add-PrepOriginal $running $true ($CreationParent -and $running -ceq $absolute)
 }
}
$script:prepHeld=New-Object 'System.Collections.Generic.List[object]'
$script:prepReadNative=$null
$invoked=$false;$nativeExit=$null;$closeKnown=$true
function Assert-FcaPin([string]$Path,[string]$Pin){
 if((Get-FileHash -LiteralPath $Path -Algorithm SHA256 -ErrorAction Stop).Hash -ne $Pin){throw 'PACKAGE_PIN_DENY'}
}
function Assert-FcaIdentity([string]$Path,[string]$Id){
 $entries=@($script:prepHeld | Where-Object Path -CEQ $Path)
 if($entries.Count -ne 1){throw 'REFERENCE_DENY'}
 $v=$entries[0].Info
 if(('{0}:{1}:{2}' -f $v[7],$v[11],$v[12]) -cne $Id){throw 'ORIGINAL_ID_DENY'}
}
function Assert-FcaFile([string]$Path,[long]$Size,[string]$Pin){
 Add-PrepOriginal $Path $false
 $entry=@($script:prepHeld | Where-Object Path -CEQ $Path)[0]
 if(([uint64]$entry.Info[8]*4294967296+[uint64]$entry.Info[9]) -ne $Size -or (Get-PrepBorrowedHash $entry.Handle) -cne $Pin){throw 'FILE_PIN_DENY'}
 Assert-PrepOriginal $entry
}
try {
 if($PSVersionTable.PSEdition -cne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess){throw 'POWERSHELL_51_AMD64_DENY'}
 if([Environment]::MachineName -cne 'TCORP-TOBIAS' -or [Console]::IsInputRedirected -or [Console]::IsOutputRedirected){throw 'HOST_CONSOLE_DENY'}
 Assert-FcaPin $PSCommandPath $ExpectedScriptSha256
 Assert-FcaPin (Join-Path $PSScriptRoot 'ONE-FCA-SID-CAPACITY-PREPARATION-REQUEST.CANDIDATE.md') $ExpectedOperationalRequestSha256
 Assert-FcaPin (Join-Path $PSScriptRoot 'OWNER-FCA-SID-CAPACITY-PREPARATION-CONTINUATION.txt') $ExpectedDecisionSha256
 Assert-FcaPin (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-first-instance-preparation-assurance-fca0-fca7-20261103.md') 'a4f8b9700c66bb8488ec78295d549596721b54f1fd31b4b430dc0fa9b7f0d1a7'
 Assert-FcaPin (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-data-parent-pdp0-pdp7-20261103.md') 'b8c9ae9573a9c010fde25f8856f6efb78575f0a45b500eeefae0dd42e93d4065'
 Assert-FcaPin (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-fca-pdp-physical-floor-3gib-20261103.md') 'c8ef29c776d02f36b3badcfdc40058b1313d14be705730a3b3b481b4e2e78d9f'
 $impersonated=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 try{if($null -ne $impersonated){throw 'IMPERSONATION_DENY'}}finally{if($impersonated){$impersonated.Dispose()}}
 $actor=[Security.Principal.WindowsIdentity]::GetCurrent()
 try{if($actor.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108' -or -not ([Security.Principal.WindowsPrincipal]::new($actor)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ACTOR_DENY'}}finally{$actor.Dispose()}
 $script:prepReadNative=New-PrepReadNative
 $binary='C:\Program Files\Papinho\Accelerator'
 $parent='C:\ProgramData\Papinho'
 $image=Join-Path $binary 'papacc_server.exe'
 $config=Join-Path $binary 'FIRST-INSTANCE-PREPARATION.CONFIG.ORIGINAL.conf'
 Add-PrepDirectoryChain $binary
 Add-PrepDirectoryChain $parent $true
 Assert-FcaIdentity 'C:\' '417614100:327680:5'
 Assert-FcaIdentity 'C:\Program Files' '417614100:131072:226316'
 Assert-FcaIdentity $binary '417614100:8257536:71209'
 Assert-FcaIdentity 'C:\ProgramData' '417614100:131072:227641'
 Assert-FcaIdentity $parent '417614100:1310720:452619'
 Assert-FcaFile $image 912384 'a3adc6a253bae9fd6589c7e6f74a9b2d4cc04ea17dcf13882f2e84ef28767f8c'
 Assert-FcaIdentity $image '417614100:1835008:1093619'
 Assert-FcaFile $config 43 '16c4847a27d32689ad7eb2c0bd29f6f0f698df74092e9df107cc8203e07e0ae7'
 Assert-FcaIdentity $config '417614100:2490368:315690'
 Assert-FcaFile (Join-Path $binary 'libssl-3-x64.dll') 1321472 '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
 Assert-FcaFile (Join-Path $binary 'libcrypto-3-x64.dll') 7217664 '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
 Write-Host 'ONE_INDEPENDENT_FCA_SID_CAPACITY; NO_D1_NO_PDP_NO_UPDATE_NO_START_NO_RETRY; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 Write-Host 'Compare native source417614100:2490368:315690 parent417614100:1310720:452619 leafAccelerator and actor/image. Cancel on mismatch. Only ONE exclusive directory and ONE MACHINE CA key create/finalize.'
 Write-Host 'Use only original OWNER_NEW_INSTALL_CA_INITIALIZE nonce shown by native owner. No automatic response. Fresh resource/protection/Stop/deadline/consumption gates remain mandatory. P4/K5/readiness not granted.'
 $nativeArguments=@('config','first-install','--declare','NEW_INSTALL_CA_PREPARE','--source',$config,'--parent',$parent,'--container','Accelerator')
 if($nativeArguments.Count -ne 10){throw 'EXACT_ARGV_DENY'}
 $invoked=$true
 & $image @nativeArguments
 $nativeExit=$LASTEXITCODE
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
 Write-Host ('MANUAL_FCA_EXIT='+$nativeExit+'; INVOKED='+$invoked+'; EXIT_NOT_AUTHORITY; NO_RETRY; HISTORICAL_RETENTION_AND_OS_QUIESCENCE_UNKNOWN')
 if($nativeExit -ne 0){throw 'FCA_FAILED_OR_UNKNOWN_NO_RETRY'}
}finally{
 for($index=$script:prepHeld.Count-1;$index -ge 0;$index--){
  $entry=$script:prepHeld[$index]
  if(-not $entry.CloseAttempted){$entry.CloseAttempted=$true
   if(-not $script:prepReadNative::CloseHandle($entry.Handle)){$closeKnown=$false;break}
  }
 }
 Write-Host ('FCA_WRAPPER_CLOSE_KNOWN='+$closeKnown+'; NOT_APPLICATION_CLOSE_NOT_READINESS; NO_RETRY')
 if(-not $closeKnown){Write-Host 'UNCERTAIN_REFERENCE_RETAINED_NO_TERMINATION';while($true){[Threading.Thread]::Sleep(60000)}}
}