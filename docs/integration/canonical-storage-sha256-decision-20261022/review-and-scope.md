# Canonical storage SHA-256 dependency review and scope

Task PACC-PHASE6-CANONICAL-STORAGE-SHA256-DECISION-1, 2026-10-22.
HEAD/main 913c90cf2328f7939d8f5b927640601903ac6e91. Result PROPOSED/STOP.
No functional implementation, builds, tests or network. Prior source/input hashes
match; WIP preserved. The original audit and proposals remain unchanged.

## Exact first unresolved call

server_configuration_v3_backings_win32.inc:62, backing_capture_data_version,
passes papacc_configuration_issuer_hash06 to pc_root8_v4_validate (or v3_validate).
Lines65,67,68 subsequently use the same callback for issuer-image validation,
canonical CA SPKI, whole packet and CA digest. The function also serves nonstorage
issuer APIs. Replacing store03_hash alone leaves this BCrypt closure reachable;
replacing issuer_hash06 globally silently changes issuer behavior. This shared
boundary requires an explicit storage callback split, not a format/authority
change. It is the first unresolved call in the acceptance review.

A second independent blocker is provenance: local Browser files have exact
hashes and MIT notices, but the prior audit did not establish a verified upstream
version/commit/archive correspondence. The VC6 shim's BearSSL0.6 label is not
upstream provenance. No official download or network access was performed here.
Snapshot acquisition/pin must be resolved before the dependency can be accepted.

## Scope classification

| Function or path | Classification | Exact boundary |
| --- | --- | --- |
| store03_hash | IN_SCOPE | Deterministic SHA-256 for unchanged storage bytes |
| papacc_store_hash | SHARED_REQUIRES_SPLIT | Also called by legacy digest; no blind global replacement |
| digest(record,480) | SHARED_REQUIRES_SPLIT | BODY04/05 nested decode/encode needs storage use; old commit/record paths remain distinct |
| store02_hash | OUT_OF_SCOPE | Existing standalone historical BODY0.2 mode; not a v3/v4 predecessor rule |
| pending_v3_image_guard/packet_valid | IN_SCOPE | Current version3 frozen-image/CP3 checks through store03_hash; not implemented v4 support |
| pending_compare_file and native CAS | OUT_OF_SCOPE | Full ReadFile+memcmp, native identity and transaction semantics remain exact |
| v3/v4 backing_capture_data_version and borrow validation | IN_SCOPE | Packet, CA and issuer DATA integrity hashes require deterministic storage binding |
| issuer_hash06 implementation/public entry | SHARED_REQUIRES_SPLIT | Storage callbacks separate from preserved issuer hash API/native resources |
| v3b_ca canonical SPKI hash | SHARED_REQUIRES_SPLIT | Hash can be storage-bound; CryptDecode/Encode functionality remains native and unchanged |
| v3_update_cohort/act candidate and predecessor hashes | SHARED_REQUIRES_SPLIT | Storage correlations use issuer callback today; preserve provenance/authority guards |
| generic root/issuer codec hash callback parameters | IN_SCOPE at storage callers | No generic codec/issuer API or byte-layout change |
| kind5 BODY05 callback | SHARED_REQUIRES_SPLIT | Calls nested legacy digest; do not drop integrity check |
| kind8 signer callback and native certificate validation | SHARED_REQUIRES_SPLIT | Hash callback shares issuer; signature/cert/provider primitives OUT_OF_SCOPE |
| unrelated issuer commit/history/issuance/signing/CryptHashCertificate2 | OUT_OF_SCOPE | No global backend migration |
| bootstrap/recovery API behavior | OUT_OF_SCOPE | A shared helper must preserve these consumers or get an explicit later scope decision |
| complete native v4 packet preparation/worker execution | UNKNOWN | Existing worker packet11 guard only accepts version3; v4 implementation still denied |

The adjacent callsite inventory records concrete storage symbol locations.
Declarations and direct calls are included; callback-pointer references and
transitive validation bindings are described above and in the source evidence.
It is a static inventory, not runtime coverage. Current v4 capture exists;
current v4 commit does not. No positive path is manufactured to fill that gap.

## Expected reachability after a delimited migration

A reviewed store03 and storage digest migration can remove the algorithm/hash
pair from those exact hash calls. A storage-specific callback at backing,
predecessor and candidate validation can remove their issuer_hash06 hashing
pair without changing the issuer API itself. These are proposed substitutions,
not applied changes or proved savings. No silent fallback is permitted.

Other issuer operations retain BCrypt/CryptHashCertificate2. Native DER/CA
validation and any supplied component callback can still reach cryptographic
providers. Worker transaction/file/thread and CRT/native costs remain separate.
Therefore neither v4 preparation nor the whole worker is declared provider-free.
Full v4 reachability remains unproved until its exact implementation is reviewed.
No old retained BCrypt reference is freed just because later calls use BearSSL.

## Candidate source and dependency closure

[Candidate decision](../../adr/ADR-CANDIDATE-canonical-storage-sha256-backend.md)
limits the proposed package to BearSSL SHA-256, three original C units and exact
header/helper closure. Browser source hashes, license and VC6 evidence were
verified against the preceding audit. MIT notices must be retained. No library
was imported or linked here. The future Accelerator snapshot must be independent
of Browser/PST at build/runtime and pinned to upstream provenance; copying a
label or linking the Browser static library would not meet that requirement.

## ED11 and lifecycle

Named source storage from the preceding audit: pointer+104-byte context payload
before padding (expected x64 sizeof112); compression schedule256 and working
words32; finalization local arrays96; output staging32; constants320 plus two
ABI-sized vtables. These are source-level components, not a nested compiled
stack bound. Heap/provider absence must be proved for the selected link closure;
compiler integer helpers, memcpy/memset, code/data, init and alignment remain
real costs. C89/VC6 and modern builds require their own evidence.

Existing known managed Selected1,952,704 and minimum v4 projection1,956,448 are
unchanged historical census inputs, not totals after this unimplemented choice.
No 80-byte ownership/context discount is taken. Simultaneous root/worker use,
retention and other issuer operations must be charged at their real multiplicity.
No message-sized buffer, new owner/pin table or managed quota increase is proposed.
Stop must suppress useful output and preserve borrowed inputs until completion;
CAS UNKNOWN retains dependencies and credits. No timeout/implicit native release.

Accepted ED11 section11.3 and Windows W4 keep mandatory provider/kernel,
transitive stack/host and retained evidence nonzero/UNKNOWN. The numerical
headroom cannot license this backend or v4 worker. ED11_TOTAL_STATUS=PARTIAL.

## Disposition

Do not register ACCEPTED: dependency provenance and shared callback boundaries
are material conditions. Preserve the focused candidate for exact official
review after those conditions are closed. No functional patch is accepted by
reference. No contract, format, Browser/PST/PACC wire, production0.6.4, LAB,
clone/worktree or git publication was modified. Next remains the owner-requested
migration stage, but its implementation gate stays closed until scope acceptance.
