---
status: accepted
date: 2026-11-01
scope: one PERSONAL_LOCAL BINARY_UPDATE_CLOSED limited qualification attempt
supersedes: []
---
# Decisão — UPDATE binário limitado U1–U7

O proprietário aprovou explicitamente a alternativa B e as regras indivisíveis
U1–U7 na task PACC-PHASE6-U1-U7-ACCEPTANCE-AND-OFFLINE-UPDATE. Conferência
integral byte-exata da proposta preservada:
`docs/integration/phase6-staged-binary-update-20261101/UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md`,
SHA256 `9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234`.
Aceita-se exclusivamente seu corpo U1–U7 por incorporação. Status CANDIDATE,
alternativas e históricos da fonte permanecem imutáveis. Registro precede código.

U1 estabelece o ato independente BINARY_UPDATE_CLOSED no owner existente,
reutilizando mecânicas F1–F6, sem reutilizar NEW_INSTALL, UPDATE de configuração
v3, staging/registro/Start consumidos ou grants persistidos. U4 tem precedência
somente sobre suficiência prévia dos custos opacos delimitados sob ED11
incremental20261012, W4 e F8, neste único ensaio. Custos opacos permanecem
UNKNOWN; física/commit >= K e disco R são gates observacionais fracos, não
reservas ou máximos comprovados. Quotas controladas e demais domínios intactos.

Ler U3 conjuntamente com U5: confirmação e consumo antecedem criação, cópia,
flush e renames. Preservar o objeto antigo em leaf exclusivo; um novo arquivo,
no máximo dois renames sem overwrite, readback e retenção parcial. Não assumir
atomicidade/durabilidade de metadata, cancelar IO por Stop ou rollback.
Identidade/proteção/ancestrais, referências originais, exclusividade, plano,
consumo único, Stop, fechamento factual e retenção UNKNOWN não são reduzidos.
M5/conta virtual, F3/F5/F6, W1–W3/W5 e contratos SCQ permanecem aplicáveis nos
seus domínios. Nenhum conflito residual identificado no escopo delimitado.

Autoriza somente implementação/qualificação offline, APIs mutantes simuladas.
Não seleciona automaticamente artefato futuro nem execução operacional. U7 exige
pedido byte-exato posterior e autorização própria. Não autoriza UPDATE/staging
real, restauração/delete/cleanup/refund, Start/Stop, endpoint, SCM/ACL, CA/CURRENT,
rede operacional, TPM/Q4 ou recuperação. Preservar HEAD/WIP, pins/históricos,
atos consumidos, fixture, reservas e PST0.6.4. OS_QUIESCENCE UNKNOWN;
E21 PENDING; ED11/Phase6 PARTIAL.
