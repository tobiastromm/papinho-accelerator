---
status: accepted
decision-date: 2026-10-29
scope-level: project
scope-target: PapinhoAccelerator
decision-makers: [Tobias Tromm]
---

# Intenção e ack persistidos de publicação de credencial

Autoridade: decisão explícita do proprietário em PACC-PHASE6-PUBLISHED-ACK-FOCAL-ACCEPTANCE. A conferência literal confirmou SHA256 `3fcce389e58299aaa161779b9c7976f00f8e79d76dc5622d40d60b091e34f684` da [proposta focal revisão 2](../integration/phase6-original-custody-and-ack-decision-20261029/PUBLISHED-ACK-FOCAL-DECISION.PROPOSED.md). Seu texto integral é incorporado como decisão normativa nos limites aprovados; o rótulo PROPOSED do original é preservado como proveniência histórica.

Esta decisão complementa somente a representação de PUBLISHED de [issuer/C2 P1.1/P1.2/P3](DECISION-issuer-c2-epoch-authority-relation-20261019.md). Não altera M3/M7, custódia, CURRENT/CAS, Model E, ED08, quotas ou formatos históricos. A é contexto administrativo de publicação pós-commit; P é contexto operacional de policy. Nenhum dos dois é sinônimo de commit, revisão ou do outro.

Ficam aceitas a intenção autorizada, a confirmação original pós-publicação e a confirmação CURRENT da história que contém o ack, versionadas no mesmo root/arquivo/worker, com os campos mínimos, sequência finita, rejeições, retenção e compatibilidade da proposta. Fonte original e custódia permanecem obrigatórias. Hash/flag/fixture/checkpoint genérico não produz autoridade. A revisão técnica exata de encoding será registrada no pacote de implementação antes de integrar código dependente.

A autorização cobre implementação e qualificação offline dentro dessas cláusulas. Não concede operações CURRENT/root reais, criação de CA/key/fonte inexistente, assinatura/emissão produtiva, bootstrap, rede/serviço/Start/Stop, SCM/ACL, TPM/NV/Q4, cleanup/refund, publicação PST ou recuperação autoritativa. Não aceita desvio semântico ou aumento de limites por escolha de layout. ED11/Phase 6 permanecem PARTIAL até prova aplicável.
