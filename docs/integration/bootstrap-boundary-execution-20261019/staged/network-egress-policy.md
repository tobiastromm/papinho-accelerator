# PapinhoAccelerator --- Future Architecture Direction: Network Egress Policy & Egress Profiles

**Status:** partially realized; remaining future architecture direction / deferred requirements\
**Implementation status:** Phase 6 v1 subset implemented; remaining items deferred\
**Wire status:** not defined\
**Current repository baseline audited:** `main`, Phase 4 complete,
ADR-0001 through ADR-0012 present.

## Estado implementado da base v1

A base aprovada foi formalizada em
`docs/adr/ADR-0026-referencias-de-policy-profile-e-regras-exatas-de-egress.md`.
O Configuration Model possui referências opacas de policy/profile e o backend
Win32 resolve, antes de RUN, snapshot bounded de exact allow rules. Extensões
como wildcards, suffix domains, CIDR, subnets e port ranges continuam futuras e
não estão autorizadas pela implementação atual.

O Outbound TLS Security Profile foi separado e formalizado no ADR-0027. As
dimensões arquiteturais preservadas são Transport Security, Admission Mode,
Authentication, Source/Context Policy, Capability/Authorization Policy,
Destination Policy, Egress Profile e Outbound TLS Security Profile; nenhuma
deve ser inferida de outra. Essa arquitetura ortogonal e sua representabilidade
futura são normativas no ADR-0028.

Direções futuras, ainda não implementadas nem congeladas como policy runtime:

- Admission `OPEN` significa ausência de login de aplicação, não ausência de
  TLS ou policy; `CLOSED` exige autenticação de acesso;
- mTLS é TLS com autenticação por certificado de cliente e não implica sozinho
  `CLOSED`;
- Source/Context Policy poderá considerar source IP/subnet, Principal/grupo,
  listener/interface e janelas de dia/horário;
- Destination Policy deverá poder oferecer `DENY_ALL`, `PUBLIC_INTERNET` e
  `EXACT_ALLOWLIST`; `PUBLIC_INTERNET` permitirá apenas public unicast e manterá
  todas as classes sensíveis negadas por padrão.

Essas direções exigem ADRs próprios antes de implementação. Elas não ampliam a
linguagem exata do ADR-0026 e não alteram
`LEGACY_ENDPOINT_NETWORK_EGRESS_V1=NO`.

## Governance classification

This document preserves future Network Egress architecture without
prematurely creating a new accepted ADR.

Phase 6 subsequently implemented the v1 subset governed by ADR-0018 through
ADR-0024: capability keys/negotiation, server-owned policy/profile composition,
single-resolution validated endpoints, sensitive-destination protection,
`TCP_PLAIN` and Mode B TLS. The remaining material here is future only,
especially explicit renegotiation, Mode C, failover, richer policy and Browser
integration. Current factual state is summarized in
`docs/phase6-network-tls-offload.md`.

### Already covered by accepted ADRs

#### ADR-0005 --- Capability / backend / policy authority

Already accepted:

``` text
capability != backend
supported != enabled != authorized != requested != effective
remote compute != network egress permission
server policy > client preference
```

Therefore this future document must not duplicate those decisions as a
new ADR.

#### ADR-0012 --- Session Capability Snapshot and contextual enforcement

Already accepted:

``` text
Session Capability Snapshot
= immutable upper bound
```

and:

``` text
operation-time policy/context
= may deny immediately
= may re-enable a capability already in the snapshot
= may not silently add a capability absent from the snapshot
```

ADR-0012 also already establishes:

``` text
capability absent from snapshot
→ new Session
OR
→ future explicit capability renegotiation
```

and requires future renegotiation to be explicit, deliberate and
auditable.

Therefore this document does not create a separate ADR merely to restate
those semantics.

### Future ADR candidates when Network Egress enters implementation

Evaluate, at the correct phase, whether to create one or more ADRs for:

