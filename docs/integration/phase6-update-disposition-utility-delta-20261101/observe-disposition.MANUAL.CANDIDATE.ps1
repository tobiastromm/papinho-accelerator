param(
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedScriptSha256,
 [Parameter(Mandatory=$true)][ValidatePattern('^[a-f0-9]{64}$')][string]$ExpectedRequestSha256
)
# CANDIDATE. One manual READ-ONLY observation only after exact approval.
# Helpers reuse original-read machinery. OPEN_EXISTING only, no product entry.
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
  $builder=$type.DefineMethod($method.Name,[Reflection.MethodAttributes]'Public,Static,PinvokeImpl',$method.Return,$method.Parameters)
  if($true){
   $ctor=[Runtime.InteropServices.DllImportAttribute].GetConstructor([Type[]]@([string]))
   $fields=[Reflection.FieldInfo[]]@([Runtime.InteropServices.DllImportAttribute].GetField('EntryPoint'),[Runtime.InteropServices.DllImportAttribute].GetField('SetLastError'),[Runtime.InteropServices.DllImportAttribute].GetField('ExactSpelling'),[Runtime.InteropServices.DllImportAttribute].GetField('CharSet'))
   $values=[object[]]@($method.Name,$true,$true,[Runtime.InteropServices.CharSet]::Unicode)
   $attribute=[Reflection.Emit.CustomAttributeBuilder]::new($ctor,[object[]]@('kernel32.dll'),$fields,$values)
   $builder.SetCustomAttribute($attribute)
  }
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
function Get-DispOpenStatus([IntPtr]$Handle,[int]$ErrorCode){
 if($Handle -ne [IntPtr]::Zero -and $Handle -ne [IntPtr](-1)){return 'PRESENT'}
 if($ErrorCode -eq 2){return 'ABSENT_SNAPSHOT'}
 return 'UNKNOWN'
}
function Get-DispOriginal([string]$Path){
 $handle=$script:prepReadNative::CreateFileW($Path,[uint32]0x00120081,[uint32]1,[IntPtr]::Zero,[uint32]3,[uint32]0x02200000,[IntPtr]::Zero)
 $errorCode=[Runtime.InteropServices.Marshal]::GetLastWin32Error()
 $status=Get-DispOpenStatus $handle $errorCode
 if($status -ne 'PRESENT'){return [pscustomobject]@{path=$Path;status=$status;win32=$errorCode}}
 $entry=[pscustomobject]@{Path=$Path;Handle=$handle;Info=$null;Directory=$false;CloseAttempted=$false}
 $script:prepHeld.Add($entry);$entry.Info=Get-PrepOriginalInfo $handle
 if(($entry.Info[0] -band 0x410) -ne 0){throw 'FILE_REPARSE_OR_DIRECTORY_DENY'}
 return Get-DispRecord $entry
}
function Get-DispRecord($Entry){
 Assert-PrepOriginal $Entry
 $acl=Get-Acl -LiteralPath $Entry.Path -ErrorAction Stop
 $sd=$acl.GetSecurityDescriptorBinaryForm()
 if(-not $sd.Length -or $sd.Length -gt 65536){throw 'SD_LENGTH_UNKNOWN'}
 $hasher=[Security.Cryptography.SHA256]::Create()
 try {$sdHash=([BitConverter]::ToString($hasher.ComputeHash($sd))).Replace('-','').ToLowerInvariant()}finally{$hasher.Dispose()}
 $aces=@($acl.GetAccessRules($true,$true,[Security.Principal.SecurityIdentifier])|ForEach-Object {@{sid=$_.IdentityReference.Value;type=$_.AccessControlType.ToString();rights=[long]$_.FileSystemRights;inherited=$_.IsInherited;inheritance=$_.InheritanceFlags.ToString();propagation=$_.PropagationFlags.ToString()}})
 $pin=if($Entry.Directory){$null}else{Get-PrepBorrowedHash $Entry.Handle}
 Assert-PrepOriginal $Entry
 return [pscustomobject]@{path=$Entry.Path;status='PRESENT';volume=$Entry.Info[7];high=$Entry.Info[11];low=$Entry.Info[12];size_high=$Entry.Info[8];size_low=$Entry.Info[9];attributes=$Entry.Info[0];owner=$acl.GetOwner([Security.Principal.SecurityIdentifier]).Value;protected=$acl.AreAccessRulesProtected;sd_bytes=$sd.Length;sd_sha256=$sdHash;aces=$aces;sha256=$pin;continuity='HELD_SNAPSHOT_NOT_AUTHORITY'}
}
$script:prepHeld=New-Object 'System.Collections.Generic.List[object]'
$script:prepReadNative=$null
$records=@();$queries=@();$closeKnown=$true;$first=$null;$submitted=$false;$resultExit=2
try {
 if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSVersion.Minor -ne 1 -or -not [Environment]::Is64BitProcess){throw 'WINDOWS_POWERSHELL_51_X64_REQUIRED'}
 if((Get-FileHash -LiteralPath $PSCommandPath).Hash -ne $ExpectedScriptSha256){throw 'SCRIPT_HASH_DENY'}
 if((Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'ONE-READONLY-DISPOSITION-REQUEST.CANDIDATE.md')).Hash -ne $ExpectedRequestSha256){throw 'REQUEST_HASH_DENY'}
 if([Environment]::MachineName -cne 'TCORP-TOBIAS'){throw 'HOST_METADATA_DENY'}
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 try {
  if($identity.User.Value -cne 'S-1-5-21-785699550-1440458426-1682014700-1108'){throw 'OPERATOR_METADATA_DENY'}
  $principal=New-Object Security.Principal.WindowsPrincipal($identity)
  if(-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ALREADY_ELEVATED_REQUIRED'}
 } finally {$identity.Dispose()}
 if([Environment]::GetFolderPath([Environment+SpecialFolder]::ProgramFiles) -cne 'C:\Program Files'){throw 'KNOWN_FOLDER_DENY'}
 $target='C:\Program Files\Papinho\Accelerator'
 $script:prepReadNative=New-PrepReadNative
 $submitted=$true
 Add-PrepDirectoryChain $target
 foreach($entry in $script:prepHeld){$records+=Get-DispRecord $entry}
 $parent=@($script:prepHeld|Where-Object Path -CEQ $target)[0]
 if($parent.Info[7] -ne 417614100 -or $parent.Info[11] -ne 8257536 -or $parent.Info[12] -ne 71209){throw 'ORIGINAL_PARENT_DIVERGENT'}
 $attempt='742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b'
 foreach($leaf in @('papacc_server.exe',('papacc_server.'+$attempt+'.candidate'),('papacc_server.'+$attempt+'.previous'),'papacc_update.exe')){
  $record=Get-DispOriginal (Join-Path $target $leaf);$records+=$record
  if($record.status -eq 'UNKNOWN'){throw ('OBJECT_OPEN_UNKNOWN_WIN32_'+$record.win32)}
 }
 foreach($verb in @('qc','queryex','qsidtype')){
  $lines=& 'C:\Windows\System32\sc.exe' $verb PapinhoAccelerator 2>&1
  $scExit=$LASTEXITCODE
  $queries+=@{verb=$verb;exit=$scExit;output=($lines -join "`n")}
  if($scExit){throw ('SCM_QUERY_UNKNOWN_'+$scExit)}
 }
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
 $resultExit=0
} catch {$first=$_.Exception.Message;$resultExit=2}
finally {
 for($i=$script:prepHeld.Count-1;$i -ge 0;$i--){
  $entry=$script:prepHeld[$i]
  if($entry.CloseAttempted){$closeKnown=$false;break};$entry.CloseAttempted=$true
  if(-not $script:prepReadNative::CloseHandle($entry.Handle)){$closeKnown=$false;break}
 }
 if(-not $closeKnown){$resultExit=2}
 @{scope='ONE_MANUAL_DISPOSITION_READONLY_NO_PRODUCT_NO_MUTATION';first=$first;observation_submitted=$submitted;objects=$records;scm_queries=$queries;query_close_known=$closeKnown;historical_owner_close='UNKNOWN_UNCHANGED';OS_QUIESCENCE='UNKNOWN';authority='DENY';no_retry=$true;exit=$resultExit}|ConvertTo-Json -Depth 10
}
exit $resultExit