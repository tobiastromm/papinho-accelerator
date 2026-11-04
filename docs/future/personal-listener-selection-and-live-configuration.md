# Personal listeners and live configuration

Estado 2026-11-03: o [caller FCA de preparação inicial](../integration/phase6-server-update-after-utility-20261101/FCA-FIRST-PREPARATION-INTEGRATION-REPORT.md) foi integrado offline com configuração mínima de admissão desabilitada e sem listeners. Não implementa esta direção, reload/CURRENT ou rede e não remove a reconciliação normativa pendente descrita abaixo.

OWNER_APPROVED_DIRECTION / NOT_IMPLEMENTED, 2026-10-23. [Designated normative direction](../adr/DECISION-personal-listener-selection-direction-20261023.md) preserves ADR0007/0009, interface-ref I1, .conf N5 and bootstrap/update D2. No future text overrides those contracts.

Reuse machine-local canonical IDs, coherent snapshot/resolvers, .conf model/listeners collection and one owner across CLI/SCM/direct/future GUI. defaultLan resolves exactly one eligible LAN to an existing ID; none or ambiguous denies with reasons/IDs. Preserve binding across address changes/unavailability; no silent migration, all-interface selection, Internet inference or duplicate listener. Exact eligibility and selector grammar/default remain unqualified. Interface names/IP/index never replace IDs.

Future show/validate/apply/status and no-restart application must preserve authority, new/old generation coexistence, Stop, capacity, factual commit/activation, drainage/quiescence and UNKNOWN retention. Do not infer rollback or accept a second effective-state authority. Listener exposure and permitted client Internet egress remain distinct.

CURRENT/reload normative reconciliation is pending: N5/D2 require canonical commit/CURRENT for new administrative configuration; simple reload cannot itself bypass or mutate administrative identity by assumption. No exact alternative, representation or publication exception is accepted yet. The single next slice is [contract reconciliation and DATA preparation](../integration/personal-listener-live-config-direction-20261023/integration-map-and-next-slice.md), after current LAB runtime continuation. It introduces no productive listeners/network and does not reopen network identity architecture.

Update this future direction and the live capability when accepted technical clauses or actual implementation change. No implementation claim follows from owner direction or document creation.

PDP0–PDP7 foi [registrado e integrado offline](../integration/phase6-server-update-after-utility-20261101/PDP-INTEGRATION-AND-UPDATE-REPORT.md) para preparar somente o parent de dados da primeira instância. Isso não implementa listeners, live reload, CURRENT, rede ou admissão; esses estados futuros permanecem inalterados.