1.  **Network Egress Policy boundary and authority**
    -   applies only to traffic the Accelerator creates or
        intermediates;
    -   `NETWORK_EGRESS` answers whether egress may be used;
    -   Egress Policy answers where/when/under what rules a requested
        operation is allowed;
    -   Egress Profile answers through which approved
        interface/source/link an allowed operation executes;
    -   client cannot arbitrarily choose privileged server routing
        resources;
    -   Network Egress is not a generic host/network firewall;
    -   URL/path/header/content filtering is outside Network Egress
        core;
    -   TLS interception is separate and never implicit.
2.  **Egress Profile selection and failover semantics**
    -   policy/server selects approved profile;
    -   profile changes affect new outbound connections unless a later
        decision says otherwise;
    -   established connections are not silently migrated;
    -   failover/fallback must be explicit;
    -   `MUST_USE_LINK_B` with link B unavailable means
        deny/unavailable, not silent use of link A.
3.  **Network Egress abuse/security boundary**
    -   SSRF/proxy-abuse protection is mandatory when egress is
        implemented;
    -   sensitive/local/management destinations require explicit policy;
    -   client request alone never authorizes access.

Do not reserve ADR numbers now.

### Not a new ADR: explicit capability renegotiation

ADR-0012 already makes explicit capability renegotiation a future
required direction.

What remains undecided is the concrete design:

``` text
wire
message flow
who initiates
generation representation
race handling
atomic publication details
failure semantics
```

Those details require a future specification/ADR only when the design is
mature enough.

### Future decisions --- do not freeze now

-   group/RBAC model;
-   policy-subject schema;
-   Egress Profile storage/configuration;
-   persistent configuration format;
-   CLI/GUI syntax;
-   concrete interface/source-IP representation;
-   routing implementation;
-   failover algorithm;
-   domain matching semantics;
-   DNS resolution semantics;
-   DNS policy;
-   quotas/resource-limit schema;
-   scheduling/time-rule representation;
-   Web Proxy/Application Filtering design;
-   TLS interception architecture;
-   managed CA behavior;
-   wire messages/payloads;
-   capability-renegotiation wire protocol;
-   migration of established outbound connections between links.

------------------------------------------------------------------------

# Preserved future requirements

## 1. Three distinct concepts

``` text
NETWORK_EGRESS
= may this Principal/Session use outbound network access?

Network Egress Policy
= where / when / under what contextual rules is the requested egress allowed?

Egress Profile
= through which server-approved interface / source IP / logical link is it executed?
```

These concepts must remain separate.

## 2. Conceptual flow

``` text
Client requests network operation
        ↓
authenticated Principal / Session
        ↓
NETWORK_EGRESS capability in Session snapshot
        ↓
current contextual Network Egress Policy
        ↓
destination allowed?
        ↓
Egress Profile selection
        ↓
interface / source IP / logical link
        ↓
outbound connection
```

The client may request an operation, but it does not gain authority to
choose arbitrary privileged server routing resources.

## 3. Egress Profile

Conceptually:

``` text
Egress Profile
├── outbound interface
├── source IP
├── routing/link identity
├── explicit fallback policy, if any
└── other bounded network-egress parameters
```

Illustrative only:

``` text
PROFILE_A
→ Ethernet 1
→ 192.168.10.20
→ ISP-A

PROFILE_B
→ Ethernet 2
→ 10.20.0.5
→ ISP-B

PROFILE_FILTERED
→ dedicated VLAN/interface
→ restricted upstream
```

No storage, identifier, persistence, configuration or wire format is
frozen here.

## 4. Policy subjects and context

Future policy may use administrative subjects such as:

``` text
Principal
logical group / role / policy subject
Session context
```

No complete RBAC/group model is selected here.

The architecture must remain general enough for residential,
educational, business, institutional, offline and other deployments.

## 5. Destination policy

Network Egress Policy may eventually restrict
Accelerator-created/intermediated connections by:

``` text
destination IP
destination subnet
destination domain
destination port
destination port range
```

Examples are illustrative only.

## 6. Domain vs application filtering

``` text
"may connect to example.com:443?"
→ Network Egress Policy
```

but:

``` text
"may access example.com/education
but not example.com/shorts?"
→ Web Proxy / Application Filtering
```

Therefore:

