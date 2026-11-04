# Proposta focal — envelope próprio da preparação MACHINE TEST_ONLY

Status: PROPOSED / NOT_ACCEPTED / NO_OPERATIONAL_AUTHORITY.
Data: 2026-11-03. Decisão solicitada: alternativa B abaixo, integralmente.

## Evidência e limite da conclusão

A sessão fc91b7bce523b014c28ce677b60ea889e32c8bc50c39c83b264096d6f923ab5c criou e fechou os dois documentos, mas o produtor negou a fixture em source_stage=1, first=1816, consumed=0, created=0, close_known=1. Não chegou a documentos/cadeia/RNG no produtor nativo. Não repetir essa sessão.

Em server_ca_key_win32.c, papacc_ca_key_fixture_budget_begin propaga falha de personal_resource_collect ou rejeita handles>=124, físico<7239172096 ou commit<9823326208. A saída não exportou a amostra nem a etapa da consulta. Portanto a causa histórica individual permanece UNKNOWN. Não há vazamento, insuficiência específica ou defeito funcional demonstrado. O código corresponde a T2 accepted; o wrapper apenas propagou a falha.

T0–T6 accepted, incorporado por DECISION-personal-machine-fixture-preparation-assurance-t0-t6-20261103.md, manteve explicitamente esses limites. K7 de DECISION-personal-scq-read-machine-k0-k9-20261103.md exclui herança automática pela preparação. Esta proposta pede uma escolha própria, não uma interpretação retroativa nem correção de erro de implementação.

## Alternativas

A — manter T2 integralmente: preparação continua sujeita aos limites atuais e permanece sem nova operação autorizada. Não coletar repetidamente recursos nem prometer admissão futura.

B — recomendada: selecionar explicitamente o envelope F0–F5 abaixo para uma futura preparação limitada. Valores de handles e commit coincidem com escolhas diagnósticas existentes para facilitar avaliação do risco pelo proprietário, mas sua aplicabilidade à fixture depende DESTA aprovação. Não há evidência de suficiência universal de CNG, nem garantia de que a próxima preparação passará.

## F0 — Escopo exclusivo

Somente produtor existente PERSONAL_LOCAL MACHINE_TEST_ONLY/K1, Selected, para criar exclusivamente uma fixture MS Software KSP MACHINE/RSA2048/export0/signing-only e executar seu controle administrativo público positivo. Não amplia READ, TRANSPORT_ONLY, NEW_INSTALL, UPDATE, CA, issuer, produção ou outro perfil. Nenhum novo owner, serviço, diretório, transporte ou mecanismo de autoridade.

## F1 — Seleção quantitativa própria

Substituir somente os números de handles e commit de T2 para esse produtor:

| Parâmetro | Valor proposto |
| --- | ---: |
| Contagem inicial máxima | 128 handles |
| Guarda antes de aquisição/chamada protegida | rejeitar contagem >=188 |
| Máximo por participante | 192 handles |
| Commit disponível mínimo | 3221225472 bytes (3 GiB) |
| Memória física disponível mínima, preservada | 7239172096 bytes |
| Crescimento privado, preservado | 1994944 bytes |
| Crescimento working set, preservado | 1994944 bytes |
| Freshness, preservada | 100 ms |
| Prazo global, preservado | 60000 ms sem renovação |

Entrada 128 é admissível quanto a esse predicado; 129 nega. Pré-aquisição 187 é admissível quanto a esse predicado; 188 nega. Cap192 não permite adquirir quando a guarda188 já nega. Contabilizar todos os handles, inclusive internos/herdados; nenhum desconto. A diferença até192 é margem escolhida, não orçamento nativo comprovado. Não transportar a tolerância working3145728 da READ para esta preparação.

Manter baseline original, consultas antes das chamadas dependentes, overflow/erro/indisponibilidade/Stop/expiração como DENY. Sem fallback, renovação, rebase ou amostra fabricada. Não converter aprovação de risco em suficiência nativa. Preservar Selected K3071004/quota3145728/margem74724, reserva conhecida1048576; Legacy K2902436>2097152 continua DENY. Nenhum aumento da quota ED11 em bytes.

## F2 — Assurance e primeira causa

