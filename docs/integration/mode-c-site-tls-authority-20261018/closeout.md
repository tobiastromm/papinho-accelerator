# Closeout — autoridade administrativa Site TLS Mode C, 2026-10-18

Slice normativo e de fonte concluído: decisão explicitamente aprovada registrada antes do código, catálogo privado bounded integrado à configuração e commit reais, consulta vinculada a CURRENT/revisão/epoch e gate C3 fail-closed. Não se declara dispatch operacional completo, TLS ou Session qualificados.

## Autoridade
[Decisão accepted](../../adr/DECISION-mode-c-site-tls-profile-authority-20261018.md). Criação e escritas submetidas ao mecanismo oficial require_escalated e aprovadas; nenhuma negativa contornada. Autoridade é a configuração administrativa do Accelerator, não Network Egress, Browser, fixture ou CA de mTLS. Nenhum accepted anterior foi alterado. Não foi encontrado conflito com C2§7, ADR-0044/0048, ADR-0022/0027/0028 ou Model E.

## Implementação
- Modelo existente recebe um catálogo por valor, um perfil por CURRENT, com identidade/revisão/origem, ref, trust, hostname/SNI/TLS, ALPN, revogação/expiração e limites.
- BODY0.5 explícito no journal existente. Hash cobre o conteúdo; não há sidecar mutável usado como autoridade. Formatos02/03/04 permanecem separados e rejeitam catálogo novo.
- A transação root usa o mesmo participante C2, Secure source/lease e mecanismo nativo de commit. PREPARE/VALIDATE precedem COMMIT; CURRENT/epoch somente depois.
- Resolução metadata exige pin/ticket/owner/invocation e epoch atual, revisão/ref exatos, conteúdo da mesma root, validade e ausência de revogação. Cópia por valor, com guards de alias.
- O owner C3 passa a verificar a boundary de perfil/root antes de uso de template. Session/grants factuais continuam obrigatórios; o gate operacional existente permanece DENY. Não foi criado trust novo, listener ou owner. A aplicação positiva dos limites ao DATA/ALPN/concorrência e a materialização de trust Site serão completadas no dispatch antes de habilitação.
- SYSTEM é o único trust suportado inicialmente; CUSTOM/UNKNOWN rejeitados. Nenhuma fixture se torna configuração produtiva.

[Schema e limitações](private-schema.md). Origem administrativa/IDs dos testes são metadados sintéticos, nunca Principal autenticado. Perfil válido é perfil representável/aprovado na configuração; não significa operação admitida.

## Builds e regressões
MSVC19.51 x64, SDK normal API3/SPI4 e /MD. Saídas novas SiteTLSDebug/SiteTLSRelease dentro do canônico. Produto e targets focais compilam/linkam com zero warnings finais. Logs de tentativas anteriores preservados, incluindo erros de declaração/constante/escape e linkage incremental. VSLANG=1033 garante reconhecimento de dependências .inc pelo Ninja; toolchain/flags de warnings não foram trocados.

22 execuções finais offline,11 por configuração: seis casos da transação (abort, commit04, retain04, catalog05, colisão de commit05, retain05); codec/catalog; censo; C2 vazio; ownership sintético; retenção/Stop sintéticos. [Resultados](final-offline-results.json). O commit e a colisão CREATE_NEW são operações locais nativas; as falhas de socket são explicitamente simuladas e não prova nativa de closesocket.

| Oracle | Evidência | Limite |
|---|---|---|
|Perfil válido|encode/decode05 e seleção metadata do pin CURRENT após commit local|não é admissão|
|Ausente/desconhecido|select com ausência/ref divergente|componente offline|
|Revisão obsoleta|select e root query com revisão divergente|não houve reload operacional|
|Epoch divergente|pin alterado rejeitado|não é Session real|
|Policy não CURRENT|consulta negada antes de commit/publicação|root real local|
|Principal não autorizado|boundary C3 sem Session/Principal factual nega|não executou autenticação de Principal real|
|Revogado/expirado|negativos de seleção tipada|fixture, não revogação de sessão ativa|
|Falha de commit|colisão nativa local, sem epoch/CURRENT|não injeta falha kernel de flush/close|
|Retenção|referência mantida, root retida e reaquisição negada|process exit não prova destruição|
|Integridade|hash inválido e comprimento inválido rejeitados|não prova detecção universal de restore|
|Compatibilidade|04 rejeita05 e05 rejeita04;04 sem catálogo continua válido|sem migração|
|Alias|saída que apagaria catálogo/root rejeitada|não altera o owner|

## ED11 e stack
Catálogo sizeof200. Layout declarado1539192 bytes (anterior1537992, delta1200 por cópias aninhadas); root31056, participante528928, owner C2273928, bridge C3132448, fixed layout1232536. Não somar novamente catálogo embutido nas views/slots/root/participante.

Reserva combinada do caso offline1796899 (anterior1794899, delta2000), limite2097152 inalterado, margem300253. Material32768 e runtime3203 mantidos; crédito de stack/scratch coexistente131072 mantido. Sem alocação dinâmica no catálogo/query/gate. Journal continua8192 e commit mantém dois buffers locais8192 já existentes. Codec05 acrescenta cópia bounded de view/1024 bytes; estimativa de arrays/views locais conhecidos fica dentro do crédito declarado, sem alegar medição completa de stack/PST/CRT.

Snapshots e retenção são cobrados por sizeof aninhado e mantêm a reserva até encerramento factual; não há quotas novas nem devolução por timeout. Provider/kernel/allocator/stack de bibliotecas e adequação do target continuam UNKNOWN/PARTIAL. Não confundir a reserva do caso offline com qualificação de RAM operacional.

## Preservação e handoff
HEAD main c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Preimagens, diff, manifests e hashes nesta pasta. Builds anteriores e seis binários históricos preservados; SDK normal imutável. Mapas históricos continuam NOT_RECOVERED. Sem alteração PST/Browser/PACC wire/pin, rede ou Git publication.

Próximo slice: revisão focal do catálogo05 e binding, seguida de SCOPED_C2_C3_DISPATCH_COMPLETION. Integrar materialização Site trust, filtro ALPN e aplicação de limites/leases no dispatch existente antes de qualquer aprovação operacional. Recovery/reload05 e provas de Session/TLS/ED08/Model E sob tráfego não foram qualificadas nem autorizadas nesta tarefa.

CURRENT_TASK_COMPLETED=YES
SLICE_COMPLETED=YES
SITE_TLS_CATALOG_AUTHORITY=ACCELERATOR_EXISTING_ADMINISTRATIVE_CONFIGURATION
NORMATIVE_STATUS=ACCEPTED
TLS_PROFILE_REF_BINDING=CURRENT_REVISION_METADATA_AND_C3_FAIL_CLOSED_GATE
ROOT_POLICY_CURRENT=POST_COMMIT_OFFLINE_VALIDATED
EPOCH_BINDING=FACTUAL_POST_COMMIT_OFFLINE_VALIDATED
SITE_TLS_ADMISSION=DENY_UNTIL_APPROVED
CLIENT_ADMISSION=DISABLED
ED11_ADMISSION=DENY
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=22_OF_22_FINAL_EXECUTIONS_PASS
NETWORK_TESTS=NOT_RUN
C2_C3_DISPATCH=PARTIAL_PROFILE_GATE_INTEGRATED_NO_OPERATIONAL_ADMISSION
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NONE_WITHIN_SCOPED_CATALOG_SLICE
NEXT=SCOPED_C2_C3_DISPATCH_COMPLETION
