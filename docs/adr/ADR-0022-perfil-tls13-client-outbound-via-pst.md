---
adr: ADR-0022
title: Perfil TLS 1.3 client outbound via PST
status: accepted
decision-date: 2026-09-12
last-revised: 2026-09-12
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---
# ADR-0022 — Perfil TLS 1.3 client outbound via PST

## Decisão

Mode B usa somente a API pública PST CLIENT: TLS 1.3 only, certificado/chain e
peer-name obrigatórios, sem plaintext fallback, 0-RTT, resumption ou client
certificate v1. Trust SYSTEM ou CUSTOM é server-owned e não tem fallback.
Hostname DNS normalizado fornece simultaneamente SNI e expected peer name; o
cliente não fornece SNI separado. IP literal só é aceito quando PST/provider
garante IP SAN exato. ALPN ordered/bounded é filtrado por policy e mismatch
required falha fechado.

Handshake é nonblocking, PST-readiness-aware, cancellable e tem deadline
monotônico. Shutdown tenta close_notify bounded; ausência do peer é reportada
como close truncado/unclean. Nenhuma API OpenSSL/NSS/Schannel direta é usada.
Network e TLS placements são ortogonais: Mode A é local/local; B é
Accelerator/Accelerator; C futuro é local network/Accelerator TLS; D é
Accelerator network/local TLS por TCP_PLAIN opaco. Mode C não é implementado.
