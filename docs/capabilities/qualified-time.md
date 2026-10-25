# Qualified time: current implementation state

Normative authority: [corrected Roughtime AT/PT](../adr/DECISION-roughtime-at-pt-corrected-rfc10049-20261021.md), qualified UTC Q0-Q7 and existing issuer/root/Model E/ED11 contracts.

The private C89 Roughtime DATA core implements bounded packet parsing, public proof checks, interval calculations and offline exchange-state controls. The isolated modern backend uses the existing pinned OpenSSL3.5.8 for public SHA512/Ed25519. VC6 and MSVC Debug/Release component tests pass; no native UTC source, UDP producer or operational trust is enabled. Public example signature limitations remain recorded. A complete six-pair2048-byte LAB transcript now passes real public crypto verification; it is not a factual UTC source. NT4,box and ISA remain unqualified.

papacc_issuer_qualified_utc_read remains NOT_SUPPORTED. DATA_OK is not CURRENT, qualified UTC or issuer status. Every source/admin/PAL/quality/lifetime/capacity gate remains required. Stop does not release retained resources. No quotas, clocks, Browser,PST or wire changed. ED11 remains PARTIAL; nominal margin is not reservation.

[Current proof and handoff](../integration/roughtime-portable-adapter-20261021/implementation-proof-and-handoff.md) | [Closeout](../integration/roughtime-portable-adapter-20261021/closeout.md).


## Full transcript follow-up (2026-10-21)

An explicitly owner-authorized and officially reviewed offline operation generated ephemeral TEST keys and archived only public RFC packets. Six2048-byte requests and six480-byte responses pass nonce-chain, delegation, Ed25519, Merkle and interval DATA checks. Canonical VC6/modern Debug/Release tests pass without warnings; native crypto qualification is scoped to modern Windows x64 OpenSSL3.5.8, not NT4/box/ISA. Consumer builds never regenerate the fixture.

The known isolated test heap charge is406300 bytes (52904 graph plus353396 instrumented backend peak). Full transitive stack, native transport, factual sources/PAL and operational coexistence/retention remain unqualified. No activation, quota change or positive trust follows. See [full-transcript closeout](../integration/roughtime-full-transcript-ed11-20261021/closeout.md) and its ED11 census. ED11 total remains PARTIAL_UNTIL_FULL_QUALIFICATION.


## Canonical source contract (2026-10-21)

[TS0-TS8 accepted normative contract](../adr/DECISION-time-source-ts0-ts8-20261021.md) defines the distinct TIME_SOURCE purpose and same-root v4/TSC1/TSR1/CP4 representation. Original proposal and historical formats remain unchanged. No codec, source producer, native transport, factual authorization/status/CURRENT, native pin lifetime or operational ED11 is qualified by acceptance. Source/UTC/issuer/UPDATE/admission remain denied or unqualified; ED11 PARTIAL. Implementation needs its own authorization, exact patch review and prepatch lifetime/capacity proof.


## TIME_SOURCE persistence codecs (2026-10-21)

Private PACROOT8/v4,TSC1/TSR1/CP4 DATA codecs now reuse the historical SNT/PDG/issuer validators. Selected and Legacy Debug/Release regressions pass without warnings; source/issuer/status/key callbacks in these model tests are explicitly synthetic. Native public-key mathematical validation remains a required adapter; missing adapter denies. No decoded record grants TIME_SOURCE authority or UTC.

The existing canonical packet backings/worker remain v3-sized and guarded. Native v4 persistence and protected pin lifetime are not implemented or qualified; the required full coexistence/retention/capacity proof remains pending. No quotas or storage buffers grew. [Current proof and gate](../integration/time-source-canonical-persistence-20261021/implementation-proof-and-native-gate.md), [closeout](../integration/time-source-canonical-persistence-20261021/closeout.md). ED11 PARTIAL and all operational gates remain denied/unqualified.


## V4 immutable canonical backings (2026-10-21)

Existing3 same-owner backing slots now reserve packet31840/slot; total slot storage120360 and Selected known managed1952624 (delta3744). Existing canonical tickets preserve v3/v4 DATA coexistence/immutability through Stop/retention; no second owner/pin table. Maximum31840-byte v4 and30592-byte v3 models pass native memory-ticket checks. Selected/Legacy clean Debug/Release pass without warnings. Existing v3 borrows reject v4 rather than reinterpret it.

