---
adr: ADR-0011
title: Identidade de capability e modelo portátil limitado
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers:
  - Tobias Tromm
supersedes: []
superseded-by: []
---

# ADR-0011 — Identidade de capability e modelo portátil limitado

## Contexto

O ADR-0005 define a resolução efetiva, mas deixou IDs, versões, registry e wire
para decisão futura. A Phase 4 precisa representar capabilities de forma
estável, determinística, bounded e compatível com C89/VC6 sem confundir função
conceitual com backend ou posição em bitmask.

## Decisão

A identidade runtime canônica é uma key composta por ID numérico estável e
versão semântica major explícita. Zero é reservado e inválido em ambos os
campos. A comparação usa `(id, major)`, nunca nome, ponteiro, backend, target,
Session, bit position ou message type.

Nome textual é somente metadata documental/diagnóstica. Registry e sets usam
storage fixo pertencente ao caller, capacidade explícita e nenhum heap
obrigatório. A ordem canônica é ID crescente e depois major crescente.
Duplicatas são rejeitadas; capacidade esgotada falha explicitamente; key
desconhecida é unsupported/not-found e nunca é concedida implicitamente.

O modelo interno não congela encoding, largura, byte order, mensagem ou ID do
Wire Protocol. Não há namespace vendor/private público nesta fase.

## Consequências

- IDs permanecem estáveis mesmo que backend e target mudem.
- Versões major incompatíveis são capabilities distintas para eligibility.
- Registry e sets são bounded, determinísticos e portáteis.
- Evolução wire exigirá specification/decisão própria.

## Regras derivadas

1. ID e major zero são inválidos.
2. Bitmask e string não são identidade canônica.
3. Registry/set nunca concedem key desconhecida.
4. Registration order não define enumeration order.
5. Nenhum máximo global/wire é imposto; cada instância declara capacidade.

## Relações

- ADR-0002 — Core portável entre deployments e backends.
- ADR-0005 — Negociação de capabilities e autoridade de policy.
- PapinhoEngineering ADR-0002 — targets factuais e duráveis.
- PapinhoEngineering ADR-0007 — fronteiras portáteis.

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-12 | Decisão aprovada para key numérica versionada e modelo bounded. |