``` text
domain / IP / port policy
→ may belong to Network Egress

URL / path / header / content filtering
→ separate application-aware layer
```

## 7. TLS interception is separate

Network Egress Policy must never imply TLS interception.

``` text
Network Egress Policy
!=
automatic MITM
```

HTTPS content inspection would require a separate architecture and
policy for TLS termination/interception, managed CA, privacy,
certificate handling and application-aware proxying.

## 8. Firewall scope boundary

Inside future Network Egress scope:

``` text
allow/deny NETWORK_EGRESS
destination IP/subnet/domain
destination port/range
interface/source-IP selection
logical-link selection
contextual schedule
quota/resource limits
explicit failover/fallback policy
SSRF/proxy-abuse protections
```

Outside normal Accelerator scope:

``` text
generic host firewall
LAN-to-LAN filtering
arbitrary packet forwarding
general-purpose stateful TCP/UDP/ICMP firewall
general-purpose NAT/router
IDS/IPS
malware inspection of all network traffic
```

Those broader roles require separate future approval.

## 9. SSRF / proxy-abuse protection

When Network Egress is implemented, the Accelerator must not become an
arbitrary proxy or SSRF primitive.

Sensitive destination classes include:

``` text
localhost
loopback
link-local
private subnets
cloud metadata endpoints
management networks
admin-only networks
```

Access depends on explicit policy and must never be granted merely
because the client requested it.

## 10. Contextual policy and ADR-0012

ADR-0012 is authoritative for Session snapshot behavior.

If:

``` text
NETWORK_EGRESS ∈ Session Capability Snapshot
```

runtime policy may deny current use immediately:

``` text
policy DENY
→ next egress operation denied
```

and later allow it again:

``` text
policy ALLOW
→ same Session may use it again
```

provided `NETWORK_EGRESS` was already present in the immutable snapshot.

If:

``` text
NETWORK_EGRESS ∉ Session Capability Snapshot
```

runtime policy must not silently add it.

Future grant requires:

``` text
new Session
```

or:

``` text
explicit capability renegotiation
```

as established by ADR-0012.

## 11. Egress Profile changes

When `NETWORK_EGRESS` is already present in the snapshot, a contextual
policy change may select another Egress Profile for **new outbound
connections**.

This changes:

``` text
how the already-authorized capability is executed
```

It does not grant a new capability.

Established outbound connections are not automatically migrated between
links unless a future decision explicitly allows it.

## 12. Failover / fallback

Never use silent link fallback.

``` text
policy = MUST_USE_LINK_B
LINK_B down
        ↓
DENY / UNAVAILABLE
```

Not:

``` text
silently use LINK_A
```

If fallback is desired it must be explicit:

``` text
PREFER_LINK_B
FALLBACK_LINK_A
```

Concrete syntax remains future work.

## 13. Audit and observability

Future observability should safely record categories such as:

``` text
Principal / policy-subject result
destination category
selected Egress Profile
selected logical link/interface identity
allow/deny reason category
```

Do not log secrets by default.

Do not treat traffic payload logging as ordinary Network Egress
observability.

## 14. Naming

Preferred conceptual terms:

``` text
Network Egress Policy
Egress Profile
Destination Policy
Contextual Policy
```

Avoid naming the whole subsystem `Firewall`.

------------------------------------------------------------------------

# Relationship to the current project state

Current audited baseline:

``` text
Phase 4
→ complete

ADR-0011
→ stable numeric Capability Key + semantic major
→ bounded registry/sets

ADR-0012
→ immutable Session Capability Snapshot
→ contextual operation-time enforcement
→ explicit future renegotiation direction

Network Egress
→ still concept / partial configuration only
→ no production outbound path
→ no destination-policy implementation
```

Preserve this document for the later Network & TLS Offload phase. Phase
numbering may evolve.

------------------------------------------------------------------------

# Recommended repository integration

The repository already has a living Capability Document:

``` text
docs/capabilities/network-egress.md
```

That is the authoritative live document for current Network Egress
status.

Therefore:

