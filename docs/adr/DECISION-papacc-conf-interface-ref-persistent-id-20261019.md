---
status: accepted
decision-date: 2026-10-19
scope: project
decision-makers:
  - Tobias Tromm
---

# Decisão — interface_ref e PersistentID decimal

## Autoridade e precedência delimitada

Decisão explicitamente aprovada pelo owner para a resolução de referências administrativas papacc.conf. Especializa a representação textual sob ADR-0007 e as decisões accepted de gramática/comentários/listeners; não substitui nem modifica esses contratos. A proposta original em docs/integration/config-governed-reference-resolution-20261019/focused-contracts-proposed.md permanece preservada; somente a parte de interface é aceita por esta decisão.

## I1 — Token textual

interface_ref reutiliza exclusivamente o PersistentID machine-local unsigned64 já representado por --interface-id na CLI canônica. A string entre aspas contém uma sequência não vazia de dígitos ASCII com valor entre 0 e 18446744073709551615, sem sinal, whitespace interno, nomes, índices, IPs, aliases ou identificadores derivados. Zeros iniciais seguem o parser CLI existente; duplicação é por valor tipado. Não criar catálogo de aliases. Valor zero pode ser válido com presença/is_valid factual; ausência não é zero.

## I2 — Resolução factual

A referência deve corresponder ao modelo administrativo aplicável e ser resolvida obrigatoriamente num único snapshot canônico factual da máquina correspondente, com identidade, revisão e lifetime verificados. Reutilizar os resolvers canônicos de identidade persistente, seleção runtime e bind targets. Falta de referência, presença inválida, inexistência, obsolescência, ambiguidade, incompatibilidade ou indisponibilidade do target resulta em DENY, nunca fallback por nome, índice, endereço ou primeira interface disponível. Instância e endereços runtime não são persistidos como identidade administrativa.

## I3 — Lifetime e autoridade

Snapshot imutável e pin aplicável permanecem protegidos pelo owner factual durante a leitura síncrona. Stop, retenção ou divergência de CURRENT/revisão impedem RESOLVED. MODEL_BOUND não equivale a RESOLVED nem a autorização. O PersistentID identifica a interface e não concede acesso, Network Egress, TLS, grants ou capabilities. Uma API que receba somente snapshot emprestado pode estabelecer binding de modelo, mas não declarar proveniência administrativa ou CURRENT por flags do caller.

## I4 — Limites e pendências

O provider de autenticação nomeado permanece PENDING e rejeitado sem catálogo governado comprovado. Não inventar provider nem conceder autoridade por referência textual. Preservar o requisito de usuário/senha no modo CLOSED e seu vínculo futuro com o provisionamento automático já aprovado; mTLS não o substitui implicitamente.

A implementação exige patch exato e aprovação oficial correspondente. Não publicar CURRENT/A/P, ativar listeners, abrir admissão ou executar rede. Codec permanece PROPOSED; BODY0.5/ROOT06 não são alterados por esta decisão. Preservar única root, I/R/A/P, CAS, Sessions/pins, Model E, ED08/ED11, WIP, produção 0.6.4 e PST 0.6.5 candidato não publicado. CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.