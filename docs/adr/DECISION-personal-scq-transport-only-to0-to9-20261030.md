---
status: accepted
date: 2026-10-30
scope: PERSONAL_LOCAL SCQ TRANSPORT_ONLY offline integration
supersedes: []
---
# Decisão — SCQ de transporte sem KSP, TO0–TO9

O proprietário aprovou explicitamente o conjunto indivisível TO0–TO9 de
[SCQ-TRANSPORT-ONLY-NO-KSP.CANDIDATE.md](../integration/phase6-virtual-scm-integration-20261029/SCQ-TRANSPORT-ONLY-NO-KSP.CANDIDATE.md),
SHA256 `0702a2e733024118bdad7c0993b4993e20cf44e825a872ad50245e8fd2b45d30`,
na task PACC-PHASE6-TO0-TO9-ACCEPTANCE-AND-OFFLINE-INTEGRATION.
Bytes conferidos integralmente antes deste registro e de código dependente.
Aceita-se por incorporação byte-exata; proposta original PROPOSED fica imutável.

A exceção exclusiva às passagens de chave MACHINE do OHI e regra3 B é o
propósito privado2 TRANSPORT_ONLY, com ausência canônica integral de key e
proibição de KSP. READ1 e demais perfis mantêm o seletor obrigatório.
Compatível com SSO-B/S0–S7: readiness somente diagnóstica fechada, originais,
consumo anterior aos efeitos, validação bilateral, SQR1 distinto de SQC1,
Stop, lifetime, quotas124/128/1024, UNKNOWN e retenção permanecem obrigatórios.
Nenhum texto accepted anterior, N0/T0–T14 ou formato ROOT/CURRENT é editado.

Autoriza somente implementação e qualificação offline no mesmo owner/runtime.
Não concede operação, endpoint, sessão real, Start/Stop, SCM/ACL, instalação,
CA/CURRENT, KSP/CNG real, rede, TPM/Q4, limpeza/refund ou recuperação.
Aceitação não comprova autenticação bilateral nativa, fonte operacional,
headroom, validade/freshness/duração ou custódia. Pedido operacional NOT_READY.
Seletores operacionais continuam vazios/DENY. O binário Selected/Release
90123fbadc3d7e4db1e5f48ea2b7fe85f6e78f61f6adee083b00d1288b951874
é seleção de planejamento anterior: artefato derivado exige nova seleção
explícita, sem migração silenciosa. E21 PENDING; ED11/Phase6 PARTIAL.
