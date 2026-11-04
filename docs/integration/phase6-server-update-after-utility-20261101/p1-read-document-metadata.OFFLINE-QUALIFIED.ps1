# Pure policy function, to be bound into the next P1 manual driver after separate operational approval.
function Assert-DocumentMetadata([string]$Path) {
 $acl=Get-Acl -LiteralPath $Path -ErrorAction Stop
 if($acl.GetOwner([Security.Principal.SecurityIdentifier]).Value -cne $sessionActor -or -not $acl.AreAccessRulesProtected){throw 'DOCUMENT_OWNER_PROTECTION_DENY'}
 $rules=@($acl.GetAccessRules($true,$true,[Security.Principal.SecurityIdentifier]))
 if($rules.Count-ne4){throw 'DOCUMENT_DACL_DENY'}
 foreach($sid in @($sessionActor,'S-1-5-18','S-1-5-32-544','S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099')){
  $matched=@($rules|Where-Object {$_.IdentityReference.Value -ceq $sid})
  if($matched.Count-ne1){throw 'DOCUMENT_DACL_SID_DENY'};$r=$matched[0]
  $expectedRights=if($sid-ceq'S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099'){0x00120089}else{0x001f01ff}
  if($r.AccessControlType-ne[Security.AccessControl.AccessControlType]::Allow -or [int]$r.FileSystemRights-ne$expectedRights -or $r.IsInherited -or [int]$r.InheritanceFlags-ne0 -or [int]$r.PropagationFlags-ne0){throw 'DOCUMENT_DACL_RIGHTS_DENY'}
 }
}
