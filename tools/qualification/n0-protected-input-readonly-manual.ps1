#requires -Version 5.1
[CmdletBinding()]
param([ValidateSet('CompileOnly','Verify')][string]$Mode='CompileOnly')
Set-StrictMode -Version Latest
$ErrorActionPreference='Stop'
$source='C:\Projetos\PapinhoAccelerator\tools\qualification\n0-protected-input-readonly.ps1'
$expected='6732b4166a46dc93dbfade6040b084f148e5460b441cd17d5132068ee4bfc0e0'
$held=[IO.File]::Open($source,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
try {
 $buffer=[IO.MemoryStream]::new();try{$held.CopyTo($buffer);$bytes=$buffer.ToArray()}finally{$buffer.Dispose()}
 $sha=[Security.Cryptography.SHA256]::Create();try{$actual=([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($actual-cne$expected){throw 'SCRIPT_PIN_DENY'}
 & ([scriptblock]::Create([Text.UTF8Encoding]::new($false,$true).GetString($bytes).TrimStart([char]0xFEFF))) -Mode $Mode -ExpectedScriptSha256 $expected
}finally{$held.Dispose()}
