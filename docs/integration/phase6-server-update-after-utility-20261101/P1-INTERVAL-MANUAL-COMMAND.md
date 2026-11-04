# Comando manual — uma observação P1 autorizada

Autorização registrada em P1-INTERVAL-OBSERVATION-AUTHORIZATION.md.
Execução ainda pendente. Em Windows PowerShell interativo como administrador,
executar UMA vez o bloco abaixo. Pode ser chamado de qualquer pasta.
Não modificar o procedimento/pedido ou substituir o EXE selecionado.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $request = Join-Path $folder 'ONE-P1-INTERVAL-OBSERVATION-REQUEST.CANDIDATE.md'
    $script = Join-Path $folder 'observe-p1-interval.MANUAL.CANDIDATE.ps1'
    $requestHash = 'e6644ce7505c6ae54efd4daadd51b98b3b254a397d83ae77348b472db3bd345d'
    $scriptHash = '22906613fd4ff0e9206e830160caac7109225d23c845c7aadf6020ec07e5ffd9'
    if ((Get-FileHash -LiteralPath $request -Algorithm SHA256).Hash -ne $requestHash) { throw 'REQUEST_PIN_DENY' }
    if ((Get-FileHash -LiteralPath $script -Algorithm SHA256).Hash -ne $scriptHash) { throw 'SCRIPT_PIN_DENY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $script -ExpectedScriptSha256 $scriptHash -ExpectedRequestSha256 $requestHash
}
```

O procedimento confere host/token/admin/ausência de impersonation e pins EXE/DLLs,
retendo referências aos artefatos. Não abre antecipadamente o alvo request.
Quando solicitado, digitar manualmente `OBSERVE_P1_INTERVAL_ONCE`.
Nenhuma confirmação E/Q/S será solicitada ou concedida por essa prova.

Enviar toda a saída, inclusive P1_INTERVAL_NATIVE/P1_INTERVAL_RESULT e wrapper.
Qualquer DENY/falha/UNKNOWN: não repetir. Se não retornar por retenção nativa,
não realizar encerramento forçado ou limpeza por este procedimento; comunicar
a última saída disponível. Não inferir close nativo a partir do wrapper.
Após resultado, localizar deltas da mesma execução, preservando limites e
UNKNOWN histórico. Não executar nova sessão Start com esta autorização.
