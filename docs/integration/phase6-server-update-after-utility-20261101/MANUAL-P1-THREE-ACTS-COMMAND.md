# Comando manual autorizado — sessão P1 única pendente

NÃO executado pelo agente. Usar PowerShell administrativo legítimo do proprietário.
Uma execução somente. Não repetir após falha, cancelamento ou resultado UNKNOWN.
O driver faz as duas cópias protegidas e depois chama o controlador instalado;
nenhuma etapa ou confirmação é executada automaticamente pelo agente.
ExecutionPolicy Bypass apenas no processo filho, sem alteração persistente/GPO.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-P1-THREE-ACTS-REQUEST.CANDIDATE.md' = '86303aac1bf3b040a5949e8947a2bee2cb157fdbde472f31d5f2830b469a2a34'
        'OWNER-P1-THREE-ACTS-APPROVAL.txt' = 'a7d3544dd19c0639e9799486efe70b976d3ab543de0d11ab15edd2d424148607'
        'P1-THREE-ACTS-AUTHORIZATION.md' = '226db3e18c58ee20cc12bf321ab147df9b028eb850ed686f89d07cde32f14b72'
        'p1-session-originals.cs' = '07d1feb413be1685763f1ea5fc5cedfdfce3570604ae7860d1c46796909efc6b'
        'prepare-p1-originals.MANUAL.CANDIDATE.ps1' = '4c4110d8d36f92d3871c9e87b15c6d6a7bd0c7690f16df4f0a1d33d38824dd9d'
        'run-p1-diagnostic.MANUAL.CANDIDATE.ps1' = 'c4242f434730df652ec7043fcef8313e0e4a8ebbd961690bcd65a6d85cf3d79b'
        'P1-ACT-ENDPOINT.REQUEST.CANDIDATE.md' = '89343c75847d37734e582abc30656ccbdd37055d7667cad90b521c75ee85b04f'
        'P1-ACT-SCQ.REQUEST.CANDIDATE.md' = '842c9d862a116e5d5ff1990d10927341b8441b799639cfb906d4a52fd2c2a7f7'
        'P1-ACT-START.REQUEST.CANDIDATE.md' = 'b1877bd5c5ecc397fee5e927ed19b32567d0986d383c2ebf4d539fd9bad76c01'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('P1_DOCUMENT_PIN_DENY: ' + $name) }
    }
    $binaries = @{
        'C:\Program Files\Papinho\Accelerator\papacc_server.exe' = '54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50'
        'C:\Program Files\Papinho\Accelerator\libssl-3-x64.dll' = '3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed'
        'C:\Program Files\Papinho\Accelerator\libcrypto-3-x64.dll' = '09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044'
    }
    foreach ($path in $binaries.Keys) {
        if ((Get-FileHash -LiteralPath $path -Algorithm SHA256 -ErrorAction Stop).Hash -ne $binaries[$path]) { throw ('P1_BINARY_PIN_DENY: ' + $path) }
    }
    $procedure = Join-Path $folder 'run-p1-diagnostic.MANUAL.CANDIDATE.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $procedure -ExpectedScriptSha256 'c4242f434730df652ec7043fcef8313e0e4a8ebbd961690bcd65a6d85cf3d79b' -ExpectedOperationalRequestSha256 '86303aac1bf3b040a5949e8947a2bee2cb157fdbde472f31d5f2830b469a2a34' -ExpectedDecisionSha256 'a7d3544dd19c0639e9799486efe70b976d3ab543de0d11ab15edd2d424148607'
    if ($LASTEXITCODE -ne 0) { throw 'P1_SESSION_FAILED_OR_UNKNOWN_NO_RETRY' }
}
```

## Durante a única sessão

1. Conferir a declaração documental: somente as duas cópias NOVAS do pedido e
   decisão reais, com seus pins acima. Não confirma E/Q/S. Se falhar, parar.
2. Somente com cópias verificadas e fechamento conhecido o driver prossegue.
   Conferir IDs novos da saída e os originais apresentados nos planos nativos.
3. No prompt nativo P1, inserir MANUALMENTE a linha DATA abaixo; não é comando
   PowerShell, aprovação ou resposta às confirmações:

```text
P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:1769472:132345 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87
```

4. Conferir e responder individualmente, apenas se os planos coincidirem:
   OWNER_SCQ, OWNER_SCQ_PRIVATE_ENDPOINT e OWNER_SCQ_DIAGNOSTIC_START.
   Não fornecer respostas antecipadas ou automatizadas; cancelar divergência.

Guard124/cap128, G3 exato, source/identidade/proteção/continuidade, SCM parado,
namespace, Stop e deadlines são gates frescos obrigatórios. Snapshot preparatório
não substitui esses gates nem prova redução de handles. Falha/UNKNOWN material
nega progressão; não ajustar ACL/quotas/sharing, reexecutar, reiniciar ou limpar.
PID/token/bilateral/receipt/close e término continuam provas da sessão real.

Após a execução única, enviar a saída INTEGRAL para reconciliação. Não executar
Start adicional, ControlService Stop, UPDATE, endpoint extra, limpeza/refund ou
recuperação. Retenção histórica/OS UNKNOWN e readiness produtiva DENY preservados.