``` text
docs/capabilities/network-egress.md
→ current factual state, implemented/partial/not-implemented, open questions

docs/future/network-egress-policy.md
→ owner-approved deferred architectural direction that is not yet an accepted ADR
```

Recommended cross-references:

``` text
ADR-0005
ADR-0011
ADR-0012
docs/capabilities/network-egress.md
docs/phase4-capability-framework-policy-engine.md
future Network & TLS Offload docs
```

Do not modify ADR-0005 or ADR-0012 merely to paste all of this future
detail into them.

If a future implementation decision materially freezes one of the ADR
candidates listed above, create the next local ADR at that time using
the then-current next number.

------------------------------------------------------------------------

# Governance summary

``` text
ALREADY ACCEPTED
├── ADR-0005
│   ├── capability/backend/policy separation
│   ├── server policy authority
│   ├── remote compute != network egress
│   └── network egress has separate policy
│
└── ADR-0012
    ├── immutable Session capability upper bound
    ├── immediate contextual deny
    ├── re-enable only if capability already in snapshot
    ├── no silent capability growth
    └── future explicit renegotiation

KEEP AS FUTURE DIRECTION
├── Network Egress Policy concept
├── Egress Profile concept
├── destination policy
├── no-silent link fallback
├── SSRF/proxy-abuse protection
├── firewall boundary
├── Web Proxy/content-filter boundary
└── TLS-interception separation

FUTURE ADR CANDIDATES
├── Network Egress Policy boundary/authority
├── Egress Profile + failover semantics
└── Network Egress abuse/security boundary

DO NOT FREEZE YET
├── RBAC/group model
├── storage/config format
├── CLI/GUI syntax
├── routing/failover implementation
├── DNS/domain-resolution semantics
├── proxy/application filtering
├── TLS interception
└── wire protocol
```

## Principle

**Network Egress capability says whether egress may exist; Network
Egress Policy decides whether a particular operation is allowed in
context; Egress Profile decides how an allowed outbound connection is
executed. None of these turns PapinhoAccelerator into a generic firewall
or router.**

### Realização parcial — Site TLS Mode C, 2026-10-18

A [decisão accepted de autoridade administrativa](../adr/DECISION-mode-c-site-tls-profile-authority-20261018.md) designa a configuração existente do Accelerator para perfis Site TLS Mode C. A [implementação privada inicial](../integration/mode-c-site-tls-authority-20261018/closeout.md) persiste catálogo versionado e vincula referência a CURRENT/revisão/epoch. Não usa Network Egress como autoridade, não altera regras futuras de destinos e não habilita admissão. Aplicação operacional no dispatch e qualificação nativa permanecem pendentes.


### papacc.conf — subset offline, 2026-10-19

A [decisão textual focal](../adr/DECISION-papacc-conf-textual-grammar-20261019.md) aceita end explícito, indentação visual, UTF8/escapes e comentários/desativação sob N1–N8. O [slice parser/listeners](../integration/papacc-conf-parser-listeners-20261019/closeout.md) adiciona biblioteca portável opt-in, AST/candidate com presença e coleção de listeners, diagnóstico e validação estrutural bounded. Debug/Release MSVC passaram sem warnings e121 verificações por execução; cenários são offline/sintéticos. Não implementa source administrativa operacional, resolução factual de segurança/grants, ativação, config apply, CURRENT ou codec binário novo. Modelo/CLI singulares existentes ainda exigem integração posterior; não alegar conformidade operacional integral ADR0009. ACLs futuras continuam preservadas sem diretivas fictícias; blocos fora do subset são rejeitados. Quotas, produção0.6.4, PST/wire/Browser e gates DENY preservados; ED11 total PARTIAL, provider/kernel UNKNOWN. Nenhum firmware box/ISA qualificado.


### Binding administrativo de candidates — 2026-10-19

