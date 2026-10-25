---
status: proposed
revision: 1
decision-date: 2026-10-21
scope-level: project
scope-target: authenticated time source under Q0-Q7
supersedes: []
---
# Candidate: authenticated UTC observations through the existing issuer backend

TASK=PACC-PHASE6-AUTHENTICATED-TIME-SOURCE-REVIEW-1. AT0-AT9 is PROPOSED for owner review,not ACCEPTED or an implementation authorization. Q0-Q7,issuer,U0-U7,E0-E8,root/C8,Model E and ED08/ED11 remain unchanged. No concrete protocol/API/codec/source is selected. External primary specifications were not available locally and no network was used;exact protocol review remains a gate.

## AT0 Direction

Prefer investigating B: an EXISTING protocol for fresh signed UTC interval evidence,acquired on demand from an explicitly trusted deployment source and verified through the administrative issuer backend. This is a candidate for the smallest conceptual integration,not measured proof of smallest code/memory cost. Do not invent a signed JSON/blob/protocol. NTS remains alternative A if its deployed backend offers better evidence or reuse;qualified existing local hardware may be preferable where already present. No new server/service is authorized.

## AT1 One authority

The same root/policy must govern source identity,verification credentials,time-observation purpose,configuration revision,validity,status and invalidation. An external source is a declared trust dependency,not another administrative root/issuer inside Accelerator. Issuer/package signer/CA is not automatically time authority. No trust store,registry,journal,worker or global time manager. A key/endpoint from a reply cannot enroll itself;PDG/decoded data/digests/booleans are not authority.

No canonical time-source credential/configuration model has been demonstrated in current code. Map existing accepted fields before writing a binding. If insufficient,propose a minimal versioned root/layout extension for separate acceptance;do not add fields/reserved bytes to PACROOT8/v3,PDG1,CP3 or historical formats by inference. This candidate instantiates no source-key entry or new purpose enum.

## AT2 Independent bootstrap

Establish source verification identity through an approved administrative ceremony and protected canonical configuration,independent of the observation under validation. Never use the response key or TLS certificate as its sole anchor. Do not bypass certificate dates using unqualified OS time. A source credential/status path requiring qualified UTC needs independently justified bootstrap;it cannot establish its authority solely by its own observation. Bare-key or other trust modes need exact protocol/trust review;none is implicitly authorized. Existing bootstrap/CURRENT/recovery guards remain distinct and currently block operational installation where facts are absent.

## AT3 Freshness/replay

A concrete existing-protocol profile must authenticate complete response semantics,source identity and request correlation. Require a fresh unpredictable challenge from qualified entropy,one outstanding operation association,strict matching and governed single consumption. Exact challenge representation/length,generation,algorithms,signed fields,limits and replay rules come from separate profile review;no defaults here. Signing a nonce alone does not prove UTC was freshly sampled:causal capture semantics must be established by protocol and source qualification.

Old signed evidence is auditable DATA,not current time. Offline imported tokens without proved freshness/bounded age are denied. Restart/VM restore/clone/entropy reuse need replay/fencing analysis;no new journal or restored observation authority.

## AT4 Absolute bounds

Qualify finite UTC error,scale,units,quantization,health and applicability,not merely cryptography. A signed error is an authenticated assertion;accuracy still requires deployment qualification. Define conservative exact conversion for era,leap/smear and native representation. Ambiguity/overflow denies;no silent scale conversion,grace or backdating.

Conceptual acquisition example,NOT a new Q3 formula:if a trusted source bounds UTC at a causally proved generation instant by [Ls,Us],that instant is between local m0/m1,and qualified PAL gives maximum elapsed du for the ENTIRE acquisition interval,a conservative interval at m1 is [Ls,Us+du],with checked arithmetic. Include verification latency to m1. Never use half RTT or assume symmetric paths. Missing causality/source error/PAL bounds means no observation. Delay widens the interval and may deny effects. Exact wire-to-Q1 derivation remains a concrete-profile gate.

## AT5 PAL/offline

Keep rho in direction real UTC/PAL and additive E including d=0. Network authentication does not qualify the local oscillator/read error. Obtain finite target-specific rho,E and exclusive D with applicability evidence;no guessed values/holdover duration. Core remains portable without Win32/CNG. Every target needs equivalent evidence.

No continuous internet requirement:approved LAN/local or periodically reachable sources are possible. Q0-Q7 still permit only SAME boundary/operation propagation after acquisition. This is not a reusable process clock/cache:new authorizing boundaries require fresh observations. Same-operation offline completion within qualified lifetime may be possible;indefinite offline approval/new operations without fresh evidence/post-restart authority are denied. No favorable averaging or automatic source failover after inconsistency.

## AT6 Continuity/rollback

Use existing owner history and qualified continuity events. Manual wall-clock changes do not replace source time. Monotonic/native UTC regression,inconsistent intervals,source/key/config/revision change,expired quality,suspension/resume without continuity proof and UNKNOWN invalidate. Require requalification/new operation,no retry until favorable. Restart restores neither observation,CURRENT,UPDATE approval nor high-water/fencing. Signed history/hardware counters do not establish latest root revision.

## AT7 Ownership/Stop

Acquire/verify under the existing administrative backend/owner before E0. Reuse existing scheduling/transactions only with factual completion,ownership and reservation;no new worker is required/authorized. E0 acquire never performs network/device I/O. Protect credentials,request/response,verification/qualification contexts and root pins throughout lifetime. Root pins do not protect external buffers by inference.

Stop prevents effects without blocking for cancellation. In-flight/uncertain work remains reachable/charged until factual completion,quiescence and close;timeout invalidates applicability,not ownership. Preserve RELEASE_INVALID,exactly-once and no fallback. UTC alone never authorizes issuer status,UPDATE,signature,CURRENT,TRUST/M1 or admission.

## AT8 Profile/ED11 gates

Before implementation:local primary specification and exact version/errata;existing protocol/provider selection;algorithms,trust bootstrap,challenge/entropy/replay,authenticated domain,endpoint/transport policy,encoding/limits/errors/deadlines;qualified error/PAL;root binding/layout if needed;C89/VC6/platform/lifetime proof;full transitive ED11. None is accepted by this candidate.

Count transport/TLS or signature state,credentials/keys,entropy,proofs/transcripts,cookies where applicable,parsers,request/response,history,snapshots/pins,stack/provider/kernel and retention. Reuse is not zero cost. Windows3MiB only explicit,legacy/other/per-client limits intact. Full conservative scenario already exceeds3MiB. No allocation/quota authorized;relative A/B/C costs remain unmeasured.

## AT9 Owner decision

Request architecture direction,preferably B subject to the above gates,or A/C with equivalent evidence. Later acceptance must follow governance and cannot be treated as profile/source/implementation qualification. No new wire,source configuration,root layout,clock adjustment,sync service or production effect. Missing evidence returns NOT_SUPPORTED. All current authority/admission gates remain DENY.

[Comparison](../integration/authenticated-time-source-review-20261021/comparison-and-risks.md) and [plan](../integration/authenticated-time-source-review-20261021/minimum-plan-and-open-gates.md) are review evidence,not accepted protocol contracts.