This is scoped DATA lifetime/known storage, not native CURRENT/status/source authorization or full ED11. Qualified standalone Ed25519 key validation remains absent; raw OpenSSL import/public_check is insufficient and no permissive adapter was installed. Worker/committed/previous/next capacities and guards remain v3; operational activation stays denied. [Proof and gate](../integration/v4-backings-pins-ed11-20261021/ownership-and-ed11-proof.md), [closeout](../integration/v4-backings-pins-ed11-20261021/closeout.md). ED11 PARTIAL; quota/default/Legacy/production pins unchanged. Header-layout changes require clean builds in the inspected cached Ninja trees (#deps0 observed), pending separate dependency-tracking repair.


## Strict TIME_SOURCE key predicate (2026-10-21)

[Exact P0-P5 specialization](../adr/DECISION-time-source-strict-ed25519-predicate-20261021.md)
accepts canonical compressed Ed25519 decoding and nonidentity prime-order-L
membership after official document review. This closes the normative predicate,
not its implementation. Backend selection remains pending pinned corrected
source, independent vectors, VC6/target and ED11 qualification. libsodium1.0.21
or later corrected revision is candidate only; existing OpenSSL public_check
is not relabeled sufficient. No key validation/source authority, UTC, worker
activation or CURRENT is enabled. ED11 remains PARTIAL. Original proposal and
historical contracts preserved; [review](../integration/ed25519-strict-predicate-acceptance-20261021/conformance-and-vectors.md).


## Pinned Ed25519 candidate qualification (2026-10-21)

Official libsodium1.0.21 release archive and tag/commit are pinned for evaluation
only. The approved original-source VC6 compile probe failed C1083 on stdint.h;
no shim, integration or runtime proof followed. Source-level predicate review
is not executable qualification. Backend selection and mathematical key
validation remain denied; modern Debug/Release, independent vectors, full
stack/heap/init/retention ED11 and VC6 compatibility remain gates. No quota,
worker or authority change. [Closeout and focal options](../integration/pinned-ed25519-backend-qualification-20261021/closeout.md).


## Independent strict point-validator architecture (2026-10-21)

[Accepted S0-S6](../adr/DECISION-independent-time-source-ed25519-backend-20261021.md) prefers an Accelerator-owned pinned OpenSSL3.5.8 math subset for public TIME_SOURCE DATA validation only. PST modification/dependency is denied. Extraction remains unauthorized; backend selection, standalone license/symbol closure, actual VC6/target and complete ED11 qualification remain pending. Prior full-unit proofs do not qualify the new component. No libsodium port, UTC or authority activation. ED11 PARTIAL. Original proposal/audit preserved.


## Independent strict Ed25519 extraction (2026-10-21)

The Accelerator-owned pinned OpenSSL3.5.8 public-math subset now builds independently, without PST/OpenSSL runtime. MSVC and actual VC6 backend/caller Debug/Release pass296 DATA checks/configuration with zero final warnings; standalone CMake Release CTest passes. Prior296/2526/5940 oracle/crypto/LAB regressions pass again, not operational authority. Canonical valid point, nonidentity and unreduced integer-L subgroup predicate preserved. New order8 public vectors and locally derived mixed4L/8L cases reject. No codec/worker/transport integration or TIME_SOURCE authority is enabled. NT4/box/ISA remain unqualified. ED11 compiled component measurements exist, full transitive stack/init/root-issuer coexistence remain UNKNOWN/PARTIAL; no quota change. [Proof and handoff](../integration/independent-ed25519-subset-extraction-20261021/implementation-proof-and-handoff.md).


## V4 strict TIME_SOURCE key binding (2026-10-21)

Canonical private kind9 now delegates to the independent Accelerator-owned strict Ed25519 backend, with owner Stop/uncertainty guards. Public DATA only: no identity/status/revocation/authorization/CURRENT/UTC qualification. The codec layouts, core math, backings and pins are unchanged. MSVC/VC6 backend+adapter+caller Debug/Release pass313 checks each, zero warnings; native modern Debug/Release pass8 root/historical/backing/census tests each; standalone Release passes2. Full Win32 root stack VC6 and NT4/box/ISA are not qualified. No worker persistence, transport or temporal authority enabled. ED11 PARTIAL; adapter stack/code costs recorded, full operational coexistence still UNKNOWN. [Proof and handoff](../integration/v4-time-source-ed25519-integration-20261021/implementation-proof-and-handoff.md).

## BCrypt hash ownership correction (2026-10-21)

The private store03_hash backend now carries explicit native ownership in the
existing root and pending worker. Failed cleanup preserves references, blocks
reuse/finish and returns failure. Stop does not reclaim uncertain resources.
Previous BCrypt APIs remain; no Windows 10 minimum is introduced. Modern
Debug/Release regressions pass. Managed storage grows 80 bytes: Selected
1,952,704 and Legacy 1,790,952. Opaque provider/kernel bytes and complete
transitive bounds remain UNKNOWN; ED11 PARTIAL and worker v4 STOP. No UTC or
CURRENT authority. [Scoped proof](../integration/bcrypt-failed-cleanup-lifetime-20261021/ownership-and-ed11-proof.md).

## BCrypt ED11 requalification (2026-10-21)

Ownership tests and current layout census pass again without functional changes.
ED11 remains STOP/PARTIAL: the four-reference bound in the two new contexts does
not establish provider/kernel bytes or full transitive resource limits. Accepted
ED11 permits evidence-backed reserves, not guessed opaque-resource allowances.
Selected known managed 1,952,704; minimum v4 projection 1,956,448. Nominal
headroom is not operational credit. [Focused requalification](../integration/bcrypt-ownership-ed11-requalification-20261021/resource-proof.md).

## store03 deterministic hash candidate audit (2026-10-21)

A focused read-only audit recommends qualifying the existing Browser BearSSL
SHA-256 source subset for store03_hash: fixed context/arrays, no crypto provider
calls in the inspected closure, MIT attribution required. It is not incorporated
or qualified in Accelerator. Shared legacy digest and separate issuer/store02
hashes remain outside an exclusively store03 replacement, so no full ED11 closure
or savings are claimed. [Inventory and owner decision scope](../integration/store03-deterministic-sha256-audit-20261021/audit-and-migration.md).

## V4 root-cause audit (2026-10-22)

A read-only full-flow audit found independent integration gaps: cohort/act and
worker remain v3-only; canonical kind8 requires v3 header/published v3 borrow,
while positive v4 fixture components model kind8; qualified issuer status/UTC
resolution still denies. Storage hashing coupled to issuer BCrypt is a
contributing resource factor, not the sole v4 blocker. Known fixed storage fits
nominally but no complete native/transitive/retained/host bound is proved.
No implementation, quota or authority change. [Root-cause report and one next
isolated qualification slice](../integration/v4-deep-root-cause-audit-20261022/root-cause-report.md).

## Deterministic storage hash LAB qualification (2026-10-22)

The isolated Accelerator-owned BearSSL0.6 SHA-256 capsule now has official
release/tag/commit provenance, actual MSVC/VC6 Debug/Release qualification,
40 Python/independent BCrypt vectors and350,879 checks/configuration. Its
no-CRT probe imports no DLL/provider/PST/Browser and has a resolved acyclic
linked graph. Context128 bytes/two256; largest measured nested probe bound1152
bytes, with compiler flags and caller exclusions explicit. No product callback,
issuer/root/worker integration, resource-credit reduction or full ED11 approval
is granted. Canonical v4/authority/native target gates remain denied.
[Proof and bounded handoff](../integration/canonical-storage-hash-closure-lab-20261022/implementation-proof-and-handoff.md).


## Canonical prepared-storage DATA integration (2026-10-22)

The earlier LAB-only status above is historical. The same unchanged pinned
BearSSL math capsule now backs scoped storage DATA callbacks. The same owner
has explicit v3/v4 preparation and a synchronous candidate view without a fake
published pin. Kind8 handles v4/candidate DATA; kind9 remains strict math only.
Issuer historical APIs and native certificate/status operations remain separate.

Selected known census1956448 (+3744), Legacy1790952 unchanged. Nominal headroom
is not admission credit; product GS/RTC/CRT, full nested/native/host/source costs
remain unqualified. ED11 total PARTIAL; worker-v4, CURRENT, UTC source and trust
remain denied. MSVC product regressions pass; actual VC6 passes the portable
capsule, without qualifying the native Windows owner or NT4/box/ISA.
[Scoped proof and remaining handoff](../integration/v3-v4-canonical-prepared-storage-20261022/implementation-proof-and-handoff.md).


## V4 packet11 native LAB qualification (2026-10-22)

The existing packet11/worker now recognizes v3/CP3 and v4/CP4 with frozen
SNT/PDG/issuer/TIME_SOURCE continuity. MSVC Debug/Release pass18 offline suites
and14 native isolated cases each, including full-byte/native-identity CAS,
UNKNOWN retention and Stop rollback. No factual authority or production entry.
Known Selected1956448 and Legacy1790952 unchanged; full native/stack/runtime/
host/source costs remain UNKNOWN. ED11 PARTIAL, UTC NOT_QUALIFIED, CURRENT/
signature/recovery/operational persistence DENY.
[Exact scope and next factual gate](../integration/v4-canonical-packet11-worker-20261022/implementation-proof-and-handoff.md).


## Initial-bootstrap scratch correction (2026-10-22)

The earlier512-vs1024 bootstrap scratch defect is corrected: local1024-byte
BODY05 storage with sizeof capacity and explicit reservation+1024. observed
remains512 for bounded CAS. Real codec/canary/negative-guard tests pass Selected
and Legacy Debug/Release. Positive buffer path uses MODEL resolver and an always-
denying begin trap, not a real ceremony. No CURRENT/authority or certificates.
ED11 remains PARTIAL; the initial entry, operator/target/material-source and full
operational resource qualifications remain gates.
[Proof and next ceremony prerequisites](../integration/initial-bootstrap-scratch-correction-20261022/implementation-proof-and-handoff.md).
