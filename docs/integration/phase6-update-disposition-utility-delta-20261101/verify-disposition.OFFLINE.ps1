$ErrorActionPreference='Stop'
if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or -not [Environment]::Is64BitProcess){throw 'REQUIRES_51_X64'}
$procedure=Join-Path $PSScriptRoot 'observe-disposition.MANUAL.CANDIDATE.ps1'
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile($procedure,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'SCRIPT_PARSE_DENY'}
$definitions=$ast.FindAll({param($n)$n -is [Management.Automation.Language.FunctionDefinitionAst]},$false)
. ([scriptblock]::Create(($definitions|ForEach-Object {$_.Extent.Text}) -join "`r`n"))
$checks=0
function Assert-Check([bool]$Condition,[string]$Name){if(-not $Condition){throw $Name};$script:checks++}
$script:prepHeld=New-Object 'System.Collections.Generic.List[object]'
$script:prepReadNative=New-PrepReadNative
try {
 $import=$script:prepReadNative.GetMethod('CreateFileW').GetCustomAttributes([Runtime.InteropServices.DllImportAttribute],$false)[0]
 Assert-Check $import.SetLastError 'LAST_ERROR_MUST_BE_CAPTURED'
 Assert-Check ((Get-DispOpenStatus ([IntPtr](-1)) 2) -ceq 'ABSENT_SNAPSHOT') 'ABSENT2'
 foreach($code in @(3,5,32,87)){Assert-Check ((Get-DispOpenStatus ([IntPtr](-1)) $code) -ceq 'UNKNOWN') ('UNKNOWN'+$code)}
 Assert-Check ((Get-DispOpenStatus ([IntPtr]2) 2) -ceq 'PRESENT') 'STALE_ERROR_ON_SUCCESS_IGNORED'
 $workspace=(Resolve-Path (Join-Path $PSScriptRoot '..\..\..')).Path
 Add-PrepOriginal $workspace $true
 $record=Get-DispRecord $script:prepHeld[0]
 Assert-Check ($record.status -ceq 'PRESENT' -and $record.owner -and $record.sd_bytes -gt 0 -and $record.sd_sha256.Length -eq 64) 'DIRECTORY_SD_RECORD'
 $source=Join-Path $workspace 'CMakeLists.txt'
 $record=Get-DispOriginal $source
 Assert-Check ($record.status -ceq 'PRESENT' -and $record.sha256 -ceq (Get-FileHash -LiteralPath $source).Hash.ToLowerInvariant()) 'HELD_HASH'
 $file=$script:prepHeld[1]
 $saved=$file.Info[11];$file.Info[11]=$saved -bxor 1
 $denied=$false;try{Assert-PrepOriginal $file}catch{$denied=$_.Exception.Message -ceq 'ORIGINAL_CONTINUITY_DENY'}
 $file.Info[11]=$saved;Assert-Check $denied 'SUBSTITUTION_DENIED'
 $absent=Get-DispOriginal (Join-Path $workspace '__papacc_disposition_fixture_absent_never_create__')
 Assert-Check ($absent.status -ceq 'ABSENT_SNAPSHOT' -and $absent.win32 -eq 2) 'REAL_NATIVE_ERROR2'
 Assert-Check ($script:prepHeld.Count -eq 2) 'NO_REFERENCE_FOR_ABSENT'
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry;Assert-Check $true 'CONTINUITY'}
 $methods=$script:prepReadNative.GetMethods().Name
 Assert-Check (-not ($methods -contains 'WriteFile') -and -not ($methods -contains 'SetFileInformationByHandle')) 'NO_MUTATING_INTEROP'
 $raw=[IO.File]::ReadAllText($procedure)
 Assert-Check (-not ($raw -match 'Start-Process|Copy-Item|Move-Item|Remove-Item|Set-Acl|& \$executor|& \$data')) 'NO_PRODUCT_OR_MUTATOR'
 Assert-Check ($raw.Contains("@('qc','queryex','qsidtype')")) 'SCM_QUERY_VERBS_ONLY'
} finally {
 for($i=$script:prepHeld.Count-1;$i -ge 0;$i--){
  $entry=$script:prepHeld[$i];Assert-Check (-not $entry.CloseAttempted) 'CLOSE_ONCE';$entry.CloseAttempted=$true
  Assert-Check ($script:prepReadNative::CloseHandle($entry.Handle)) 'REAL_READONLY_CLOSE'
 }
}
Write-Output "PASS DISPOSITION_HELPERS checks=$checks WINDOWS_POWERSHELL_51_X64 WORKSPACE_READONLY_NATIVE_ERROR_SD_HASH_CONTINUITY_CLOSE; NO_TARGET_NO_SCM_NO_PRODUCT_NO_FULL_PROCEDURE; ABSENT_NOT_ACCESS_DENIED; AUTHORITY_DENY"