# Portable issuer core — integration contract and handoff

Task PACC-PHASE6-PORTABLE-ISSUER-CORE-IMPLEMENTATION-1. Implementation contracts, not a new accepted ADR or operational authorization. Authorities: accepted internal issuer decision clauses2,6–11; technical profile1.4,2.1–2.5,3.2–3.7; ADR-0002; Model E; root factual commit/CURRENT/epoch, ED08/ED11. No existing accepted decision changed.

## Delivered boundary

src/issuer/issuer_core.h/.c provides bounded caller-owned state, request identity, CA/generation/SPKI binding, serial calculation, same-root CAS protocol, issuance orchestration, lookup, renewal linking, revocation, recovery and Stop/retention. It contains no Windows/CNG/SID/DACL/Registry/Service/native handle types, I/O, heap allocator, crypto primitive, global mutable issuer registry or default backend. In-process structs are NOT a persistence or wire encoding.

CMake papacc_issuer builds without test permission. The offline test target compiles a separate core translation unit with PAPACC_ISSUER_OFFLINE_TEST. The real library rejects either test-only root or crypto adapter before any root read. No runtime product configuration selects a test adapter; model code exists only in tests/issuer_core_test.c. Trusted composition must never relabel test callbacks as production. Tests do not prove protection against arbitrary code tampering.

### Ports and facts

- root.read(ctx,id): same administrative root's verified CURRENT snapshot and exact immutable request record, explicit found/absent. Required facts: root/domain, authority, generation, prefix, CA SPKI digest, policy, revision, actual epoch, high-water and CA validity. Five mandatory proofs cover current state, complete verified history, fenced latest high-water, key correspondence and unique prefix against retained history. They are adapter obligations, never user-supplied positive config flags. Caller holds factual root reference/lease; no view may outlive an unprotected owner.
- root.cas(ctx,expected,old,next,high,DER,size,publish): atomically compare actual root revision, counter and record, preserve history, durably write exact record/DER/high-water in the SAME root recovery chain. No second file/journal/registry authority. CONFLICT has no effects and core never retries automatically. UNCERTAIN blocks future effects in that owner. After success the core re-reads exact committed facts before progressing. Concurrent unrelated changes may conservatively produce UNCERTAIN; never guess commit facts.
- root.copy_certificate: return exact archived DER by request and digest, without needing a private signer. Core validates size and certificate digest via the public verifier. Idempotent lookup never signs again and does not imply validity/admission of an expired/revoked certificate.
- authorize: actual administrative actor/request/purpose/policy/CURRENT/epoch AND applicable ED11 reserve/ownership checks in existing authority. No actor is inferred from a display name or merely possessing a certificate. Every mutation is authorized; queries have separate read authority.
- utc: trusted explicit civil time; failure denies. Certificate dates come from approved request, never implicit backdating. papacc_issuer_interval proposes min(requested end,start+90days,CA end), with checked addition and CONFIRMATION_REQUIRED on change; caller must obtain actual admin confirmation before preparing that interval. This helper grants no authority.
- verify_request: qualified provider validates exact CSR/public-key digests, key purpose/algorithm and proof of possession against immutable approved request. Core does not implement a fake hash/parser.
- acquire/sign/verify_certificate/release: purpose-restricted protected lease bound to selected CA generation/SPKI and approved reservation; signing produces profile1 X509 certificate only, never arbitrary payload signing by CA. Verification checks exact CSR/identity/issuer/purpose/extensions/validity/profile and returns provider-computed digest. Release returns OK only for factual closed/no remaining owned dependency. UNKNOWN or retained results keep the lease referenced; no destructive retry.
- entropy/generate_key: replaceable interfaces reserved for separately authorized initialization/rotation. Issuance/recovery never invokes them, never generates an absent CA and never substitutes a KSP/key. Native CNG adapter is not supplied or activated by this slice.

