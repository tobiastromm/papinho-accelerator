$ErrorActionPreference='Stop'
# Loads FUNCTION DEFINITIONS ONLY. Never executes the procedure top level,
# source controller, utility, request gates, preparation or native owner.
if($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or -not [Environment]::Is64BitProcess){throw 'REQUIRES_51_X64'}
$procedure=Join-Path $PSScriptRoot 'update-utility.MANUAL.DRAFT.ps1'
$parseTokens=$null;$parseErrors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile($procedure,[ref]$parseTokens,[ref]$parseErrors)
if($parseErrors.Count){throw ($parseErrors | Out-String)}
$definitions=$ast.FindAll({param($node) $node -is [Management.Automation.Language.FunctionDefinitionAst]},$false)
. ([scriptblock]::Create(($definitions | ForEach-Object {$_.Extent.Text}) -join "`r`n"))
$script:prepHeld=New-Object 'System.Collections.Generic.List[object]'
$script:prepReadNative=New-PrepReadNative
$checks=0
$workspace=(Resolve-Path -LiteralPath (Join-Path $PSScriptRoot '..\..\..')).Path
$source=Join-Path $workspace 'CMakeLists.txt'
try {
 Add-PrepOriginal $workspace $true
 Add-PrepOriginal $source $false
 $checks++;if($script:prepHeld.Count -ne 2){throw 'REFERENCE_COUNT'}
 Add-PrepOriginal $source $false
 $checks++;if($script:prepHeld.Count -ne 2){throw 'DUPLICATE_REFERENCE'}
 foreach($entry in $script:prepHeld){$checks++;Assert-PrepOriginal $entry}
 $file=$script:prepHeld[1]
 $pin=Get-PrepBorrowedHash $file.Handle
 $checks++;if($pin -ne (Get-FileHash -LiteralPath $source -Algorithm SHA256).Hash.ToLowerInvariant()){throw 'BORROW_HASH_MISMATCH'}
 $checks++;Assert-PrepOriginal $file
 $saved=$file.Info[11];$file.Info[11]=$saved -bxor 1
 $denied=$false;try {Assert-PrepOriginal $file}catch {$denied=$_.Exception.Message -eq 'ORIGINAL_CONTINUITY_DENY'}
 $file.Info[11]=$saved;$checks++;if(-not $denied){throw 'IDENTITY_REPLACEMENT_NOT_DENIED'}
 $saved=$file.Info[5];$file.Info[5]=$saved -bxor 1
 $denied=$false;try {Assert-PrepOriginal $file}catch {$denied=$_.Exception.Message -eq 'ORIGINAL_CONTINUITY_DENY'}
 $file.Info[5]=$saved;$checks++;if(-not $denied){throw 'FILE_WRITE_TIME_NOT_DENIED'}
 $directory=$script:prepHeld[0];$saved=$directory.Info[5];$directory.Info[5]=$saved -bxor 1
 $checks++;Assert-PrepOriginal $directory;$directory.Info[5]=$saved
 $checks++;if($script:prepReadNative.GetMethods().Name -contains 'WriteFile'){throw 'MUTATING_INTEROP'}
 # Real Windows loader with read locks, but ONLY the pure owner-stub entry
 # test. No product controller or utility is executed by this qualification.
 $modelDirectory=Join-Path $workspace 'build\windows-virtual-scm-integration-20261022\Selected\Release'
 $modelExecutable=Join-Path $modelDirectory 'papacc_update_entry_test.exe'
 Add-PrepOriginal $modelDirectory $true
 Add-PrepOriginal $modelExecutable $false
 $modelEntry=$script:prepHeld[$script:prepHeld.Count-1]
 $checks++;Assert-PrepOriginal $modelEntry
 & $modelExecutable
 $checks++;if($LASTEXITCODE -ne 0){throw 'MODELED_ENTRY_LOCKED_LOADER_FAILED'}
 $checks++;Assert-PrepOriginal $modelEntry
} finally {
 for($index=$script:prepHeld.Count-1;$index -ge 0;$index--){
  $entry=$script:prepHeld[$index];if($entry.CloseAttempted){throw 'DUPLICATE_CLOSE'};$entry.CloseAttempted=$true
  if(-not $script:prepReadNative::CloseHandle($entry.Handle)){throw 'CLOSE_UNKNOWN'}
  $checks++
 }
}
Write-Output "PASS READ_LOCK_HELPERS checks=$checks WINDOWS_POWERSHELL_51_X64_REAL_WORKSPACE_READONLY_INFO_HASH_CLOSE_AND_MODEL_ONLY_LOCKED_LOADER; NO_PROCEDURE_NO_CONTROLLER_NO_TARGET_NO_SCM_NO_PREPARATION; AUTHORITY_DENY"
