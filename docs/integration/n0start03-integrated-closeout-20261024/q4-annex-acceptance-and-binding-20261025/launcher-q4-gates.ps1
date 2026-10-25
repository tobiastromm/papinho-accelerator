# Extracted guard only; never invokes canonical launcher or native admission.
$ErrorActionPreference='Stop';Set-StrictMode -Version Latest
$path='C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1'
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'AST_PARSE_DENY'}
$items=$ast.FindAll({param($n)$n-is[Management.Automation.Language.IfStatementAst]-and$n.Extent.Text.StartsWith("if(`$Profile-ceq'Q4TokenDiagnostic')")},$true)
if($items.Count-ne1){throw 'Q4_GUARD_BOUNDARY'}
$code=[scriptblock]::Create($items[0].Extent.Text);$cases=0
foreach($Mode in @('Review','Prepare','Cycle')){
 $Profile='Q4TokenDiagnostic';$Attempt='old';$Operation='old';$denied=$false
 try{. $code}catch{if($_.Exception.Message-notlike'Q4_BINDING_INCOMPLETE_DENY*'){throw};$denied=$true}
 if(-not$denied-or$Attempt-cne'10584f02e11f92c92b43cddf0d9d316f'-or$Operation-cne'N0TOKEN-20261025-01'){throw 'Q4_GUARD_FAIL'};$cases++
}
foreach($Profile in @('EMPTY_DENY','N0START03ReadOnly','N0START03Positive')){
 $Attempt='unchanged';$Operation='unchanged';. $code
 if($Attempt-cne'unchanged'-or$Operation-cne'unchanged'){throw 'HISTORICAL_MUTATION'};$cases++
}
Write-Output ('Q4_LAUNCHER_EXTRACTED_GATES=PASS cases='+$cases+'; NO_LAUNCHER_NO_PROCESS_NO_FILE')