T1 continua limitado aos custos opacos explicitamente aceitos: persistem UNKNOWN, não zero. A nova seleção aceita maior exposição de handles e menor reserva de commit; inclui risco de exaustão, latência síncrona, persistência parcial e fechamento incerto. Não dispensa identidade/proteção/proveniência/continuidade, censo controlado ou gates de host.

A implementação pode exportar a primeira causa e campos já capturados pelo owner, distinguindo falha de consulta de rejeição quantitativa, sem consultas adicionais, segredos ou referências sensíveis. Isso só explica uma futura tentativa; não resolve retroativamente o1816 histórico. Não instalar imagem intermediária só para instrumentação.

## F3 — Originais existentes e decisão futura

Preservar sem alteração os dois originais existentes: pedido417614100:6815744:9313/SHAfc91b7bce523b014c28ce677b60ea889e32c8bc50c39c83b264096d6f923ab5c e decisão417614100:37617664:12786/SHA6ee864d0ec398d8fbd966822b71ea329bf908d1af2bc589f1298c22368f8a9bf. Sua autorização anterior continua utilizada e não reutilizável.

Para uma eventual nova operação F, permitir reutilizar o pedido antigo exclusivamente como original DATA/proveniência de escopo, acompanhado de NOVA decisão operacional efetivamente emitida que vincule explicitamente esse pedido, esta especialização, a nova imagem qualificada e a nova tentativa. A decisão antiga não concede autoridade. A futura decisão poderá ter uma única cópia auxiliar exclusiva no mesmo parent protegido, pelo MESMO mecanismo documental, sem recriar o par histórico. Essa exceção focal à exigência de dois documentos novos de T3/T4 não autoriza cópia agora.

Antes de pedido operacional, qualificar esse binding no owner/caller existentes: aquisição original retida dos dois arquivos selecionados, identidade/hash/tamanho/SD, operador legítimo, owner/DACL protegida com as três ACEs operador/SY/BA, ancestrais e ausência de reparse. Nenhuma string/hash/cópia é autoridade. Separar confirmação/consumo da cópia nova e confirmação/consumo F. A nova decisão real e seu caminho/hash serão definidos no pedido correspondente; não inventar aprovação, identidade futura, keyname ou publicSHA.

## F4 — Integração offline e qualificação

Aceitação autoriza somente registro normativo designado seguido de implementação e qualificação offline. Preservar accepted e proposta anteriores imutáveis, com cross-reference da especialização. Isolar a seleção legítima do produtor fixture; ausência/divergência nega, sem mudança em outros perfis ou primitivas genéricas.

Qualificar fronteiras128/129,187/188,192/193, commit imediatamente abaixo/igual/acima3GiB, erro/zero/overflow/Stop/prazo/freshness, ausência de seleção, replay, decisão antiga/reutilizada/substituída, proteção/reparse/continuidade, criação exclusiva/colisão/falha parcial/controle positivo/closeUNKNOWN. Preservar contagem completa e retenção. Reutilizar campanhas inalteradas; quatro builds/regressões afetadas se houver delta nativo. Nenhuma consulta ou operação real.

## F5 — Gates posteriores e efeitos proibidos

Depois de qualificação, congelar tamanho/SHA/delta da imagem. Se necessário, preparar UPDATE U1–U7/U2 pelo updater protegido existente e pedir autorização operacional específica. Depois, preparar UM novo pedido de preparação F, com eventual cópia auxiliar única explicitamente delimitada, hashes/identidades reais e gates frescos. Nenhum comando operacional é autorizado por esta proposta.

Não repetir D/F anterior, criar chave/cópia real, coletar recursos, UPDATE, endpoint/SCQ/P/StartStop, alterar SCM/ACL existentes, CA/CURRENT, rede, TPM/NV/Q4, cleanup/refund/kill ou produção. Não há rollback automático. Preservar documentos/chaves eventualmente parciais, EXEs anteriores, DLLs, HEAD/WIP, pins/históricos/consumos, application-close/retenção/reservas/quiescênciaOS UNKNOWN e PST0.6.4. A aceitação não garante que a fixture ou o Start serão admissíveis.

## Decisão solicitada

Aprovar exclusivamente B/F0–F5 como especialização normativa e integração OFFLINE, ou manter A. A aprovação desta proposta não é autorização de UPDATE, cópia documental ou criação de chave.
