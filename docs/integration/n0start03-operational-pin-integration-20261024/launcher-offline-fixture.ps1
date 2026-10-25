# Offline AST/native file-read pin fixture; NEVER invokes the launcher entry.
$ErrorActionPreference='Stop'
Import-Module ($PSHOME+'\Modules\Microsoft.PowerShell.Utility\Microsoft.PowerShell.Utility.psd1') -ErrorAction Stop
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

# Execute ONLY original guard statements obtained from AST, never the entry.
$guards=$ast.FindAll({param($x)$x -is [Management.Automation.Language.IfStatementAst]},$true)
foreach($case in @(
 @{mode='Prepare';profile='EMPTY_DENY';approval=('a'*64);needle='DEFAULT_PROFILE_EMPTY_DENY';error='DEFAULT_PROFILE_EMPTY_DENY'},
 @{mode='Cycle';profile='N0START03ReadOnly';approval=('a'*64);needle='POSITIVE_ROUTE_DENY_PIN_SLICE_READONLY_ONLY';error='POSITIVE_ROUTE_DENY_PIN_SLICE_READONLY_ONLY'},
 @{mode='Prepare';profile='N0START03ReadOnly';approval='';needle='SEPARATE_EXACT_READONLY_APPROVAL_REQUIRED';error='SEPARATE_EXACT_READONLY_APPROVAL_REQUIRED'})){
 $Mode=$case.mode;$Profile=$case.profile;$ExactApprovalSha256=$case.approval
 $guard=@($guards|Where-Object{$_.Extent.Text.Contains("throw '"+$case.needle+"'")})[0]
 if($null-eq$guard){throw 'ORIGINAL_GUARD_MISSING'}
 $denied=$false;try{& ([scriptblock]::Create($guard.Extent.Text))}catch{if($_.Exception.Message-cne$case.error){throw};$denied=$true}
 if(-not$denied){throw 'EXPECTED_DENIAL_MISSING'};$cases++
}
$Profile='N0START03ReadOnly';$Mode='Prepare';$ExactApprovalSha256=('a'*64)
foreach($needle in @('DEFAULT_PROFILE_EMPTY_DENY','SEPARATE_EXACT_READONLY_APPROVAL_REQUIRED')){
 $guard=@($guards|Where-Object{$_.Extent.Text.Contains("throw '"+$needle+"'")})[0]
 & ([scriptblock]::Create($guard.Extent.Text));$cases++
}
Write-Output ('LAUNCHER_PROFILE_GUARDS=PASS total_cases='+$cases+'; ORIGINAL_AST_STATEMENTS_ONLY; NO_ENTRY_NO_PREFLIGHT')

$pins=[Collections.Generic.List[IDisposable]]::new()
try{
 $denied=$false;try{Pin 'C:\Projetos\PapinhoAccelerator\docs\adr\DECISION-n0start03-n7-and-ed11-evaluation-plan-20261024.md' ('0'*64)}catch{if($_.Exception.Message-cne'HASH_DIVERGENCE'){throw};$denied=$true}
 if(-not$denied){throw 'N7_HASH_DIVERGENCE_NOT_DENIED'}
 $denied=$false;try{Pin 'C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-operational-pin-integration-20261024\EXACT-READONLY-APPROVED.json' ('a'*64)}catch{$e=$_.Exception;while($e.InnerException){$e=$e.InnerException};if($e -isnot [IO.FileNotFoundException]){throw};$denied=$true}
 if(-not$denied){throw 'MISSING_SPECIFIC_APPROVAL_NOT_DENIED'}
}finally{foreach($pin in $pins){$pin.Dispose()}}
Write-Output 'N7_HASH_AND_MISSING_EXACT_APPROVAL=DENY; ORIGINAL_PIN_FUNCTION; NO_ENTRY'
