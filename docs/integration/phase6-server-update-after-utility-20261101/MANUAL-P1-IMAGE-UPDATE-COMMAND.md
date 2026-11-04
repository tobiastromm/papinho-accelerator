# Comando manual autorizado — UPDATE P1 único pendente

Não executado pelo agente. Rodar UMA vez no PowerShell administrativo legítimo
do proprietário. Conferir plano nativo/identidades/pins/nomes antes de confirmar.
Sem resposta automática. Cancelar em divergência; não repetir após falha.
Bypass apenas no processo filho, sem alterar política persistente/GPO.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md' = '9a2d514469efb1c1ccc02847ac02e60e54de740ac72aae5c9c84c39f93306e24'
        'P1-IMAGE-UPDATE-AUTHORIZATION.md' = 'bc1b3f4cd663de6b00418a5743aafb62d3ffc30c4f1b5dcca9f135daf0e12c65'
        'update-p1-image.MANUAL.CANDIDATE.ps1' = '89cb7ef92a5cc7e8d22affe6c0ea366ba69e280865b3b95dd969a29e886b9ad9'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('UPDATE_DOCUMENT_PIN_DENY: ' + $name) }
    }
    $binaries = @{
        'C:\Program Files\Papinho\Accelerator\papacc_update.exe' = '084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a'
        'C:\Program Files\Papinho\Accelerator\papacc_server.exe' = '7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d'
        'C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe' = '54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50'
        'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' = '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
        'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' = '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
    }
    foreach ($path in $binaries.Keys) {
        if ((Get-FileHash -LiteralPath $path -Algorithm SHA256 -ErrorAction Stop).Hash -ne $binaries[$path]) { throw ('UPDATE_BINARY_PIN_DENY: ' + $path) }
    }
    $procedure = Join-Path $folder 'update-p1-image.MANUAL.CANDIDATE.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $procedure -ExpectedScriptSha256 '89cb7ef92a5cc7e8d22affe6c0ea366ba69e280865b3b95dd969a29e886b9ad9' -ExpectedRequestSha256 '9a2d514469efb1c1ccc02847ac02e60e54de740ac72aae5c9c84c39f93306e24'
    if ($LASTEXITCODE -ne 0) { throw 'P1_IMAGE_UPDATE_FAILED_OR_UNKNOWN_NO_RETRY' }
}
```

O procedimento verifica os mesmos cinco arquivos por referências retidas,
identidade/tamanho/hash/continuidade e contexto antes da chamada única ao executor.
SCM STOPPED/PID0/configuração, proteção estrita U2, volume/recursos/Stop são gates
nativos frescos antes dos efeitos. Snapshot preparatório não os substitui.
Retenção histórica e OS_QUIESCENCE permanecem UNKNOWN. Se impedirem predicado
material, STOP; não limpar/restaurar/ajustar ACL nem contornar sharing/guards.

Após a execução única, enviar a saída integral. Não executar Start/Stop, endpoint,
SCQ ou nova tentativa. Aguardar reconciliação de consumo/dois renames/anterior/
canônico/hash/proteção/SCM/fechamento. Nada aqui concede autoridade produtiva.
