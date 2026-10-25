#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('ModelTest','VerifyRoot','Place')][string]$Mode='ModelTest')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$source='C:\Projetos\PapinhoAccelerator\tools\qualification\n0-raw1-input-placement.ps1'
$expected='7b15a9d73420d9754596efc685501ea836bd98af346c3d3978a3f83356406f0c'
$held=[IO.File]::Open($source,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
try {
 $buffer=[IO.MemoryStream]::new();try{$held.CopyTo($buffer);$bytes=$buffer.ToArray()}finally{$buffer.Dispose()}
 $sha=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($actual-cne$expected){throw 'SCRIPT_PIN_DENY'}
 & ([scriptblock]::Create([Text.UTF8Encoding]::new($false,$true).GetString($bytes).TrimStart([char]0xFEFF))) -Mode $Mode -ExpectedScriptSha256 $expected
}finally{$held.Dispose()}