O [bridge read-only canônico](../integration/config-admin-resolution-listener-binding-20261019/closeout.md) separa VALIDATED, MODEL_BOUND e RESOLVED. Reutiliza modelos/validador/bind targets e o pin CURRENT/source existentes. Somente wrapper da única root pode marcar resolução factual; sem publication/grants/config apply/listener/rede. Subset ALL/Secure/OPEN/NONE; provider_ref nomeado e interface_ref textual sem fonte governada são rejeitados, não fabricados. Identidade/policy/refs correspondem ao snapshot existente, não viram registry paralelo. Debug/Release do servidor e testes offline passaram sem warnings finais; positive MODEL_BOUND sintético e native guard de negação não qualificam positive RESOLVED num produto real. Codec da coleção é PROPOSED, BODY/ROOT intactos, source AST/modelo sem Legacy implícito; CLI/runtime singular produtivo ainda não migrado. Root layout176680/known managed1801168 inalterado; workspace caller1072 além do AST/input e allowance síncrono contabilizados pelo guard, sem quota aumentada ou ED11 PASS. Provider/kernel UNKNOWN; todos gates DENY, PST0.6.4 preservado.


### interface_ref decimal — 2026-10-19

A [decisão focal accepted](../adr/DECISION-papacc-conf-interface-ref-persistent-id-20261019.md) adota somente o PersistentID decimal uint64 da CLI, sob ADR-0007. O [binding portável](../integration/config-interface-persistent-binding-20261019/closeout.md) compara o token com o modelo administrativo existente e resolve um snapshot emprestado pelos resolvers canônicos, produzindo apenas MODEL_BOUND. Não cria aliases, nomes derivados ou autoridade de Network Egress/TLS/acesso. A entrada nativa continua sem caminho SELECTED para RESOLVED enquanto a proveniência/revisão/lifetime do snapshot administrativo não forem comprovados. Provider nomeado permanece PENDING e rejeitado; CLOSED preserva usuário/senha conforme contrato de provisionamento, sem substituição implícita por mTLS. Codec permanece PROPOSED; nenhum CURRENT, listener, grant, reserva ou admissão foi publicado. Testes são offline/sintéticos, não qualificação operacional. As descrições anteriores registram os slices históricos.


### Lifetime do snapshot de preparação — 2026-10-19

O [slice de ownership](../integration/config-interface-snapshot-lifetime-20261019/closeout.md) integra uma cópia imutável bounded no participante da única root administrativa. Ela resolve seleção/targets durante preparação sem reter buffers/presentation do produtor, usa invocation/revisão de preparação factuais e preserva retenção até o cleanup existente. Tentativas de replacement e uso com revisão divergente são negadas. Debug/Release e24invocações offline/nativas locais passaram, sem rede ou publicação de configuração. O novo storage embutido custa5928bytes, incluídos uma vez no censo; ED11 total permanece PARTIAL e provider/kernel UNKNOWN. Revisão inicial pending0 não é CURRENT nem epoch. Esta realização não estabelece provenance/freshness de source arbitrária nem autoriza SELECTED/RESOLVED sob CURRENT: esse vínculo continua negado até a transação factual aplicável. Provider nomeado PENDING/rejeitado, codec PROPOSED, quotas e produção0.6.4 preservadas. As notas anteriores descrevem seus respectivos slices históricos.


### Correlação CURRENT e comparação de capturas — 2026-10-19

O [slice focal](../integration/config-interface-current-topology-20261019/closeout.md) adiciona metadados ao participante existente, escritos somente no ramo de sucesso pós-commit/publicação da root, copiando R/A/P reais. A consulta exige pin CURRENT factual, mesma invocation/revisão e Stop válido; não atesta topologia nem produz RESOLVED. O hook positivo pós-commit está compilado, mas não foi executado nesta tarefa, que preservou CONFIG_PUBLICATION=DENY. O comparador portável reutiliza os resolvers por PersistentID e compara estado/endpoints entre observações sem usar índice/nome como identidade. Duas capturas nativas foram iguais para4IDs em cada configuração; isso não prova continuidade/freshness.24invocações finais Debug/Release passaram, zero warnings. A prova nativa de topologia aplicável à resolução permanece ausente: entrada SELECTED/RESOLVED negada, provider PENDING, codec PROPOSED, ED11 PARTIAL e admissão/rede negadas.80bytes adicionais de metadados, sem elevar quotas; cópia5928bytes permanece contada uma vez. Notas anteriores são históricas e não qualificam este ramo positivo novo.


