Set-StrictMode -Version Latest
function Convert-EkPublicEvidence {
 param($Value)
 if ($null -eq $Value -or $Value -is [string]) { throw 'NO_TYPED_EK_EVIDENCE' }
 if ($null -eq $Value.PSObject.Properties['IsPresent'] -or $Value.IsPresent -isnot [bool] -or -not $Value.IsPresent) { throw 'EK_NOT_PRESENT' }
 if ($null -eq $Value.PSObject.Properties['PublicKeyHash']) { throw 'EK_HASH_ABSENT' }
 $raw=$Value.PublicKeyHash
 if ($raw -is [byte[]]) { if($raw.Length -ne 32){throw 'EK_HASH_SIZE_DENY'}; $digest=([BitConverter]::ToString($raw)).Replace('-','').ToLowerInvariant() }
 elseif($raw -is [string] -and $raw -cmatch '^[0-9a-fA-F]{64}$') { $digest=$raw.ToLowerInvariant() }
 else { throw 'EK_HASH_FORMAT_DENY' }
 $certs=@()
 foreach($kind in @('ManufacturerCertificates','AdditionalCertificates')) {
  if($null -eq $Value.PSObject.Properties[$kind]){throw 'CERT_COLLECTION_ABSENT'}
  foreach($cert in $Value.$kind) {
   if($null -eq $cert){continue}
   if($cert -isnot [Security.Cryptography.X509Certificates.X509Certificate2]){throw 'CERT_TYPE_DENY'}
   $sha=[Security.Cryptography.SHA256]::Create()
   try{$pin=([BitConverter]::ToString($sha.ComputeHash($cert.RawData))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
   $certs+=@{origin=$kind;der_sha256=$pin;format='X509_DER';trust='NOT_VALIDATED_OFFLINE_NO_NETWORK'}
  }
 }
 [pscustomobject]@{public_key_sha256=$digest;certificates=@($certs);certificate_count=$certs.Count;origin='Get-TpmEndorsementKeyInfo -HashAlgorithm Sha256';format='SHA256_HEX_OF_CMDLET_PUBLIC_KEY';authenticated_device='NOT_ESTABLISHED';proof_of_possession='NOT_EXECUTED'}
}
