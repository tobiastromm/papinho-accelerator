# Offline AST/native file-read pin fixture; NEVER invokes the launcher entry.
$ErrorActionPreference='Stop'
$subject='C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1'
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile($subject,[ref]$tokens,[ref]$errors)
if($errors.Count){throw ('POWERSHELL51_PARSE_ERROR '+$errors[0].Message)}
$pins=[Collections.Generic.List[IDisposable]]::new()
foreach($name in @('Hash-Stream','Pin')){
 $f=$ast.Find({param($x) $x -is [Management.Automation.Language.FunctionDefinitionAst] -and $x.Name -ceq $name},$true)
 if($null-eq$f){throw 'ORIGINAL_FUNCTION_MISSING'}
 . ([scriptblock]::Create($f.Extent.Text))
}
$cases=0
try {
 $hash=(Get-FileHash -LiteralPath $subject -Algorithm SHA256).Hash.ToLowerInvariant()
 Pin $subject $hash
 if($pins.Count-ne1){throw 'ORIGINAL_PIN_OPEN_NOT_HELD'};$cases++
 $denied=$false
 try{Pin $subject ('0'*64)}catch{if($_.Exception.Message-cne'HASH_DIVERGENCE'){throw};$denied=$true}
 if(-not$denied){throw 'HASH_MISMATCH_NOT_DENIED'};$cases++
 $before=$pins.Count;$denied=$false
 try{Pin $subject ''}catch{if($_.Exception.Message-cne'MISSING_QUALIFIED_PIN'){throw};$denied=$true}
 if(-not$denied -or $pins.Count-ne$before){throw 'EMPTY_PIN_NOT_DENIED_BEFORE_OPEN'};$cases++
 $denied=$false
 try{Pin 'C:\Projetos\PapinhoAccelerator\tools\qualification\..\qualification\scm-n0-protected-single-powershell.ps1' $hash}catch{if($_.Exception.Message-cne'NONCANONICAL_PIN_PATH'){throw};$denied=$true}
 if(-not$denied){throw 'NONCANONICAL_PATH_NOT_DENIED'};$cases++
} finally {foreach($pin in $pins){$pin.Dispose()}}
Write-Output ('LAUNCHER_OFFLINE=PASS cases='+$cases+'; PS='+$PSVersionTable.PSVersion+'; ORIGINAL_FUNCTIONS_ONLY; OWN_READ_REFERENCES_DISPOSED; NO_ENTRY_NO_PREFLIGHT_NO_SCM')
