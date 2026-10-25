# Escopo consolidado do Accelerator pessoal — 2026-10-22

Uma proposta normativa [M0–M11](../../adr/ADR-CANDIDATE-phase6-personal-mvp.md), ainda não aceita. Produto: instalação/configuração Windows, SCM instalar/start/Stop/status/manual-auto/remove, CA interna protegida, provisionamento manual E automático, emissão/gestão de credenciais, views ativos/histórico revogado, ocultação manual/por idade e uninstall/reinstall previsíveis. .conf restaura só opções não secretas, nunca identidade/pareamentos. CLI canônica, sem segunda GUI obrigatória ou infraestrutura institucional.

Conclusão exige jornada demonstrada: instalar → configurar → criar CA → parear Browser → usar TLS1.3/mTLS → listar → revogar/impedir nova admissão/tratarSessions → histórico/ocultar → parar/reiniciar → remover preservando dados. Ambos modos de provisão accepted continuam obrigatórios; outros clientes não são declarados qualificados. Não encerrar Phase6 só por esses documentos; demais gates vigentes permanecem.

Futuro: backup/restore antigos/migração, instituição/MDM, sync bilateral. Não implementar .papaccbak. Especialização pretende retirar backup completo como requisito de disponibilidade pessoal, NÃO proteção, serial/reserva durável, UTC/cert/status/revogação, replay, CURRENT/CAS, Stop/UNKNOWN/pins ou ED11. Novo fault model M3 e R-ACTIVE M7 exigem aprovação explícita.

Fato atual: componentes DATA/LAB, não instalador operacional. F10 positivo, exclusive child LAB, KSP Create/Finalize LAB, binding privado, packet11v4 LAB, SHA256 determinístico e F0–F9 são preservados sem promoção. [Inventário instalação/serviço](installation-service-inventory.md), [clientes](clients-revocation-inventory.md), [conformidade](conformance-matrix.md), [configuração](config-and-retention.md), [roadmap](roadmap.md), [próximo prompt único](NEXT-IMPLEMENTATION-PROMPT.md).

Sem código ou execução operacional nesta tarefa. Falta aprovação normativa e autorização funcional específica; entregar prompt em vez de executar. Sem novo projeto/owner/worker/cadastro.



## Owner listener/live-configuration addendum, 2026-10-23

[Approved direction](../../adr/DECISION-personal-listener-selection-direction-20261023.md) preserves canonical interface IDs, multiple-listener model and existing .conf/owner; defaultLan is a logical single-ID selector with explicit denial on none/ambiguity, no unsafe LAN/Internet inference. No-restart configuration application is a requirement, not implementation. [Integration map and ONE next contract slice](../personal-listener-live-config-direction-20261023/integration-map-and-next-slice.md) records singular runtime limitations and unresolved N5/D2 CURRENT/reload boundary. Current listener-disabled LAB runtime continues first; no network or syntax change from this addendum. Historical roadmap entries remain history; MVP acceptance already occurred. ED11 PARTIAL and operational authority DENY.
