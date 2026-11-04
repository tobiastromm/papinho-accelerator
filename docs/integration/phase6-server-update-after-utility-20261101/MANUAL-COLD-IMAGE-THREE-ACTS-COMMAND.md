# Uma sessão manual autorizada — execução pendente

Decisão real preservada literalmente em OWNER-COLD-IMAGE-THREE-ACTS-APPROVAL.txt,
SHA256 ef8b47fa4dba37224bef904488ff0494db4e5804c1e696463ddc0dd864f3e199.
Pedido af8529...67bc9 aprovado; canônico763904bytes/417614100:1835008:134613/
pin7d29...ca7d reconfirmado, DLLs e proteção conferidas. SCM observado
STOPPED/PID0/exit1077/configuração original. Snapshot não é lease.

Estado: AUTHORIZED_ONE_MANUAL_SESSION_PENDING. Nenhuma cópia protegida,
endpoint, SCQ ou Start foi executado pelo agente. Não reutilizar os comandos
anteriores. Executar UMA vez no PowerShell administrativo legítimo do proprietário,
com console original e sem redirecionamento. Não há resposta automática.
ExecutionPolicy Bypass somente no processo filho; não altera política persistente.

Primeiro haverá a confirmação de UMA cópia documental auxiliar exclusiva;
somente com seu readback/close conhecido o driver chama o canônico. Conferir IDs,
imagem atual e planos. As três confirmações independentes obrigatórias são
OWNER_SCQ, OWNER_SCQ_PRIVATE_ENDPOINT e OWNER_SCQ_DIAGNOSTIC_START.
A decisão nova reconhece expressamente a imagem atual e preserva o documento-base
histórico com a seleção anterior. Não confundir esse documento ou a decisãoC6
consumida com nova autoridade. Hash/arquivo/QUERY/carrier não autorizam isolados.

G3: cap128/guard124; privado/WS1994944 por participante; físico7239172096,
commit9823326208 bytes; freshness100ms; janela240000/global60000ms, sem renovar.
Gates frescos podem negar mesmo com aprovação. Stop, retenção, bilateral,
receipt/close e término finito são obrigatórios. Nenhuma prontidão presumida.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-COLD-IMAGE-THREE-ACTS-REQUEST.CANDIDATE.md' = 'af8529db7f447fa6af6fc462fcb3cba4583abfc41f2e867c255b801974a67bc9'
        'OWNER-COLD-IMAGE-THREE-ACTS-APPROVAL.txt' = 'ef8b47fa4dba37224bef904488ff0494db4e5804c1e696463ddc0dd864f3e199'
        'run-cold-image-diagnostic.MANUAL.CANDIDATE.ps1' = '88b9c28775715b92f93b47d377dbebd1ca1c2dc6d033fa66d59e0bf51ef913bf'
        'prepare-cold-image-decision.MANUAL.CANDIDATE.ps1' = '13c6a92f76eca42c36afe251aa087b8a7ab5257ac723def0a09d4a5e7b471f27'
        'cold-image-operational-decision.cs' = '95b5df17c83fc646b4aff2b29b2c7d4d96c5581549c695235f8c73b6eaf77f5e'
        'COLD-IMAGE-ACT-ENDPOINT.REQUEST.CANDIDATE.md' = 'fdb74fbb22b1b50d572b403b05edaf7192e8f5b80ed54ffe7711f393ad2a89ff'
        'COLD-IMAGE-ACT-SCQ.REQUEST.CANDIDATE.md' = 'd6037fc2e05807299bee8e399ed905980d8eb1c9d38a39dfe9e09f70f490b140'
        'COLD-IMAGE-ACT-START.REQUEST.CANDIDATE.md' = '843bebe4074e53fa86b7d18d1709b252916538e1df582691855c797bf37015cf'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('COLD_SESSION_DOCUMENT_PIN_DENY: ' + $name) }
    }
    $script = Join-Path $folder 'run-cold-image-diagnostic.MANUAL.CANDIDATE.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $script -ExpectedScriptSha256 '88b9c28775715b92f93b47d377dbebd1ca1c2dc6d033fa66d59e0bf51ef913bf' -ExpectedDecisionSha256 'ef8b47fa4dba37224bef904488ff0494db4e5804c1e696463ddc0dd864f3e199' -ExpectedOperationalRequestSha256 'af8529db7f447fa6af6fc462fcb3cba4583abfc41f2e867c255b801974a67bc9'
    if ($LASTEXITCODE -ne 0) { throw 'COLD_IMAGE_SESSION_FAILED_OR_UNKNOWN_NO_RETRY' }
}
```

Não repetir em qualquer falha, inclusive cópia documental ou erro antes das
confirmações. Não apagar a decisão, reiniciar a sessão ou alterar quotas/ACL.
Ctrl+C/Ctrl+Break sinaliza Stop do controller, não concede ControlService Stop
ou kill. Se houver retenção/estacionamento, preservar a evidência e UNKNOWN.

Após a execução, enviar saída completa desde a cópia até o retorno final.
Reconciliação: primeira causa, consumo de cada ato, IDs/pins/SD/readback da cópia,
endpoint/SCQ/Start, peer/runtime real, autenticação bilateral, receipt/close,
RUNNING diagnóstico se alcançado e término normal/SCM STOPPED/PID0/exit0.
SCQ_START=DIAGNOSTIC_CLOSED_QUALIFIED é o marcador esperado; exit0 ou submissão
isolados não bastam. Não executar segundo Start nem Stop SCM por este comando.
Retenção histórica, quiescência OS e causa1816 passada UNKNOWN. Sem UPDATE,
limpeza/refund/restauração, SCM/ACL, CA/CURRENT, rede operacional, TPM/NV/Q4 ou
Start produtivo. Históricos/pins/EXEs anteriores/PST0.6.4 preservados.

Qualificação nesta preparação: parsing AST do comando PASS; pins do pacote
reconferidos. Modelos/guards existentes reutilizados, sem campanha repetida.
Comando somente preparado, NÃO EXECUTADO. Aprovação não é prova de resultado.