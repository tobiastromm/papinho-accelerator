# CANDIDATE ONLY: separate owner approval for new public-byte observation required; never auto-executed.
#requires -Version 5.1
[CmdletBinding()]
param([Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or [IntPtr]::Size -ne 8){throw 'WINDOWS_POWERSHELL_51_X64_REQUIRED'}
if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash.ToLowerInvariant() -cne $ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
$helper=Join-Path $PSScriptRoot 'ek-public-normalize.ps1'
if((Get-FileHash -LiteralPath $helper -Algorithm SHA256).Hash.ToLowerInvariant() -cne '5623d7fbd248772c4b2d8c9fbb6bd492dbd78c96deafb45299421e41834b7ef4'){throw 'HELPER_PIN_DENY'}
. $helper
$publicBundle=$null;$first='NONE';$operator=$null;$controlledClose='NOT_OPENED';$evidence=$null;$context=$null;$properties=$null;$provenance=$null;$queryStart=$null;$queryEnd=$null;$process=$null;$processClose='NOT_OPENED'
try {
 $operator=[Security.Principal.WindowsIdentity]::GetCurrent()
 $controlledClose='OPEN'
 if(-not ([Security.Principal.WindowsPrincipal]::new($operator)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ALREADY_ELEVATED_OPERATOR_REQUIRED_NO_UAC_BYPASS'}
 $thread=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 if($null -ne $thread){$thread.Dispose();throw 'IMPERSONATION_DENY'}
 $expectedModuleBase=Join-Path $PSHOME 'Modules\TrustedPlatformModule'
 Import-Module (Join-Path $expectedModuleBase 'TrustedPlatformModule.psd1') -ErrorAction Stop
 $cmd=Get-Command 'TrustedPlatformModule\Get-TpmEndorsementKeyInfo' -CommandType Cmdlet -ErrorAction Stop
 if(-not [string]::Equals($cmd.Module.ModuleBase,$expectedModuleBase,[StringComparison]::OrdinalIgnoreCase)){throw 'TPM_MODULE_ORIGIN_DENY'}
 $assemblyPath=$cmd.ImplementingType.Assembly.Location
 $provenance=@{cmdlet=$cmd.Name;assembly_path=$assemblyPath;assembly_file_sha256=(Get-FileHash -LiteralPath $assemblyPath -Algorithm SHA256).Hash.ToLowerInvariant();scope='ON_DISK_BYTES_NOT_MAPPED_IMAGE_ATTESTATION'}
 $boot=Get-WinEvent -FilterHashtable @{LogName='System';ProviderName='Microsoft-Windows-Kernel-General';Id=12} -MaxEvents 1
 $xml=[xml]$boot.ToXml();$computer=$xml.SelectSingleNode('/*[local-name()="Event"]/*[local-name()="System"]/*[local-name()="Computer"]')
 if($null -eq $computer){throw 'BOOT_HOST_UNKNOWN'}
 $start=@($xml.Event.EventData.Data|Where-Object{$_.Name -eq 'StartTime'})
 if($start.Count -ne 1){throw 'BOOT_START_UNKNOWN'}
 $sha=[Security.Cryptography.SHA256]::Create()
 try{$hostPin=([BitConverter]::ToString($sha.ComputeHash([Text.Encoding]::UTF8.GetBytes($computer.InnerText)))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 $sha=[Security.Cryptography.SHA256]::Create()
 try{$currentHostPin=([BitConverter]::ToString($sha.ComputeHash([Text.Encoding]::UTF8.GetBytes([Environment]::MachineName)))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 $process=[Diagnostics.Process]::GetCurrentProcess();$processClose='OPEN'
 $context=@{operator_sid=$operator.User.Value;pid=$PID;process_creation_filetime=$process.StartTime.ToUniversalTime().ToFileTimeUtc();boot_event_computer_sha256=$hostPin;current_computer_name_sha256=$currentHostPin;name_sources='Kernel-General12/Computer and Environment.MachineName; metadata not physical device identity';boot_record_id=$boot.RecordId;boot_start_utc=[DateTimeOffset]::Parse($start[0].'#text').UtcDateTime.ToString('o');binding='CURRENT_OBSERVATION_NOT_APPROVED_HARDWARE_GENERATION'}
 $queryStart=[Diagnostics.Stopwatch]::GetTimestamp()
 $tpm=TrustedPlatformModule\Get-Tpm -ErrorAction Stop
 if($null -eq $tpm -or $tpm -is [string] -or $null -eq $tpm.PSObject.Properties['TpmPresent']){throw 'NO_TYPED_TPM_PROPERTIES'}
 $properties=@{present=$tpm.TpmPresent;ready=$tpm.TpmReady;enabled=$tpm.TpmEnabled;manufacturer=$tpm.ManufacturerIdTxt;firmware=$tpm.ManufacturerVersion;device_identity='METADATA_ONLY'}
 if($tpm.TpmPresent -ne $true){throw 'TPM_NOT_PRESENT'}
 $value=& $cmd -HashAlgorithm Sha256 -ErrorAction Stop
 $evidence=Convert-EkPublicEvidence $value
 if($evidence.public_key_sha256 -cne '406874753bd60db238226c888765b49940cc6f0229b8a0296f4ad1547e92aba2'){throw 'OBSERVED_EK_CONTINUITY_DENY'}
 if($value.PublicKey -isnot [Security.Cryptography.AsnEncodedData]){throw 'PUBLIC_KEY_ASN_TYPE_DENY'}
 [byte[]]$publicBytes=$value.PublicKey.RawData
 if($publicBytes.Length -eq 0 -or $publicBytes.Length -gt 16384){throw 'PUBLIC_KEY_SIZE_DENY'}
 $sha=[Security.Cryptography.SHA256]::Create()
 try{$rawPin=([BitConverter]::ToString($sha.ComputeHash($publicBytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($rawPin -cne $evidence.public_key_sha256){throw 'RAW_KEY_HASH_ENCODING_DENY'}
 $matched=@()
 foreach($kind in @('ManufacturerCertificates','AdditionalCertificates')){
  foreach($cert in $value.$kind){
   if($null -eq $cert){continue}
   if($cert -isnot [Security.Cryptography.X509Certificates.X509Certificate2]){throw 'CERT_TYPE_DENY'}
   $sha=[Security.Cryptography.SHA256]::Create()
   try{$certPin=([BitConverter]::ToString($sha.ComputeHash($cert.RawData))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
   if($certPin -ceq '965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761'){
    if($cert.RawData.Length -gt 65536){throw 'CERT_SIZE_DENY'}
    $matched+=@{origin=$kind;der_base64=[Convert]::ToBase64String($cert.RawData);sha256=$certPin}
   }
  }
 }
 if($matched.Count -ne 1){throw 'EXACT_CERTIFICATE_NOT_UNIQUE'}
 $publicBundle=@{raw_asn_key_base64=[Convert]::ToBase64String($publicBytes);format='CMDLET_ASN_ENCODED_DATA_NOT_ASSUMED_SPKI';certificates=$matched;scope='PUBLIC_BYTES_ONLY_FOR_OFFLINE_PARSE_NOT_TRUST_NOT_POSSESSION'}
 $queryEnd=[Diagnostics.Stopwatch]::GetTimestamp()
 if($queryEnd -lt $queryStart){throw 'LOCAL_CLOCK_REGRESSION'}
}catch{if($first -eq 'NONE'){$first=$_.Exception.Message}}
finally{
 if($null -ne $process){try{$process.Dispose();$processClose='PROCESS_DISPOSE_RETURNED'}catch{if($first -eq 'NONE'){$first='PROCESS_CLOSE_UNKNOWN'};$processClose='UNKNOWN_NO_RETRY'}}
 if($null -ne $operator){try{$operator.Dispose();$controlledClose='WINDOWS_IDENTITY_DISPOSE_RETURNED'}catch{if($first -eq 'NONE'){$first='OPERATOR_CLOSE_UNKNOWN'};$controlledClose='UNKNOWN_NO_RETRY'}}
}
$global:LASTEXITCODE=2;if($first -eq 'NONE'){$global:LASTEXITCODE=0}
[pscustomobject]@{first=$first;exit=$global:LASTEXITCODE;context=$context;collection_clock=@{source='Stopwatch.GetTimestamp';frequency=[Diagnostics.Stopwatch]::Frequency;start=$queryStart;end=$queryEnd;policy='NO_TARGET_FRESHNESS_GRANTED'};tpm=$properties;provenance=$provenance;ek=$evidence;public_bundle=$(if($first -eq 'NONE'){$publicBundle}else{$null});controlled_reference_close=$controlledClose;process_reference_close=$processClose;cmdlet_internal_context_close='UNOBSERVED_NOT_ASSERTED';private_material='NOT_REQUESTED';ownerAuth='NOT_QUERIED';NV_mutations='NONE';Q4='NOT_EXECUTED';authority='DENY';no_retry=$true}|ConvertTo-Json -Depth 7