### Interface consumption guard (2026-10-19)

Actual consumer: papacc_server_launcher_activate -> papacc_tcp_socket_win32_bind_owned. The offline config_source_resolve export is not consumed by this path. SELECTED now requires the existing factual CURRENT/topology report before activation acquisition and again before bind. The report does not establish topology applicability; SELECTED remains denied. No additional authority, producer, worker, snapshot or quota was introduced. Native bind consumes address/port/scope; success alone cannot establish PersistentID continuity.

Debug/Release compiled without warnings; 30/30 focused invocations passed, including native enumeration and dormant-root SELECTED denial, Stop, rollback and retention. CURRENT-positive, bind/listen and operational qualification remain unproved. No CURRENT, socket/listener or network was activated. Persistent storage delta is zero. ED11 remains PARTIAL; named authentication provider remains PENDING. Evidence and gates: docs/integration/config-interface-consumption-binding-20261019/closeout.md.


### LST1 offline codec accepted and implemented (2026-10-19)

DECISION-lst1-offline-codec-revision2-20261019 accepts only C1-C5 of the exact revision2. The new opt-in PAPACC_ENABLE_OFFLINE_LST1 library owns explicit header/record byte fields and delegates SHA256 to the existing adapter. It is not linked into papacc_server. No AST dump, root/BODY persistence, CURRENT, CLI apply, reference resolution, grants or listener activation. CLOSED/CERTIFICATE_IDENTITY remain candidate representations but are UNSUPPORTED in this first codec. SELECTED roundtrip is data-only; topology applicability and named-provider gates remain denied/pending.

Debug/Release compile with zero warnings and 20 selected test invocations pass; the codec performs6723 internal checks per configuration using synthetic configuration and the existing native SHA256 adapter. Core also compiles under GCC strict C90 without Win32/CNG headers; this does not qualify box/ISA/NT4. Compiled model2176 bytes, no persistent product delta, no new pin/worker, provider/kernel UNKNOWN and ED11 PARTIAL. Existing BODY/root bytes and history are untouched. See docs/integration/lst1-revision2-implementation-20261019/closeout.md for census, approval/preimages, failures corrected and future same-root integration gate. Earlier codec-pending notes are historical.


### D3 ROOT8 normative acceptance (2026-10-19)

[DECISION-config-root-envelope-d3-revision3-20261019](../adr/DECISION-config-root-envelope-d3-revision3-20261019.md) accepts only exact D3.0-D3.8 of Revision3 after official executor review. ROOT8/version1 layout2592..25792 bytes is normative; no codec/root integration or operational qualification was implemented by this acceptance. LST1 remains historical base-only fragment; BODY05/ROOT06/PACICP01 are unchanged. Historical ROOT06 migration, safe issuer adapter integration, bootstrap/update authority, native lifecycle and complete ED11 reservation remain independent gates. D2/D4 and their existing document statuses were not changed or promoted by reference. CURRENT/publication/recovery and admission remain DENY; no build/test/network or CA/certificate operation. See docs/integration/config-d3-revision3-acceptance-20261019/closeout.md for exact source hashes, scope and review. Earlier proposed-layout notes remain historical.


### D3 independent codec implementation, root integration denied (2026-10-19)

The accepted D3 private ROOT8 codec is implemented as an opt-in OFF library, not linked to papacc_server. It reuses LST1 record validation/base codec and existing BODY05/SHA adapters with owned byte results. No historical BODY/root bytes, root participant, worker, CAS, store or CURRENT were changed. Native data adapter accepts BODY05 validation only; ROOT06 issuer carrier is explicitly UNSUPPORTED pending the separate integration gate. Encoding bootstrap/update is data-only, not administrative proof.

Debug/Release zero warnings; 34 selected offline invocations pass, codec44782 checks per configuration plus existing LST1/regressions. GCC strict C90 core compilation passed. Synthetic histories/checkpoints and real existing native BODY/SHA calls are reported separately from native administrative evidence. Root8 CAS, authorized bootstrap/update, Stop/retained publication and issuer/migration qualification are NOT established by these tests. Existing dormant-root Stop/retention/selected-denial tests remain scoped evidence.

