# Resultado manual contemporâneo — captura read-only

[Recibo factual](../canonical-preflight-busy-diagnosis-20261023/manual-prepare-134372761903030040-final.json), SHA2569ef63149e6eaf7eeb8ebed4b7deb25948ba0a60508d2110aadbc21436ae2abe4, conferido no disco. Launcher60a6dd761fee00143cdf88d7fcf629ad1a47986f41875ab1e8020c641b10bca0 e helpere76df9084e62e761bd24e818074c4aed1dcff520b63aa38660275400ed153a51 correspondem aos pins qualificados. Owner executou uma vez; executor não repetiu nem modificou código.

## Fatos novos

- Launcher original: PID21068, creation31286096:2729691230.
- PowerShell elevado: PID2496, creation31286096:2763513624.
- Qualificador: PID4280, creation31286096:2782760166.
- Processo rejeitado: nome observado conhost.exe, PID12312, parent2496, creation31286096:2763651651; handle original mantido durante a captura e wait258 (WAIT_TIMEOUT, ainda não sinalizado naquela consulta).
- Esse processo nasceu13.8027ms após o PowerShell elevado e1910.8515ms ANTES do qualificador. A instância observada já existia antes da execução do qualificador. Isso direciona a análise de topologia à etapa de criação do PowerShell elevado; não identifica, sozinho, a API exata que criou o host.
- Mesma sessão1. Membros do console antes/depois:2496 e4280. Isso prova a associação dos CLIENTES à mesma lista observada, não autentica o host12312.
- HWND1181714 retornou creator_pid2496, o próprio elevado, não12312. Portanto essa rota de correlação não identifica o host rejeitado neste ambiente; window_owner_candidate=0. Não há fallback por nome ou parent.

O processo abriu caminho C:\Windows\System32\conhost.exe. Arquivo observado: native417614100:327680:1591236,867840 bytes, SHA256b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963. Esses são fatos do arquivo aberto, não vínculo autenticado ao arquivo mapeado no processo.

WinVerifyTrust retornou LONG-2146762496 = HRESULT0x800B0100, TRUST_E_NOSIGNATURE, confirmado no SDK local winerror.h (MessageId/TRUST_E_NOSIGNATURE). Essa chamada não confirmou assinatura. Não concluir que o Windows file é malicioso ou universalmente não assinado: assinatura de catálogo/publisher/qualificação específica não foi estabelecida por essa evidência.

identity_stable=1 refere-se ao handle/creation mantidos durante a coleta. image_stable=0 é um predicado COMPOSTO: igualdade do caminho e memcmp de todo BY_HANDLE_FILE_INFORMATION. O log não informa qual componente divergiu. LastAccessTime faz parte dessa estrutura; não se pode concluir substituição de arquivo, nem simplesmente ignorar a divergência. Se necessário, a próxima instrumentação mínima registra diferenças individuais de caminho, FileID, size, attributes, links e cada timestamp, sem normalizar nada ainda.

## Métricas observadas

| Processo | Private/commit bytes | Working Set bytes | Handles | Threads | Stack comprometida VA bytes |
|---|---:|---:|---:|---:|---:|
| Console host12312 |3751936|13680640|191|7|163840|
| Launcher21068 |55668736|83783680|724|25|11010048|
| Elevado2496 |55365632|76742656|703|26|11063296|
| Qualificador4280 |1482752|7127040|147|6|147456|

As amostras foram sequenciais num intervalo observado de aproximadamente16ms; não são pico simultâneo atômico, teto universal ou prova de custos kernel/cache. Campos não observados permanecem UNKNOWN.

BLOQUEIO INDEPENDENTE: a amostra do qualificador contém147 handles, acima do guard nativo accepted de128 em B4.B.4/ig_metric. Esse valor não foi promovido a admissão: o censo já havia parado em170. Não aumentar o guard nem atribuir automaticamente o excesso a determinado subsistema sem censo de handles/ownership por fase. A próxima prova mínima para esse ponto é decompor os handles do qualificador por etapa/tipo e distinguir baseline, transitórios e retenção, preservando os limites.

O console host não tem um guard próprio accepted. A proposta anterior que reutilizava128 handles para ele também não acomodaria191, mas continua apenas proposta. Na composição PROPOSED do papel elevado, somas por métrica desta amostra seriam: private/commit59117568 bytes, WS90423296, handles894,threads33,stack11227136. Caberiam nos guards escolhidos para esse papel, mas isso NÃO aceita a composição, não resolve identidade/associação nem resolve os147 handles do qualificador. Não somar private+commit+stack como consumos independentes; WS somado pode contar compartilhamento.

## Resultado e menor continuação

Captura útil concluída, prova autenticada não concluída. Permanecem três fatos a resolver, sem repetir automaticamente:

1. A rota HWND observada aponta ao elevado, não ao conhost. Investigar prova suportada alternativa ou topologia da elevação; não inferir ownership apenas de nascimento, sessão e parent.
2. Decompor image_stable=0, se esse dado for necessário à classificação, antes de alterar seu verificador. Trust/publisher/mapped-image permanecem UNKNOWN.
3. Qualificar o excesso de handles do qualificador sob o guard128, sem aumentar threshold ou declarar créditos.

Não aplicar patch com base apenas neste resultado. Revisão B4 de composição continua PROPOSED; nenhum quinto slot. Histórico de PIDs/handles acima não autoriza consulta a instâncias atuais como se fossem as mesmas.

```text
CONSOLE_CAPTURE=FACTUAL_PARTIAL
CONSOLE_ASSOCIATION_PROOF=UNKNOWN
POWERSHELL_PARENT_ROLE=ELEVATED_PID2496
CONHOST_RESOURCE_ACCOUNTING=OBSERVED_SAMPLE_KERNEL_TRANSITIVE_UNKNOWN
QUALIFIER_HANDLE_SAMPLE=147
QUALIFIER_HANDLE_GUARD=128
CANONICAL_CENSUS=UNCHANGED_STOP_170
OWNED_CAPTURE_REFERENCES=CLOSED_REPORTED
QUALIFIER_CLOSE=0_REPORTED
HOST_TERMINATION=NOT_CLAIMED
STARTS=0
STOPS=0
INTENT_CONSUMED=0
SCM_EFFECTS=NONE
ACL_MUTATIONS=NONE
DIAGNOSTIC_OPERATIONAL_FILE_CREATED=0
LOG_FAILED=false
PREFLIGHT_RETRY=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
```

Nenhuma alteração funcional, ACL/SCM/quotas/produção, bootstrap, CA, chave, certificado ou CURRENT. Sem liberação forçada, commit/push/tag/release ou criação de clone/worktree.
