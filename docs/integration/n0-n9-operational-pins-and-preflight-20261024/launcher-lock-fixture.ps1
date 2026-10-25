$ErrorActionPreference='Stop'
$path='C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1'
$tokens=$null;$errors=$null;$ast=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'AST_DENY'}
foreach($name in @('Hash-Stream','Lock-QualifierPath')){$fn=$ast.Find({param($n)$n-is[Management.Automation.Language.FunctionDefinitionAst]-and$n.Name-eq$name},$true);if($null-eq$fn){throw 'FUNCTION_MISSING'};. ([scriptblock]::Create($fn.Extent.Text))}
$Root='C:\Projetos\PapinhoAccelerator'
$Qualifier=$Root+'\build\n0-n9-operational-pins-and-preflight-20261024\Selected\Release\papacc_scm_instrumented_lab.exe'
$pins=[Collections.Generic.List[IDisposable]]::new();$locks=[Collections.Generic.List[IntPtr]]::new()
try{Lock-QualifierPath;if($locks.Count-ne7){throw 'LOCK_CHAIN_COUNT'};'LAUNCHER_NATIVE_PATH_LOCK_FIXTURE=PASS; LOCKS=7; NO_PROCESS_START_NO_PREFLIGHT'}finally{foreach($pin in $pins){$pin.Dispose()};for($i=$locks.Count-1;$i-ge0;$i--){if(-not[PaccFileAcl]::CloseHandle($locks[$i])){throw 'FIXTURE_CLOSE_UNKNOWN'};$locks[$i]=[IntPtr]::Zero}}
'OWNED_FIXTURE_REFERENCES_CLOSED'
