param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
# Candidate procedure. One manual invocation only after exact owner approval.
# Interop below can OPEN_EXISTING/read metadata/close only. No helper binary,
# temp compilation, directory/file creation, ACL edit or child authority channel.
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
 $rights=if($Directory){[uint32]0x001200a0}else{[uint32]0x00120081}
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
$prepInvoked=$false;$prepExit=2;$prepCloseUncertain=$false
try {
 if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess){throw 'POWERSHELL_51_X64_DENY'}
 if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash -ne $ExpectedScriptSha256){throw 'SCRIPT_HASH_DENY'}
 $prepRequest=Join-Path $PSScriptRoot 'ONE-UTILITY-PREPARATION-REQUEST.CANDIDATE.md'
 if((Get-FileHash -LiteralPath $prepRequest -Algorithm SHA256).Hash -ne $ExpectedRequestSha256){throw 'REQUEST_HASH_DENY'}
 $prepNorm=Join-Path $PSScriptRoot '..\phase6-update-utility-20261101\ONE-UTILITY-PREPARATION.CANDIDATE.md'
 if((Get-FileHash -LiteralPath $prepNorm -Algorithm SHA256).Hash -ne '029a611bb19e41827719c4269f6e029895a8f57f2d125141258274da8b2707fb'){throw 'NORMATIVE_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'FINAL-ARTIFACTS.json') -Algorithm SHA256).Hash -ne '103790944d90fbdcf18de1f5cb9c8a1fac340c71f1a6e1caf927b6411ac29cdc'){throw 'QUALIFIED_ARTIFACT_MANIFEST_DENY'}
 if([Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_METADATA_DENY'}
 $prepIdentity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  if($prepIdentity.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108'){throw 'OPERATOR_METADATA_DENY'}
  $prepPrincipal=New-Object Security.Principal.WindowsPrincipal($prepIdentity)
  if(-not $prepPrincipal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ADMIN_CONTEXT_DENY'}
 } finally {$prepIdentity.Dispose()}
 $prepKnownFolder=[Environment]::GetFolderPath([Environment+SpecialFolder]::ProgramFiles)
 if($prepKnownFolder -cne 'C:\Program Files'){throw 'KNOWN_FOLDER_DENY'}
 $prepTarget=Join-Path $prepKnownFolder 'Papinho\Accelerator'
 $prepSource='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
 $prepController=Join-Path $prepSource 'papacc_server.exe'
 $prepUtility=Join-Path $prepSource 'papacc_update.exe'
 if(Test-Path -LiteralPath (Join-Path $prepTarget 'papacc_update.exe')){throw 'EXISTING_LEAF_DENY_NO_ADOPTION'}
 $script:prepReadNative=New-PrepReadNative
 Add-PrepDirectoryChain $prepTarget $true
 Add-PrepDirectoryChain $prepSource
 # Exact final artifacts; neither the pins nor this wrapper grant authority.
 $prepFiles=@(
  @{Path=$prepController;Bytes=758272;Pin='7612d1114b641875ad4536a6e9a2a4899a9115674b8a098fe7cc8b7767b1cf7f'},
  @{Path=$prepUtility;Bytes=104448;Pin='6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87'},
  @{Path=(Join-Path $prepSource 'libssl-3-x64.dll');Bytes=1321472;Pin='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'},
  @{Path=(Join-Path $prepSource 'libcrypto-3-x64.dll');Bytes=7217664;Pin='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'},
  @{Path=(Join-Path $prepTarget 'papacc_server.exe');Bytes=571392;Pin='c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf'}
 )
 foreach($file in $prepFiles){
  Add-PrepOriginal $file.Path $false
  $entry=@($script:prepHeld | Where-Object Path -CEQ $file.Path)[0]
  if($entry.Info[8] -ne 0 -or $entry.Info[9] -ne $file.Bytes -or (Get-PrepBorrowedHash $entry.Handle) -cne $file.Pin){throw 'ORIGINAL_BYTES_HASH_DENY'}
  Assert-PrepOriginal $entry
 }
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
 $prepParent=@($script:prepHeld | Where-Object Path -CEQ $prepTarget)[0].Info
 $prepOld=@($script:prepHeld | Where-Object Path -CEQ (Join-Path $prepTarget 'papacc_server.exe'))[0].Info
 if($prepParent[7] -ne 417614100 -or $prepParent[11] -ne 8257536 -or $prepParent[12] -ne 71209 -or
  $prepOld[7] -ne 417614100 -or $prepOld[11] -ne 6225920 -or $prepOld[12] -ne 71237){throw 'ORIGINAL_TARGET_ID_DENY'}
 Write-Host 'ONE_MANUAL_UTILITY_PREPARATION; ONE_EXCLUSIVE_FILE; NO_RETRY_NO_UPDATE_NO_START; ORIGINAL_NATIVE_ACT_AND_CONFIRMATION_REQUIRED; BOOTSTRAP_OPAQUE_RISK; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 Write-Host 'Confirm only the exact native plan and original identities. No response is supplied automatically. Existing leaf denies. Partial file retained; no delete, rename, rollback or permission change.'
 $prepArguments=@('--owner-manual-prepare-update-utility',$prepUtility,'417614100','8257536','71209','417614100','6225920','71237',
  $prepFiles[4].Pin,$prepFiles[1].Pin,$prepFiles[2].Pin,$prepFiles[3].Pin,$prepFiles[0].Pin)
 $prepInvoked=$true
 & $prepController @prepArguments
 $prepExit=$LASTEXITCODE
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
} catch {
 Write-Host ('UTILITY_PREPARATION_WRAPPER_DENY: '+$_.Exception.Message)
 $prepExit=2
} finally {
 for($index=$script:prepHeld.Count-1;$index -ge 0;$index--){
  $entry=$script:prepHeld[$index]
  if($entry.CloseAttempted){$prepCloseUncertain=$true;break};$entry.CloseAttempted=$true
  if(-not $script:prepReadNative::CloseHandle($entry.Handle)){$prepCloseUncertain=$true;break}
 }
 Write-Host ('MANUAL_UTILITY_PREP_EXIT={0}; INVOKED={1}; WRAPPER_CLOSE_UNCERTAIN={2}; OS_QUIESCENCE=UNKNOWN; NO_RETRY_NO_UPDATE_NO_START' -f $prepExit,$prepInvoked,$prepCloseUncertain)
}
if($prepCloseUncertain){exit 2}
exit $prepExit
