#requires -Version 5.1
[CmdletBinding()]
param([Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-f]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
if($PSVersionTable.PSEdition-ne'Desktop' -or $PSVersionTable.PSVersion.Major-ne5 -or [IntPtr]::Size-ne8){throw 'WINDOWS_POWERSHELL_51_X64_REQUIRED'}
if((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash.ToLowerInvariant()-cne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
$operator=[Security.Principal.WindowsIdentity]::GetCurrent()
try {
 if(-not([Security.Principal.WindowsPrincipal]::new($operator)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){throw 'ALREADY_ELEVATED_OPERATOR_REQUIRED_NO_UAC_BYPASS'}
 $impersonation=[Security.Principal.WindowsIdentity]::GetCurrent($true)
 if($null-ne$impersonation){$impersonation.Dispose();throw 'IMPERSONATION_DENY'}
 $ek=Get-TpmEndorsementKeyInfo -HashAlgorithm Sha256 -ErrorAction Stop
 if($null-eq$ek -or $ek -is[string]){throw 'NO_TYPED_EK_EVIDENCE'}
 if($null-eq$ek.PSObject.Properties['PublicKeyHash'] -or $null-eq$ek.PublicKeyHash){throw 'EK_HASH_ABSENT'}
 $certs=@()
 foreach($entry in @(@{kind='manufacturer';values=$ek.ManufacturerCertificates},@{kind='additional';values=$ek.AdditionalCertificates})) {
  foreach($cert in $entry.values){
   if($null-eq$cert){continue}
   if(-not($cert-is[Security.Cryptography.X509Certificates.X509Certificate2])){throw 'CERT_TYPE_DENY'}
   $hash=[Security.Cryptography.SHA256]::Create()
   try{$digest=([BitConverter]::ToString($hash.ComputeHash($cert.RawData))).Replace('-','').ToLowerInvariant()}finally{$hash.Dispose()}
   $certs+=@{kind=$entry.kind;der_sha256=$digest;subject=$cert.Subject;issuer=$cert.Issuer;not_before_utc=$cert.NotBefore.ToUniversalTime().ToString('o');not_after_utc=$cert.NotAfter.ToUniversalTime().ToString('o')}
  }
 }
 [pscustomobject]@{scope='PUBLIC_METADATA_ONLY_NOT_AUTHENTICATED_DEVICE_ID';operator_sid=$operator.User.Value;public_key_hash=$ek.PublicKeyHash;is_present=$ek.IsPresent;certificates=$certs;certificate_chain_validation='NOT_EXECUTED_NO_NETWORK';proof_of_possession='NOT_EXECUTED';owner_auth='NOT_QUERIED';NV_mutations='NONE'}|ConvertTo-Json -Depth 5
}finally{$operator.Dispose()}
