# A1 — integração funcional de reserva de ownership

A correção A1 foi integrada ao qualificador existente e qualificada offline. O helper privado conserva exatamente dois valores HANDLE. Não cria owner de admissão, mutex, registry, autorização ou créditos. O owner canônico, o consumo único, B1 e os limites ED11 permanecem.

## Fonte e escopo

HEAD/main: `913c90cf2328f7939d8f5b927640601903ac6e91`. Preimagens e baseline preservam o WIP anterior. O candidato anterior foi conferido pelo SHA256 `fbc9850e96692ebd981d49d1e664d569e744d077840789e0700d51c3daff4d04` e aplicado considerando os fins de linha existentes. Nenhum documento ACCEPTED foi alterado. As decisões Model E, ED11/B0–B6, N0–N9, L0–L9 e H0–H9 continuam restringindo a implementação. A direção futura C2 não muda por esta correção privada.

Os dez callers estão em `caller-coverage.csv`. As entradas indiretas estão em `indirect-boundaries.csv`; também foram protegidas as consultas de ausência relativas ao pai. O guard no adaptador canônico é compilado somente quando o qualificador define PAPACC_AUX_RESERVATION_GUARD; não introduz dependência no caminho normal do serviço.

## Ownership e falhas

FREE → RESERVED precede toda aquisição auxiliar. A API recebe diretamente o endereço persistente do slot; o caller recebe apenas lease com índice e geração e borrow limitado ao uso. Pares de console reservam os dois slots antes da primeira abertura. Uma terceira reserva é negada antes da API. Nenhum terceiro auxiliar obrigatório foi encontrado nos caminhos síncronos revistos.

BUSY permanece durante aquisição, uso emprestado e close. Reentrada de aquisição ou close é negada. A afinidade usa o thread já existente no owner; nenhuma nova thread ou lock foi criada. O encerramento de borrow é obrigatório antes de close. Gerações inválidas e reutilizadas são rejeitadas.

CloseHandle incerto mantém o mesmo valor no slot UNKNOWN, preserva o primeiro erro e o primeiro erro de close separadamente, congela aquisições e sinaliza Stop/retention. O outro slot continua rastreável, inclusive se OWNED. Não há segundo close, retry, descarte ou conversão para CLOSED. As rotas de cleanup retêm dependências e exclusividade. O encerramento do main de registro também estaciona antes de liberar dependências se houver retenção auxiliar.

O loop instrumentado mantém a oportunidade de um Stop já autorizado, somente quando SERVICE_ACCEPT_STOP e o estado factual permitirem; freeze não equivale a Stop aceito/concluído ou quiescência. Depois disso, segue retenção, sem novos censos, tokens, PSS ou extração. Não foram executados controles SCM para validar esse comportamento: sua evidência aqui é de fonte e fixtures.

## Qualificação

Selected/Legacy × Debug/Release: builds /W4 /WX sem warnings. Compilados os três executáveis de qualificação (registro, ciclo e instrumentado), sem executá-los. Testes executados: 56 invocações, todas exit 0, em `test-results.json`. São 14 executáveis por configuração, incluindo os modelos de BD, política ED11, runtime fechado/arquivo, ciclo, perfil default/opt-in e RAW1 em --model.

O núcleo funcional, e não uma cópia anterior do modelo, passou os 89 casos dirigidos e 271.453 nós de exploração finita (profundidade 5, alfabeto 12) em cada configuração. A exploração finita não prova todos os programas possíveis. Quatro mutantes foram detectados: perda do valor incerto, aquisição ignorando freeze, sobrescrita do primeiro erro e borrow de par sem BUSY durante uso.

Os corpos C reais dos callers são incluídos nos dois testes de API simulada. Por configuração, 74 verificações na variante instrumentada e 35 na variante de registro. Cobrem confirmação correta/divergente, primeira/segunda falha de close, falha de segunda aquisição, saturação, Stop, reentrada durante leitura, lease inválido, thread divergente, extração válida e negada, identidade de token negada, cycle_live com ausência de impersonation e com token inesperado/close incerto, e negação das entradas indiretas após freeze. Os falsos valores e reset de storage existem somente nos testes. O entry operacional é renomeado e nunca chamado. Leituras de módulo, token, endpoints e memória impressas por esse fixture são respostas simuladas, não observações do host/LAB.

