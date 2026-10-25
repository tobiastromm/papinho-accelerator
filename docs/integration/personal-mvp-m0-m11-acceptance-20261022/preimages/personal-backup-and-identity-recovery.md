# Future direction — backup pessoal e recuperação de identidade

DEFERRED / PROPOSED scope specialization; não implementado. .papaccbak é nome/extensão CANDIDATA, sem versão/bytes/crypto/KDF/encoding/limites/compatibilidade aceitos. Não é .papacc de provisão. Não implementar nesta Phase6 pessoal conforme direção atual.

Normas vigentes: [CA profile](../adr/DECISION-internal-ca-technical-profile-20261018.md)3.5–3.7, [procedimento pessoal accepted](../adr/DECISION-personal-ca-preinitialization-20261022.md), issuer/C2P6 e SDK/provisão. A [especialização MVP PROPOSED](../adr/ADR-CANDIDATE-phase6-personal-mvp.md) pretende adiar obrigação para pessoal não recuperável; até aceitação normas atuais vigoram. Future nunca altera accepted.

Futuro artefato autenticado/criptografado poderá preservar configurações/identidade/estado e SOMENTE material de recuperação permitido pela custódia, com consistência root/history/serial/revogação. KSPnãoexportável não vira chave portátil por chamá-lo backup. Proteção/recuperação/segredos/autenticidade/freshness/rollback/fencing/restore transacional/custos precisam decisão e testes próprios, sem novo gerenciador/criptografia por inferência.

MVP proposto só restaura opções não secretas.conf: semCA/CURRENT/confiança/pareamentos. Perda exige nova identidade e novo pareamento, nunca recuperação prometida. Migração/instituição/MDM/syncbilateral ficamfuturos sem novos formatos aqui. [Scope vivo](../integration/personal-mvp-consolidation-20261022/scope.md) e [roadmap](../integration/personal-mvp-consolidation-20261022/roadmap.md) fazem cross-reference. Nenhum gate ou capacidade implementada por este registro.

