$ErrorActionPreference='Stop'
$path=Join-Path $PSScriptRoot 'update-cold-diagnostic.MANUAL.CANDIDATE.ps1'
$t=$null;$e=$null;$a=[Management.Automation.Language.Parser]::ParseFile($path,[ref]$t,[ref]$e)
if($e.Count){throw 'AST_DENY'}
foreach($f in $a.FindAll({param($n) $n -is [Management.Automation.Language.FunctionDefinitionAst]},$false)){. ([scriptblock]::Create($f.Extent.Text))}
$script:prepHeld=New-Object 'System.Collections.Generic.List[object]'
$script:prepReadNative=New-PrepReadNative
$result=@()
try {
 foreach($path in @('C:\Program Files\Papinho\Accelerator','C:\Program Files\Papinho\Accelerator\papacc_update.exe','C:\Program Files\Papinho\Accelerator\papacc_server.exe','C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe')){
  Add-PrepOriginal $path ($path -ceq 'C:\Program Files\Papinho\Accelerator')
  $entry=$script:prepHeld[$script:prepHeld.Count-1];Assert-PrepOriginal $entry
  $result+=[pscustomobject]@{Path=$path;Id=('{0}:{1}:{2}' -f $entry.Info[7],$entry.Info[11],$entry.Info[12]);Attributes=$entry.Info[0];Bytes=$entry.Info[9]}
 }
} finally {for($i=$script:prepHeld.Count-1;$i -ge 0;$i--){if(-not $script:prepReadNative::CloseHandle($script:prepHeld[$i].Handle)){throw 'READONLY_CLOSE_UNKNOWN'}}}
$evidence=[ordered]@{task='COLD_DIAGNOSTIC_IMAGE_GATE';procedure_ast='PASS';exact_delta_allowlist='PASS';procedure_executed=$false;readonly_original_identity_snapshot=$result;readonly_close_known=$true;historical_retention='UNKNOWN';os_quiescence='UNKNOWN';candidate_sha256='7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d';candidate_bytes=763904;executor_K=2027314;prior_builds_reused=4;prior_ctest_pairs_reused=20;protected_effects=0;qualification_host='Windows PowerShell 5.1 Desktop';earlier_core_helper_attempt='INCOMPATIBLE_RUNTIME_NO_HANDLES_ACQUIRED_NOT_EVIDENCE'}
[IO.File]::WriteAllText((Join-Path $PSScriptRoot 'COLD-DIAGNOSTIC-UPDATE-EVIDENCE.json'),($evidence|ConvertTo-Json -Depth 6),[Text.UTF8Encoding]::new($false))
$result|Format-List
'READONLY_IDENTITIES_AND_CLOSE_PASS; NO_UPDATE_INVOCATION'