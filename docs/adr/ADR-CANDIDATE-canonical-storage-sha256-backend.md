---
status: proposed
revision: 1
decision-date: 2026-10-22
scope-level: project
scope-target: private canonical storage SHA-256 dependency and call-site boundary
---
# Candidate: deterministic canonical storage SHA-256

Owner task PACC-PHASE6-CANONICAL-STORAGE-SHA256-DECISION-1 directs an
Accelerator-owned minimal pinned BearSSL SHA-256 snapshot. This candidate records
the delimited direction; it is NOT ACCEPTED and authorizes no implementation.
The review found incomplete upstream provenance and shared issuer callback scope.

## Proposed dependency boundary

Use unchanged BearSSL SHA-256 arithmetic through br_sha256_init/update/out,
sha2small.c and the two required codec translation units dec32be.c/enc32be.c.
Package the exact closed header/helper set and MIT license/original notices.
Pin upstream version/revision, independently verifiable origin and source hashes,
plus any compatibility patch separately. The Browser local snapshot is a source
candidate, not sufficient proof of upstream revision or an Accelerator build
input. No TLS/RSA library, Browser checkout, PST SDK/runtime or network fetch at
build time. No generic crypto API or alternative crypto framework.

## Proposed scope rules requiring exact completion

Storage-only whole-image/component/checkpoint/predecessor hashes in preparation,
immutable backing capture/reborrow, candidate binding and pre-effect packet
validation are IN_SCOPE. Native byte-identity/full-file CAS comparison remains
unchanged; SHA-256 is not substituted for byte equality or CURRENT authority.

store03_hash is IN_SCOPE. papacc_store_hash and digest(record,480) are
SHARED_REQUIRES_SPLIT: their existing unrelated legacy consumers must not be
silently rerouted. A later exact patch must distinguish canonical storage callers
from historical API paths without creating another owner or resource manager.

papacc_configuration_issuer_hash06 is SHARED_REQUIRES_SPLIT. Storage callers may
receive a private deterministic callback only after enumerating their full
closure, preserving context, owner guards, Stop and status semantics. The issuer
entry itself, its unrelated signing/issuance/history operations and APIs must not
be globally replaced. Unmodified generic issuer codecs can receive an approved
storage-specific SHA callback at a storage call site; that does not change issuer
authority or permit modifying native certificate validation by inference.

store02_hash standalone historical mode is OUT_OF_SCOPE. Cross-version storage
validation reaching it, if found during the exact patch, requires explicit scope
completion before migration. Accepted v4 UPDATE representation is from v3/v4;
no v2 predecessor or historical format reinterpretation is introduced.

Current packet11 validates version3 only. A complete implemented v4 worker call
closure is UNKNOWN, because that worker integration remains gated/unimplemented.
This decision cannot assert that an absent v4 path has been provider-qualified.
Bootstrap, recovery, productive issuer/signature primitives, certificate decode/
verification, Roughtime and manual-package crypto are OUT_OF_SCOPE unless an exact
storage hash call-site split is separately enumerated. No global substitution.

## Invariants and qualification

SHA-256 input bytes, length, domains, output and persisted hashes remain exact.
No ROOT06/v2/v3/v4, CP/PDG/TSC offset/version/format changes. No fallback to BCrypt.
Retained preexisting BCrypt resources remain owned; a new backend cannot reset,
release or relabel them. Same root/worker, CAS UNKNOWN, pins, Stop and factual
quiescence govern all other buffers/resources. No CURRENT, trust or signing grant.

Measure context, nested stack, scratch, code/const data, CRT/compiler helpers,
initialization, concurrency and retained coexistence. Avoid message-sized heap or
provider handles in the hash closure; verify linked symbols and exact platform
builds. Source arrays are not a full compiled peak. No quota change or assumed
savings. VC6/modern differential SHA vectors, streamed/empty/large inputs,
Stop/no-output-on-failure and historical codec/CAS tests remain later gates.

## Acceptance condition

STOP before acceptance until upstream source provenance/version pin and the
storage-versus-issuer/legacy call-site split are complete and officially reviewed.
No ACCEPTED record or implementation authorization follows from this candidate.
See [focused review and exact unresolved call](../integration/canonical-storage-sha256-decision-20261022/review-and-scope.md).
