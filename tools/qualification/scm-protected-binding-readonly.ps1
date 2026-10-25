# Read-only LAB qualification only. Run manually in an eligible console.
$ErrorActionPreference = 'Stop'
if ($args.Count -ne 0) { throw 'No arguments are accepted.' }
$qualificationExe = 'C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server.exe'
$qualificationConfiguration = 'C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release\papacc.conf'
if ((Get-FileHash -LiteralPath $qualificationExe -Algorithm SHA256).Hash -ne '3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb') { throw 'Executable hash mismatch; STOP.' }
if ((Get-FileHash -LiteralPath $qualificationConfiguration -Algorithm SHA256).Hash -ne 'a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44') { throw 'Configuration hash mismatch; STOP.' }
Write-Host 'READ-ONLY SCM LAB binding: no service, account, ACL, CA or CURRENT will be created.'
& $qualificationExe service install --prepare-lab --installation 'C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release' --configuration papacc.conf
$qualificationExit = $LASTEXITCODE
Write-Host ('SCM_READONLY_BINDING_EXIT=' + $qualificationExit)
Write-Host 'No elevation or retry is performed. A zero exit qualifies preparation only; CreateService remains DENY.'
exit $qualificationExit
