# Uma sessão manual autorizada — novos originais e E/Q/S P1 corrigida

Registro: P1-CHAIN-THREE-ACTS-AUTHORIZATION.md. Ainda não executada.
PowerShell5.1 Desktop x64 elevado, host aprovado. Cole o bloco UMA vez.
Não repetir se falhar/cancelar, mesmo se Start não for alcançado.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $p1ChainFolder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $p1ChainRequestHash = '608b6ca768dd35f35c827ee29498a2bdf5796cc5a3d66843a8568fa002934441'
    $p1ChainDecisionHash = 'f54d0bf3c6c609d5e7f93d348d4332364fdee0edc1592ea2a30137ba31ead0e1'
    $p1ChainScriptHash = 'cdf8d918a7e6e114f69b4fda5be3a3983687f108e65dfb8b11a2ac0d4249a13e'
    $p1ChainScript = Join-Path $p1ChainFolder 'run-p1-chain-diagnostic.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath (Join-Path $p1ChainFolder 'ONE-P1-CHAIN-THREE-ACTS-REQUEST.CANDIDATE.md') -Algorithm SHA256).Hash -ne $p1ChainRequestHash) { throw 'CHAIN_SESSION_REQUEST_PIN_DENY' }
    if ((Get-FileHash -LiteralPath (Join-Path $p1ChainFolder 'OWNER-P1-CHAIN-THREE-ACTS-APPROVAL.txt') -Algorithm SHA256).Hash -ne $p1ChainDecisionHash) { throw 'CHAIN_SESSION_DECISION_PIN_DENY' }
    if ((Get-FileHash -LiteralPath $p1ChainScript -Algorithm SHA256).Hash -ne $p1ChainScriptHash) { throw 'CHAIN_SESSION_SCRIPT_PIN_DENY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $p1ChainScript -ExpectedScriptSha256 $p1ChainScriptHash -ExpectedOperationalRequestSha256 $p1ChainRequestHash -ExpectedDecisionSha256 $p1ChainDecisionHash
}
```

1. No materializador, conferir pedido/decisão e digitar a linha
   OWNER_P1_CHAIN_SESSION_DOCUMENTS exibida, somente se corresponder ao pacote.
   Isso confirma só as duas cópias auxiliares.
2. No prompt nativo P1 DATA, digitar MANUALMENTE a seleção aprovada abaixo:
```
P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:1638400:813992 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87
```
   Não executar como comando PowerShell. DATA não confirma nenhum ato.
3. Se os planos E/Q/S forem apresentados, conferir arquivos/IDs contra a saída
   dos novos originais, serviço/imagem/propósito e recursos. Só então fornecer
   as três confirmações EXATAS geradas naquela sessão. Não copiar nonces antigos.

Se houver divergência/insuficiência/UNKNOWN bloqueante, cancelar e enviar a
saída; não alterar guard, quota, ACL ou comando. Não executar Start/Stop extra,
limpeza/UPDATE ou repetir a sessão. Enviar saída integral para reconciliação.
