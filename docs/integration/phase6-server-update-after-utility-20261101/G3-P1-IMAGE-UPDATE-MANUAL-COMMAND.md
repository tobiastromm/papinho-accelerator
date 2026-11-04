# Comando manual único — UPDATE G3-P1

Autorização registrada; execução ainda pendente. Executar UMA vez em PowerShell
administrativo x64. Conferir o plano nativo antes de responder. Não repetir em
falha/cancelamento/UNKNOWN. Este bloco não fornece confirmação automática.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-G3-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md' = 'bafa1b8a58c8eed646f00998597c1f4b053262967a78c3ac3603af860270dda0'
        'OWNER-G3-P1-UPDATE-APPROVAL.txt' = '31d4aa39c96890c1b8f418bb21202214b6448ad8f1d027c47a52ec17c0eb7c8a'
        'G3-P1-IMAGE-UPDATE-AUTHORIZATION.md' = 'fbfc7c2f2be3989ae583412f644c2456abfc84bb52a1426bd8c0a0239cd32283'
        'update-g3-p1-image.MANUAL.CANDIDATE.ps1' = 'fa96e9c9cc5ee61cec79ca0599ecd5105dbe1092f2b141518d41b05bc6e3f318'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('G3_P1_UPDATE_PIN_DENY: ' + $name) }
    }
    $procedure = Join-Path $folder 'update-g3-p1-image.MANUAL.CANDIDATE.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $procedure -ExpectedScriptSha256 'fa96e9c9cc5ee61cec79ca0599ecd5105dbe1092f2b141518d41b05bc6e3f318' -ExpectedRequestSha256 'bafa1b8a58c8eed646f00998597c1f4b053262967a78c3ac3603af860270dda0'
}
```

Bypass somente no processo filho; não altera política persistente.
Enviar saída integral para reconciliação; sem retry, Start, limpeza ou rollback.
Não inferir consumo/sucesso antes da saída nativa. Retenção histórica/OS UNKNOWN.
