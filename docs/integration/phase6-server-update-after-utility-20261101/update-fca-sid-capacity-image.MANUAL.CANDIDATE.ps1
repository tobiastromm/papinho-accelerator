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
 $request=Join-Path $PSScriptRoot 'ONE-FCA-SID-CAPACITY-IMAGE-UPDATE-REQUEST.CANDIDATE.md'
 if((Get-FileHash -LiteralPath $request).Hash -ne $ExpectedRequestSha256){throw 'REQUEST_HASH_DENY'}
 $decision=Join-Path $PSScriptRoot 'OWNER-FCA-SID-CAPACITY-IMAGE-UPDATE-APPROVAL.txt'
 if((Get-FileHash -LiteralPath $decision).Hash -ne $ExpectedDecisionSha256){throw 'DECISION_HASH_DENY'}
 $norm=Join-Path $PSScriptRoot '..\phase6-staged-binary-update-20261101\UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md'
 $u2=Join-Path $PSScriptRoot '..\phase6-binary-update-u1-u7-20261101\U2-EXECUTOR-AND-CANDIDATE-DATA.CANDIDATE.md'
 if((Get-FileHash -LiteralPath $norm).Hash -ne '9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234' -or (Get-FileHash -LiteralPath $u2).Hash -ne '58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da'){throw 'NORM_HASH_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'MACHINE-FIXTURE-RESOURCE-ENVELOPE.CANDIDATE.md')).Hash -ne '611c1ad8a78de7cf7c07aa8773906875c64aebaf25b72c6ed2d80e754ef5feda'){throw 'FIXTURE_ENVELOPE_NORM_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'MACHINE-FIXTURE-GROWTH-ENVELOPE.CANDIDATE.md')).Hash -ne 'fa443a1cc70011ccd60ed40a602df45c13acb59d4ae1c0fe48cd2667436f8000'){throw 'FIXTURE_GROWTH_NORM_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'MACHINE-FIXTURE-GA-READBACK.CANDIDATE.md')).Hash -ne '3b090974b4abbf5db2543e8ef223cebf78ec8a4b08581d9fa3d7e7beb9b53d47'){throw 'FIXTURE_GA_READBACK_NORM_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-machine-fixture-ga-readback-s0-s5-20261103.md')).Hash -ne '8eeb6da08d28c9f22d3df75cd840758db7e5f57ceda7db3f6f4db2e428ac1186'){throw 'FIXTURE_GA_READBACK_ACCEPTED_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'READ-INCONCLUSIVE-FINITE-CLOSURE.CANDIDATE.md')).Hash -ne 'ca514af6bc860d88305d14534f60244d22f6692311ac6fab6965b97040596e42'){throw 'READ_INCONCLUSIVE_PROPOSAL_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-scq-read-inconclusive-i0-i6-20261103.md')).Hash -ne '5d3e745483bbe9d7e72e141a5708b5474a03c99037eef13f0b54fdef8d728e46'){throw 'READ_INCONCLUSIVE_ACCEPTED_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'FIRST-INSTANCE-PREPARATION-ASSURANCE-FCA0-FCA7.CANDIDATE.md')).Hash -ne '365103d7f06cbe55dd0a1a260e4a274ad5db6dab40ae9dee6864edaf83803f5a'){throw 'FCA_PROPOSAL_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-first-instance-preparation-assurance-fca0-fca7-20261103.md')).Hash -ne 'a4f8b9700c66bb8488ec78295d549596721b54f1fd31b4b430dc0fa9b7f0d1a7'){throw 'FCA_ACCEPTED_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'FCA-DATA-PARENT-PDP0-PDP7.CANDIDATE.md')).Hash -ne 'e68969bed2a8d84ab366378cedc991bc1f105964cfcfff4c7e099d56cd8acd14'){throw 'PDP_PROPOSAL_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-data-parent-pdp0-pdp7-20261103.md')).Hash -ne 'b8c9ae9573a9c010fde25f8856f6efb78575f0a45b500eeefae0dd42e93d4065'){throw 'PDP_ACCEPTED_PIN_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot '..\..\adr\DECISION-personal-fca-pdp-physical-floor-3gib-20261103.md')).Hash -ne 'c8ef29c776d02f36b3badcfdc40058b1313d14be705730a3b3b481b4e2e78d9f'){throw 'FCA_PHYSICAL_3GIB_NORM_PIN_DENY'}
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
 $oldPin='c4419f97a2e4499da4e30b8a5316a16e55097d18384fc70a0db58870c4188556'
 $newPin='a3adc6a253bae9fd6589c7e6f74a9b2d4cc04ea17dcf13882f2e84ef28767f8c'
 $executorPin='084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a'
 $sslPin='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
 $cryptoPin='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
 $script:prepReadNative=New-PrepReadNative
 Add-PrepDirectoryChain $target $true
 Add-PrepDirectoryChain $source
 $files=@(
  @{Path=$executor;Bytes=104448;Pin=$executorPin},
  @{Path=$data;Bytes=912384;Pin=$newPin},
  @{Path=$old;Bytes=912384;Pin=$oldPin},
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
 foreach($pair in @(@{Ref=$parent;Id=@(417614100,8257536,71209)},@{Ref=$oldRef;Id=@(417614100,851968,606522)},@{Ref=$executorRef;Id=@(417614100,30670848,57859)})){
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
 Write-Host 'ONE_MANUAL_FCA_SID_CAPACITY_IMAGE_UPDATE; PROTECTED_INDEPENDENT_EXECUTOR; WORKSPACE_DATA_ONLY; U1_U7_U2; ORIGINAL_NATIVE_GATES_AND_CONFIRMATION_REQUIRED; NO_RETRY_NO_ROLLBACK_NO_START; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION'
 Write-Host 'Confirm ONLY the exact native plan, executor ID, DATA and old pins, two exclusive leaves, K/R and retained-old effects. No response supplied automatically. SCM STOPPED/PID0 and strict U2 are checked by native owner before effects.'
 $arguments=@('--owner-manual-binary-update',$data,
  '417614100','8257536','71209','417614100','851968','606522',
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