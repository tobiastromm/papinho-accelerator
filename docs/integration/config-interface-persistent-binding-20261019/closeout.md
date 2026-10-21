# Closeout — binding decimal de PersistentID

A aprovação do owner foi registrada normativamente e a parte portável autorizada foi implementada após revisão oficial. O provider nomeado continua pendente. A tarefa geral continua parcial: SELECTED não obtém RESOLVED na entrada nativa enquanto a proveniência/revisão/lifetime do snapshot administrativo não estiverem comprovadas.

[Decisão accepted](../../adr/DECISION-papacc-conf-interface-ref-persistent-id-20261019.md), [prova/hunks/handoff](safety-proof-and-handoff.md), [ED11](ed11.json), [preservação](verification.json), [hashes](final-manifest.json).

## Implementado

Nova entrada portável pc_bind_existing_models_snapshot compara interface_ref decimal com a seleção administrativa existente e reutiliza os resolvers canônicos de PersistentID/instância/endpoints. Aceita zero e uint64 máximo com presença factual; rejeita léxico inválido, overflow, identidade ausente/alterada/ambígua, divergência do modelo, duplicação numérica e capacidade insuficiente. Mantém ordem e saída atômica: erro limpa todo output, sem truncar listeners ou endpoints. Limites internos oito IDs/dois endpoints permanecem bounded e falham explicitamente. Nenhum alias, catálogo, owner, registry, grant ou identificador derivado.

O core só produz MODEL_BOUND, nunca resolved=1. Snapshot é empréstimo do caller; não há flags inventadas de proveniência. O wrapper compatível continua passando NULL, portanto SELECTED é negado inclusive sob uma root real até existir a fronteira administrativa correta. Guards CURRENT/source/thread/Stop, Sessions/pins/reaper, provider e permissões não foram alterados. A descoberta temporária de network_start não é storage pinado reutilizável depois de sua liberação.

## Compilado e validado

Debug/Release finais build3 passaram com linkage completo e zero warnings (/WX) para servidor API3 normal e componentes afetados. Build1 falhou C4996 nos sprintf de teste; build2 falhou C4459 no parâmetro que ocultava global; correções limitadas às fixtures aprovadas oficialmente, sem supressão ou troca de toolchain. Todos os logs preservados.

Oito CTests por configuração,16/16 PASS. Bridge agora54 verificações por execução, incluindo asserts auxiliares; não54 cenários independentes. Novo binding positivo é sintético, com modelos e snapshots de teste. Testes confirmam máximo uint64/zero/zeros iniciais, overflow/sinal/alias textual inválido, duplicação por valor, mudança/ausência/ambiguidade de interface, troca de instância, modelo divergente, ausência de endereço e terceiro endpoint negado. Parser, autenticação sintética, resolvers existentes, layout e native guard também passaram. Nenhuma descoberta nativa de interfaces nem resolução SELECTED positiva sob CURRENT real executada; Stop/stale/retenção sob pin real continuam lacunas, não cobertas pelos falsos owners de testes. Não executar uma publicação como atalho para preencher a lacuna.

## ED11 e preservação

Output1072, listener248, AST59880, config920, root176680 e knownmanaged1801168 permanecem inalterados no censo compilado. Helper adiciona somente scratch automático bounded (IDs128 + instâncias32 embutidos na frame); disassembly Debug registra bridge160/helper528/decimal32/wrapper48 bytes de reservas locais de stack. Isso não é pico transitivo ou medição operacional. Allowance síncrono existente16384 e guard85560 não aumentados; nenhuma alocação persistente, heap, thread, pin, callback, reserva ou crédito adicionado. Snapshot caller-owned não é criado neste patch. Futuro produtor administrativo terá de demonstrar orçamento/lifetime/retenção próprios antes do caminho SELECTED nativo. Provider/kernel UNKNOWN e ED11 PARTIAL.

WIP anterior preservado fora dos três arquivos exatos e dois appends factuais;2344 outros arquivos preexistentes intactos, SDK46/46. HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Diff-check exit0. Norma nova criada com ausência prévia verificada, proposta original preservada; hashes pós-gravação em manifesto. Todas operações protegidas desta tarefa aprovadas, nenhuma negativa contornada. Browser, PST, wire, pin0.6.4 e evidências históricas não alterados. Sem rede, listener, CA, credenciais, CURRENT/A/P, admissão, clean, commit/push/tag/release ou worktree.

```ini
CURRENT_TASK_COMPLETED=NO
SCOPED_PORTABLE_BINDING_COMPLETED=YES
INTERFACE_REFERENCE_AUTHORITY=ACCEPTED_DECIMAL_UINT64_UNDER_ADR0007
AUTH_PROVIDER_AUTHORITY=PENDING_NAMED_PROVIDER_REJECTED
FACTUAL_RESOLUTION=PORTABLE_MODEL_BOUND_SELECTED_IMPLEMENTED_NATIVE_SELECTED_DENIED_PENDING_PROVENANCE
CURRENT_PIN=EXISTING_GUARDS_UNCHANGED
CODEC_LAYOUT=PROPOSED
NORMATIVE_EXECUTOR_REVIEW=APPROVED
EXACT_PATCH_EXECUTOR_REVIEW=APPROVED
BUILDS=DEBUG_RELEASE_FINAL_PASS
WARNINGS=0_FINAL_PRIOR_FAILURES_PRESERVED
OFFLINE_TESTS=16_OF_16_INVOCATIONS_PASS_SYNTHETIC_LIMITS_DOCUMENTED
NETWORK_TESTS=NOT_RUN
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_EXCEPT_EXACT_APPROVED_HUNKS_AND_LIVE_APPENDS
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=NATIVE_ADMINISTRATIVE_DISCOVERY_SNAPSHOT_PROVENANCE_REVISION_AND_LIFETIME_NOT_ESTABLISHED
NEXT=PROVE_NATIVE_ADMINISTRATIVE_SNAPSHOT_BOUNDARY_KEEP_NAMED_PROVIDER_PENDING_AND_CODEC_PROPOSED
```
