---
status: proposed
revision: 2
date: 2026-10-18
scope: private Browser–Accelerator evidence correlation only
---
# Mode C pair evidence — private schema v2 (PROPOSED)

Documentary authority: PACC-PHASE6-MODE-C-PAIR-EVIDENCE-V2-PROPOSED-SUBMISSION. This proposal preserves the previously prepared representation. It is not an accepted ADR, resource permit, implemented codec, live publication protocol or authorization to implement one. No normative decision is created or modified.

CLIENT_ADMISSION=DISABLED. SITE_TLS_ADMISSION=DENY. ED11_ADMISSION=DENY. ADR-0052/C8 CONTROL does not extend to Mode C. Provider/kernel UNKNOWN remains UNKNOWN. PST 0.6.5 remains candidate until completion of Phase 6.

## Scope and compatibility

Name: PACC_MODE_C_PAIR_EVIDENCE; version: 2. The artifact is a private offline correlation receipt, separate from the historical S6 in-memory ledger version 1. It is not PACC wire. Preserve historical layouts, names, readers and exports; unknown versions reject, with no automatic migration or reinterpretation. No new collector, supervisor, credit manager or admission mechanism is proposed.

Reuse existing producers, provenance, gates, local accounting and post-quiescence export. Missing producers or factual association remain explicit dependencies, never synthetic replacements. A receipt cannot grant capabilities, publish policy CURRENT, issue epoch, mutate reserves, refund credit, change ownership or qualify native behavior.

## Canonical carrier and bounds

UTF-8 JSON without BOM; LF ending, compact separators and lexicographically sorted object keys. Arrays retain their declared order. Reject duplicate keys, unknown fields, unsupported enum values, malformed encoding, invalid ranges and missing required fields. Run IDs and enum strings are printable ASCII; run IDs are 1..127 characters, preserved exactly. Hashes are exactly 64 lowercase hexadecimal characters. Principal16 is exactly 32 hexadecimal characters encoding the existing resolver's original 16 bytes, never a certificate fingerprint or derived identity.

All counts, sizes, timestamps and scalar numeric identifiers are unsigned decimal strings: no signs, leading zeros except zero, exponents, whitespace or fractions. Check unsigned 64-bit overflow before arithmetic and the original narrower type's range where applicable. Do not convert through floating point. Zero never establishes authority; validity follows the original field contract. Null is allowed only where specified below.

Exactly two participants, ordered BROWSER then ACCELERATOR. Maximum serialized artifact size: 32768 bytes. Reject excess without truncation. This export bound does not allocate or approve a 32 KiB owner buffer. Future implementation must stream through existing bounded export facilities and demonstrate scratch, layout, multiplicity, retained coexistence and quota compliance before integration.

No pointers, handles, tokens, attachment authorities, keys, credentials, certificate bodies or sensitive payloads are serialized. Identity is not inferred from payload hashes or timestamps.

## Top-level fields

Exact keys: format, version, declared_pair_manifest_sha256, participants, correlation, integrity. format is PACC_MODE_C_PAIR_EVIDENCE; version is string 2. The manifest hash references actual immutable administratively selected bytes. It is integrity/reference evidence, not authentication of either participant. Do not fabricate a run, invocation or manifest to fill a required field; absence makes artifact production unavailable.

Each participant has exact keys: role, run_id, identity, configuration, authorization, observation, accounting, lifecycle.

## Identity matrix

identity keys: process_id, process_creation_filetime, owner_thread_id, root_invocation, root_identity, executable_sha256, sdk_inventory_sha256, sdk_profile, build_configuration.

| Field | Factual source and comparison |
|---|---|
| process_id and process_creation_filetime | Same actual process lifetime; PID equality alone is insufficient |
| owner_thread_id and root_invocation | Existing durable owner; no slot, address or ordinal substitution |
| root_identity | Existing typed identity as {type,value}, exact contract length/value |
| executable_sha256 | Actual selected executable bytes, independently bound to that process lifetime by existing provenance machinery |
| sdk_inventory_sha256 | Actual governed normal inventory; version equality alone is insufficient |
| sdk_profile | NORMAL_PST_065_API3_SPI4 only, never R2 TEST_ONLY |
| build_configuration | DEBUG or RELEASE, exactly matching the selected image |

An expected plan hash is not a measured executable identity. The existing Accelerator target byte array encodes process/thread/creation/invocation; it must not be relabelled executable SHA-256. If no factual producer supports a required binding, report that implementation dependency instead of inventing identity. Typed identities retain their existing representation; no padding, truncation, normalization or hashing into a substitute identity.

## Configuration and authorization

configuration keys: committed, domain, revision, policy_current, policy_identity, epoch, site_tls_profile_ref, site_tls_profile_revision, site_tls_profile_identity, configuration_sha256.

committed/policy_current are YES, NO or UNKNOWN. Domain and policy/profile identities are typed {type,value} objects or null before factual existence. Scalar IDs and configuration hash are null before factual existence. CURRENT requires factual commit and the matching revision/epoch. Browser association must be factually received/validated, not an independently issued server epoch. Hash actual governed configuration bytes, not pointers or unrelated subsets. A present profile is not authorization to use it.

authorization keys: principal, sessions, operation, generation, grants, binding_status.

