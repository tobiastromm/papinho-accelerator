$ErrorActionPreference='Stop'
$f=$PSScriptRoot
$c=[CodeDom.Compiler.CompilerParameters]::new();$c.GenerateInMemory=$true;$c.WarningLevel=4;$c.TreatWarningsAsErrors=$true
[void]$c.ReferencedAssemblies.Add('System.dll');[void]$c.ReferencedAssemblies.Add('System.Core.dll')
$source=Get-Content -LiteralPath (Join-Path $f 'read-terminal-controller-disposition.CANDIDATE.cs') -Raw -Encoding UTF8
if(([regex]::Matches($source,'\[DllImport\(')).Count-ne15){throw 'NATIVE_CENSUS_DENY'}
if(([regex]::Matches($source,'Native\(TerminateProcess\(')).Count-ne1){throw 'TERMINATE_CENSUS_DENY'}
Add-Type -TypeDefinition $source -CompilerParameters $c
$t=[RetainedP1Disposition];$stats=$t.GetNestedType('Stats',[Reflection.BindingFlags]::NonPublic);$m=$t.GetMethod('OriginalToken',[Reflection.BindingFlags]'NonPublic,Static')
for($fault=0;$fault-le7;$fault++){
 $v=[Activator]::CreateInstance($stats);$stats.GetField('type').SetValue($v,[uint32]1);$stats.GetField('groups').SetValue($v,[uint32](17+[int]($fault-eq1)))
 $i=0;foreach($row in @(@('token',73773258),@('authentication',620529),@('modified',73772818))){$field=$stats.GetField($row[0]);$l=[Activator]::CreateInstance($field.FieldType);$l.GetType().GetField('low').SetValue($l,[uint32]($row[1]+[int]($fault-eq(2+2*$i))));$l.GetType().GetField('high').SetValue($l,[int]($fault-eq(3+2*$i)));$field.SetValue($v,$l);$i++}
 if([bool]$m.Invoke($null,@($v))-ne($fault-eq0)){throw 'PURE_TOKEN_DENY'}
}
$tokens=$null;$errors=$null;[void][Management.Automation.Language.Parser]::ParseFile((Join-Path $f 'dispose-read-terminal-controller.MANUAL.CANDIDATE.ps1'),[ref]$tokens,[ref]$errors);if($errors.Count){throw 'AST_DENY'}
'PASS COMPILE_WARNINGS_AS_ERRORS /8 PURE TOKEN CASES /AST /ONE TERMINATE_CENSUS; NO_NATIVE_EXECUTION'
