#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('Review','Create')][string]$Mode='Review')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$source='C:\Projetos\PapinhoAccelerator\tools\qualification\n0-protected-root-create-manual.ps1'
$expected='2805150203bfe851543209b7cb87cb6f8718b4926254d11875b372f27bc534a2'
$held=$null
try {
    $held=[IO.File]::Open($source,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
    $buffer=[IO.MemoryStream]::new()
    try {$held.CopyTo($buffer);$bytes=$buffer.ToArray()} finally {$buffer.Dispose()}
    $hash=[Security.Cryptography.SHA256]::Create()
    try {$actual=([BitConverter]::ToString($hash.ComputeHash($bytes))).Replace('-','').ToLowerInvariant()} finally {$hash.Dispose()}
    if($actual-cne$expected){throw 'SCRIPT_PIN_DENY'}
    & ([scriptblock]::Create([Text.UTF8Encoding]::new($false,$true).GetString($bytes))) -Mode $Mode -ExpectedScriptSha256 $expected
} finally {
    if($null-ne$held){$held.Dispose()}
}
