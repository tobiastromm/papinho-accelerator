# Future direction — backup pessoal e recuperação de identidade

Nota de estado 2026-11-03: a [preparação inicial limitada FCA](../adr/DECISION-personal-first-instance-preparation-assurance-fca0-fca7-20261103.md) recebeu integração offline do caller/worker/key owner, [qualificada em modelo](../integration/phase6-server-update-after-utility-20261101/FCA-FIRST-PREPARATION-INTEGRATION-REPORT.md). Não implementa backup, recuperação, exportação ou prova P4, nem concede execução real ou assinatura. A disposição MVP/deferida abaixo permanece.

DEFERRED / PROPOSED scope specialization; não implementado. .papaccbak é nome/extensão CANDIDATA, sem versão/bytes/crypto/KDF/encoding/limites/compatibilidade aceitos. Não é .papacc de provisão. Não implementar nesta Phase6 pessoal conforme direção atual.

Normas vigentes: [CA profile](../adr/DECISION-internal-ca-technical-profile-20261018.md)3.5–3.7, [procedimento pessoal accepted](../adr/DECISION-personal-ca-preinitialization-20261022.md), issuer/C2P6 e SDK/provisão. A [especialização MVP PROPOSED](../adr/ADR-CANDIDATE-phase6-personal-mvp.md) pretende adiar obrigação para pessoal não recuperável; até aceitação normas atuais vigoram. Future nunca altera accepted.

Futuro artefato autenticado/criptografado poderá preservar configurações/identidade/estado e SOMENTE material de recuperação permitido pela custódia, com consistência root/history/serial/revogação. KSPnãoexportável não vira chave portátil por chamá-lo backup. Proteção/recuperação/segredos/autenticidade/freshness/rollback/fencing/restore transacional/custos precisam decisão e testes próprios, sem novo gerenciador/criptografia por inferência.

MVP proposto só restaura opções não secretas.conf: semCA/CURRENT/confiança/pareamentos. Perda exige nova identidade e novo pareamento, nunca recuperação prometida. Migração/instituição/MDM/syncbilateral ficamfuturos sem novos formatos aqui. [Scope vivo](../integration/personal-mvp-consolidation-20261022/scope.md) e [roadmap](../integration/personal-mvp-consolidation-20261022/roadmap.md) fazem cross-reference. Nenhum gate ou capacidade implementada por este registro.



## 2026-10-22 - current normative disposition

[The personal MVP M0-M11 decision is ACCEPTED](../adr/DECISION-phase6-personal-mvp-20261022.md) after explicit owner approval. This update controls the earlier paragraphs' historical PROPOSED/pending description only. Full backup, recovery of the old CA, migration, institutional deployment and bilateral deletion sync are deferred capabilities rather than availability prerequisites of the personal non-recoverable MVP, within the decision's exact precedence. Protected custody, serial/CURRENT consistency, internal replay/detectable rollback defenses and all ED11/Model E gates remain mandatory. .papaccbak is still an unspecified candidate name, not an implemented or compatible format. No key/account/service/backup/restore or bootstrap occurred. Authoritative recovery remains DENY. [Current acceptance closeout](../integration/personal-mvp-m0-m11-acceptance-20261022/closeout.md); historical proposal/scope preserved.

## Estado factual — 2026-11-04
A preparação limitada FCA foi executada manualmente e reconciliada com diretório e chave MACHINE CA criados e fechamento conhecido. Ver [reconciliação](../integration/phase6-server-update-after-utility-20261101/FCA-FIRST-PREPARATION-SUCCESS-RECONCILIATION-REPORT.md). Não qualifica backup/restauração, assinatura/CURRENT, isolamentoP4/K5 ou autoridade recuperável; o escopo diferidoMVP e as demais restrições acima permanecem.

2026-11-04: [FCR0–FCR7 accepted](../adr/DECISION-personal-prepared-instance-validation-fcr0-fcr7-20261104.md) delimita a validação original read-only ATUAL do par FCA preparado. [Integração offline](../integration/phase6-server-update-after-utility-20261101/FCR-INTEGRATION-AND-UPDATE-REPORT.md) qualificada; operação real pendente. Não é recovery/clone/restore, não restaura grant ou inventa public digest histórico, não habilita assinatura/CURRENT/Start. O escopo de backup/recovery diferido M0–M11 permanece intacto.
