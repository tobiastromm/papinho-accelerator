# Registration validation only: no candidate script invocation or native/root call.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$pkg='C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025'
$ExpectedScriptSha256='79e3e4fac40b34b3e93021329821c16583dd635b64acbf6db98599d5338d934e'
$path=$pkg+'\q4-input-placement.CANDIDATE.ps1'
if((Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()-cne$ExpectedScriptSha256){throw 'SCRIPT_PIN_DENY'}
$t=$null;$e=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$t,[ref]$e);if($e.Count){throw 'AST_DENY'}
$f=$ast.FindAll({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-eq'Assert-Q4PlacementApproval'},$true)
if($f.Count-ne1){throw 'GUARD_BOUNDARY_DENY'}
. ([scriptblock]::Create($f[0].Extent.Text))
$a=[IO.File]::Open(($pkg+'\EXACT-PROTECTED-PLACEMENT-APPROVED.json'),[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
$n=$null
try {
 if($a.Length-le0-or$a.Length-gt16384){throw 'APPROVAL_SIZE_DENY'}
 $bytes=[byte[]]::new([int]$a.Length);if($a.Read($bytes,0,$bytes.Length)-ne$bytes.Length){throw 'APPROVAL_READ_DENY'}
 $sha=[Security.Cryptography.SHA256]::Create();try{$digest=([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($digest-cne'29ad02b6b9e33868d473cfd11256a02a65e636a327a02113dd8fb75daca704d9'){throw 'APPROVAL_SHA_DENY'}
 $record=[Text.UTF8Encoding]::new($false,$true).GetString($bytes)|ConvertFrom-Json
 Assert-Q4PlacementApproval $record
 $n=[IO.File]::Open($record.N7_decision_path,[IO.FileMode]::Open,[IO.FileAccess]::Read,[IO.FileShare]::Read)
 if($n.Length-le0-or$n.Length-gt65536){throw 'N7_SIZE_DENY'}
 $bytes=[byte[]]::new([int]$n.Length);if($n.Read($bytes,0,$bytes.Length)-ne$bytes.Length){throw 'N7_READ_DENY'}
 $sha=[Security.Cryptography.SHA256]::Create();try{$digest=([BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-','').ToLowerInvariant()}finally{$sha.Dispose()}
 if($digest-cne$record.N7_decision_sha256){throw 'N7_SHA_DENY'}
 $text=[Text.UTF8Encoding]::new($false,$true).GetString($bytes)
 if($text-cnotmatch'(?m)^status: accepted\r?$'-or-not$text.Contains($record.attempt)-or-not$text.Contains($record.N7_proposed_sha256)){throw 'N7_EXACT_SCOPE_DENY'}
 [Console]::Out.WriteLine('S1_S2_REGISTRATION_GUARD=PASS; ACTUAL_APPROVAL_AND_N7_DIGESTS_MATCH; NO_PLACE_NO_ROOT_NO_PREFLIGHT')
}finally{if($null-ne$n){$n.Dispose()};$a.Dispose()}
