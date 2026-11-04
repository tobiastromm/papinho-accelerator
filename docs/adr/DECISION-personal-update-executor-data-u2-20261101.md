---
status: accepted
date: 2026-11-01
scope: one PERSONAL_LOCAL BINARY_UPDATE_CLOSED executor/DATA boundary only
supersedes: []
---
# Decisão — executor protegido e candidato DATA no UPDATE

O proprietário aprovou explicitamente na task
PACC-PHASE6-U2-EXECUTOR-DATA-ACCEPTANCE-AND-INTEGRATION a proposta integral
`docs/integration/phase6-binary-update-u1-u7-20261101/U2-EXECUTOR-AND-CANDIDATE-DATA.CANDIDATE.md`,
SHA256 `58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da`.
Conferência byte-exata e leitura confirmadas. Incorpora-se exclusivamente seu
corpo normativo; o candidato original e seus status históricos são preservados.
Este registro precede a implementação dependente.

Especializa U2 apenas para a entrada DATA: owner/DACL são inspecionados e
retidos, mas não precisam conceder à origem a autoridade/proteção do executor.
Exige conjuntamente executor original independente, protegido e capaz; ato
administrativo atribuído; custódia original e hash selecionado; candidato
exclusivo protegido verificado antes dos renames. Descritor genericamente
válido ou hash isolado nunca satisfazem a admissão.

U2 permanece estrita para executor, dependências e originais protegidos.
U1/U3–U7, F1–F6 e demais perfis/quotas/autoridades não são reduzidos. Nenhum
grant, setter permissivo, serviço, owner, root, canal ou distribuição adicional.
Ausência de executor independente qualificado mantém DENY inclusive após esta
aceitação. Não legitima o próprio candidato como controller, self-update do
executor vivo, alias/hardlink ou prova retroativa de imagem/continuidade.

Não há incompatibilidade identificada no escopo da precedência delimitada;
demais obrigações continuam necessárias. Autoriza implementação e qualificação
offline, não execução operacional, provisionamento de controller ou UPDATE.
As autorizações anteriores permanecem utilizadas. Sem Start/Stop, endpoint,
SCM/ACL, CA/CURRENT, TPM/Q4, limpeza/refund/rollback. Preservar HEAD/WIP,
históricos/pins, EXE anterior, UNKNOWN, reservas e PST0.6.4.
