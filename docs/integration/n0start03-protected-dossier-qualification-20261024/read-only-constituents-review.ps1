# Review-only orchestration of existing native readers; no admission or mutation.
$ErrorActionPreference='Stop'
$base='C:\Projetos\PapinhoAccelerator'
$pkg=$base+'\docs\integration\n0start03-protected-dossier-qualification-20261024'
$helper=$base+'\tools\qualification\scm-file-acl-native.ps1'
$tool=$base+'\build\successor-exact-binding-20261024\Selected\Release\papacc_service_n0_sd_test.exe'
if((Get-FileHash -LiteralPath $helper -Algorithm SHA256).Hash.ToLowerInvariant()-cne'f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75'){throw 'HELPER_PIN_DENY'}
if((Get-FileHash -LiteralPath $tool -Algorithm SHA256).Hash.ToLowerInvariant()-cne'836a52adc86ea41b818ec8bd0f53a72345c5e50baf8cb587cb5781f833876d7c'){throw 'NATIVE_TOOL_PIN_DENY'}
. $helper
$record=[ordered]@{status='REVIEW_ONLY';collected_utc=[DateTime]::UtcNow.ToString('o');root_observations=@();root_first_error='NONE';root_references_closed=$false;windows_file=$null;windows_first_error='NONE';windows_references_closed=$false;SCM='NOT_ACCESSED';ACL_MUTATIONS='NONE';admission='DENY'}
$id=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
 $e=0;$used=0;$type=0
 if(-not[PaccFileAcl]::GetTokenInformation($id.Token,20,[ref]$e,4,[ref]$used)){throw 'TOKEN_ELEVATION_QUERY_FAILED'}
 if(-not[PaccFileAcl]::GetTokenInformation($id.Token,18,[ref]$type,4,[ref]$used)){throw 'TOKEN_TYPE_QUERY_FAILED'}
 $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 try {$record.collector=[ordered]@{sid=$id.User.Value;token_elevated=$e;token_elevation_type=$type;administrators_enabled=([Security.Principal.WindowsPrincipal]::new($id)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator);impersonation=($null-ne$thread);approved_lab_operator='NOT_QUALIFIED_BY_THIS_COLLECTOR'}}finally{if($thread){$thread.Dispose()}}
}finally{$id.Dispose()}
$paths=@('C:\','C:\ProgramData','C:\ProgramData\PapinhoAccelerator','C:\ProgramData\PapinhoAccelerator\Protected','C:\ProgramData\PapinhoAccelerator\Protected\N0')
$hi=@(327680,131072,786432,851968,720896);$lo=@(5,227641,1087874,2672648,2672656)
$rawsha=@('9205e0643299861cdba58173a4e9e01db5a3bc63c4d8c5538bc2292dae214fc8','c0811643edc9703aa06642dfe053b7a0c3062cbaba03b6b169bde6962248f6a6','688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579','688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579','688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579')
$held=[Collections.Generic.List[IntPtr]]::new();$stage='NONE'
try {
 for($i=0;$i-lt$paths.Count;$i++){
  $stage=$paths[$i];$handle=[PaccFileAcl]::Open($stage,$false,$true);$held.Add($handle)
  [PaccFileAcl]::Identity($handle,$hi[$i],$lo[$i],0,0,0,$true)
  $raw=[PaccFileAcl]::Read($handle);$digest=[PaccFileAcl]::Digest($raw)
  if($digest-cne$rawsha[$i]){throw 'NATIVE_RAW_DESCRIPTOR_DIVERGENCE'}
  $view=[Security.AccessControl.RawSecurityDescriptor]::new($raw,0)
  $record.root_observations+=@{path=$stage;native=('417614100:'+$hi[$i]+':'+$lo[$i]);raw_sha256=$digest;raw_hex=([BitConverter]::ToString($raw)).Replace('-','').ToLowerInvariant();control=[int]$view.ControlFlags;owner=$view.Owner.Value;group=$view.Group.Value;sddl_for_display_only=$view.GetSddlForm([Security.AccessControl.AccessControlSections]::Owner-bor[Security.AccessControl.AccessControlSections]::Group-bor[Security.AccessControl.AccessControlSections]::Access);status='ORIGINAL_HANDLE_ID_AND_RAW_MATCH'}
 }
}catch{$record.root_first_error=$_.Exception.ToString();$record.root_failed_path=$stage}
finally {
 if([PaccFileAcl]::DescriptorRetained){throw 'DESCRIPTOR_RETAINED_NO_FURTHER_CLOSE'}
 for($i=$held.Count-1;$i-ge0;$i--){if(-not[PaccFileAcl]::CloseHandle($held[$i])){throw 'ROOT_CLOSE_UNKNOWN_NO_RETRY'}}
 $record.root_references_closed=$true
}
# This independent static constituent is not protected-root qualification or authority.
$h=[IntPtr]::Zero;$toolHandle=$null
try {
 $h=[PaccFileAcl]::Open('C:\Windows\System32\conhost.exe',$false,$false)
 $before=[PaccFileAcl]::FileDigest($h)
 $toolHandle=[IO.File]::Open($tool,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 $native=@(& $tool --file);$exit=$LASTEXITCODE
 $signature=Get-AuthenticodeSignature -LiteralPath 'C:\Windows\System32\conhost.exe'
 $after=[PaccFileAcl]::FileDigest($h)
 if($exit-ne0 -or $before-cne$after){throw 'NATIVE_FILE_REVALIDATION_FAILED'}
 $record.windows_file=[ordered]@{file_sha256_before=$before;file_sha256_after=$after;native_exit=$exit;native_output=($native-join"`n");signature_status=[string]$signature.Status;signature_type=[string]$signature.SignatureType;is_os_binary=$signature.IsOSBinary;signer=$signature.SignerCertificate.Subject;thumbprint=$signature.SignerCertificate.Thumbprint;opened_file_handle_held_during_signature=$true;native_tool_handle_held=$true;tool_sha256='836a52adc86ea41b818ec8bd0f53a72345c5e50baf8cb587cb5781f833876d7c';helper_sha256='f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75';mapped_image='UNPROVEN';console_association='UNPROVEN';signature_network_policy='CMDLET_POLICY_NOT_CLAIMED_NO_OPERATIONAL_NETWORK_OR_NETWORK_TEST'}
 if($signature.Status-ne'Valid' -or $signature.SignatureType-ne'Catalog' -or -not$signature.IsOSBinary){throw 'EXTERNAL_PROVENANCE_NOT_QUALIFIED'}
}catch{$record.windows_first_error=$_.Exception.ToString()}
finally{if($toolHandle){$toolHandle.Dispose()};if($h-ne[IntPtr]::Zero){if(-not[PaccFileAcl]::CloseHandle($h)){throw 'WINDOWS_FILE_CLOSE_UNKNOWN_NO_RETRY'}};$record.windows_references_closed=$true}
$record.status=if($record.root_first_error-eq'NONE'-and$record.windows_first_error-eq'NONE'){'READONLY_CONSTITUENTS_OBSERVED_NOT_ADMISSION'}else{'PARTIAL_STOP_NO_PLACEMENT'}
$json=$record|ConvertTo-Json -Depth 10
# Ordinary documentary evidence only; no protected input is written.
[IO.File]::WriteAllText(($pkg+'\readonly-constituents-result.json'),$json,[Text.UTF8Encoding]::new($false))
Write-Output ('READONLY_CONSTITUENTS='+$record.status+'; ROOT_FIRST_ERROR='+$record.root_first_error+'; WINDOWS_FIRST_ERROR='+$record.windows_first_error)
