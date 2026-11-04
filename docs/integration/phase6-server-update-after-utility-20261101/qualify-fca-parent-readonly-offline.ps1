# Windows PowerShell CodeDom, same existing document/observer qualification mechanism.
$ErrorActionPreference='Stop'
$code=[IO.File]::ReadAllText((Join-Path $PSScriptRoot 'fca-parent-readonly.cs'))
$model=[IO.File]::ReadAllText((Join-Path $PSScriptRoot 'fca-parent-readonly.model.cs'))
$existing=[IO.File]::ReadAllText((Join-Path $PSScriptRoot 'machine-fixture-originals.cs'))
$all=$code+"`n"+$model+"`n"+$existing
$usings=@([regex]::Matches($all,'(?m)^using [A-Za-z0-9_.]+;')|ForEach-Object {$_.Value}|Sort-Object -Unique)
$body=[regex]::Replace($all,'(?m)^using [A-Za-z0-9_.]+;','')
$compiler=[CodeDom.Compiler.CompilerParameters]::new();$compiler.GenerateInMemory=$true;$compiler.WarningLevel=4;$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll');[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings -join "`n")+"`n"+$body) -CompilerParameters $compiler
$history=Join-Path $PSScriptRoot '../n0start03-protected-dossier-qualification-20261024/readonly-constituents-result.json'
$old=Get-Content -LiteralPath $history -Raw|ConvertFrom-Json
$hex=($old.root_observations|Where-Object {$_.path -ceq 'C:\ProgramData'}).raw_hex
$raw=[byte[]]::new($hex.Length/2);for($i=0;$i-lt$raw.Length;$i++){$raw[$i]=[Convert]::ToByte($hex.Substring(2*$i,2),16)}
[FcaParentReadModel]::Run($raw)
