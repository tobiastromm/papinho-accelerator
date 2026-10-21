# Closeout — revisão/aplicação exata C2/C3, 2026-10-18

A nova revisão oficial aprovou8 hunks funcionais em3 arquivos e, separadamente,4 hunks de regressão em2 testes existentes. O patch foi aplicado com verificação integral de HEAD, SHA256 das preimagens, offsets/conteúdo old e SHA256 proposto antes de qualquer escrita. Debug/Release API3 normal compilam e linkam sem warnings finais;22 execuções offline passaram.

A negativa anterior permanece válida para a operação ampla rejeitada. Esta tarefa tem nova autorização explícita e patch menor/exato; não reaplicou substituições globais nem mudou ferramenta/SDK para contornar a negativa.

## Autoridade e proveniência
HEAD main c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Governança AGENTS e decisões accepted de consolidação e Site TLS; C2§7/§11, ADR-0044/0048, Model E e ED08/ED11. Nenhuma decisão accepted foi editada. Headers reais do SDK normal API3/SPI4 determinam nomes, tamanhos e enums; nenhuma função equivalente foi presumida.

[Patch](exact-patch.diff), [manifest funcional](exact-patch.json), [manifest dos testes](regression-patch.json), [matriz por função](hunk-matrix.md). Os arquivos .proposed são artefatos de revisão dentro do canônico, não outra árvore ou configuração executável.

Preparação teve dois erros locais do script (ordenação de dicionários e alias PowerShell H); ambos ocorreram antes de qualquer escrita de fonte. As três preimagens continuaram byte-exatas. Não houve nova negativa do executor nesta tarefa.

## Correções aplicadas
- PST01: substituiu o símbolo inexistente por memset e preenchimento explícito dos cabeçalhos struct_size/api_version aninhados, provider exato já mantido pelo owner e capabilities CLIENT/SYSTEM/name/ALPN/SNI/graceful reais. Sem novo símbolo PST, stub ou alteração SDK.
- GRACE01/02: produtor e guard exigem PST_FEATURE_REQUIRED2; boolean1 não é REQUIRED. Nenhuma ordem de shutdown/release mudou.
- ALPN01: verifica n>=256 antes de subtrair o espaço restante, evitando underflow unsigned. A ponte C3 existente continua exigindo fatos públicos PST/peer e protocolo negociado pertencente às ofertas; não se presume negociação por ter ofertas.
- DEADLINE01/02: a revalidação do deadline exige owner, Session/grants/root CURRENT e payload válido, sem contar novamente o próprio slot RESERVED. Mantém cálculo checked now+max_duration, atribuição inicial única e verificação de expiração no caminho live. Não altera idle/scheduling nem renova prazo por progresso.
- SITE01/02: sucesso de preparo requer trust não nulo. Após apply dormente, catálogo presente exige SYSTEM trust factual e pool C3 vazio pertencente à root antes de preparar store/commit/publicação. Falhas incertas mantêm referência/retenção; não há retry.

As checks de Principal16, grants publicados, policy CURRENT, revisão e epoch foram preservadas. Gate operacional existente continua DENY; não se criou fonte paralela de autorização ou autoridade de observação.

## Builds
SDK normal governado, x64/MSVC19.51, /MD, targets candidate API3/SPI4. Diretórios novos ExactCorrectionDebug e ExactCorrectionRelease, receitas preservadas. Produto e testes focais linkados, zero warnings/erros nos logs finais. O build anterior falho está preservado em canonical-c2-c3-dispatch-20261018/Debug-build1.log; não foi reclassificado como PASS.

Não houve clean, modificação de flags para ocultar warnings, mistura R2, execução do produto ou rede.

## Regressões e limites da prova
[22 resultados](offline-results.json):11 por configuração. Transações locais reais: abort04, commit04, retenção04, catálogo05, colisão CREATE_NEW05 e retenção05. Demais: catálogo/ALPN/codec puro, censo sizeof, manager C2 vazio, ownership sintético e Stop/retention com registros sintéticos.

