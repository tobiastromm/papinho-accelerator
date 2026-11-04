# Closeout — B0–B8 aceita e perfil limitado integrado offline

Task PACC-PHASE6-LIMITED-STAGING-ASSURANCE-ACCEPTANCE. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP anterior preservado, sem commit/reset/clean.

## Decisão registrada antes do código

../../adr/DECISION-personal-limited-staging-assurance-b0-b8-20261031.md incorpora integralmente B0–B8/hashb4b705cc20899a58295c67e95105a6ad3792df4505fe51c0cd68a10d9e8acfbb, conferidos e lidos antes da integração. Candidato PROPOSED preservado byte-exatamente. B2 especializa apenas provisão opaca nesse ensaio; D0–D7, capacidade incremental, W4/F8 e outros contratos mantêm a regra geral fora desse escopo. Não altera LAB/TPM/SCQ ou quotas geridas. Sem obrigação superior contraditória identificada no confronto aplicável; nenhum accepted anterior editado.

## Código integrado

- personal_qualification_staging_win32.h: enum privado STRICT/LIMITED_QUALIFICATION, campo de perfil no mesmo packet e pendência de plano exato B3 (bit128, não número de quota). Sem formato persistido/wire/grant novo. Root/censo gerido permaneceram1.988.704Selected/1.820.136Legacy; campo usa espaço da composição existente sem novo pool.
- staging_prepare_profile valida seletor antes de aquisição e congela-o no packet; wrappers antigos de preparação/observação selecionam explicitamente STRICT. Novo entry privado papacc_server_launcher_qualification_staging_limited usa LIMITED_QUALIFICATION no owner existente; não é CLI, serviço ou endpoint e nenhum caller operacional a executa nesta tarefa.
- staging_policy_missing continua relatando UNKNOWN dos custos opacos, sem converter em PASS. staging_policy_blockers separa a obrigação de provisão integral da especialização B1, SOMENTE no perfil LIMITED. Native/FS opacos não são prova prévia integral nesse perfil; margem/observação/volume/ownership/retidos ainda negam quando ausentes/incertos e B3_EXACT_PLAN é obrigatório, sem fonte positiva atual.
- staging_admit rejeita perfil inválido/produtivo não definido, estado/thread/ownership/consumo/Stop/erro e ausência do plano. Zerar a máscara DATA ou escolher perfil LIMITED não produz grant. O fallback também é DENY: não há caminho operacional positivo por constante ou flag.
- Entry LIMITED chega ao wrapper de admissão antes de consumo/efeitos, reporta o risco e plano ausente, fecha/retém conforme estado e retorna negação. Composição bruta é privada e só exercitada diretamente por testes API-mocked; no produto só é chamada pelo wrapper. Perfil não vira productive após receipt/close.

B7 exige política B3 completa antes de revisar guards conservadores. Portanto124 ainda pode cortar preparação nativa; não foi silenciosamente removido pela aceitação. A integração não afirma resolver a futura medição132 ou tornar entrada operacional pronta. Não se cria setter de plano/capacidade para contornar ausência da fonte original.

## Garantias/riscos

Preservados: fontes/ancestrais/token originais, no reparse/substituição, criação exclusiva/proteção, consumo único, bytes/hash/readback, Stop antes de cada write/flush, quotas controladas, retenção/fechamento UNKNOWN e nenhuma promoção automática. Risco reduzido só no nível de garantia prévia do custo opaco; continuam possíveis exaustão/pressão entre samples, falha nativa, IO bloqueante e objetos parciais persistentes. Não chamar isso capacidade produtiva ou quiescência.

## Qualificação

Selected/Legacy Debug/Release: quatro builds PASS com /W4 /WX, sem warnings MSVC.17 CTests/configuração,68execuções PASS. Logs/preimages/HASHES.json preservados.

Por configuração: staging56cenários/169checks e recursos34casos/126checks. Acrescentados LIMITED correto, STRICT preservado, seletor3 inválido/produtivo negado antes de aquisição, máscara DATA falsa sem capacidade, blockers limitados distintos sem apagar UNKNOWN, entry LIMITED sem efeitos/plano, composição LIMITED com cinco criações/flush em modelo, consumo duplicado e falhas de escrita/zero progresso/hash/flush com retenção. Casos existentes cobrem originais ausentes/trocados, proteção, insuficiência, Stop, concorrência/owner/state e close incerto. Mesmo caminho de originais/cópia/close, sem callback permissivo de autoridade. Nenhuma API mutante real, endpoint ou coleta. SHA/parsing SD/SID em memória reais; políticas positivas/fatos de execução operacional não demonstrados por modelos.

## Pacote/hash finais

PACKAGE-CANDIDATE.json contém três fontes/tamanhos/destinos de planejamento/hashes; total9.094.144bytes. Exe555008bytes/hash1dad7c09571e0b0fafe71f802bd0965d05ccd2466259b2b4d4917ea253ee23f6; DLLs preservadas conforme manifesto. Não há seleção/autoridade operacional. Binárioeee02c93... anterior preservado/conferido em build/staging-observation-preserved-20261031/pre-limited-eee02c93 antes do rebuild. Pins/aprovações históricas não atualizados.

## Estado do pedido e primeiro gate

ONE-LIMITED-STAGING-REQUEST.DRAFT.md delimita efeitos/riscos/gates/procedimento; é NOT_READY_B3_EXACT_PLAN, sem comando ou placeholders executáveis. A aceitação B não selecionou critérios quantitativos, janela/duração ou artefatos/ato operacionais. Inventar thresholds para fazer caber132 infringiria B3.4. O primeiro gate é completar/justificar e selecionar integralmente esse plano B3 no contexto dos owners existentes, com observador/artefato e bindings frescos, sem nova decisão normativa para correções ordinárias. Não basta uma confirmação genérica.

Com B3 qualificado, conferir caminho capaz de adquirir originais/volume sem importar limites de outro domínio e preparar pedido byte-exato único para aprovação operacional. Não repetir coletor consumido ou emitir comando prematuro. Depois do staging autorizado/readback, pedir separadamente SCM_REGISTER_CLOSED parado; não Start/SCQ/CNG/CA/CURRENT/NEW_INSTALL por consequência.

Nenhuma operação protegida executada. Staging DENY/NOT_READY; E21 PENDING; ED11/Phase6 PARTIAL. UNKNOWN, retenção/reservas, HEAD/WIP/históricos/pins/chave fixture e PST0.6.4 preservados.