Compiled ROOT8 model25760 bytes. Naively coexisting current dormant ALL reservation1974203 + existing source caller85560 + ROOT8 model25760 + output25792 totals2111315, exceeding2097152 by14163 before further proven transitive costs. Integration stays DENY without quota increase or premature release. No new product persistent storage, pins or worker. See docs/integration/config-d3-envelope-implementation-20261019/closeout.md and ed11-census.json. All admission/publication/recovery gates remain closed; provider/kernel UNKNOWN.


### D3 no-carrier bounded memory reconciliation (2026-10-19)

Existing portable codec now provides bounded owned encode/decode for the explicitly no-issuer profile:9376-byte fields +9408-byte encoded output, retaining all four candidate/base listeners and full validation. General codec/layout remain unchanged; issuer carrier stays UNSUPPORTED. No buffer sharing, root/issuer reserve reduction or retained/pinned reuse. Official exact patches, Debug/Release zero warnings,34 final offline invocations and GCC strict C90 pass.

The previously reported ALL coexistence scenario drops2111315->2078547 (delta-32768, margin18605 under2097152); SELECTED2078563/margin18589. This is a diagnostic single-operation subtotal, not an operational reservation or complete future ROOT8 root census. Concurrent/retained buffers remain separately charged; native/transitive/provider/kernel costs and actual root integration not qualified. CURRENT/recovery/admission DENY. See docs/integration/d3-ed11-coexistence-20261019/closeout.md and lifetime-proof-and-root-handoff.md. This updates the preceding factual memory blocker without changing the architecture or accepted gates.


### Same-worker D3 no-issuer adapter (2026-10-19)

Existing canonical pending worker now has explicit ROOT06 and opt-in offline ROOT8/version1 update paths. Native identity for D3 is carried outside serialized bytes. Full data/history validation and actual CURRENT/base pin are required by entry; existing ROOT06 issuer roots/carrier remain rejected by D3. Same thread/arrays; no bootstrap/active owner call, CURRENT/epoch, new registry or operational authority. D3 native success retains a confirmed packet and denies old authority until future root transaction completion; issuer/Session slots untouched. Option OFF by default.

Final Debug/Release zero warnings;58 final offline invocations pass, including real local TxF/CAS and historical ROOT06 snapshots/history. CURRENT/CP contexts are synthetic; UNKNOWN/pause explicitly injected. Do not interpret this as operational publication/Mode C/ED11 qualification. Managed scenario conservatively retains caller pair and adds all new explicit scratch: ALL2089095, SELECTED2089111 under2097152. Margins8057/8041, no quota increase; provider/kernel/transitive and future collection publication remain unqualified. See docs/integration/d3-canonical-worker-no-issuer-20261019/closeout.md and worker-proof-and-root-handoff.md. All admission/recovery/publication gates remain DENY.


### D2/D4 bootstrap/update normative boundary accepted (2026-10-19)

DECISION-config-bootstrap-update-boundary-20261019.md now directly accepts ONLY exact D2–D2.3 and D4 from the preserved Revision2 proposal, after the owner's eight conditions and official executor approval. Accepted D3 Revision3 and all codecs/other norms unchanged. Explicit authorized virgin-store proof is required for bootstrap; CURRENT/base pin/CAS for update. Absence of CURRENT/file or CREATE_NEW alone does not establish no prior history. No authority or grants from preparation, no fallback on conflict/UNKNOWN.

This normative step applies no functional patch or operational publication. Existing D3 worker remains an offline confirmed-packet adapter; actual bootstrap/collection snapshots/transaction completion and their complete ED11 graph remain pending. Source-level integration review resumed; details and exact preimages/hashes in docs/integration/config-d2-d4-normative-acceptance-20261019/closeout.md and transaction-resumption-handoff.md. All admission/CURRENT/recovery gates remain closed; no previous tests are new implementation evidence.


### D3 confirmed COMMITTED export; bootstrap proof remains denied (2026-10-19)

