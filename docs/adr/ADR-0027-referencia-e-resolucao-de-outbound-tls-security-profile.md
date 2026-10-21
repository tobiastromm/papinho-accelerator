---
adr: ADR-0027
title: Referência e resolução de Outbound TLS Security Profile
status: accepted
decision-date: 2026-09-29
last-revised: 2026-09-29
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0027 — Referência e resolução de Outbound TLS Security Profile

## Contexto

O caminho produtivo `TLS_REQUIRED` usava exclusivamente trust do sistema. Isso
impedia targets controlados assinados por CA privada e não podia ser corrigido
reutilizando implicitamente a autoridade inbound do Secure Principal.

Transport Security, Admission, Authentication, Source/Context Policy,
Capability Policy, Destination Policy, Egress Profile e Outbound TLS Security
Profile são dimensões independentes. Em particular:

```text
INBOUND_CLIENT_TRUST != OUTBOUND_SITE_TRUST
```

## Decisão

O Shared Configuration Model recebe a identidade opaca e bounded
`outbound_tls_profile_ref`. Um resolver application-owned materializa antes de
RUN inputs normalizados para o PST em papel CLIENT. Referência ausente,
desconhecida, malformada ou trust material inválido falha fechado.

O profile v1 preserva TLS 1.3 only, validação de certificado e server name,
0-RTT desabilitado, resumption desabilitado, ausência de client certificate,
handshake não bloqueante bounded e graceful shutdown. Ele pode usar trust do
sistema ou CA customizada explicitamente configurada pelo mesmo profile.

Destination Policy não possui trust material. Egress Profile não possui trust
material. A CA inbound do Secure Principal nunca é reutilizada implicitamente.
Não há fallback para TLS sem verificação ou plaintext.

A source Win32 de laboratório pode mapear a referência para arquivo de CA DER
em base64, mas sua sintaxe de environment não é arquitetura nem formato de
persistência congelado.

## Consequências

- `allow_network_egress=TRUE` exige policy, egress profile e outbound TLS
  profile explicitamente referenciados;
- TCP_PLAIN não se torna TLS, embora o profile esteja resolvido como parte do
  snapshot de configuração da execução;
- profiles outbound diferentes podem futuramente ser resolvidos sem acoplar
  trust à Destination Policy;
- seleção per-destination de profile permanece fora do escopo;
- nenhum formato wire é alterado.

## Relações

- ADR-0005 — autoridade de policy e capabilities.
- ADR-0006 — Transport Security e credenciais inbound.
- ADR-0019 — Destination Policy e Egress Profile server-owned.
- ADR-0022 — perfil TLS 1.3 client outbound via PST.
- ADR-0026 — referências de Destination Policy/Egress Profile e regras exatas.
