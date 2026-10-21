---
adr: ADR-0028
title: Modelo ortogonal de admissão, autenticação e policies
status: accepted
decision-date: 2026-09-29
last-revised: 2026-09-29
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0028 — Modelo ortogonal de admissão, autenticação e policies

## Contexto

O PapinhoAccelerator possui decisões concretas para Transport Security,
capabilities, Destination Policy, Egress Profile e Outbound TLS. Direções já
aprovadas para admissão e policies futuras ainda precisavam de registro
normativo que impedisse a fusão acidental dessas responsabilidades, sem
congelar prematuramente suas linguagens ou wire.

## Decisão

O PapinhoAccelerator trata como dimensões arquiteturais independentes:

1. Transport Security;
2. Admission Mode;
3. Authentication;
4. Source / Context Policy;
5. Capability / Authorization Policy;
6. Destination Policy;
7. Egress Profile;
8. Outbound TLS Security Profile.

Uma dimensão não concede, seleciona nem enfraquece implicitamente outra.

### Transport Security

Transport Security responde como Browser ↔ Accelerator é protegido. TLS com
mTLS é TLS com autenticação mútua/por certificado de cliente, não um transporte
não TLS. O estado factual atual é:

```text
SECURE_PRINCIPAL = TLS 1.3 + mTLS + ALPN papacc/1
LEGACY_ENDPOINT  = PACC plaintext
```

Transport Security não determina Admission Mode.

### Admission Mode

`OPEN` significa que nenhum username/password/login de aplicação é exigido
para usar o serviço. Não significa plaintext, ausência de TLS, ausência de
policy, Network Egress irrestrito, origens irrestritas ou destinos irrestritos.

`CLOSED` significa que uma etapa de autenticação de acesso é exigida antes do
uso do serviço. Credencial username/password, quando futuramente aprovada, deve
ser enviada dentro de TLS; seu armazenamento não deve exigir password em texto
claro.

`SECURE_PRINCIPAL != CLOSED`, `mTLS != CLOSED` e `OPEN != plaintext`.

### Authentication

Authentication responde quem é o usuário/dispositivo e permanece separada do
requisito de admissão. A arquitetura deve manter representáveis, sem afirmar
implementação atual:

```text
NONE
USERNAME_PASSWORD
CERTIFICATE_IDENTITY
CERTIFICATE_PLUS_USERNAME_PASSWORD
PAIRING / ENROLLMENT
```

Combinações que devem permanecer possíveis incluem TLS + OPEN + NONE, TLS +
CLOSED + USERNAME_PASSWORD, e TLS/mTLS + CLOSED com identidade por certificado,
isolada ou combinada com username/password.

### Source / Context Policy

Source / Context Policy responde de onde, por quem e quando o serviço ou uma
capability pode ser usado. Seu futuro modelo bounded deve manter possíveis
inputs como IPv4/IPv6 de origem, subnet, listener/interface, Principal, grupo,
dia da semana e janela de horário. Ela não é Destination Policy.

Exemplo motivador canônico:

```text
student subnet 192.168.30.0/24, Monday-Friday, 08:00-12:00
    -> Network Egress permitido
mesma subnet fora da janela
    -> Network Egress negado
teacher subnet 192.168.20.0/24
    -> policy diferente
```

Um Accelerator OPEN também pode permitir Internet somente à subnet X durante
uma janela e negar a subnet Y.

### Capability / Authorization Policy

Admission não concede capabilities. `NETWORK_EGRESS`, `TLS_OFFLOAD`, Compute,
Image, Media e outras capabilities continuam sujeitas à própria autorização.
Um cliente OPEN admitido ou um cliente CLOSED autenticado pode receber deny
para qualquer capability.

### Destination Policy

Destination Policy responde onde uma conexão outbound autorizada pode chegar.
ADR-0026 é a fundação `EXACT_ALLOWLIST` v1, não a linguagem futura completa.
A arquitetura deve manter representáveis:

```text
DENY_ALL
PUBLIC_INTERNET
EXACT_ALLOWLIST
```

`PUBLIC_INTERNET` poderá permitir public unicast, preservando deny-by-default
para loopback, link-local, RFC1918/ULA, metadata endpoints, management networks,
self-listeners do Accelerator e demais classes sensíveis governadas.

CIDR/subnet, domain suffix, port ranges e explicit deny rules exigem ADR ou
revisão governada própria; não são retrofits implícitos do ADR-0026.

### Egress Profile

Egress Profile responde por qual interface/source/link o tráfego sai. Ele
permanece separado da Destination Policy. O cliente não escolhe arbitrariamente
interface/source/link privilegiado e não há failover silencioso sem decisão
explícita.

### Outbound TLS Security Profile

O `outbound_tls_profile_ref` do ADR-0027 responde como o Accelerator autentica
e protege o site TLS remoto. Trust source, CA customizada, server-name
verification, versão TLS, ALPN e eventual client certificate pertencem a esse
eixo, não à Destination Policy ou ao Egress Profile.

```text
INBOUND_CLIENT_TRUST != OUTBOUND_SITE_TRUST
```

Material idêntico somente pode ser usado quando configurado explicitamente por
profiles separados; nunca há reutilização implícita.

### Legacy Endpoint

O estado atual permanece:

```text
LEGACY_ENDPOINT = plaintext PACC
LEGACY_ENDPOINT_NETWORK_EGRESS_V1 = NO
```

Legacy OPEN e Legacy CLOSED devem permanecer conceitualmente possíveis, mas não
são implementados por este ADR. CLOSED Legacy não poderá transmitir password
reutilizável em texto claro sem decisão/protocolo de segurança separado. Legacy
Network Egress também exige decisão explícita futura.

## Combinações canônicas preservadas

- Open proxy-like: TLS server-auth, OPEN, NONE, Source/Context opcional,
  PUBLIC_INTERNET, sensitive deny e default-route.
- Closed authenticated: TLS server-auth, CLOSED, username/password dentro de
  TLS e Destination Policy pública ou restrita.
- Closed device-trusted: TLS+mTLS, CLOSED, certificate identity e password
  opcional, com policy por Principal/grupo.
- Open school lab: TLS server-auth, OPEN, NONE, subnet+jornada permitidas,
  PUBLIC_INTERNET e sensitive deny.
- Legacy client: LEGACY_ENDPOINT plaintext, Admission futuro OPEN ou CLOSED e
  Network Egress v1 negado.

## Consequências e implementação futura

Este ADR registra boundaries e representabilidade; não define wire, schemas,
precedência detalhada ou runtime. Antes de implementar, são exigidos ADRs
específicos para:

1. Admission Mode e Authentication Model;
2. Source / Context Policy, incluindo precedência e avaliação bounded;
3. Public Internet Destination Policy e interação com exact allowlists;
4. futura autorização ou Network Egress de Legacy Endpoint.

## Relações

- ADR-0001 — Secure Principal e Legacy Endpoint.
- ADR-0005 — capabilities, backend e autoridade de policy.
- ADR-0006 — perfil de Transport Security.
- ADR-0019 — Destination Policy e Egress Profiles server-owned.
- ADR-0021 — destinos sensíveis deny-by-default.
- ADR-0026 — Exact Destination Policy v1 e referências policy/profile.
- ADR-0027 — Outbound TLS Security Profile reference/resolver.
