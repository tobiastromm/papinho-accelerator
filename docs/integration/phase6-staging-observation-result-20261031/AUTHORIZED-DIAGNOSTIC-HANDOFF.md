# Diagnóstico read-only — autorização registrada e comando manual

O proprietário aprovou o pedido integral ao responder `pode aprovar` à sua
apresentação com SHA256a4cbd102a217f9ce3f34cdc9bd8b6750608912c004c9d2ccb8a2633962268e71.
Conferência byte-exata do pedido/script/três artefatos PASS. Proposta e script
originais preservados, inclusive comentários históricos, sem mudança de pins.

Registro: OWNER-DIAGNOSTIC-AUTHORIZATION.json, SHA256
`594b1f6b5a68f02e85b285a65438d2d246ed2f23bfbca988fd3d376be9a4bd2f`.
Uma única nova coleta diagnóstica manual, incluindo a exceção Process/BYPASS
prevista no pedido, sem herdar a autorização consumida anterior. Não é grant
de runtime, ledger, autoridade de staging ou prova de execução/consumo.

Estado: autorização registrada, nenhuma execução pelo agente, evidência/consumo
pendentes da saída manual. Sem código/build/teste reexecutado: somente conferência
de bytes e parsing estático. Contexto/identidade/recursos frescos serão conferidos
pela invocação; dados históricos/sandbox não substituem esses fatos.

Em Windows PowerShell5.1 Desktop x64 já elevado, colar o bloco inteiro uma vez.
Não executar se já iniciou esta nova coleta diagnóstica. Não usar o comando antigo.

```powershell
& {
    $stagingCollector = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-staging-observation-result-20261031\collect-staging-diagnostic.MANUAL.ps1'
    $stagingScriptSha256 = '7c1ca380bf8e01fdee7fa3f564131843de7381bc61c80ac3b9ce4d46b7dfe496'
    if ((Get-FileHash -LiteralPath $stagingCollector -Algorithm SHA256 -ErrorAction Stop).Hash -ne $stagingScriptSha256) { throw 'SCRIPT_HASH_DENY' }
    $stagingPreviousPolicy = Get-ExecutionPolicy -Scope Process
    try {
        Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force -ErrorAction Stop
        if ((Get-ExecutionPolicy) -ne 'Bypass') { throw 'EXECUTION_POLICY_DENY' }
        & $stagingCollector -ExpectedScriptSha256 $stagingScriptSha256
    } finally {
        Set-ExecutionPolicy -Scope Process -ExecutionPolicy $stagingPreviousPolicy -Force -ErrorAction Stop
    }
}
```

O hash externo precede a chamada. O script confere de novo seu hash e os três
pins antes de chamar o exe. A política anterior de Process é restaurada no
finally; não alterar CurrentUser/LocalMachine/GPO nem elevar automaticamente.
Política efetiva incompatível: STOP, sem fallback ou nova tentativa.

Enviar a saída integral, inclusive primeira falha, frames, STAGING_POST_ENTRY_HANDLES,
STAGING_HANDLE_GUARD, fonte, close e exit do wrapper. Count_known0 significa
UNKNOWN, não contagem0. Contagem do frame1 não é contagem do guard posterior.
Não confundir crescimento anterior ao layout com custo de etapa não executada.
Exit0 indica coleta, não admissão. Guard124/cap128/cohort1024 não são dispensados.

Falha, Stop, interrupção, pending ou close incerto: não repetir, preservar UNKNOWN
e aguardar reconciliação da tentativa. Nenhuma recuperação, cleanup/refund,
SCM/ACL, Start/Stop, instalação, staging, endpoint, CA/CURRENT, KSP/ownerAuth,
TPM/Q4 ou rede autorizados. E21 PENDING; ED11/Phase6 PARTIAL; staging DENY.