All callbacks are synchronous, non-reentrant effects on trusted adapter code, not user/config-installed callbacks. Backend/root contexts and immutable vtables outlive owner and retained work. Stack views/records and CSR input cannot be retained by a callback; pending native work needs backend-owned durable storage under its returned lease. Output remains caller-owned but is pinned by retained_output on uncertain signature/persistence. Caller must keep that buffer/credit alive until a separately reviewed reconciliation resolves ownership. Core deliberately supplies no unsafe reset/force-release API.

## State/serial matrix

| Action | Required factual prior state | Durable outcome / failure |
|---|---|---|
| prepare | complete root facts, exact approved request; absent correlation | PREPARED; same request idempotent, changed request conflicts |
| renew | new request/key digest linked to old PUBLISHED or REVOKED request, same device/purpose, admin authorization | new PREPARED and later new serial; old history/status unchanged |
| reserve | PREPARED, current same policy, available unsigned64 counter | CAS high-water+1 and SERIAL_RESERVED together before any signature |
| issue | SERIAL_RESERVED, authorized/verified CSR, time valid | commit SIGN_REQUESTED before acquiring/signing; recheck policy/time/Stop |
| sign result | exact persisted SIGN_REQUESTED | verify and atomically store DER/digest + ISSUED_UNPUBLISHED; uncertainty retains output and marker |
| publish | ISSUED_UNPUBLISHED, current same policy and valid time | only root.cas publish=1 plus readback of actual publication epoch can produce PUBLISHED |
| revoke | PUBLISHED, explicit authorization | persistent REVOKED; no active Session cleanup/policy invented |
| abandon | PREPARED or SERIAL_RESERVED | ABANDONED; high-water never decreases, no serial reuse |
| recover | verified same-root complete/fenced history | SIGN_REQUESTED becomes RECONCILIATION_REQUIRED; no retry on timeout or unknown result |
| Stop | owner-serialized cancellation request | deny new effects, allow authorized read-only query; preserve leases/buffers/records |

Serial is16 bytes: exact persisted8byte prefix (nonzero, top bit clear) plus unsigned64 big-endian counter. Existing CA self-certificate consumes1; issuer root must report high-water>=1, next leaf is2. Maximum denies without wrap. Prefix generation/history collision check belongs to explicit root generation preparation. Core does not invent a prefix or new authority. No conclusive-no-sign recovery retry is implemented; conservative reconciliation remains required.

Per-owner operations must be serialized by owning runtime. busy rejects reentrancy; Stop is delivered on that same serialized owner boundary (not a cross-thread atomic API). Different owners/processes require truly atomic same-root CAS; a mutex in one core is not sufficient. Model tests use deterministic interleaving of two owners, not native concurrent filesystem writers. Callback latency/cancellation still requires qualification before plugging into active owner; no claim that Stop preempts a blocked OS/provider call.

## Actual root integration status — NOT a durability claim

The only root supplied in tests is an explicit synthetic model. No production root adapter is registered. The canonical root transaction still rejects expected revision!=0/active updates and BODY0.5 has no issuer records. No source guard was bypassed, no unused BODY bytes repurposed and no second registry created. A context without the actual adapter returns UNSUPPORTED, not a synthetic readiness callback.

The portable SAME-authority port is implemented; physical root persistence/live publication are NOT implemented. Native execution remains denied. This is the remaining integration item, not a successful persistent/live issuer. Windows CNG remains the private backend candidate; original NTE_PERM/key disposition unresolved.

### Exact next integration work

