---
adr: ADR-0012
title: Snapshot de capabilities da Session e enforcement contextual
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

# ADR-0012 — Snapshot de capabilities da Session e enforcement contextual

## Contexto

O ADR-0005 não definiu quando a Effective Configuration é congelada nem como
mudanças administrativas afetam uma Session viva. A solução precisa permitir
revogação imediata sem expandir silenciosamente autoridade já publicada.

## Decisão

Cada Session pode possuir um Capability Snapshot separado, keyed pelo
`session_instance_id`. Ele é publicado quando todos os inputs necessários
estão disponíveis e contém a interseção:

```text
SERVER_SUPPORTED ∩ SERVER_ENABLED ∩ PRINCIPAL_ALLOWED
∩ CLIENT_SUPPORTED ∩ CLIENT_PREFERENCE
```

O snapshot é um upper bound imutável durante seu lifetime. Enforcement por
operação exige membership no snapshot e consulta configuração/policy/contexto
atual. Disable ou revocation pode negar imediatamente. Re-enable pode voltar a
permitir uma capability que já pertence ao snapshot. Policy/contexto nunca
adiciona uma capability ausente.

Capability nova exige nova Session ou futura renegociação explícita. O produto
deve suportar renegociação explícita em fase/specification futura, reavaliando
todos os cinco inputs e publicando atomicamente uma nova geração. Phase 4 não
define wire, initiation, races, generation field ou mensagem de renegociação.

Snapshot ausente nega. CONTROL loss/Session cascade remove o snapshot. Sessions
do mesmo Principal permanecem isoladas e podem ter snapshots diferentes.

## Consequências

- poderes podem ser reduzidos imediatamente;
- poderes não crescem silenciosamente;
- re-enable temporário não exige nova Session quando a key já estava elegível;
- renegociação futura permanece deliberada, revalidada e auditável;
- o runtime não reconstrói snapshots implicitamente no hot path.

## Regras derivadas

1. Snapshot pertence semanticamente à Session, sem precisar integrar sua struct.
2. Membership é necessária, nunca suficiente, para executar operação.
3. Falha/ausência/erro contextual nega.
4. Replacement automático ou silent add são proibidos.
5. `allow_network_egress` não é migrado nesta fase.

## Relações

- ADR-0003 — Session ownership e lifecycle.
- ADR-0005 — Effective Configuration e policy authority.
- ADR-0011 — Capability Key e sets bounded.
- ADR-0006 — Transport Security permanece fora da negociação.

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-12 | Decisão aprovada de upper bound imutável, enforcement contextual e renegociação futura explícita. |
