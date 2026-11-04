# Offline only: compiles the fixed adapter and exercises injected IO in memory.
$ErrorActionPreference='Stop'
$sourceDirectory=Split-Path -Parent $MyInvocation.MyCommand.Path
$code=[IO.File]::ReadAllText((Join-Path $sourceDirectory 'pdp-config-original.cs'))
$model=[IO.File]::ReadAllText((Join-Path $sourceDirectory 'pdp-config-original.model.cs'))
$usings=@([regex]::Matches($code+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;') | ForEach-Object {$_.Value} | Sort-Object -Unique)
$body=[regex]::Replace($code+"`n"+$model,'(?m)^using [A-Za-z0-9_.]+;','')
$compiler=[CodeDom.Compiler.CompilerParameters]::new()
$compiler.GenerateInMemory=$true
$compiler.WarningLevel=4
$compiler.TreatWarningsAsErrors=$true
[void]$compiler.ReferencedAssemblies.Add('System.dll')
[void]$compiler.ReferencedAssemblies.Add('System.Core.dll')
Add-Type -TypeDefinition (($usings -join "`n")+"`n"+$body) -CompilerParameters $compiler
[PdpConfigurationOriginalModel]::Run()