Os testes de política e BD preservam replay, tentativa histórica, identidade/RAW1/token divergentes, freshness, limites 128/1024 e retenção. Nenhuma execução positiva ou preflight ocorreu. Não há alegação de qualificação factual nativa dos lifetimes do produto.

## Storage e ED11

Layout x64 medido: A1_OWNER=48 bytes; agregado equivalente anterior=24; acréscimo=24 bytes de metadata/alinhamento, com exatamente dois HANDLEs. Lease=16, par=32 mais flag/local, LAB_AUX_REQUEST=40. Esses valores não representam custo total de memória, stack commit ou pico.

`compiled-frames.json` deriva das listagens /FAs do próprio qualificador nas quatro configurações. Exemplos iguais em Selected e Legacy: lab_operator_current subtrai 376 bytes de RSP em Debug e 200 em Release; cycle_live 18.536/18.136; ig_confirm 744/384; ig_extract 1.448/1.248. Pushes de registradores, retorno, callees, inlining, pilhas Windows, cache/kernel e custos transitivos não são cobertos por esses números. `storage-ledger.csv` registra o agregado e os frames sem créditos. Não se afirma que +24 seja o delta total de RAM ou de pilha.

Limites ED11 128 nativos, 1.024 administrativos, quatro papéis, 60s/1s, headroom e NONE_GRANTED não mudaram. UNKNOWN históricos e reservas reais continuam retidos. Nenhuma economia de handles estáveis foi medida ou reivindicada.

## Próximo experimento isolado

Preparar fixture nativo sem efeitos LAB para lifetimes positivos, usando a mesma sequência de ownership e fixtures de arquivo/processo isoladas. Antes de executá-lo, fixar ferramentas, objetos, observações por fase e alcance de Stop/fechamento. Medir handles próprios versus overhead de observação, simultaneidade e lifetimes; manter o limite128 e distinguir números medidos de custos UNKNOWN. Não há comando operacional positivo neste pacote. Qualquer execução LAB e atualização de pins exige revisão e autorização separadas.

CURRENT_TASK_COMPLETED=YES
A1_FUNCTIONAL_IMPLEMENTATION=OFFLINE_QUALIFIED
CALLER_COVERAGE=10_SOURCE_AND_API_SIMULATED
OWNERSHIP=TWO_SLOTS_RESERVED_BEFORE_ACQUISITION
REENTRANCY=DENY_WHILE_BUSY_CONTROL_THREAD_ONLY
STOP_RETENTION=PRESERVED_SOURCE_AND_FIXTURE_NOT_NATIVE_LAB_PROOF
STORAGE_COST=48_AGGREGATE_PLUS24_METADATA_FRAMES_SEPARATELY_MEASURED
BUILDS=4_CONFIGURATIONS_3_QUALIFIERS_AND_TESTS_PASS
WARNINGS=0
OFFLINE_TESTS=56_PASS_89_CASES_AND271453_NODES_PER_CONFIGURATION_4_MUTANTS_DETECTED
NATIVE_TESTS=API_SIMULATED_NO_FACTUAL_PRODUCT_LIFETIME_QUALIFICATION
NETWORK_TESTS=NOT_RUN
START_EXECUTED=NO
STOP_EXECUTED=NO
PREFLIGHT_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_10215_BASELINE_FILES_COMPARED_ONLY_SCOPED_SOURCE_AND_LIVE_DOC_CHANGES
FIRST_UNRESOLVED_DIVERGENCE=NO_OFFLINE_FAILURE_NATIVE_POSITIVE_LIFETIMES_UNQUALIFIED
NEXT=NATIVE_C_POSITIVE_LIFETIME_ISOLATED_QUALIFICATION
