# Offline-only qualification; native candidate is compiled, NEVER invoked.
$ErrorActionPreference='Stop'
$folder='C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
$source=Get-Content -LiteralPath (Join-Path $folder 'read-postoffer-retained-participants-disposition.CANDIDATE.cs') -Raw -Encoding UTF8
if(([regex]::Matches($source,'\[DllImport\(')).Count-ne17){throw 'NATIVE_EDGE_CENSUS_DENY'}
if($source-match 'ControlService|StartService|ChangeServiceConfig|WriteProcessMemory|ReadProcessMemory|DuplicateToken|AdjustTokenPrivileges|FileMode.Create|FileAccess.Write'){throw 'UNAUTHORIZED_OPERATION_DENY'}
if(([regex]::Matches($source,'Native\(TerminateProcess\(')).Count-ne2){throw 'TERMINATION_CALL_CENSUS_DENY'}
Add-Type -TypeDefinition $source -Language CSharp -ErrorAction Stop
Write-Output 'REAL_CANDIDATE_COMPILED_ONLY; NO_DISPOSITION_METHOD_INVOKED'
Write-Output 'PREVIOUS_52_MODEL_CASES_REUSED; NEW_BINDINGS_ONLY'
$path=Join-Path $folder 'dispose-read-postoffer-retained-participants.MANUAL.CANDIDATE.ps1'
$tokens=$null;$errors=$null
$ast=[System.Management.Automation.Language.Parser]::ParseFile($path,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'WRAPPER_AST_DENY'}
$commands=@($ast.FindAll({param($n) $n -is [System.Management.Automation.Language.CommandAst]},$true) | ForEach-Object {$_.GetCommandName()} | Sort-Object -Unique)
foreach($name in $commands){if($name-notin@('Add-Type','Assert-Pin','Get-Content','Get-FileHash','Join-Path','Set-StrictMode','Write-Host')){throw ('WRAPPER_COMMAND_DENY '+$name)}}
$manual=Get-Content -LiteralPath $path -Raw
if(([regex]::Matches($manual,'\[READPOSTOFFERRetainedParticipantsDisposition\]::RunFixed\(\)')).Count-ne1){throw 'WRAPPER_INVOCATION_CENSUS_DENY'}
if($manual.Contains('DISPOSE_READPOSTOFFER_5364_4792_ONCE')){throw 'AUTOMATIC_CONFIRMATION_DENY'}
Write-Output 'WRAPPER_AST_PASS; ONE_FUTURE_FIXED_INVOCATION; NO_AUTOMATIC_REPLY; NO_SCM_PROCESS_TOKEN_QUERY_OR_TERMINATION'

$t=[READPOSTOFFERRetainedParticipantsDisposition];$stats=$t.GetNestedType('Stats',[Reflection.BindingFlags]::NonPublic)
$pinChecks=0
foreach($row in @(@('ControllerOriginalToken',17,48429409,620529,48428675),@('RuntimeObservedToken',10,48451502,48451224,48451271))){
 $method=$t.GetMethod($row[0],[Reflection.BindingFlags]'NonPublic,Static')
 for($fault=0;$fault-le7;$fault++){
  $v=[Activator]::CreateInstance($stats);$stats.GetField('groups').SetValue($v,[uint32]($row[1]+[int]($fault-eq1)))
  $i=2
  foreach($name in @('token','authentication','modified')){
   $field=$stats.GetField($name);$l=[Activator]::CreateInstance($field.FieldType)
   $lowFault=2+2*($i-2);$highFault=$lowFault+1
   $l.GetType().GetField('low').SetValue($l,[uint32]($row[$i]+[int]($fault-eq$lowFault)))
   $l.GetType().GetField('high').SetValue($l,[int]($fault-eq$highFault));$field.SetValue($v,$l);$i++
  }
  if([bool]$method.Invoke($null,@($v))-ne($fault-eq0)){throw ('ACTUAL_TOKEN_PREDICATE_DENY '+$row[0]+' '+$fault)};$pinChecks++
 }
}
'PASS ACTUAL_DISPOSITION_TOKEN_PINS cases='+$pinChecks+' NO_NATIVE_QUERY_NO_TERMINATE'
