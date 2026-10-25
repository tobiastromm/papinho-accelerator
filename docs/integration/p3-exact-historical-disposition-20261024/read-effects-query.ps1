# Read-only orchestration of existing PaccFileAcl APIs; no new native instrumentation.
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$root='C:\Projetos\PapinhoAccelerator'
$out=$root+'\docs\integration\p3-exact-historical-disposition-20261024\current-file-observations.json'
$helper=$root+'\tools\qualification\scm-file-acl-native.ps1'
if((Get-FileHash -LiteralPath $helper -Algorithm SHA256).Hash.ToLowerInvariant()-cne'f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75'){throw 'HELPER_PIN_DENY'}
. $helper
$parent=$root+'\build\scm-protected-installation-binding-20261022\Selected\Release'
$spec=@(
 @{role='parent';path=$parent;hi=1966080;lo=79426;size=0;directory=$true;hash=''},
 @{role='intent';path=$parent+'\papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.intent';hi=2097152;lo=2358097;size=103;directory=$false;hash='9e54d105d484c2201b5b280f9354038e9373f355f137c7e8a0debbca1ca45895'},
 @{role='image';path=$parent+'\papacc_server_lab_host_ntstatus.exe';hi=655360;lo=2416599;size=434688;directory=$false;hash='eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b'},
 @{role='sink';path=$parent+'\papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.diag';hi=1048576;lo=2416813;size=1024;directory=$false;hash='240d658c0cd8fdc2fdd6980f81c3a6419b1c8427876b766a664d686ebdd661e0'},
 @{role='configuration';path=$parent+'\papacc.conf';hi=393216;lo=1120240;size=116;directory=$false;hash='a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44'}
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
