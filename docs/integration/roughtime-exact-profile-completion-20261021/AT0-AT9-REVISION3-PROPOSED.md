---
status: proposed
revision: 3
decision-date: 2026-10-21
scope-level: project
scope-target: authenticated time source under Q0-Q7
supersedes: []
---
# Candidate: authenticated UTC observations through the existing issuer backend

TASK=PACC-PHASE6-AUTHENTICATED-TIME-AT0-AT9-REVIEW-1. AT0-AT9 is PROPOSED for owner review,not ACCEPTED or an implementation authorization. Q0-Q7,issuer,U0-U7,E0-E8,root/C8,Model E and ED08/ED11 remain unchanged. The companion PT Revision2 proposes RFC 10049 version 1 and bounded UDP acquisition; no supplier, API or codec implementation is selected. Relevant official protocol sections were reviewed as documented in verified-references.json. Documentary HTTPS research is distinct from operational time traffic. Full concrete-profile/implementation/errata qualification remains a gate;no source is qualified by this review.

## AT0 Direction

Evidence refinement: prioritize R1 in protocol-matrix.md for the next protocol-profile review. No provider is selected. Preserve the complete referenced security model;do not silently reduce the assurance model to one signer or claim a measured footprint advantage.

Prefer investigating B: an EXISTING protocol for fresh signed UTC interval evidence,acquired on demand from an explicitly trusted deployment source and verified through the administrative issuer backend. This is a candidate for the smallest conceptual integration,not measured proof of smallest code/memory cost. Do not invent a signed JSON/blob/protocol. NTS remains alternative A if its deployed backend offers better evidence or reuse;qualified existing local hardware may be preferable where already present. No new server/service is authorized.

## AT1 One authority

The same root/policy must govern source identity,verification credentials,time-observation purpose,configuration revision,validity,status and invalidation. An external source is a declared trust dependency,not another administrative root/issuer inside Accelerator. Issuer/package signer/CA is not automatically time authority. No trust store,registry,journal,worker or global time manager. A key/endpoint from a reply cannot enroll itself;PDG/decoded data/digests/booleans are not authority.

No canonical time-source credential/configuration model has been demonstrated in current code. Map existing accepted fields before writing a binding. If insufficient,propose a minimal versioned root/layout extension for separate acceptance;do not add fields/reserved bytes to PACROOT8/v3,PDG1,CP3 or historical formats by inference. This candidate instantiates no source-key entry or new purpose enum.

## AT2 Independent bootstrap: exact proposed bare-key boundary

Use only an explicitly authorized long-term Ed25519 public key, exactly 32 raw bytes, bound to source identity, time-observation purpose, protocol profile revision and administrative status by the existing canonical root/policy. The initial key and its expected fingerprint must arrive in a separately authenticated administrative ceremony; compare SHA-256 over those exact 32 bytes with the independently authenticated expected value before proposing installation. A matching digest identifies bytes but is not authorization. Operator identity, authority for this configuration change, source independence, evidence origin and protected root revision must also be proved by existing administrative mechanisms. A discovered list, endpoint name, response CERT, OS clock or package signer cannot supply that authorization.

No prior UTC is needed for mathematical verification of the pinned bare key or its protocol delegation: perform RFC 10049 validation against the authenticated response timestamp, separately from administrative source status. This avoids using an unqualified local civil clock to validate X.509 dates. It does not prove actual UTC, absence of compromise, administrative freshness or source correctness. The first observation additionally requires a protected, currently applicable administrative trust/status decision whose authority does not depend solely on that observation. If its expiry/status needs qualified UTC, obtain independently qualified evidence; otherwise DENY. There is no date bypass, timeless positive status or self-qualification.

The RFC's temporary delegation is accepted only under the authorized long-term pin and exact protocol checks; it never enrolls another long-term key or delegates issuer privileges. Long-term rotation, replacement, withdrawal or compromise requires a separately authorized canonical revision. No automatic key overlap or old-key fallback is allowed: invalidate all operations using the changed profile before activating a replacement. Unknown or revoked status, missing independent revocation evidence or suspected compromise denies; a valid signature cannot clear that state. Requalification of source correctness and status is required before a new operation.

Protect the entire trust revision and external evidence with their existing owners and the applicable canonical CURRENT/pin. Revalidate identity, purpose, status, revision and operation before acquisition and immediately before effect. A historical pin protects memory, not latest-revision authority. Stop prevents effects without releasing in-flight evidence. Retain changed/withdrawn revisions while any borrow or uncertain operation exists; release only at factual quiescence. Restart/restore never restores an observation, CURRENT, approval or a positive trust/status result; fresh governed revalidation is mandatory.

This defines the proposed trust contract, not an implemented source. No current canonical source-key/status representation has been proved. Map existing accepted fields first; if insufficient, a separately accepted minimal versioned layout is required before persistence. Do not modify PACROOT8/v3, PDG1, CP3, reserved bytes or historical codecs here. Missing administrative source or CURRENT remains DENY, not a reason to invent one.

## AT3 Freshness/replay

Treat an acquisition campaign as distinct from an individual packet. Preserve every request/response and causal correlation required by the selected protocol until verification/quiescence;serial transport alone does not prove that earlier evidence storage can be reused. Signed-data freshness and freshness of the underlying time reading are separate obligations. Use the selected existing protocol rules rather than inventing a new challenge envelope.

A concrete existing-protocol profile must authenticate complete response semantics,source identity and request correlation. Require a fresh unpredictable challenge from qualified entropy,one outstanding operation association,strict matching and governed single consumption. Exact challenge, wire validation, limits and replay rules are proposed in companion PT Revision2; no fallback defaults are authorized. Signing a nonce alone does not prove UTC was freshly sampled:causal capture semantics must be established by protocol and source qualification.

