$ErrorActionPreference='Stop'
. (Join-Path $PSScriptRoot 'ek-public-normalize.ps1')
$script:checks=0
function Check($ok){$script:checks++;if(-not $ok){throw "CHECK_$script:checks"}}
function Bad($v){$failed=$false;try{Convert-EkPublicEvidence $v|Out-Null}catch{$failed=$true};Check $failed}
Bad $null
Bad 'Administrator required'
Bad ([pscustomobject]@{IsPresent=$false;PublicKeyHash=('a'*64)})
Bad ([pscustomobject]@{IsPresent='True';PublicKeyHash=('a'*64)})
Bad ([pscustomobject]@{IsPresent=$true;PublicKeyHash='bad'})
Bad ([pscustomobject]@{IsPresent=$true;PublicKeyHash=[byte[]]@(1,2)})
Bad ([pscustomobject]@{IsPresent=$true;PublicKeyHash=('a'*64)})
$v=[pscustomobject]@{IsPresent=$true;PublicKeyHash=('A'*64);ManufacturerCertificates=$null;AdditionalCertificates=$null}
$x=Convert-EkPublicEvidence $v
Check ($x.public_key_sha256 -ceq ('a'*64))
Check ($x.certificate_count -eq 0)
Check ($x.authenticated_device -eq 'NOT_ESTABLISHED')
$v.PublicKeyHash=[byte[]](0..31)
$x=Convert-EkPublicEvidence $v
Check ($x.public_key_sha256.Length -eq 64)
$v.ManufacturerCertificates=@('not-a-certificate')
Bad $v
$tokens=$null;$errors=$null
[System.Management.Automation.Language.Parser]::ParseFile((Join-Path $PSScriptRoot 'collect-ek-public.MANUAL.ps1'),[ref]$tokens,[ref]$errors)|Out-Null
Check ($errors.Count -eq 0)
"PASS checks=$script:checks; SYNTHETIC_EK_ONLY; NO_NATIVE_COLLECTION"
