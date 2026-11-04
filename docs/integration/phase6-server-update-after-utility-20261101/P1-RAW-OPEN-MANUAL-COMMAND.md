# Uma execução manual autorizada — somente observação raw0

Autorização: P1-RAW-OPEN-OBSERVATION-AUTHORIZATION.md. Ainda não executada.
Use PowerShell x64 elevado, no host aprovado. Cole este bloco uma única vez.
Não executar novamente em falha, cancelamento ou resultado inconclusivo.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $rawFolder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $rawRequestHash = 'ce5b3f33ee311eae9f5771e0117a112e4a81d6e0197e388c049617da13bee166'
    $rawScriptHash = '087f5a164f770cd3d239e50765e18fda3577e9bef2c55004ea4e9b77de650c3c'
    $rawScript = Join-Path $rawFolder 'observe-p1-raw-open.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath (Join-Path $rawFolder 'ONE-P1-RAW-OPEN-OBSERVATION-REQUEST.CANDIDATE.md') -Algorithm SHA256).Hash -ne $rawRequestHash) { throw 'RAW_REQUEST_PIN_DENY' }
    if ((Get-FileHash -LiteralPath $rawScript -Algorithm SHA256).Hash -ne $rawScriptHash) { throw 'RAW_SCRIPT_PIN_DENY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $rawScript -ExpectedScriptSha256 $rawScriptHash -ExpectedRequestSha256 $rawRequestHash
}
```

Somente no prompt próprio do observador, confirme digitando:
`OBSERVE_P1_REQUEST_RAW_ONCE`

O driver também confere fonte/binário; não abre previamente o alvo.
Se houver pin divergente, contexto inválido, guard ou Stop: parar, não contornar.
Envie a saída integral depois. Não use exit0 como autoridade ou solução do
histórico. Não executar E/Q/S, Start, UPDATE ou coleta adicional.
