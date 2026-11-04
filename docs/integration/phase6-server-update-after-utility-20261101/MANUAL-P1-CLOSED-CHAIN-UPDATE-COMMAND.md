# Comando manual autorizado — uma execução UPDATE da cadeia P1

P1-CLOSED-CHAIN-UPDATE-AUTHORIZATION.md; execução ainda PENDENTE.
PowerShell5.1 x64 elevado, host TCORP-TOBIAS. Executar UMA vez; não repetir se
falhar/cancelar, inclusive se consumed interno0. Nenhuma confirmação automática.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $chainFolder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $chainRequestHash = '28fee308a5c509e72d842a85d8cd45ebffc028f1bb65c5a878d9cd46cf5670a6'
    $chainScriptHash = 'cbfc1290ed5cffaedfacea70442931fe450ed36197b61a21b68872f75283077e'
    $chainScript = Join-Path $chainFolder 'update-p1-closed-chain.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath (Join-Path $chainFolder 'ONE-P1-CLOSED-CHAIN-UPDATE-REQUEST.CANDIDATE.md') -Algorithm SHA256).Hash -ne $chainRequestHash) { throw 'CHAIN_UPDATE_REQUEST_PIN_DENY' }
    if ((Get-FileHash -LiteralPath $chainScript -Algorithm SHA256).Hash -ne $chainScriptHash) { throw 'CHAIN_UPDATE_SCRIPT_PIN_DENY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $chainScript -ExpectedScriptSha256 $chainScriptHash -ExpectedRequestSha256 $chainRequestHash
}
```

O procedimento confere normas/artefatos e adquire os originais. No plano nativo,
conferir executor protegido, DATA774656/pin9816585b...8d0220, antigo772608/
ID417614100:1769472:132345/pin54d3...4a50, parent e dois leaves exclusivos,
K2027314 e R factual. Divergência ou retenção UNKNOWN bloqueante: cancelar.
Somente depois digitar a confirmação EXATA OWNER_BINARY_UPDATE exibida pelo
próprio plano, no prompt nativo. Não copiar confirmação histórica.

Enviar saída integral. Não executar Start/Stop, endpoint/SCQ, cleanup ou retry.
