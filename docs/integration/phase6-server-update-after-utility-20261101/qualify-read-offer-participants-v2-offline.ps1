# Offline only: compile native declarations, invoke only the PURE original-token predicate by reflection.
$ErrorActionPreference='Stop'
$code=Get-Content -LiteralPath (Join-Path $PSScriptRoot 'read-offer-participants-v2-readonly.CANDIDATE.cs') -Raw -Encoding UTF8
$c=[CodeDom.Compiler.CompilerParameters]::new();$c.GenerateInMemory=$true;$c.WarningLevel=4;$c.TreatWarningsAsErrors=$true
[void]$c.ReferencedAssemblies.Add('System.dll');[void]$c.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition $code -CompilerParameters $c
$flags=[Reflection.BindingFlags]'NonPublic,Static'
$t=[READOFFERV2ParticipantsReadOnly];$stats=$t.GetNestedType('Stats',[Reflection.BindingFlags]::NonPublic);$method=$t.GetMethod('OriginalControllerToken',$flags)
if($null-eq$method){throw 'PURE_PREDICATE_MISSING'}
function New-Stats([int]$fault){
 $v=[Activator]::CreateInstance($stats)
 $stats.GetField('groups').SetValue($v,[uint32]$(if($fault-eq1){18}else{17}))
 foreach($entry in @(@('token',40848941,2,3),@('authentication',620529,4,5),@('modified',40848611,6,7))){
  $field=$stats.GetField($entry[0]);$l=[Activator]::CreateInstance($field.FieldType)
  $l.GetType().GetField('low').SetValue($l,[uint32]($entry[1]+[int]($fault-eq$entry[2])))
  $l.GetType().GetField('high').SetValue($l,[int]($fault-eq$entry[3]));$field.SetValue($v,$l)
 }
 return $v
}
for($i=0;$i-le7;$i++){$v=New-Stats $i;$pass=[bool]$method.Invoke($null,@($v));if($pass-ne($i-eq0)){throw ('TOKEN_PIN_CASE_'+$i)}}
# Old tuple must fail too.
$v=New-Stats 0
foreach($entry in @(@('token',235999083),@('authentication',570063),@('modified',235998740))){$field=$stats.GetField($entry[0]);$l=$field.GetValue($v);$l.GetType().GetField('low').SetValue($l,[uint32]$entry[1]);$field.SetValue($v,$l)}
if([bool]$method.Invoke($null,@($v))){throw 'OLD_SESSION_TOKEN_ACCEPTED'}
'PASS TOKEN_PINS cases=9 EXACT_CURRENT_ORIGINAL_AND_EACH_COMPONENT_NEGATIVE NO_NATIVE_RUN_NO_QUERY'
