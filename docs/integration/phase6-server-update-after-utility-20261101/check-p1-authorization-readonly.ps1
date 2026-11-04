$ErrorActionPreference='Stop'
# One preparation check expressly required by the owner's authorization.
# Reuse ONLY the approved read-only function definitions. NEVER run procedure body.
$procedure=Join-Path $PSScriptRoot 'update-p1-image.MANUAL.CANDIDATE.ps1'
if((Get-FileHash -LiteralPath $procedure).Hash -ne '89cb7ef92a5cc7e8d22affe6c0ea366ba69e280865b3b95dd969a29e886b9ad9'){throw 'PROCEDURE_PIN_DENY'}
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile($procedure,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'AST_DENY'}
$helpers=@($ast.FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst]},$false))
if($helpers.Count -ne 6){throw 'READONLY_HELPER_COUNT_DENY'}
foreach($helper in $helpers){. ([scriptblock]::Create($helper.Extent.Text))}
$script:prepHeld=New-Object 'System.Collections.Generic.List[object]'
$script:prepReadNative=New-PrepReadNative
$target='C:\Program Files\Papinho\Accelerator'
$source='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
$rows=New-Object 'System.Collections.Generic.List[object]'
$closed=$false;$first=$null
try {
 Add-PrepDirectoryChain $target
 Add-PrepDirectoryChain $source
 $files=@(
  @{Path=(Join-Path $target 'papacc_update.exe');Bytes=104448;Pin='084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a';Id=@(417614100,30670848,57859)},
  @{Path=(Join-Path $target 'papacc_server.exe');Bytes=763904;Pin='7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d';Id=@(417614100,1835008,134613)},
  @{Path=(Join-Path $source 'papacc_server.exe');Bytes=772608;Pin='54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50';Id=$null},
  @{Path=(Join-Path $target 'libssl-3-x64.dll');Bytes=1321472;Pin='3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed';Id=$null},
  @{Path=(Join-Path $target 'libcrypto-3-x64.dll');Bytes=7217664;Pin='09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044';Id=$null}
 )
 foreach($file in $files){
  Add-PrepOriginal $file.Path $false
  $entry=@($script:prepHeld|Where-Object Path -CEQ $file.Path)[0]
  if($entry.Info[8] -ne 0 -or $entry.Info[9] -ne $file.Bytes -or (Get-PrepBorrowedHash $entry.Handle) -cne $file.Pin){throw 'BINARY_PIN_DENY'}
  if($file.Id){if($entry.Info[7] -ne $file.Id[0] -or $entry.Info[11] -ne $file.Id[1] -or $entry.Info[12] -ne $file.Id[2]){throw 'BINARY_ID_DENY'}}
 }
 foreach($entry in $script:prepHeld){
  Assert-PrepOriginal $entry
  $acl=Get-Acl -LiteralPath $entry.Path
  $owner=$acl.GetOwner([Security.Principal.SecurityIdentifier]).Value
  $sd=$acl.GetSecurityDescriptorBinaryForm();$hash=[Security.Cryptography.SHA256]::Create()
  try{$digest=([BitConverter]::ToString($hash.ComputeHash($sd))).Replace('-','').ToLowerInvariant()}finally{$hash.Dispose()}
  # Read native SIDs directly: never resolve ACL names through a domain.
  $aces=@($acl.GetAccessRules($true,$true,[Security.Principal.SecurityIdentifier])|ForEach-Object {[ordered]@{sid=$_.IdentityReference.Value;type=[string]$_.AccessControlType;rights=[int]$_.FileSystemRights;inheritance=[string]$_.InheritanceFlags;propagation=[string]$_.PropagationFlags;inherited=$_.IsInherited}})
  if($entry.Path -ceq $target -or $entry.Path.StartsWith($target+'\',[StringComparison]::Ordinal)){
   if($owner -cne 'S-1-5-32-544' -or -not $acl.AreAccessRulesProtected){throw 'INSTALLED_OWNER_OR_PROTECTION_DENY'}
  }
  Assert-PrepOriginal $entry
  $rows.Add([ordered]@{path=$entry.Path;id=('{0}:{1}:{2}' -f $entry.Info[7],$entry.Info[11],$entry.Info[12]);bytes=$entry.Info[9];directory=$entry.Directory;owner=$owner;dacl_protected=$acl.AreAccessRulesProtected;sd_sha256=$digest;aces=$aces;continuity_snapshot=$true})
 }
 $parent=@($script:prepHeld|Where-Object Path -CEQ $target)[0]
 if($parent.Info[7] -ne 417614100 -or $parent.Info[11] -ne 8257536 -or $parent.Info[12] -ne 71209){throw 'PARENT_ID_DENY'}
} catch {$first=$_.Exception.Message}
finally {
 $closed=$true
 for($i=$script:prepHeld.Count-1;$i -ge 0;$i--){
  $entry=$script:prepHeld[$i]
  if($entry.CloseAttempted){$closed=$false;break};$entry.CloseAttempted=$true
  if(-not $script:prepReadNative::CloseHandle($entry.Handle)){$closed=$false;break}
 }
}
$result=[ordered]@{scope='OWNER_REQUIRED_AUTHORIZATION_CHECK_ONLY';first=$first;close_known=$closed;objects=$rows;update_invoked=$false;effects=0;os_quiescence='UNKNOWN';historical_retention='UNKNOWN';not_fresh_lease=$true}
[IO.File]::WriteAllText((Join-Path $PSScriptRoot 'P1-UPDATE-AUTHORIZATION-READONLY-SID.json'),($result|ConvertTo-Json -Depth 9),[Text.UTF8Encoding]::new($false))
if($first -or -not $closed){throw 'AUTHORIZATION_CHECK_DENY_NO_UPDATE_NO_RETRY'}
'P1_AUTHORIZATION_READONLY_CHECK_PASS; CLOSE_KNOWN; NO_UPDATE_NO_START_NO_RETRY'