The same-root offline adapter now exports a full caller-owned D3 collection and128-byte COMMITTED receipt only after factual same-worker native success and closed resources, with integrity/history checks. No A/P/I, CURRENT, slot/pin/credit mutation or new worker/registry. Original input buffer can be reused only after begin returned and worker owns its frozen copies. Debug/Release zero warnings,58 offline invocations pass, including native file readback; operational CURRENT/CP contexts remain synthetic fixtures. Peak managed scenario2089223, margin7929, provider/kernel/transitive/full root publication still unqualified.

Owner confirms no approved positive first-initialization history procedure/source exists: INITIAL_BOOTSTRAP=DENY; absent file/CURRENT, CREATE_NEW and empty directory are insufficient. No automatic marker/procedure/new authority created. Existing factual CURRENT/base-pin/CAS update guards preserved. Full transaction/collection publication remains pending. See docs/integration/d3-root-transaction-continuation-20261019/closeout.md and initial-bootstrap-proof-handoff-proposed.md (PROPOSED only). All admission/publication/recovery gates closed; historical formats and accepted D2/D4/D3 intact.


### Candidate configuration CLI; local bootstrap still denied (2026-10-19)

The accepted local-bootstrap ceremony is normative; no native bootstrap adapter is qualified by this slice. The current path_ok/absolute-path worker lacks proof of continuous namespace custody and factually resolved bootstrap participants. No container is created, no marker or new authority introduced, no CURRENT/epoch published.

An OFF-by-default PAPACC_ENABLE_CONFIG_ADMIN_CLI frontend reuses papacc_server.exe in the API3 candidate. Dispatch precedes console/root/network acquisition and always returns before runtime for config commands. validate executes the existing bounded parser and target validator, reports source location, does not echo tokens/secrets, and rejects external references without factual authority. Structural-only candidates with no external objects can pass offline validation; this is not RESOLVED. diff and show-effective explicitly report CURRENT pin unavailable; apply explicitly denies before IO. These commands do not implement factual comparison, effective-state export or application yet.

Debug/Release /WX builds pass without compiler/linker warnings. Fifty actual CLI cases, eighteen focused CTests and twenty-four D3/ROOT06 component regressions pass. Native local persistence and synthetic authority/pause/UNKNOWN contexts are distinguished; there is no native ceremony or operational qualification. Root storage delta zero, prior managed runtime scenario2089223 unchanged; CLI workspace68112 occurs before runtime construction, not concurrently with its retained graph. CRT/transitive/native costs remain unqualified; provider/kernel UNKNOWN and ED11 PARTIAL. All admissions/publication/recovery denied. Details: docs/integration/local-bootstrap-config-cli-20261019/closeout.md.


### Bootstrap namespace phase and initial resolver boundary (2026-10-19)

A new opt-in phase in the existing pending worker atomically creates a directory relative to a held native parent and retains both handles/identities in the existing root packet. OBJ_DONT_REPARSE, enabled-admin checks, Stop, nonwaiting poll, exact-close retention and overwrite denial are implemented. No new worker/root/registry/journal. The separate initial resolver checks this exact custody plus initial source/model/policy without requiring or fabricating CURRENT; its explicit initial result leaves epochs absent. The update CURRENT resolver is unchanged.

This is PARTIAL: no initial D3 store commit using custody and no positive authorized initial reference case are qualified. The current medium-integrity executor is not an enabled administrator. Native tests use a compile-isolated diagnostic primitive path that cannot assert administrative readiness. Production bootstrap preflight and CLI apply remain denied; no CURRENT/epoch, listeners, CA/certificates or network.16 native namespace cases,18 focused CTests,24 historical D3/ROOT06 component regressions,50 CLI cases and4 census/guard invocations passed across Debug/Release; final builds zero warnings. Conservative managed subtotal2093407/2097152; provider/kernel UNKNOWN, ED11 PARTIAL. Full evidence and exact reviewed operations: docs/integration/bootstrap-boundary-execution-20261019/implementation-proof-and-remaining-gates.md. Earlier findings of no implementation are historical; full bootstrap completion remains pending.
