$ErrorActionPreference='Stop'
$helper=Join-Path $PSScriptRoot '..\phase6-one-utility-preparation-20261101\prepare-utility.MANUAL.CANDIDATE.ps1'
if((Get-FileHash -LiteralPath $helper).Hash -ne '3c16e6b50a369b011ee9df9dee8b1276bb5bb781b86d98eaa11a427eebb63df5'){throw 'HELPER_PIN_DENY'}
$tokens=$null;$errors=$null
$ast=[Management.Automation.Language.Parser]::ParseFile($helper,[ref]$tokens,[ref]$errors)
if($errors.Count){throw 'HELPER_PARSE_DENY'}
$definitions=$ast.FindAll({param($node)$node -is [Management.Automation.Language.FunctionDefinitionAst]},$false)
. ([scriptblock]::Create(($definitions|ForEach-Object {$_.Extent.Text}) -join "`r`n"))
$script:prepHeld=New-Object 'System.Collections.Generic.List[object]'
$script:prepReadNative=New-PrepReadNative
$target='C:\Program Files\Papinho\Accelerator'
$source='C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
$records=@();$closeKnown=$true
try {
 Add-PrepDirectoryChain $target
 Add-PrepDirectoryChain $source
 foreach($path in @((Join-Path $target 'papacc_update.exe'),(Join-Path $target 'papacc_server.exe'),(Join-Path $target 'libssl-3-x64.dll'),(Join-Path $target 'libcrypto-3-x64.dll'),(Join-Path $source 'papacc_server.exe'))){Add-PrepOriginal $path $false}
 foreach($entry in $script:prepHeld){
  Assert-PrepOriginal $entry
  $acl=Get-Acl -LiteralPath $entry.Path
  $pin=if($entry.Directory){$null}else{Get-PrepBorrowedHash $entry.Handle}
  Assert-PrepOriginal $entry
  $records+=@{path=$entry.Path;directory=$entry.Directory;volume=$entry.Info[7];high=$entry.Info[11];low=$entry.Info[12];size_high=$entry.Info[8];size_low=$entry.Info[9];attributes=$entry.Info[0];owner=$acl.Owner;protected=$acl.AreAccessRulesProtected;sddl=$acl.Sddl;sha256=$pin;continuity='HELD_SNAPSHOT_ONLY'}
 }
 $sc=@{}
 foreach($verb in @('qc','queryex','qsidtype')){
  $lines=& 'C:\Windows\System32\sc.exe' $verb PapinhoAccelerator 2>&1
  $sc[$verb]=@{exit=$LASTEXITCODE;output=($lines -join "`n")}
 }
 foreach($entry in $script:prepHeld){Assert-PrepOriginal $entry}
} finally {
 for($i=$script:prepHeld.Count-1;$i -ge 0;$i--){if(-not $script:prepReadNative::CloseHandle($script:prepHeld[$i].Handle)){$closeKnown=$false;break}}
}
@{scope='CURRENT_READONLY_NOT_AUTHORITY_NO_PRODUCT_EXECUTION';utc=[DateTime]::UtcNow.ToString('o');records=$records;scm=$sc;close_known=$closeKnown;OS_QUIESCENCE='UNKNOWN'}|ConvertTo-Json -Depth 8
if(-not $closeKnown){exit 2}