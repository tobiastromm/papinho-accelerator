# Comando manual — uma observação ProgramFiles-only autorizada

Execução pendente, NÃO realizada pelo agente. Use PowerShell administrativo legítimo do proprietário, console original sem redirecionamento. Uma chamada ao modo read-only: não instala ou atualiza o servidor. O wrapper confere e retém EXE/DLLs antes de invocar o argumento exato.

Autorização registrada SHA256: f2022907a17d3381d09d500996cfdca0b178de9e2eab778ecf85ef544ea7f717.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-COLD-BINARY-PATHS-OBSERVATION-REQUEST.CANDIDATE.md' = '3a8611020ef681eff3838898772b7179759b281886ccbe7215a289d531074beb'
        'COLD-BINARY-PATHS-OBSERVATION-AUTHORIZATION.md' = 'f2022907a17d3381d09d500996cfdca0b178de9e2eab778ecf85ef544ea7f717'
        'observe-cold-binary-paths.MANUAL.CANDIDATE.ps1' = 'f9cd6042dfabf559c16e2812f145514772b00dc0096ddae8e6f515a572e0d17a'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('BOUNDARY_DOCUMENT_PIN_DENY: ' + $name) }
    }
    $procedure = Join-Path $folder 'observe-cold-binary-paths.MANUAL.CANDIDATE.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $procedure -ExpectedScriptSha256 'f9cd6042dfabf559c16e2812f145514772b00dc0096ddae8e6f515a572e0d17a' -ExpectedRequestSha256 '3a8611020ef681eff3838898772b7179759b281886ccbe7215a289d531074beb'
    if ($LASTEXITCODE -ne 0) { throw 'COLD_BOUNDARY_FAILED_OR_UNKNOWN_NO_RETRY' }
}
```

STOP se qualquer hash/contexto/lock/query divergir. Não repetir após falha, mesmo antes da coleta. Não alterar ACL/guards/quotas. ExecutionPolicy Bypass limita-se ao processo filho, sem autoelevação/política persistente.

Envie a saída completa. Compararemos os cinco cortes com112/112/112/113/136, sem presumir redução, ownership ou segurança de fechamento. Exit0 é somente coleta; guard124/máximo128 intactos e Start DENY. Se houver close incerto/estacionamento, preservar UNKNOWN; não matar, limpar ou repetir.

Sem endpoint, SCQ, Start/Stop, UPDATE, SCM/ACL, CA/CURRENT, rede, TPM/NV/Q4, limpeza/refund ou autoridade produtiva. Retenção histórica/OS_QUIESCENCE UNKNOWN. AST PASS; solicitação/procedimento preservados byte-exatos.
