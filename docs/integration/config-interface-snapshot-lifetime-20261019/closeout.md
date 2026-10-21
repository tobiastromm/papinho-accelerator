# Closeout — PACC-PHASE6-CONFIG-INTERFACE-SNAPSHOT-LIFETIME-1

Lifetime de preparação implementado e validado no owner canônico existente. O participante possui agora uma cópia imutável dos fatos necessários à resolução, sem reter ponteiros dos buffers temporários do produtor. A implementação não publica configuração e não declara resolução operacional CURRENT.

[Prova por função, cobertura e handoff](lifetime-proof-and-codec-handoff.md), [hunks exatos](hunk-review.md), [ED11](ed11.json), [preservação](verification.json), [SHA-256](final-manifest.json).

## Estado factual

A cópia bounded32interfaces/128endereços é embutida no participante, não um catálogo, registry ou owner novo. Validação ocorre antes da escrita; presença completa é publicada por último. Nomes/presentation pointers são descartados. A seleção e os targets da preparação usam somente essa cópia. O caller pode liberar/substituir seus buffers após retorno. Nenhum self-pointer da variável local next é transferido para a root.

Invocation e revisão de preparação vêm dos campos reais do owner/modelo. R inicial0 permanece explicitamente pending, não CURRENT/epoch. Replacement de cópia presente/retida é negado; view exige mesma invocation/revisão e lifetime síncrono do owner. Não existe novo sistema de pins. Na raiz, acesso continua confinado à thread proprietária; Stop é revalidado antes de expor o participante. Rollback existente zera o storage somente depois da quiescência e liberações comprovadas; retenção conserva bytes/crédito e impede view/reuso, inclusive se a retenção da source falhar.

A cópia estabelece ownership, não autenticação da source nem freshness de topologia. Receber um snapshot externo não prova origem nativa. Os testes da preparação usam o produtor canônico real; fixtures TLS são laboratoriais e não autoridade de cliente. SELECTED/RESOLVED na entrada nativa continua negado enquanto faltarem vínculo pós-commit à revisão aplicável e revalidação de CURRENT/pin/source/topologia. Não converter revisão pending em CURRENT ou assumir que um pin de configuração protege buffers externos. Essa pendência permanece no handoff de aplicação transacional, sem criar contrato permissivo.

## Revisões e validação

Operações oficiais aprovadas: sete operações iniciais em operations.json/diffs0–6, suplemento root-test.diff, suplemento root-supplement.diff e dois appends documentais. Preimagens e pós-imagens preservadas; nenhuma negativa ou bypass. Nenhuma decisão accepted alterada. Defaults, scheduler, listener operacional, Serial/issuer, grants e fonte AuthN não modificados.

Debug/Release finais build3 passam /WX, zero warnings e linkage completo do servidor candidato API3 normal e componentes afetados. Build1 e build2 foram preservados; build3 valida o último suplemento de teste. Sem clean, troca de toolchain ou supressão de warnings.

24/24 invocações finais PASS:9CTests+3modos de root por configuração. Native discovery observou4interfaces/8endereços/4PersistentIDs válidos e bindáveis. Fixtures de lifetime testam substituição de buffers, ausência de ponteiros de apresentação, revisão/invocation divergentes, retained, copy excedente/inválida. Modos novos de root real dormente confirmam cópia independente, recusa de thread concorrente não proprietária, capacidade excessiva sem estado parcial, Stop durante pipeline de preparação, rollback normal e retenção factual do graph. Handles da thread diagnóstica fechados com resultado verificado. Nenhuma thread foi adicionada ao produto.

Os modos snapshot retornam antes de prepare_store/commit/publish. Não geraram CURRENT, epoch, CA, certificado ou credencial, não abriram listener/rede ou autenticaram Session. Remoção/renumeração física do SO não executadas: esses negativos permanecem fixtures tipadas. Stop no meio de memcpy não foi injetado; o teste verifica Stop antes da preparação do listener, durante o pipeline já iniciado. Retenção foi explícita, não uma falha nativa de fechamento induzida. Concorrência nos mesmos buffers de input é proibida pelo contrato de ownership; não é mascarada como snapshot seguro. Estes limites estão separados na matriz.

## ED11 e preservação

Cópia5928bytes; root182608 (antes176680), knownmanaged1807096 (antes1801168): delta5928 uma única vez, sem duplicar arrays embutidos. Participante6328bytes, reserva temporária sizeof(next)+4096=10424 com gate contra combined_reserved factual. Preparação nativa observou combined1974123; somada ao temporário1984547, abaixo2097152 no perfil testado. O temporário não é storage persistente extra. Nenhuma quota elevada, heap/worker/pin/sistema de crédito novo. Allowance não é prova do pico transitivo de stack/CRT. Buffers/allocações opacas do produtor ainda exigem contabilidade da composição; provider/kernel UNKNOWN. A cópia retida continua cobrada até cleanup factual. ED11 PARTIAL, sem headroom/freshness operacional aprovado.

HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a.2414arquivos preexistentes inventariados, oito alterados somente por hunks/appends aprovados;2406outros hashes intactos. Dois novos arquivos portáveis sem Win32/CNG, na biblioteca network existente (padrãoC99 existente, sem dependência de toolchain Browser). SDK normal46/46 rehashado. Diff-check PASS, avisos de normalização anteriores preservados no log. Evidências/binários históricos não sobrescritos; nenhuma nova qualificação histórica alegada. Browser/PST/PACC wire, pin produtivo0.6.4 e release PST0.6.5 preservados. Nenhum clone/worktree/commit/push/tag/release.

```ini
CURRENT_TASK_COMPLETED=YES
COMPLETION_SCOPE=ADMINISTRATIVE_PREPARATION_SNAPSHOT_OWNERSHIP_AND_LIFETIME_ONLY
INTERFACE_SNAPSHOT_OWNER=EXISTING_CANONICAL_ROOT_PREPARATION_PARTICIPANT_EMBEDDED_VALUE_COPY
INTERFACE_SNAPSHOT_REVISION=FACTUAL_INVOCATION_AND_PREPARATION_REVISION_INITIAL_ZERO_PENDING_NOT_CURRENT
INTERFACE_SNAPSHOT_LIFETIME=WRITE_ONCE_OWNER_THREAD_COPY_RETAINED_UNTIL_EXISTING_QUIESCENT_CLEANUP
FACTUAL_RESOLUTION=PREPARATION_USES_OWNED_COPY_NATIVE_CURRENT_RESOLVED_STILL_DENIED
AUTH_PROVIDER=PENDING
BUILDS=DEBUG_RELEASE_FINAL_PASS
WARNINGS=0
OFFLINE_TESTS=24_OF_24_FINAL_INVOCATIONS_PASS_NATIVE_AND_FIXTURE_LIMITS_DOCUMENTED
NETWORK_TESTS=NOT_RUN
CONFIG_PUBLICATION=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_OUTSIDE_EXACT_APPROVED_HUNKS_AND_LIVE_APPENDS
EXECUTOR_APPROVALS=ALL_APPROVED_NO_DENIAL
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=POST_COMMIT_CURRENT_AND_NATIVE_TOPOLOGY_SOURCE_BINDING_NOT_ESTABLISHED
NEXT=CANONICAL_CONFIG_CODEC_AND_TRANSACTIONAL_APPLICATION_WITH_CURRENT_AND_TOPOLOGY_BINDING_GATES
```
