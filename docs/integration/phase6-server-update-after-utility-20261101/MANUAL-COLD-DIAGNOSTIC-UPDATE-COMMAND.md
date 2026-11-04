# Comando manual autorizado — UMA execução pendente

Não executado pelo agente. Usar PowerShell administrativo legítimo. Conferir o plano nativo antes de confirmar; não repetir após falha. ExecutionPolicy Bypass somente no processo filho. Retenção histórica e OS_QUIESCENCE UNKNOWN. Após executar, enviar a saída completa para reconciliação; não iniciar o serviço.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-COLD-DIAGNOSTIC-UPDATE-REQUEST.CANDIDATE.md' = '2ecb433f63a15e35c8244dcfa090df2aaeaf9b0bb76e2f6336f8d4ac13a9d2b2'
        'COLD-DIAGNOSTIC-UPDATE-AUTHORIZATION.md' = '6be87aa493727eb2ed8e93f5a2f4279a128bbb9b3d9079e83303840887db42b6'
        'update-cold-diagnostic.MANUAL.CANDIDATE.ps1' = 'e81f1e410cb7c6f87f9ae6a5a166a39994559f7ca622179f95a32cc432e503ae'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('UPDATE_DOCUMENT_PIN_DENY: ' + $name) }
    }
    $binaries = @{
        'C:\Program Files\Papinho\Accelerator\papacc_update.exe' = '084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a'
        'C:\Program Files\Papinho\Accelerator\papacc_server.exe' = '3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b'
        'C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe' = '7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d'
        'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' = '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
        'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' = '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
    }
    foreach ($path in $binaries.Keys) {
        if ((Get-FileHash -LiteralPath $path -Algorithm SHA256 -ErrorAction Stop).Hash -ne $binaries[$path]) { throw ('UPDATE_BINARY_PIN_DENY: ' + $path) }
    }
    $procedure = Join-Path $folder 'update-cold-diagnostic.MANUAL.CANDIDATE.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $procedure -ExpectedScriptSha256 'e81f1e410cb7c6f87f9ae6a5a166a39994559f7ca622179f95a32cc432e503ae' -ExpectedRequestSha256 '2ecb433f63a15e35c8244dcfa090df2aaeaf9b0bb76e2f6336f8d4ac13a9d2b2'
    if ($LASTEXITCODE -ne 0) { throw 'COLD_DIAGNOSTIC_UPDATE_FAILED_OR_UNKNOWN_NO_RETRY' }
}
```

Parsing AST PASS; nenhum comando de efeito foi invocado nesta preparação.
