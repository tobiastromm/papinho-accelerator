# PapinhoAccelerator --- Future Architecture Direction: Network Egress Policy & Egress Profiles

**Status:** future architecture direction / deferred requirements\
**Implementation status:** not implemented by this document\
**Wire status:** not defined\
**Current repository baseline audited:** `main`, Phase 4 complete,
ADR-0001 through ADR-0012 present.

## Governance classification

This document preserves future Network Egress architecture without
prematurely creating a new accepted ADR.

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
