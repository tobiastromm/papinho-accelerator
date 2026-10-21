param(
    [Parameter(Mandatory = $true)]
    [string]$OutputDirectory,
    [string]$ServerDnsName = 'papacc-test-server',
    [string]$OpenSsl = 'openssl'
)

$ErrorActionPreference = 'Stop'

function Invoke-OpenSsl {
    & $OpenSsl @args
    if ($LASTEXITCODE -ne 0) {
        throw "OpenSSL failed with exit code $LASTEXITCODE."
    }
}

$repositoryRoot = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$outputRoot = [System.IO.Path]::GetFullPath($OutputDirectory)
if ($outputRoot.StartsWith($repositoryRoot + [System.IO.Path]::DirectorySeparatorChar,
        [System.StringComparison]::OrdinalIgnoreCase) -or
    $outputRoot -eq $repositoryRoot) {
    throw 'Lab private material must be generated outside the repository.'
}

$caDirectory = Join-Path $outputRoot 'ca'
$serverDirectory = Join-Path $outputRoot 'accelerator'
$browserDirectory = Join-Path $outputRoot 'browser'
New-Item -ItemType Directory -Force $caDirectory, $serverDirectory,
    $browserDirectory | Out-Null

$serverExtensions = Join-Path $outputRoot 'server-ext.cnf'
$clientExtensions = Join-Path $outputRoot 'client-ext.cnf'
$opensslConfig = Join-Path $outputRoot 'openssl.cnf'
Set-Content -LiteralPath $opensslConfig -Encoding Ascii -Value @"
openssl_conf=openssl_init
[openssl_init]
providers=provider_sect
[provider_sect]
default=default_sect
[default_sect]
activate=1
[req]
distinguished_name=req_dn
[req_dn]
"@
$env:OPENSSL_CONF = $opensslConfig
Set-Content -LiteralPath $serverExtensions -Encoding Ascii -Value @"
basicConstraints=critical,CA:FALSE
keyUsage=critical,digitalSignature,keyEncipherment
extendedKeyUsage=serverAuth
subjectAltName=DNS:$ServerDnsName
"@
Set-Content -LiteralPath $clientExtensions -Encoding Ascii -Value @"
basicConstraints=critical,CA:FALSE
keyUsage=critical,digitalSignature
extendedKeyUsage=clientAuth
"@

Invoke-OpenSsl req -x509 -newkey rsa:2048 -sha256 -nodes -days 3650 `
    -subj '/CN=Papinho Lab CA' `
    -addext 'basicConstraints=critical,CA:TRUE' `
    -addext 'keyUsage=critical,keyCertSign,cRLSign' `
    -keyout (Join-Path $caDirectory 'ca.key.pem') `
    -out (Join-Path $caDirectory 'ca.crt.pem')
Invoke-OpenSsl req -newkey rsa:2048 -sha256 -nodes `
    -subj "/CN=$ServerDnsName" `
    -keyout (Join-Path $serverDirectory 'server.key.pem') `
    -out (Join-Path $serverDirectory 'server.csr.pem')
Invoke-OpenSsl x509 -req -sha256 -days 825 `
    -in (Join-Path $serverDirectory 'server.csr.pem') `
    -CA (Join-Path $caDirectory 'ca.crt.pem') `
    -CAkey (Join-Path $caDirectory 'ca.key.pem') -CAcreateserial `
    -extfile $serverExtensions `
    -out (Join-Path $serverDirectory 'server.crt.pem')
Invoke-OpenSsl req -newkey rsa:2048 -sha256 -nodes `
    -subj '/CN=PapinhoBrowser Lab Client' `
    -keyout (Join-Path $browserDirectory 'client.key.pem') `
    -out (Join-Path $browserDirectory 'client.csr.pem')
Invoke-OpenSsl x509 -req -sha256 -days 825 `
    -in (Join-Path $browserDirectory 'client.csr.pem') `
    -CA (Join-Path $caDirectory 'ca.crt.pem') `
    -CAkey (Join-Path $caDirectory 'ca.key.pem') -CAcreateserial `
    -extfile $clientExtensions `
    -out (Join-Path $browserDirectory 'client.crt.pem')

Invoke-OpenSsl x509 -in (Join-Path $serverDirectory 'server.crt.pem') `
    -outform DER -out (Join-Path $serverDirectory 'server.der')
Invoke-OpenSsl pkcs8 -topk8 -nocrypt `
    -in (Join-Path $serverDirectory 'server.key.pem') `
    -outform DER -out (Join-Path $serverDirectory 'server.pk8')
Invoke-OpenSsl x509 -in (Join-Path $browserDirectory 'client.crt.pem') `
    -outform DER -out (Join-Path $browserDirectory 'client.der')
Invoke-OpenSsl pkcs8 -topk8 -nocrypt `
    -in (Join-Path $browserDirectory 'client.key.pem') `
    -outform DER -out (Join-Path $browserDirectory 'client.pk8')
Invoke-OpenSsl x509 -in (Join-Path $caDirectory 'ca.crt.pem') `
    -outform DER -out (Join-Path $caDirectory 'ca.der')

foreach ($item in @(
        @{ In = Join-Path $serverDirectory 'server.der'; Out = Join-Path $serverDirectory 'server.der.b64' },
        @{ In = Join-Path $serverDirectory 'server.pk8'; Out = Join-Path $serverDirectory 'server.pk8.b64' },
        @{ In = Join-Path $browserDirectory 'client.der'; Out = Join-Path $browserDirectory 'client.der.b64' },
        @{ In = Join-Path $browserDirectory 'client.pk8'; Out = Join-Path $browserDirectory 'client.pk8.b64' },
        @{ In = Join-Path $caDirectory 'ca.der'; Out = Join-Path $caDirectory 'ca.der.b64' })) {
    [System.IO.File]::WriteAllText($item.Out,
        [Convert]::ToBase64String([System.IO.File]::ReadAllBytes($item.In)),
        [System.Text.Encoding]::ASCII)
}

$sha = [System.Security.Cryptography.SHA256]::Create()
try {
    $clientDerPath = Join-Path $browserDirectory 'client.der'
    $clientDerBytes = [System.IO.File]::ReadAllBytes($clientDerPath)
    $fingerprint = ([BitConverter]::ToString(
        $sha.ComputeHash($clientDerBytes))).Replace('-', '').ToLowerInvariant()
} finally {
    $sha.Dispose()
}
Remove-Item -LiteralPath $serverExtensions, $clientExtensions, $opensslConfig

Write-Output "PAPACC_SECURITY_CONFIGURATION_REF=nt4-lab-secure"
Write-Output "PAPACC_SERVER_CERTIFICATE_DER_B64=$(Join-Path $serverDirectory 'server.der.b64')"
Write-Output "PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64=$(Join-Path $serverDirectory 'server.pk8.b64')"
Write-Output "PAPACC_CLIENT_CA_DER_B64=$(Join-Path $caDirectory 'ca.der.b64')"
Write-Output "PAPACC_ALLOWED_CLIENT_CERT_SHA256=$fingerprint"
Write-Output "BROWSER_CERTIFICATE_DER_B64=$(Join-Path $browserDirectory 'client.der.b64')"
Write-Output "BROWSER_PRIVATE_KEY_PKCS8_DER_B64=$(Join-Path $browserDirectory 'client.pk8.b64')"
Write-Output "BROWSER_CA_DER_B64=$(Join-Path $caDirectory 'ca.der.b64')"
