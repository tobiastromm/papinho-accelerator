---
adr: ADR-0029
title: Contrato Mode C/C2 e autorização laboratorial controlada
status: accepted
decision-date: 2026-10-07
last-revised: 2026-10-07
revision: 1
scope: project
decision-makers: [Tobias Tromm]
---

# ADR-0029 — Contrato Mode C/C2 e autorização laboratorial controlada

## Contexto

Mode C preserva a origem de rede no Browser enquanto delega o Site TLS ao
PapinhoAccelerator/PST. Auditorias factuais do Browser, Accelerator/PACC e PST
confirmaram que o PST 0.6.4 pode participar de um experimento C2 por transporte
socket publicado, mas o Browser ainda possui socket local bloqueante e o PACC
atual não associa os dois fluxos exigidos a uma operação comum.

O gate conjunto `docs/mode-c-c2-joint-contract-gate.md` consolidou D1–D10. O
owner aprovou D1–D8, a autorização laboratorial D9A e D10 como contrato/matriz,
com execução de E1–E12 sujeita a um novo gate após implementação dos
pré-requisitos. A policy produtiva Legacy D9B permanece proposta.

## Decisão

### 1. Placement e ownership

Mode C significa:

```text
Browser
  -> original hostname, DNS, endpoint numérico selecionado,
     site TCP e site-facing IP egress

Accelerator/PST
  -> Site TLS, SNI/expected peer name, chain/hostname verification
     e application-data TLS

PACC
  -> transporte bounded dos roles APPLICATION e RECORD
```

Um `SOCKET` numérico nunca cruza máquinas. Site TCP aberto pelo Accelerator é
Mode B, não Mode C. BadVPN, tunnel/IP forwarding, open proxy, segunda resolução
silenciosa, substituição do hostname TLS por IP e fallback implícito são
proibidos.

### 2. Identidade da operação e attachment

Uma operação Mode C possui identidade de correlação não secreta e uma authority
de attachment separada, imprevisível e single-use. O registro bounded vincula:

- Session/CONTROL e sua geração;
- profile/assurance class;
- hostname original normalizado, endpoint numérico e porta;
- policy/grant epoch e inputs de Site TLS;
- exatamente um role `APPLICATION` e um role `RECORD`;
- geração de cancel/terminal, expiry e recursos reservados.

Authority previsível, DATA ticket atual, ponteiro ou runtime ID não serve como
segredo. Replay, geração/profile/endpoint/epoch incorreto, role duplicado ou
cross-operation attachment falham antes de bytes ou PST setup.

No perfil Legacy, a authority protege apenas correlação/attachment conforme o
contexto operacional disponível. Ela **não** autentica criptograficamente o
peer, não adiciona confidencialidade/integridade ao PACC plaintext e não protege
contra atacante on-path. Legacy continua com assurance inferior ao Secure
Principal.

### 3. Dois roles

```text
APPLICATION
  Browser application bytes <-> PACC <-> PST plaintext API

RECORD
  Browser site TCP <-> PACC <-> private C2 bridge <-> PST TLS records
```

CONTROL autoriza/reserva. Browser conecta o endpoint exato. RECORD é anexado e
C2/PST preparado; APPLICATION é anexado. Ativação ocorre somente quando ambos
os roles e o site socket estão prontos e é failure-atomic. Role ausente,
duplicado ou inválido expira/aborta a reserva. Browser não interpreta TLS e o
Accelerator não interpreta HTTP como responsabilidade do transporte Mode C.

### 4. Endpoint Browser-owned

Browser resolve uma vez por tentativa, aplica sua policy local a todos os
candidatos, seleciona um endpoint permitido e preserva snapshot imutável:

```text
original hostname + address family + numeric endpoint + port + resolution epoch
```

O connect usa exatamente esse endpoint, sem segunda resolução. O hostname
original é SNI e expected peer name. Redirect ou outro candidato exige nova
autorização/tentativa; IP nunca vira identidade TLS.

### 5. Socket não bloqueante e progresso

O Browser implementará contrato interno nonblocking, readiness-driven,
wakeable, bounded e cancelável: exact connect, partial send/recv, WOULD_BLOCK,
read/write interest, navigation cancel, deadline, half-close/EOF/RST factual e
close idempotente. Sleep polling não é mecanismo de correctness.

O primitive WinSock concreto não é congelado por este ADR. Ele será escolhido
e qualificado no slice C1 e na qualificação NT4, usando somente APIs comprovadas
no target.

### 6. Backpressure e budgets experimentais

Existem quatro filas direcionais bounded, uma por direção dos roles APPLICATION
e RECORD. Fila downstream cheia remove interesse de leitura upstream; retomada
ocorre abaixo do low watermark. TCP, PACC e PST mantêm readiness distintos. O
scheduler alterna operação e role/direção com tentativas bounded.

Para o experimento exclusivamente:

| Recurso | Limite aprovado para experimento |
|---|---:|
| operações | 2 globais; 1 por Session |
| cada fila | 64 KiB; high 48 KiB; low 24 KiB |
| payload agregado | 512 KiB |
| fragmento PACC | no máximo 16 KiB |
| serviço por wake | 16 KiB/flow; máximo 8 oportunidades |

Esses números não são limites de produção nem afirmação de segurança no NT4.
A qualificação deve medir e varrer 8/16/32/64 KiB antes de propor limites de
produto.

### 7. Cancelamento, causa factual e ação terminal

