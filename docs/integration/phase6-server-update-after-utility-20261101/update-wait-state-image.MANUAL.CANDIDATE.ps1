param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedDecisionSha256
)
# CANDIDATE ONLY. Do not execute before specific exact owner authorization.
# The read-only helper definitions below are reused byte-for-byte from the
# qualified preparation wrapper. They create no file or authority channel.
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
$updateInvoked=$false;$updateExit=2;$updateCloseUncertain=$false
try {
 if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess){throw 'POWERSHELL_51_X64_DENY'}
 if((Get-FileHash -LiteralPath $PSCommandPath).Hash -ne $ExpectedScriptSha256){throw 'SCRIPT_HASH_DENY'}
 $request=Join-Path $PSScriptRoot 'ONE-WAIT-STATE-IMAGE-UPDATE-REQUEST.CANDIDATE.md'
 if((Get-FileHash -LiteralPath $request).Hash -ne $ExpectedRequestSha256){throw 'REQUEST_HASH_DENY'}
 $decision=Join-Path $PSScriptRoot 'OWNER-WAIT-STATE-UPDATE-APPROVAL.txt'
 if((Get-FileHash -LiteralPath $decision).Hash -ne $ExpectedDecisionSha256){throw 'DECISION_HASH_DENY'}
 $norm=Join-Path $PSScriptRoot '..\phase6-staged-binary-update-20261101\UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md'
 $u2=Join-Path $PSScriptRoot '..\phase6-binary-update-u1-u7-20261101\U2-EXECUTOR-AND-CANDIDATE-DATA.CANDIDATE.md'
 if((Get-FileHash -LiteralPath $norm).Hash -ne '9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234' -or (Get-FileHash -LiteralPath $u2).Hash -ne '58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da'){throw 'NORM_HASH_DENY'}
 if([Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_METADATA_DENY'}
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  if($identity.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108'){throw 'OPERATOR_METADATA_DENY'}
  $principal=New-Object Security.Principal.WindowsPrincipal($identity)
  if(-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ADMIN_CONTEXT_DENY'}
 } finally {$identity.Dispose()}
 if([Environment]::GetFolderPath([Environment+SpecialFolder]::ProgramFiles) -cne 'C:\Program Files'){throw 'KNOWN_FOLDER_DENY'}
 $target='C:\Program Files\Papinho\Accelerator'
 $source='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
 $executor=Join-Path $target 'papacc_update.exe'
 $data=Join-Path $source 'papacc_server.exe'
 $old=Join-Path $target 'papacc_server.exe'
 $oldPin='11cbef6508b1b06fb56031b22b93dccc6ae3e05df22528190f3cf7215636d815'
 $newPin='044f10de34010b83075976ba39599a92381bce483ade9f780960b0fcc73010b1'
 $executorPin='084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a'
 $sslPin='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
 $cryptoPin='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
 $script:prepReadNative=New-PrepReadNative
 Add-PrepDirectoryChain $target $true
 Add-PrepDirectoryChain $source
 $files=@(
  @{Path=$executor;Bytes=104448;Pin=$executorPin},
  @{Path=$data;Bytes=805376;Pin=$newPin},
  @{Path=$old;Bytes=804352;Pin=$oldPin},
  @{Path=(Join-Path $target 'libssl-3-x64.dll');Bytes=1321472;Pin=$sslPin},
  @{Path=(Join-Path $target 'libcrypto-3-x64.dll');Bytes=7217664;Pin=$cryptoPin}
 )
 foreach($file in $files){
  Add-PrepOriginal $file.Path $false
  $entry=@($script:prepHeld|Where-Object Path -CEQ $file.Path)[0]
  if($entry.Info[8] -ne 0 -or $entry.Info[9] -ne $file.Bytes -or (Get-PrepBorrowedHash $entry.Handle) -cne $file.Pin){throw 'ORIGINAL_BYTES_HASH_DENY'}
  Assert-PrepOriginal $entry
 }
 $parent=@($script:prepHeld|Where-Object Path -CEQ $target)[0]
 $oldRef=@($script:prepHeld|Where-Object Path -CEQ $old)[0]
 $executorRef=@($script:prepHeld|Where-Object Path -CEQ $executor)[0]
 foreach($pair in @(@{Ref=$parent;Id=@(417614100,8257536,71209)},@{Ref=$oldRef;Id=@(417614100,5242880,349380)},@{Ref=$executorRef;Id=@(417614100,30670848,57859)})){
  if($pair.Ref.Info[7] -ne $pair.Id[0] -or $pair.Ref.Info[11] -ne $pair.Id[1] -or $pair.Ref.Info[12] -ne $pair.Id[2]){throw 'ORIGINAL_ID_DENY'}
 }
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
 # The readonly preflight lock on OLD must end BEFORE the native owner opens
 # OLD with DELETE for its two governed renames. Do not share DELETE broadly
 # or hold a conflicting wrapper handle across invocation. Native owner then
 # captures and pins OLD afresh; snapshot here never replaces that gate.
 $oldRef.CloseAttempted=$true
 if(-not $script:prepReadNative::CloseHandle($oldRef.Handle)){$updateCloseUncertain=$true;throw 'OLD_PREFLIGHT_CLOSE_UNKNOWN'}
 [void]$script:prepHeld.Remove($oldRef)
 Write-Host 'ONE_MANUAL_WAIT_STATE_IMAGE_UPDATE; PROTECTED_INDEPENDENT_EXECUTOR; WORKSPACE_DATA_ONLY; U1_U7_U2; ORIGINAL_NATIVE_GATES_AND_CONFIRMATION_REQUIRED; NO_RETRY_NO_ROLLBACK_NO_START; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 Write-Host 'Confirm ONLY the exact native plan, executor ID, DATA and old pins, two exclusive leaves, K/R and retained-old effects. No response supplied automatically. SCM STOPPED/PID0 and strict U2 are checked by native owner before effects.'
 $arguments=@('--owner-manual-binary-update',$data,
  '417614100','8257536','71209','417614100','5242880','349380',
  $oldPin,$newPin,$sslPin,$cryptoPin,
  '417614100','30670848','57859',$executorPin)
 if($arguments.Count -ne 16){throw 'EXACT_NATIVE_ARGV_DENY'}
 $updateInvoked=$true
 & $executor @arguments
 $updateExit=$LASTEXITCODE
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
} catch {
 if($updateInvoked){$updateCloseUncertain=$true}
 Write-Host ('CORRECTED_UPDATE_WRAPPER_DENY: '+$_.Exception.Message)
 $updateExit=2
} finally {
 for($i=$script:prepHeld.Count-1;$i -ge 0;$i--){
  $entry=$script:prepHeld[$i]
  if($entry.CloseAttempted){$updateCloseUncertain=$true;break};$entry.CloseAttempted=$true
  if(-not $script:prepReadNative::CloseHandle($entry.Handle)){$updateCloseUncertain=$true;break}
 }
 Write-Host ('MANUAL_SERVER_UPDATE_EXIT={0}; INVOKED={1}; WRAPPER_CLOSE_UNCERTAIN={2}; OS_QUIESCENCE=UNKNOWN; NO_RETRY_NO_ROLLBACK_NO_START_NO_PRODUCTIVE_PROMOTION' -f $updateExit,$updateInvoked,$updateCloseUncertain)
}
if($updateCloseUncertain){exit 2}
exit $updateExit