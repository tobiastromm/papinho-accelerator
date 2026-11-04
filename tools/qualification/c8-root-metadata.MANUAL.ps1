#requires -Version 5.1
[CmdletBinding()]
param([Parameter(Mandatory=$true)][ValidatePattern('^[a-fA-F0-9]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$identity=$null
$result=[ordered]@{first='NONE';scope='C8_LOCATOR_METADATA_ONLY';root='C:\ProgramData\PapinhoAccelerator';entries=@();complete=$false;publication='UNKNOWN';original_object_identity='NOT_ACQUIRED';RAW1='NOT_VERIFIED';ownerAuth='NOT_ACCESSED';authority='DENY';no_retry=$true}
try {
 if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash -ine $ExpectedScriptSha256){throw 'SCRIPT_HASH_DENY'}
 if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or -not [Environment]::Is64BitProcess){throw 'PS51_X64_REQUIRED'}
 $identity=[Security.Principal.WindowsIdentity]::GetCurrent()
 $principal=[Security.Principal.WindowsPrincipal]::new($identity)
 if(-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ALREADY_ELEVATED_REQUIRED'}
 $result.operator_sid=$identity.User.Value
 $result.pid=$PID
 $result.observed_utc=[DateTime]::UtcNow.ToString('o')
 # Bounded name/ACL metadata only. Never follow reparse points or open file contents.
 # No file hashes, registry, service configuration, credential APIs or process memory.
 $queue=[Collections.Generic.Queue[object]]::new()
 $queue.Enqueue(@($result.root,0))
 $count=0
 while($queue.Count -gt 0){
  $item=$queue.Dequeue();$path=[string]$item[0];$depth=[int]$item[1]
  $dir=Get-Item -LiteralPath $path -Force
  if(($dir.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0){throw 'REPARSE_POINT_STOP'}
  foreach($entry in (Get-ChildItem -LiteralPath $path -Force)){
   $count++;if($count -gt 256){throw 'ENTRY_BOUND_STOP'}
   $link=($entry.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0
   $candidate=$entry.Name -match '(?i)c8|root|manifest|publication|current|epoch'
   $owner=$null
   if($candidate -and -not $link){$owner=(Get-Acl -LiteralPath $entry.FullName).Owner}
   $result.entries+= [ordered]@{path=$entry.FullName;directory=[bool]$entry.PSIsContainer;reparse=$link;candidate_name_only=$candidate;acl_owner_metadata=$owner;classification='UNVERIFIED_METADATA_NOT_AUTHORITY'}
   if($entry.PSIsContainer -and -not $link){
    if($depth -lt 2){$queue.Enqueue(@($entry.FullName,$depth+1))}
    else {$result.depth_limited=$true}
   }
  }
 }
 $result.complete=$true
} catch {$result.first=$_.Exception.Message;$result.complete=$false}
finally {if($null -ne $identity){$identity.Dispose()}}
$result | ConvertTo-Json -Depth 7
# Output is a locator receipt, never a publication/continuity or custody proof.
if($result.first -ne 'NONE'){exit 2}