| Requisito | Evidência final | Ainda não comprovado |
|---|---|---|
|Config PST|Fonte contra headers reais e linkage completo|Config CLIENT consumida por handshake real|
|ALPN|Allow/mismatch/truncado/vazio/oversize offline; bound corrigido; verificador negociado preservado|Negociação TLS real e negativos de peer|
|Deadline|Sem Session/owner não emite prazo; atribuição/overflow/expiração auditados em fonte|Expiração de operação autenticada sob tráfego|
|Site TLS|Preparo SYSTEM local; remoção temporária da referência real nega store antes de efeitos; commit/CURRENT/epoch reais locais|Site TLS operacional|
|Grants/autorização|Manager vazio/Session inválida nega; root/policy/epoch negativos preservados|Principal autenticado e grants reais de rede|
|Graceful/ED08|Enums corrigidos; ordem de fences/release intacta na ponte|close_notify e ordem nativa sob tráfego|
|Stop/Model E|Entry points reais com registros sintéticos retêm; root com referência pendente não é reutilizada|Exactly-once nativo sob falhas de rede/PST|

No teste de ausência de trust, a referência real permanece em variável local, nenhuma chamada PST/cleanup/callback ocorre no intervalo, e a root é restaurada antes de assert. Isso invalida uma precondição, não fabrica readiness ou destruição.

As fontes da ponte C3 permanecem intactas (hash em preservation.json). KNOWN_TRUE de certificado/chain/nome/auth e TLS1.3 continuam obrigatórios; GetNegotiatedALPN deve fornecer protocolo efetivamente negociado compatível. Overflow/UNKNOWN não viram sucesso. Nenhum token/credential/payload sensível foi registrado.

## ED11
Censo compilado efetivo:
- layout gerenciado declarado1539208;
- root31056, participante528944, owner C2273928, bridge C3132448;
- layout fixo1232552, arrays base276992, Secure25088, scheduler4432, bind144;
- reserva combinada offline1796915, limite2097152, margem300237.

Os16 bytes adicionais em relação à última composição qualificada de catálogo pertencem ao deadline absoluto de duas operações, introduzido no WIP anterior e agora compilado. O patch exato não adiciona arrays, heap ou quotas. Pools C3 já pertenciam ao censo e não foram cobrados novamente. Fonte/template por valor e cópias locais permanecem sob créditos existentes.

Provider/kernel, allocator e stack interna de bibliotecas continuam UNKNOWN; não se declara RAM/stack/concorrência operacional qualificada. Retenção mantém quota e dependências; não há liberação por timeout ou process exit.

## Preservação e próximo gate
SDK46/46 checksums,37 corpos API2 e seis binários históricos verificados. Preservados14 binários do slice de catálogo e quatro produtos anteriores; pin0.6.4 intacto. Dois mapas históricos permanecem NOT_RECOVERED. Sem alterações PST/Browser/PACC wire, Git publication, clones ou worktrees.

A tarefa de correção exata está concluída. Isso não conclui qualificação operacional do dispatch. Próximo gate: revisão focal dos hunks/ownership e evidência ED11 aplicável antes de qualquer autorização separada para Mode C HTTPS conjunto. Não reabrir admissão com base apenas nestes testes.

CURRENT_TASK_COMPLETED=YES
EXACT_PATCH_PREPARED=8_FUNCTIONAL_HUNKS_3_FILES
OFFICIAL_EXECUTOR_REVIEW=APPROVED
PATCH_APPLIED=EXACT_HASH_VERIFIED
PST_CONFIG_INITIALIZATION=REAL_API3_STRUCTS_NO_NEW_PST_SYMBOL
ALPN_GUARD=BOUNDS_FIXED_NEGOTIATED_VERIFIER_PRESERVED
DEADLINE_GUARD=REVALIDATED_WITHOUT_DOUBLE_COUNT_ABSOLUTE_EXPIRY_PRESERVED
SITE_TLS_PREPARATION=FACTUAL_TRUST_AND_POOL_GATED_OFFLINE_VALIDATED
GRACEFUL_SHUTDOWN=REQUIRED_ENUM_FIXED_NATIVE_PROOF_PENDING
DEBUG_BUILD=PASS
RELEASE_BUILD=PASS
WARNINGS=0_FINAL
OFFLINE_TESTS=22_OF_22_EXECUTIONS_PASS
ED11_RESERVATION=1796915_BYTES
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
NETWORK_TESTS=NOT_RUN
PRODUCTION_064=UNCHANGED
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
DIFF_CHECK=PASS
FIRST_UNRESOLVED_DIVERGENCE=NONE_WITHIN_EXACT_CORRECTION_SCOPE
NEXT=FOCUSED_REVIEW_AND_SCOPED_ED11_TARGET_EVIDENCE_BEFORE_JOINT_RUNTIME
