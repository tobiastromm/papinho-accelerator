$ErrorActionPreference='Stop'
$source=Get-Content -LiteralPath (Join-Path $PSScriptRoot 'read-offer-participants-readonly.CANDIDATE.cs') -Raw -Encoding UTF8
if($source -match 'TerminateProcess|ControlService|StartService|WriteProcessMemory|ReadProcessMemory|DuplicateToken|AdjustTokenPrivileges'){throw 'MUTATOR_DENY'}
$c=[CodeDom.Compiler.CompilerParameters]::new();$c.GenerateInMemory=$true;$c.WarningLevel=4;$c.TreatWarningsAsErrors=$true
[void]$c.ReferencedAssemblies.Add('System.dll');[void]$c.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition $source -CompilerParameters $c
'PASS READONLY_SOURCE_COMPILED_ONLY_RUNFIXED_NOT_CALLED WARNINGS_AS_ERRORS'
