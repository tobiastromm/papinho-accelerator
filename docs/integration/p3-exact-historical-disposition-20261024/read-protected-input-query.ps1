# Read-only orchestration of existing PaccFileAcl APIs; no new native instrumentation.
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator'
$out=$root+'\docs\integration\p3-exact-historical-disposition-20261024\current-protected-input-observations.json'
$helper=$root+'\tools\qualification\scm-file-acl-native.ps1'
if((Get-FileHash -LiteralPath $helper -Algorithm SHA256).Hash.ToLowerInvariant()-cne'f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75'){throw 'HELPER_PIN_DENY'}
. $helper
$parent=$root+'\build\scm-protected-installation-binding-20261022\Selected\Release'
$spec=@(
 @{role='protected_root';path='C:\ProgramData\PapinhoAccelerator';hi=786432;lo=1087874;size=0;directory=$true;hash=''},
 @{role='protected_branch';path='C:\ProgramData\PapinhoAccelerator\Protected';hi=851968;lo=2672648;size=0;directory=$true;hash=''},
 @{role='protected_N0';path='C:\ProgramData\PapinhoAccelerator\Protected\N0';hi=720896;lo=2672656;size=0;directory=$true;hash=''},
 @{role='protected_evidence';path='C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.evidence';hi=196608;lo=2841429;size=5358;directory=$false;hash='babab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0'},
 @{role='protected_approval';path='C:\ProgramData\PapinhoAccelerator\Protected\N0\papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.approved';hi=131072;lo=2841430;size=326;directory=$false;hash='5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294'}
)
$handles=[Collections.Generic.List[IntPtr]]::new();$records=[Collections.Generic.List[object]]::new();$first=$null;$closeErrors=[Collections.Generic.List[string]]::new()
try {
 foreach($s in $spec){
  $m=Get-Item -LiteralPath $s.path -Force
  $ft=[uint64]$m.LastWriteTimeUtc.ToFileTimeUtc();$wh=[uint32]($ft-shr32);$wl=[uint32]($ft-band4294967295L)
  $h=[PaccFileAcl]::Open($s.path,$false,$s.directory);$handles.Add($h)
  [PaccFileAcl]::Identity($h,[uint32]$s.hi,[uint32]$s.lo,[uint64]$s.size,$wh,$wl,$s.directory)
  $sd=[PaccFileAcl]::Read($h);$sdHash=[PaccFileAcl]::Digest($sd)
  $contentHash=if($s.directory){$null}else{[PaccFileAcl]::FileDigest($h)}
  if(-not$s.directory-and$contentHash-cne$s.hash){throw ('CONTENT_HASH_DENY '+$s.role)}
  $sdAgain=[PaccFileAcl]::Read($h)
  if([PaccFileAcl]::Digest($sdAgain)-cne$sdHash){throw ('SECURITY_CHANGED_DURING_QUERY '+$s.role)}
  [PaccFileAcl]::Identity($h,[uint32]$s.hi,[uint32]$s.lo,[uint64]$s.size,$wh,$wl,$s.directory)
  $records.Add([ordered]@{role=$s.role;path=$s.path;native=('417614100:'+ $s.hi+':'+$s.lo);bytes=$s.size;directory=$s.directory;content_sha256=$contentHash;raw_owner_group_dacl_sha256=$sdHash;raw_base64=[Convert]::ToBase64String($sd);sddl=[PaccFileAcl]::Sddl($sd);identity_before_after='MATCH';same_original_handle_read=$true;last_write_filetime=$ft;protection='OBSERVED_NOT_CHANGED';sacl='UNQUERIED_UNKNOWN'})
 }
}catch{$first=$_.Exception.ToString()}
finally {
 for($j=$handles.Count-1;$j-ge0;$j--){if(-not[PaccFileAcl]::CloseHandle($handles[$j])){$closeErrors.Add('CLOSE_UNCERTAIN index='+$j)}else{$handles[$j]=[IntPtr]::Zero}}
 $result=[ordered]@{operation='P3_READONLY_REVALIDATION_ONLY';utc=[DateTime]::UtcNow.ToString('o');helper_sha256='f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';observations=$records.ToArray();first_error=$first;close_errors=$closeErrors.ToArray();descriptor_retained=[PaccFileAcl]::DescriptorRetained;owned_query_references_closed=($closeErrors.Count-eq0);administrative_transition_applied=$false;protected_writes='NONE';native_api_scope='Existing Open(false), Identity, Read/GetSecurityInfo mask7, FileDigest, CloseHandle; no Apply/SetSecurityInfo';not_atomic_admission=$true}
 $result|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $out -Encoding UTF8
}
if($first-or$closeErrors.Count-or[PaccFileAcl]::DescriptorRetained){throw 'P3_READONLY_STOP_SEE_EVIDENCE'}
'P3_READONLY_QUERY_COMPLETE; OWN_REFERENCES_CLOSED; TRANSITION_NOT_APPLIED'
