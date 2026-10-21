---
status: accepted
decision-date: 2026-10-18
scope: previously authorized Phase6 canonical consolidation only
decision-makers: [Tobias Tromm]
---
# Decisão do owner — consolidação canônica da Phase 6

Autoridade explícita: PACC-PHASE6-CANONICAL-CONSOLIDATION-GOVERNANCE-ACCEPTANCE-1, OWNER_DECISION=EXPLICITLY_APPROVED, NORMATIVE_STATUS=ACCEPTED. A decisão dá continuidade à tarefa PACC-PHASE6-CANONICAL-FULL-CONSOLIDATION-AND-INTEGRATION-1.

1. C:\Projetos\PapinhoAccelerator é o destino oficial de integração e builds de produto do Accelerator.
2. Árvores históricas, _tmp e diretórios isolados de Phase 6 são referências e fontes de evidência, não novos repositórios principais de desenvolvimento.
3. As implementações existentes de Model E, C2/C3, C7/E10, C8, E5/E11, ED08 e ED11 devem ser reconciliadas e integradas seletivamente ao canônico, preservando contratos accepted, ownership, segurança e evidências.
4. O listener Secure existente deve ser reutilizado. Não criar outro listener, outro owner ou autoridade paralela sem necessidade demonstrada e autorização própria.
5. Preservar produção PST 0.6.4 e manter PST 0.6.5 API3/SPI4 como candidato normal, separado de R2 TEST_ONLY.
6. Não alterar PACC wire, não fabricar epoch, Principal, grants, policy CURRENT ou evidência ED11.
7. Manter admissão DENY até que os gates operacionais aplicáveis sejam aprovados.
8. Preservar WIP, preimagens, hashes, SDKs, binários históricos e qualificações anteriores em seus escopos.
9. A integração canônica não concede automaticamente qualificação C8, E10, ED11 ou dos modos A–D.
10. PST 0.6.5 somente será publicado após o encerramento integral da Phase 6.

Esta decisão não substitui nem revoga ADRs accepted anteriores, não autoriza exceções de segurança e não altera os critérios de conformidade existentes. Não converte ADR candidates históricos em accepted nem amplia qualificações diagnósticas anteriores.

## Compatibilidade e limites

A consolidação permanece restringida pelos contratos accepted aplicáveis, incluindo ownership e pre-close factual ADR-0003/0030/0031/0032, Model E da ADR-0033 histórica e PST API3/SPI4, C2 accepted, ADR-0036/ED11 e ADR-0044/0048 para commit, policy e epoch. Não há nova semântica de wire, autoridade operacional ou exceção de recursos.

Após aprovação oficial e registro, retomar integração real de fontes, builds e testes offline no canônico. Não executar rede, alterar Browser/PST, promover versões ou realizar commit/push/tag/release. Escritas que exigirem escalonamento continuam sujeitas ao mecanismo oficial do executor; negativa não pode ser contornada. Evidência de fonte, build, teste offline, execução nativa e qualificação conjunta deve permanecer separada.