Uma geração de operação coordena cancel, deadlines, terminação e cleanup dos
dois processos. Cada owner físico fecha seu recurso exatamente uma vez; filas e
slots são limpos antes de reuso.

O modelo registra separadamente:

```text
terminal_fact
  -> o que ocorreu factualmente na camada de origem

termination_action / security_disposition
  -> ação fail-closed que a policy/security determina para a operação
```

Uma ação de segurança pode prevalecer para abortar ou restringir cleanup, mas
não reescreve a causa factual original. Precedência de ação nunca transforma
falha em sucesso nem converte raw EOF em `close_notify`. Completude da aplicação
permanece separada do terminal de transporte conforme PapinhoEngineering
ADR-0013.

Deadlines experimentais: attach 5 s, handshake 10 s, no-progress 30 s,
shutdown 5 s e cleanup forçado 5 s. São valores experimentais, não produção.

### 8. Bridge C2 privado

O Accelerator cria par conectado IPv4 loopback privado: listener
`127.0.0.1:0`, connector owned, validação do tuple exato, um accept e fechamento
imediato do listener. Um endpoint é transferido pelo transporte socket público
do PST; o peer pertence à operação.

Não há bind público/wildcard, firewall exception, listener reutilizável,
destino arbitrário ou proxy. Peer local incorreto é fechado e setup falha. O
registro de readiness é removido antes do close idempotente. C1 continua não
selecionado por falta de prova.

### 9. Capability e service model

`TLS_OFFLOAD` continua representando Site TLS executado no Accelerator e é
ortogonal à origem de rede. Mode C não requer Accelerator `NETWORK_EGRESS`,
pois o Browser possui site TCP/IP egress.

Mode C exige contrato versionado próprio de service/operation para endpoint,
roles e lifecycle. Este ADR não decide se sua identidade wire será nova key,
novo major ou outro descriptor compatível, e não aloca IDs. A especificação
PapinhoPACC posterior decide encoding e registry sob gate próprio.

### 10. D9A — autorização lab/teste da fase atual

Fica aprovada como arquitetura uma autorização laboratorial explícita,
test-only, opt-in, auditable, bounded e fail-closed para qualificar Secure e
Legacy Mode C. Antes de execução ela deverá ser materializada pelos slices
aprovados e pelo novo gate de experimento.

A autorização limita listener/interface/ambiente, hostname, endpoints, porta,
trust de fixture, duração, quotas e capability/service exatos. Ela nunca é
default de produto, não aceita destino wildcard, não cria Browser ACL, não
permite fallback Secure→Legacy e não congela CLI, Registry, GUI ou schema de
produção. Legacy usa listener/profile e evidência separados, warning de lower
assurance e isolamento de lab. Policy epoch integra o binding; revogação,
CONTROL loss ou perda do isolamento declarado aborta as operações.

### 11. D9B e Legacy de produção

D9B permanece `proposed` para C9–C11 da fase atual. Accelerator server continua
owner de admission, grants, destination/port policy, quotas, audit e revocation.
Browser apenas seleciona profile, consome grant/deny e mostra assurance.

```text
LEGACY_FUNCTIONAL_PARITY_TARGET=YES
LEGACY_CRYPTOGRAPHIC_PARITY=NO
LEGACY_PRODUCTION_MODE_C_AUTHORIZED=NO
```

### 12. D10 e autorização de execução

A matriz E1–E12 do gate conjunto é aprovada como contrato de validação. Ela
inclui TLS/SNI/trust/name, fragmentation/partial I/O, stalls/watermarks,
cancel/deadlines, EOF/reset/truncation, perda PACC, attachment race,
cross-operation swap, reverse teardown, concorrência e prova de Browser egress.

Esta aprovação **não autoriza executar** o experimento. Execução exige novo
gate humano após implementação e revisão dos pré-requisitos, fixtures, hashes,
budgets instrumentados e critérios determinísticos. E1 e E12 continuam
obrigatórios.

## Consequências

- Mode C pode avançar pelos slices C0–C12 da fase atual sem confundir network
  ownership e TLS ownership.
- Secure e Legacy compartilham função, mas não assurance criptográfica.
- O protocolo PACC precisará de desenho versionado posterior; nada é congelado
  aqui.
- C2 é o mecanismo experimental selecionado; produção continua dependente de
  evidência e decisões posteriores.
- Limites de experimento não se tornam defaults de produto.
- D9B, execução E1–E12 e limites de produção permanecem pendentes.

## Relações

- ADR-0001 — Secure Principal e Legacy Endpoint.
- ADR-0003 — ownership de Connection, Session e Channel.
- ADR-0004 — scheduling não bloqueante, bounded e justo.
- ADR-0005 — capability, backend e policy.
- ADR-0012 — Session Capability Snapshot.
- ADR-0017/0018 — Network Offload e registry Network Egress/TLS Offload.
- ADR-0028 — dimensões ortogonais e restrição Legacy atual.
- PapinhoEngineering ADR-0013 — terminalidade TLS e completude da aplicação.
- `docs/mode-c-c2-joint-contract-gate.md` — alternativas e matriz aprovada.

## Estado de implementação

```text
CONTRACT_ACCEPTED=YES
IMPLEMENTED=NO
EXPERIMENT_EXECUTION_AUTHORIZED=NO
D9A_ARCHITECTURE_ACCEPTED=YES
D9B_PRODUCTION_POLICY=PROPOSED
WIRE_IDS_ALLOCATED=NO
PRODUCTION_LIMITS_APPROVED=NO
```
