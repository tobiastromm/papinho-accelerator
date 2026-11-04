# Coleta manual autorizada — cinco cortes

Uma execução pendente; NÃO executada pelo agente. Copie o bloco para PowerShell administrativo legítimo, em console original sem redirecionamento. O procedimento confere e retém EXE/DLLs antes de chamar somente o modo read-only. Sem autoelevação; ExecutionPolicy Bypass somente no processo filho.

Registro da autorização SHA256: 0d945ec3fe27bb12e62de858bd9b84b85fcb232d2069755d8b35cb194451cdd6.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-COLD-BOUNDARY-OBSERVATION-REQUEST.CANDIDATE.md' = 'e7b641632fff1cc6c3d66c04f111c389d07c21866a3bcd5e9f448b8c369f4a42'
        'COLD-BOUNDARY-OBSERVATION-AUTHORIZATION.md' = '0d945ec3fe27bb12e62de858bd9b84b85fcb232d2069755d8b35cb194451cdd6'
        'observe-cold-boundaries.MANUAL.CANDIDATE.ps1' = 'b68ba77cce2144bf1755ac48032c733604b0bfb181e9781994c89cc94f93a87c'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('BOUNDARY_DOCUMENT_PIN_DENY: ' + $name) }
    }
    $procedure = Join-Path $folder 'observe-cold-boundaries.MANUAL.CANDIDATE.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $procedure -ExpectedScriptSha256 'b68ba77cce2144bf1755ac48032c733604b0bfb181e9781994c89cc94f93a87c' -ExpectedRequestSha256 'e7b641632fff1cc6c3d66c04f111c389d07c21866a3bcd5e9f448b8c369f4a42'
    if ($LASTEXITCODE -ne 0) { throw 'COLD_BOUNDARY_FAILED_OR_UNKNOWN_NO_RETRY' }
}
```

Não repetir após qualquer falha. Envie a saída completa, incluindo os cinco slots e o resultado de fechamento. Não execute Start, Stop ou limpeza. Se houver retenção, preserve UNKNOWN; não mate o processo para fabricar fechamento.

Slot0 inclui CRT/loader/herdados. Deltas não demonstram fechamento seguro nem origem histórica dos136. Guard124/máximo128 intactos; exit0 significa coleta, nunca capacidade. Retenção histórica e OS_QUIESCENCE UNKNOWN.

Comando validado por AST, não invocado. Solicitação e procedimento aprovados preservados byte-exatos.
