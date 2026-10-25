---
status: proposed
revision: 1
scope: candidate existing-protocol to Q0-Q7 verification boundary
---
# PT0-PT7:conditional protocol-source profile

This focal proposal accompanies AT0-AT9 Revision2. No field/API/wire/algorithm/layout is accepted or implemented. R1 denotes the official reference in protocol-matrix.md;that record is the sole concise protocol-fact summary. Prioritize its study as the owner-preferred B candidate,not operational selection. No vendor,key or endpoint is assigned.

## PT0 Reference and assurance

Any future R1 profile must use the exact published wire/version and full validation/acquisition/security model identified by the official specification,not a draft layout,custom envelope or silent reduced-assurance mode. Resolve normative updates/errata and all implementation limits before acceptance. If the deployment cannot provide the required source model,return NOT_SUPPORTED;do not claim its security properties for a smaller campaign. R3 remains an independently reviewed alternative;no combined synchronizer is proposed.

## PT1 Canonical trust

Preestablish source verification identity,purpose and configuration/status revision through the one administrative root/policy. First-contact trust must not come solely from the reply,discovered server list,system clock or package signer. Long-term trust bootstrap,rotation,withdrawal and compromised-key response require protected administrative evidence independent of the observation under validation. Unknown status denies;offline revocation freshness cannot be fabricated. Do not convert protocol-local delegation to issuer grants. No new trust store/CA/registry. If current root fields cannot express the binding,accept a minimal separate layout contract before persistence.

## PT2 Operation and evidence

Bind the whole acquisition campaign to the same existing owner,source profile,root CURRENT/pin,process/invocation,boundary,candidate/predecessor and entropy provenance. Match the existing protocol's complete request correlation and authenticated proof. Preserve all evidence required by that model;one packet is not the whole campaign. Do not use nonce freshness alone as reading freshness. New operation/restart/clone cannot reuse prior positive evidence or entropy state. No new worker,producer or journal.

## PT3 Q1 construction

Validate protocol bytes first,then independent source trust/status and semantic/time/quality facts. Only with a qualified source interval at a proved causal reading instant and qualified PAL bracket may the conditional receipt-interval proof in review-bootstrap-and-plan.md construct [L,U] at m1. Include source resolution/quantization,conversion and entire acquisition/verification delay. Do not treat a signed accuracy assertion as calibration proof. Preserve endpoint semantics and project validity intervals separately. Unknown error/scale/era/leap handling,underflow or overflow denies;no saturation or free tolerance.

## PT4 Q2/Q3 use

Require actual target/backend/source provenance,quality and finite rho,E,D. Keep rho in real UTC/PAL direction and E including d=0. Missing PAL guarantees cannot be replaced by protocol assertions about a remote oscillator. D is exclusive,monotonic and derived from qualified applicability,not a caller TTL;its derivation must not mix civil and monotonic units. Revalidate m1<=m<D,source status,profile,CURRENT,pins and operation immediately before effect. New authorizing boundary requires fresh acquisition. No indefinite offline clock/cache.

## PT5 Continuity/status

Manual steps,source/configuration/key/revision changes,regression,inconsistent intervals,unknown health and unproved suspension continuity invalidate the operation. Define measurable event coverage in the native adapter;generic successful reads are insufficient. No favorable source selection or automatic fallback. Fresh measurements never prove root latest revision/issuer uniqueness. Restart/archive integrity does not restore authority or bypass U6/fencing.

## PT6 Lifetime/ED11

Acquisition and crypto belong under existing administrative ownership before E0 exposure. Protect external buffers/keys/status references by their owners,not root pins alone. Stop prevents effects without waiting for I/O/crypto;uncertain completion retains graph/credits until factual quiescence/closure. Timeout invalidates applicability,not ownership. Keep E0 acquire synchronous borrow-only and RELEASE_INVALID without attempt/effect.

Compile/probe full memory before hunks:source credential revisions,all campaign buffers/transcripts,crypto/transport/status,history,candidate/predecessor,pins,snapshots,stack/native/provider and retentions. No new quota or default Windows3MiB selection. Missing bounds or excess capacity stop dependent integration.

## PT7 Acceptance gates

Still UNSET/unqualified:actual source/key/status/bootstrap representation;exact transport/version/backend profile;source calibration/health/fault model;max reply/proof sizes,campaign storage/concurrency,deadlines/backoff/age/uncertainty policy;PAL rho/E/continuity;primitive availability/C89/VC6 compatibility;full operational ED11. These are mandatory named gaps,not permissive defaults. Filling them requires separate owner/normative review and exact functional patch approval. Native tests/network authorization are separate. UTC_SOURCE=NOT_QUALIFIED;all issuer/UPDATE/CURRENT/signature/recovery/TRUST/admission gates stay DENY.