1. Attach versioned issuer data/participant to existing PAPACC_CONFIGURATION_COMMIT/application root, not to a side file authority. Review explicit codec/layout/capacity against technical profile2.5 before write integration. Preserve historical BODY0.2–0.5 encodings; never persist native structs or pointer/size_t representation.
2. root.read must pin/copy verified same-root CURRENT records and unpin factually, returning actual revision/policy/epoch, generation/SPKI, complete checkpoint/high-water and fencing evidence. Missing issuer capability must remain UNSUPPORTED; process startup must not initialize it.
3. root.cas must extend the actual transaction with interprocess exclusion/CAS against durable revision/high-water, exact immutable request/actor/CSR/purpose/policy, full certificate DER/hash and retained recovery history. Reservation and certificate-current publication are separate transactions. Write/flush/crash/recovery evidence and anti-rollback/fencing remain required. Current dormant-only API cannot do this by passing another expected value or fabricating commit.
4. publish=1 must call actual root publication and return the epoch issued there. Core never supplies a new epoch. Credential publication still does not grant runtime capabilities/client admission. Revoke must retain separate pending active-Session policy.
5. Bind normal Windows CNG lease adapter only after exact native failure/protection/ownership disposition; no use of unknown historical diagnostic key or new generation without authorization. Validate CSR/X509 via qualified provider, not synthetic DER model. Entropy/init/backup/restore gates remain separate.
6. Account actual root journal/DER/generations, retained output, native provider costs and callbacks before enabling any effect. Test physical crash windows, competing processes, restore high-water, leases and Stop under actual adapters. Box/ISA remain unqualified and may expose a smaller capability subset.

## Offline coverage and limits

Authoritative artifacts: Debug/Release cmake-build6 and ctest6 logs, current CMake libraries/executables.117 core assertions and4 production gate checks per configuration;2/2 CTests in each. Native keys/CA/certificates/network NOT_RUN. Synthetic DER is the three-byte marker DER and is not cryptographic evidence.

Covered: same-id equality/conflict; no signature without reservation; counter endian/overflow; two-writer interleaving; uncertain committed reservation; crash after SIGN_REQUESTED before signer; unknown signature retained/burned counter; no re-sign recovery; archived exact DER without signer; corrupt archived DER; revocation/no resurrection; new-key renewal link; abandonment/no quota/counter refund; UTC regression/expiry during acquire; expiry blocks publication; policy/key substitution; malformed serial/recovery proof/rollback; Stop during signing; retained lease/no re-close/reinit; missing signer/root; test backend rejected before root access; no silent fallback. Tests are not117 native scenarios and do not prove real fsync, provider ACLs, OS crash or active scheduling.

Incremental Ninja validation exposed one stale test object after a header layout change. Explicit OBJECT_DEPENDS for issuer_core.h and types.h was added only for new source objects. Subsequent logs show rebuild of gate object and pass; failed log is preserved. No global toolchain/scanner change or clean was used.

## ED11

Compiled x64 sizes equal in Debug/Release: core216bytes, record288, root view176, backend vtable88, request136, root port40. The port copy is already INCLUDED in core216; do not add40 again per core. No internal record table, heap, journal or certificate buffer in core. General reservation must include216*Nowners +288*Nroot-records + actual DER/journal/snapshots +88*Ndistinct-vtables (if newly resident) + caller CSR/output buffers + retained generations/backend costs and peak stack. Records/snapshots owned by the root must not be charged twice as a second registry.

Synthetic fixture model5408bytes already INCLUDES16 records and16x32 certificate slots plus view/counters; do not add those embedded records again. Fixture output128, separate root port40/backend88 coexist as test inputs; none are productive quota constants. Owner/caller records on stack are separate copies included by compiled frames, not an additional persistent journal.

Saved Debug disassembly shows rsp allocations prepare1256, issue1224, store640, load64 bytes. These are frame allocations, NOT a complete stack upper bound: include saved registers/caller/CRT, nested calls, platform compiler differences and backend callbacks. E.g prepare/store/load allocations coexist on that path; do not take max(single frame) as total. Full backend/process/provider/kernel peak remains UNKNOWN. No product/root owner is instantiated by adding the library; no existing operational quota is raised. Real issuer census remains PARTIAL until adapter composition/storage review.