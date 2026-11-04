# CANDIDATE. Run once ONLY after approval of ONE-STOPPED-SCM-REQUEST.CANDIDATE.md.
# No retry, elevation, Start/Stop, copying, ACL changes or cleanup.
[CmdletBinding()]
param([Parameter(Mandatory=$true)][ValidatePattern('^[0-9a-fA-F]{64}$')][string]$ExpectedScriptSha256)
Set-StrictMode -Version 2.0
$ErrorActionPreference = 'Stop'
$scmInvoked = $false
$scmExit = 2
try {
    if ((Get-FileHash -LiteralPath $PSCommandPath -Algorithm SHA256).Hash -ne $ExpectedScriptSha256) { throw 'SCRIPT_HASH_DENY' }
    if ($PSVersionTable.PSEdition -ne 'Desktop' -or $PSVersionTable.PSVersion.Major -ne 5 -or [IntPtr]::Size -ne 8) { throw 'POWERSHELL_51_X64_DENY' }
    if ([Environment]::MachineName -ne 'TCORP-TOBIAS') { throw 'HOST_METADATA_DENY' }
    $scmIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
    try {
        if ($scmIdentity.User.Value -ne 'S-1-5-21-785699550-1440458426-1682014700-1108') { throw 'OPERATOR_SID_DENY' }
        $scmPrincipal = New-Object Security.Principal.WindowsPrincipal($scmIdentity)
        if (-not $scmPrincipal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'ADMIN_CONTEXT_DENY' }
    } finally { $scmIdentity.Dispose() }
    if ([Environment]::GetFolderPath([Environment+SpecialFolder]::ProgramFiles) -ne 'C:\Program Files') { throw 'KNOWN_FOLDER_DENY' }
    $scmControllerDirectory = 'C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release'
    $scmTargetDirectory = 'C:\Program Files\Papinho\Accelerator'
    $scmFiles = @(
        @{ Path = ($scmControllerDirectory + '\papacc_server.exe'); Bytes = 582144; Hash = '63291745746f9f7441bec0894ef2bf1fc2b57485cca38ece4e70a3536d511c83' },
        @{ Path = ($scmControllerDirectory + '\libssl-3-x64.dll'); Bytes = 1321472; Hash = '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed' },
        @{ Path = ($scmControllerDirectory + '\libcrypto-3-x64.dll'); Bytes = 7217664; Hash = '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044' },
        @{ Path = ($scmTargetDirectory + '\papacc_server.exe'); Bytes = 571392; Hash = 'c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf' },
        @{ Path = ($scmTargetDirectory + '\libssl-3-x64.dll'); Bytes = 1321472; Hash = '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed' },
        @{ Path = ($scmTargetDirectory + '\libcrypto-3-x64.dll'); Bytes = 7217664; Hash = '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044' }
    )
    foreach ($scmFile in $scmFiles) {
        $scmItem = Get-Item -LiteralPath $scmFile.Path -Force -ErrorAction Stop
        if ($scmItem.PSIsContainer -or (($scmItem.Attributes -band [IO.FileAttributes]::ReparsePoint) -ne 0) -or $scmItem.Length -ne $scmFile.Bytes) { throw 'FILE_METADATA_DENY' }
        if ((Get-FileHash -LiteralPath $scmFile.Path -Algorithm SHA256).Hash -ne $scmFile.Hash) { throw 'FILE_HASH_DENY' }
    }
    Write-Output 'ONE_MANUAL_STOPPED_SCM_REGISTRATION; R0_R9_OPAQUE_RISK; ON_DISK_PINS_NOT_MAPPED_IMAGE_ATTESTATION; ORIGINAL_NATIVE_GATES_AND_CONFIRMATION_REQUIRED; NO_START_NO_ENDPOINT_NO_KSP_NO_CA_NO_CURRENT; NO_RETRY'
    Write-Output 'Confirm ONLY if native target, three staged pins and exact command match the approved request. Cancel on any difference. No response is supplied automatically.'
    $scmController = $scmControllerDirectory + '\papacc_server.exe'
    $scmInvoked = $true
    & $scmController service install --scq-closed
    $scmExit = $LASTEXITCODE
} catch {
    Write-Output ('MANUAL_STOPPED_SCM_DENY=' + $_.Exception.Message)
    $scmExit = 2
} finally {
    Write-Output ('MANUAL_STOPPED_SCM_EXIT=' + $scmExit + '; INVOKED=' + $scmInvoked + '; NO_RETRY; NO_START; OS_QUIESCENCE=UNKNOWN; NO_PRODUCTIVE_PROMOTION')
}
exit $scmExit