principal is null or {type:AUTHENTICATED_PRINCIPAL16,value:<exact 32 hex characters>}. sessions is an ordered array, maximum 2, of {purpose,id,generation}; purpose is PARENT or C2, with existing typed Session identity and generation. operation/generation are jointly null or jointly present with their exact existing types. grants is an array, maximum 2, of {capability,version,snapshot_identity}, representing only factual applicable 2/1 and 3/1 grants; never serialize secret grant material. binding_status is UNAVAILABLE, OBSERVATION_ONLY or FACTUALLY_ASSOCIATED.

Baseline may lack Principal, Session, operation, grants and epoch. That is observation only. FACTUALLY_ASSOCIATED requires actual same-context Session/Principal/root/policy/epoch/grant/profile validation; it does not imply ED11 approval. No field creates authority retrospectively. Existing identities must not be presented as absent to avoid checks.

## Observation, freshness and provenance

observation keys: origin, phase, local_clock_domain, started_ms, finished_ms, expires_ms, series_id, sequence, baseline_private_bytes, private_bytes, peak_private_bytes, handles, physical_available_bytes, commit_available_bytes, provider, kernel, incomplete.

origin is NATIVE or SYNTHETIC. phase is BASELINE, PREPARED, ACTIVE, DRAINING, TEARDOWN, RETAINED or SHUTDOWN_PENDING. Clock/series identity comes from existing producers. Unknown timing/series fields are null and cannot support correlation. incomplete is YES or NO, copied from factual capture health; missing health is conservatively YES.

Each resource metric is {classification,value,evidence_ref}. classification is UNKNOWN, DIRECTLY_MEASURED, BOUNDED_BY_CONTRACT or EMPIRICALLY_QUALIFIED. UNKNOWN requires null value and evidence_ref. Other classes require a valid numeric value and an actual immutable supporting evidence reference, preserving its existing typed identity. A filled field, fixture or process pool observation never promotes provider/kernel to known or qualified.

Baseline and peak must belong to the same factual process/configuration/origin series. Preserve explicit historical series across phase changes; do not silently reset an operation baseline. Synthetic and native samples never merge. Compare timestamps only within a verified local clock domain, never to establish interprocess causal order. Freshness and duration requirements must come from the applicable governed profile and approved evidence, not the S6 historical 512 MiB margin or inherited C8 exception. Missing applicable limits denies approval. Expiry, regression, collection failure and overflow preserve existing sticky incompleteness; serialization cannot clear it.

## Local accounting and lifecycle

accounting keys: local_reservation_identity, local_managed_limit_bytes, local_reserved_bytes, local_live_bytes, local_retained_bytes, local_credit_state, local_credit_owner, control_cancel_final_reservation_ref.

Identities/owner references preserve existing typed ledger/Session/job/result ownership, never addresses. Missing factual references are null. Limit is decimal string 2097152. States: RESERVED, LIVE, RETAINED, RELEASED or UNAVAILABLE. Amounts are typed metrics as above; unknown is not zero. Preserve each ledger's metric meanings: reserved/live/retained amounts are not assumed disjoint. Charge embedded/shared storage once within its actual local owner. Never sum two processes against one process limit or mint a global budget from their numbers. This revision provides correlation, not host-credit escrow.

lifecycle keys: stop_state, quiescence, disposition, dependencies_pending, local_teardown_evidence_ref. stop_state is {type,value} preserving the existing owner's state or null if unknown; future adapters must provide an explicit exhaustive mapping and reject unmapped values. quiescence is UNKNOWN, PENDING or CONFIRMED. disposition is UNKNOWN, NOT_ACQUIRED, LIVE, DESTROYED, PRECONDITION_RETAINED or CLOSE_UNRESOLVED_RETAINED. dependencies_pending is the factual bounded count or null if unknown. Teardown reference is null until factual evidence exists.

DESTROYED requires native/PST/lease/callback/queued-result proof appropriate to every acquired local obligation covered by the charge. A remote FINAL, close attempt, timeout, process exit or synthetic flag is insufficient. Preserve original retained charges and graph; no reset, eviction, alternate cleanup or destructive retry. Cleanup callbacks may return existing references, never create operational authority. Receipt export has no refund side effect.

## Correlation and integrity

correlation keys: administrative_root_ref, peer_association_ref, state, missing. References are existing typed administrative/run records or null, not new authorities. state is INCOMPLETE or CORRELATED_NOT_ADMITTED only. missing is a sorted unique list of absent required field paths or producer dependencies.

CORRELATED_NOT_ADMITTED requires complete factual participant identities, an existing supported peer association, matching applicable root/policy/epoch/profile facts, and valid relevant local observations. SYNTHETIC receipts can only be INCOMPLETE. Structural decoding or matching expected hashes is not sufficient. Without a proven association producer, peer_association_ref is null and missing identifies it. Neither state admits operations.

integrity keys: algorithm, body_bytes, body_sha256. Algorithm SHA256. Body is the canonical top-level object with integrity omitted, serialized under these rules. Check its exact byte count and hash; repeated export of an immutable snapshot is byte-exact. Integrity is not authenticity, completeness, peer liveness or approval. Short writes, mismatches and unknown formats reject; failure cannot mutate functional ownership or gates.

## Non-goals and open implementation dependencies

No live IPC, mapping namespace, administrative trust protocol, host-credit issuer or new admission publication is authorized by this proposal. Do not reuse the historical mapping as identity proof. Locate/reuse existing factual binary/root/peer association and Browser evidence sources before implementing adapters; report missing sources explicitly. Any newly required normative decision or operational authority must be presented separately. The detailed schema remains PROPOSED pending owner review.