Old signed evidence is auditable DATA,not current time. Offline imported tokens without proved freshness/bounded age are denied. Restart/VM restore/clone/entropy reuse need replay/fencing analysis;no new journal or restored observation authority.

## AT4 Absolute bounds

The acquisition example below is a conditional inclusion proof,not a claim that cryptography establishes clock correctness. Prove the source reading time is inside the locally bracketed interval;cover quantization,scale conversion and verification delay without underflow/overflow. Distinguish native protocol interval endpoint rules from issuer/X509 [notBefore,notAfter) rules. Missing source calibration/health,qualified PAL or endpoint conversion invalidates the proof.

Qualify finite UTC error,scale,units,quantization,health and applicability,not merely cryptography. A signed error is an authenticated assertion;accuracy still requires deployment qualification. Define conservative exact conversion for era,leap/smear and native representation. Ambiguity/overflow denies;no silent scale conversion,grace or backdating.

Conceptual acquisition example,NOT a new Q3 formula:if a trusted source bounds UTC at a causally proved generation instant by [Ls,Us],that instant is between local m0/m1,and qualified PAL gives maximum elapsed du for the ENTIRE acquisition interval,a conservative interval at m1 is [Ls,Us+du],with checked arithmetic. Include verification latency to m1. Never use half RTT or assume symmetric paths. Missing causality/source error/PAL bounds means no observation. Delay widens the interval and may deny effects. The exact conditional wire-to-Q1 derivation is proposed in companion PT3 Revision2; factual premises remain mandatory.

## AT5 PAL/offline

Keep rho in direction real UTC/PAL and additive E including d=0. Network authentication does not qualify the local oscillator/read error. Obtain finite target-specific rho,E and exclusive D with applicability evidence;no guessed values/holdover duration. Core remains portable without Win32/CNG. Every target needs equivalent evidence.

No continuous internet requirement:approved LAN/local or periodically reachable sources are possible. Q0-Q7 still permit only SAME boundary/operation propagation after acquisition. This is not a reusable process clock/cache:new authorizing boundaries require fresh observations. Same-operation offline completion within qualified lifetime may be possible;indefinite offline approval/new operations without fresh evidence/post-restart authority are denied. No favorable averaging or automatic source failover after inconsistency.

## AT6 Continuity/rollback

A valid delegation interval is not evidence that its private key was never compromised. Independent key status/rotation/withdrawal and source trust revisions remain required. Disagreement handling cannot be replaced by picking a favorable response or assumed majority accuracy. Root freshness,fencing and source replay history remain distinct;no state restoration through signed archives.

Use existing owner history and qualified continuity events. Manual wall-clock changes do not replace source time. Monotonic/native UTC regression,inconsistent intervals,source/key/config/revision change,expired quality,suspension/resume without continuity proof and UNKNOWN invalidate. Require requalification/new operation,no retry until favorable. Restart restores neither observation,CURRENT,UPDATE approval nor high-water/fencing. Signed history/hardware counters do not establish latest root revision.

## AT7 Ownership/Stop

Acquire/verify under the existing administrative backend/owner before E0. Reuse existing scheduling/transactions only with factual completion,ownership and reservation;no new worker is required/authorized. E0 acquire never performs network/device I/O. Protect credentials,request/response,verification/qualification contexts and root pins throughout lifetime. Root pins do not protect external buffers by inference.

Stop prevents effects without blocking for cancellation. In-flight/uncertain work remains reachable/charged until factual completion,quiescence and close;timeout invalidates applicability,not ownership. Preserve RELEASE_INVALID,exactly-once and no fallback. UTC alone never authorizes issuer status,UPDATE,signature,CURRENT,TRUST/M1 or admission.

## AT8 Profile/ED11 gates

Review record R1-R7 distinguishes verified protocol evidence from project requirements. Exact parser/profile validation,full primary-text byte archive and complete errata/update review are still prerequisites to implementation acceptance. Count all campaign transcripts and status/rotation evidence,including retained intermediates;no resource limit is inferred from a small base message.

Before implementation:local primary specification and exact version/errata;existing protocol/provider selection;algorithms,trust bootstrap,challenge/entropy/replay,authenticated domain,endpoint/transport policy,encoding/limits/errors/deadlines;qualified error/PAL;root binding/layout if needed;C89/VC6/platform/lifetime proof;full transitive ED11. None is accepted by this candidate.

Count transport/TLS or signature state,credentials/keys,entropy,proofs/transcripts,cookies where applicable,parsers,request/response,history,snapshots/pins,stack/provider/kernel and retention. Reuse is not zero cost. Windows3MiB only explicit,legacy/other/per-client limits intact. Full conservative scenario already exceeds3MiB. No allocation/quota authorized;relative A/B/C costs remain unmeasured.

## AT9 Owner decision

Revision3 is a separate proposed artifact with the exact normative correction patch against preserved Revision2; the companion PT Revision2 supplies the bounded profile. It is not incorporated into any accepted ADR. Documentary references refine the recommendation,not operational authority. Candidate-profile terms are supplied by companion PT Revision2; missing factual qualification keeps NOT_SUPPORTED.

Request architecture direction,preferably B subject to the above gates,or A/C with equivalent evidence. Later acceptance must follow governance and cannot be treated as profile/source/implementation qualification. No new wire,source configuration,root layout,clock adjustment,sync service or production effect. Missing evidence returns NOT_SUPPORTED. All current authority/admission gates remain DENY.

[Protocol matrix](../authenticated-time-at0-at9-review-20261021/protocol-matrix.md) and [review/plan](../authenticated-time-at0-at9-review-20261021/review-bootstrap-and-plan.md) are review evidence,not accepted protocol contracts.
