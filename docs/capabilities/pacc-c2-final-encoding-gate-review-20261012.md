# C2 Revision3 — final encoding gate review
Date: 2026-10-12. Mode: documentary protocol/security engineering.
C2 semantic direction CONDITIONALLY_APPROVED; candidate1 wire PROPOSED.
No runtime qualification, implementation or E1-E12 execution.

The [owner decision record](../adr/DECISION-pacc-c2-conditional-semantic-approval-20261012.md) accepts only the conditional semantic direction. Existing accepted ADRs, [spec0.3](../protocol/pacc-c2-protocol-specification-draft-revision-0.3.md), twelve layouts, fourteen candidate vectors and ED matrix remain byte-exact. This review is a proposed final-gate supplement, not a silent correction to the frozen candidate inputs.

## 1. Baseline and authority
Accelerator main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, pre-existing dirty WIP preserved. Baseline456 Accelerator files,3142 Browser files,1566 isolated Browser-source files. C1 seven hashes and NSS normal SDK53 entries reverified; OpenSSL API3 normal SDK44 entries additionally verified. No old qualification rerun.

ADR-0025 designates include/papacc/pacc and src/protocol as canonical PapinhoPACC implementation, package target copies these sources. The captured canonical local package1.1.0 ZIP SHA256 is 1E3637506A8746EC19B75F41AE54022F037E3D6305D003459967BB20586328A4; packaged message_types.h/capability_wire.h match source bytes. No separate PapinhoPACC repository is required or presumed. Source registry is0x0000 reserved,1..15 assigned; builtin keys(1,1),(1,2),(2,1). Candidate capability3/major1 and0x0010..0x001B do not collide here. They are NOT allocated by this review.

Older overview registry0x000E and historical security-model PST0.6.1 statements do not override current public headers/package generation2 or production dependency0.6.4. These historical files are preserved.

## 2. Final ED01–ED14 recommendations
APPROVE_WITH_PROOF_GATES means documentary recommendation only, not owner acceptance or observed behavior. HOLD means an unresolved design/cap decision blocks declaring the exact wire ready.

| Gate | Documentary finding | Explicit owner recommendation | Mandatory implementation evidence |
| --- | --- | --- | --- |
| ED01 | Authoritative source and governed local package agree; no candidate collision | APPROVE candidate allocations3/1 and16..27 at final owner gate, record canonical registry/API/package revision then | Published package identity; one shared registry/codecs; no functional old-ID change |
| ED02 | Old Envelope/CONTROL1.0, two-key offer28 and result14 preserved | APPROVE dedicated C2 grant composition/one owner, no silent migration/fallback | Old server/new client and new server/old client, missing-key zero effects, bounded retained Session admission |
| ED03 | All12 field offsets/widths and14 example lengths consistent; no unassigned enum implicit acceptance | APPROVE exact tables subject to ED08 closure clarification; keep enums/reserved/length checks closed | Independent codec/golden agreement, all extrema/trailing/reserved/state/overflow/split/coalesced negatives on VC6/MSVC |
| ED04 | DNS/IPv4 first-major limitation explicit, no IP substitution or arbitrary trust | APPROVE restricted first major, require canonical DNS bytes for SNI and expected-name; reject unsupported inputs | All-candidate local policy, exact numeric connect, tuple/provenance races, no re-resolution, no Accelerator public site socket |
| ED05 | Scoped protocol-only U64 IDs/generation/epoch and local U32 provenance distinguish authority | APPROVE checked nonreuse/exhaustion scopes; no private runtime IDs | Delayed callback/generation/retained-owner and restart/Session mismatch negatives |
| ED06 | Server-modern CSPRNG selection available; Browser needs no RNG; comparison/erase portability gates identified | APPROVE token policy below; approve/pin portability utility source before code, no provider coupling | RNG error/duplicate/zero, secret wipe after borrowers, target machine-code timing/erase audit, finite abuse tests |
| ED07 | Public API3 exposes all TLS_READY facts; exact true-state/version mapping available | APPROVE public-fact predicate below; deny unknown/unavailable rather than fabricate | Same operation CLIENT summary/ALPN after handshake, wrong trust/name/version, partial result, revoke race; normal package runtime evidence |
| ED08 | Four prefix/ACK definitions consistent; COMPLETE is not bridge drain; early shutdown disables plaintext reads | HOLD: approve concrete producer-quiescence/seal coordination and release/drain partial order before exact wire acceptance | Bounded no-cycle closure both initiators, full final output, no premature seal, stalled/retained/late-record negatives |
| ED09 | Owner-local revocation and escaped prefix clearly separated | APPROVE limitation and exact narrow notification authority; no instantaneous distributed revoke claim | Cancel at each write byte, lower-layer accepted count, no post-local-revoke submissions or stale delivery |
| ED10 | Server FINAL scope distinct from Browser-owned native destruction | APPROVE remote summary plus local finalization guard; retained cannot become clean | Per-lifetime attempt/result/disposition/destruction, remote-clean/local-retained and summary-loss negatives |
| ED11 | Full ledger/formula specified; provider/kernel numerical maxima and NT4 headroom absent | HOLD total cap/target policy: do not claim complete measured budget or old-machine compatibility | Metadata sizes, callback cap, lazy provider peak, buffers/allocator/kernel/retained peaks,8/16/32/64KiB sweeps and actual NT4 |
| ED12 | Mode C Browser network/Accelerator Site TLS and independent authorization preserved | APPROVE service+TLS grant independent of Network Egress; separate destination policy | Browser site egress identity; Accelerator only private exact C3 bridge; denied local/server policy zero effects |
| ED13 | Legacy functional grammar does not protect first-hop token/APP bytes | APPROVE explicit lower-assurance D9A conditions only; DENY production authority at this gate | Separate listener/profile/lab admission/isolation; no Secure downgrade; C7 execution approval remains separate |
| ED14 | Current spec/schema/vector hashes and widths match; future codecs/tests do not exist | APPROVE progressive consistency gate, not documentary runtime qualification | Same approved revision/codecs/test oracles, independent peer, old goldens, target/native results before qualification |

Spec0.3 remains historical candidate1 until an explicitly approved supplement/revision absorbs ED08/ED11 resolutions. Do not turn this table into accepted C2/C3 implementation permission.

## 3. ED07 — exact public PST fact predicate
Normal Accelerator SDK: PST0.6.5/API3.0.0/SPI4.0, OpenSSL x64 /MD, source commit da7190930700f54ecc92611f5edbcb0932caa5ec. Normal Browser SDK same candidate commit/API/SPI, RetroZilla NSS x86 VC6/MD. Distinct providers/targets remain distinct artifacts; no test-only output used for these facts. Headers are byte-equal across these normal SDKs; manifests and SHA256SUMS identities are captured in audit.

Public declarations are pst_connection_handshake, pst_connection_get_peer_info, pst_peer_info_get_summary, PST_PEER_INFO_SUMMARY and pst_connection_get_negotiated_alpn. Consumer MUST:
1. Configure Site connection as CLIENT with certificate REQUIRED, server-approved trust, exact canonical DNS expected-name/SNI, TLS minimum=maximum PST_TLS_VERSION_1_3, resumption/early-data disabled and graceful shutdown REQUIRED. Gate actual CLIENT capability mask, not aggregate SERVER union.
2. Observe API call PST_RESULT_OK, operation PST_OPERATION_COMPLETE and operation-result PST_RESULT_OK. NEED_READ/WRITE, CLOSED or FAILED is not handshake completion.
3. Before shutdown/read-CLOSED/release, obtain peer snapshot and initialize summary struct_size/api_version from governed header. Require every query return OK, local_role CLIENT, certificate_present/chain_validated/peer_name_validated/peer_authenticated EXACTLY PST_KNOWN_TRUE (numeric2), tls_version EXACTLY PST_TLS_VERSION_1_3 (numeric13).
4. Map public TLS13 to proposed wire0x0304 and positive version/chain/name facts to mask0x0007. Do not serialize13 as0x0304 automatically by cast, or treat any nonzero known-state as true. UNKNOWN0,FALSE1,UNSUPPORTED3,NOT_APPLICABLE4 deny required facts.
5. Query selected ALPN with bounded255-byte storage, compare exact opaque offer/profile selection. UNAVAILABLE plus public alpn_available FALSE and optional/disabled authorized ALPN may mean no selection; UNSUPPORTED/error/insufficient capacity is not empty negotiated ALPN. Required ALPN absence fails. No result fabrication or diagnostic-log parsing.
6. Copy only safe bounded facts to operation-owned result, release peer snapshot once, revalidate operation/generation/policy and complete CONTROL TLS_READY write before APPLICATION. Snapshot may outlive connection, but old-generation snapshot cannot authorize a new operation.

Source audit: pst_runtime.c permits peer/ALPN queries only in C_ESTABLISHED; read CLOSED transitions C_PEER_CLOSED; shutdown enters C_SHUTTING and then C_CLOSED. Public read only permits C_ESTABLISHED. Therefore collect facts before these transitions and never read after shutdown as a drainage workaround. Public snapshots contain validation facts; API existence is established. Source tests verify these public summary/ALPN fields, but no candidate C2 execution occurred here. Historical credentials-trust-peer.md's earlier NSS ALPN unsupported phase is not current client mask evidence. Binary availability follows normal package public declarations/exported contract; actual C2 invocation remains an implementation proof gate, not inferred source PASS.

## 4. ED06 — entropy, comparison, erasure and abuse
Proposed server policy: for this modern MSVC/x64 Accelerator target, BCryptGenRandom(NULL,buffer,32,BCRYPT_USE_SYSTEM_PREFERRED_RNG), require STATUS_SUCCESS, separately for each role. This is OS CSPRNG use, not PST-provider access or a new cryptographic algorithm. Missing API/RNG failure denies admission, no rand(), ticket/counter/time/hash fallback. Do not impose BCrypt on VC6/NT4 Browser or claim support for an old Accelerator target without separate qualification. [Microsoft BCrypt requirements](https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptgenrandom) state Vista minimum and system-preferred flag unavailable before VistaSP2.

Tokens32 bytes, nonzero, distinct from both live/retained roles, ≤4 generation attempts then denial; one atomic claim, no restoration on lost ACK. Public mismatch checks precede a fixed32-byte content comparison. Recommended comparator source for review is [OpenBSD timingsafe_bcmp](https://github.com/openbsd/src/blob/master/lib/libc/string/timingsafe_bcmp.c), source revision1.3: fixed-length full-byte equality utility, not a new crypto scheme. Pin exact reviewed source/hash/license and remove only OpenBSD export plumbing in a documented portability recipe before code. Do not link directly to PST's private OpenSSL/NSS APIs. No portable timing guarantee is inferred from C source: MSVC optimized machine code and first/last-byte mismatch cases require inspection/testing.

Modern owner buffers use documented secure erasure rather than ordinary memset. [Microsoft SecureZeroMemory](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa366877(v=vs.85)) documents optimization-safe clearing and XP minimum; it is not an NT4 qualification. NT4 Browser needs a separately reviewed/pinned C89 erase port (volatile stores/approved compiler barrier as applicable), VC6 optimized-output inspection and runtime wipe test. No new erasure code or dependency is imported in this task.

Clear every token copy: generated staging, immutable role record, OPEN_RESULT writer, Browser receipt/staging, BIND writer and temporary compare buffers. After revoke/claim/expiry, mark logically invalid immediately but wipe/free only after every borrowed write/callback is quiescent; retained state preserves lifetime dependencies, not permission to use tokens. Never secret-log/dump as normal telemetry; unavoidable OS paging/crash/admin access is a residual host-trust boundary, not an absolute erasure promise.

Existing candidate limits: one BIND/unbound candidate; four global/two Session unbound candidates; eight failures/rolling monotonic10s/Session, candidate closes only; inactive victims/other roles not destroyed. Charge TLS before BIND, certificate/handshake CPU/memory and response/close retention under independent pre-auth admission caps. Post-bind rate limits alone do not prevent unauthenticated connection floods. Enforce actual server listener caps and handshake deadlines before expensive effects; exact existing/C2 shared cap must join ED11 ledger. Legacy on-path attackers can steal/alter these tokens despite entropy; do not claim its cryptographic replay resistance.

## 5. ED08 — closure dependency and unresolved boundary
Four fences mean admitted APPLICATION/RECORD BA/AB prefixes, each with declared downstream ACK. They are not TLS half-close or peer TCP receipt. APPLICATION BA END_PREFIX MUST NOT silently mean an HTTP request end followed by more TLS response reads after pst_connection_shutdown. ADR-0023 disallows emulated TLS directional half-close; PST API3 still has terminal reciprocal shutdown. Current source switches to C_SHUTTING immediately and public read thereafter returns INVALID_STATE. A consumer must finish any required plaintext delivery BEFORE initiating shutdown, or report bounded non-clean completion.

Second unresolved boundary: shutdown COMPLETE means PST reciprocal completion; it does not expose how many previously accepted RECORD bytes remain in private loopback transport/provider/kernel queues. A single C3 WOULD_BLOCK/select idle turn is not proof of all output. No sleep, timestamp, raw EOF fabrication, private provider counter or new PST API is authorized.

Proposed resolution to review (not yet normative): keep operation manager/C3 peer/DATA/CONTROL alive; after factual reciprocal COMPLETE and correct wait-set removal, attempt PST release through Model E. Only OK+DESTROYED permits finalizing that PST child, while manager/bridge peer remain for bounded ordered drain. An observed C3 EOF after valid native teardown can delimit that producer's admitted output; preserve all received records and confirm C1 downstream before RECORD AB fence ACK. PRECONDITION_RETAINED resolves only membership; CLOSE_UNRESOLVED prevents drain-as-success and retains all required dependencies. Partial child destruction is distinct from full owner finalization.

This could remove the cycle in which proof of RECORD output completion waits on a fence while the fence waits on producer destruction. It requires explicit approval of the release-before-last-RECORD-fence partial order and proof that native close/drain preserves output. It also requires an exact rule for Browser RECORD BA sealing when PST is complete but site TCP has not produced EOF: voluntary end of the admitted prefix is distinct from fabricating native EOF, and needs authorized coordination. The existing generic END_PREFIX alone is not claimed to prove that condition. Determine whether a constrained phase rule using current messages suffices; if not, return to owner rather than silently add a thirteenth message/change FENCE kind1 semantics.

~~~mermaid
flowchart TD
    A[Finish required plaintext before shutdown] --> B[Factual reciprocal PST COMPLETE]
    B --> C[Remove membership; governed PST try_release]
    C --> D{Model E disposition}
    D -->|DESTROYED only| E[Keep manager and C3 peer; factual bounded drain]
    D -->|Retained| R[Retained non-clean; no fallback]
    E --> F[Exact admitted RECORD AB prefix and downstream ACK]
    F --> G[Governed Browser RECORD BA seal: owner decision required]
    G --> H[Four matched ACKs and all owned resource confirmations]
    H --> I[Server FINAL; Browser separate local clean guard]
~~~

This is a candidate dependency plan, NOT a proven no-deadlock execution or approved replacement order. Missing close_notify, peer not completing, filled storage, trailing records, loss or deadline yields non-clean bounded handling, not infinite drainage. Required implementation cases include both initiators, final plaintext/records split every byte, kernel pending bytes after COMPLETE, missing/partial four ACKs, EOF-less site, repeated Stop, revoke during fence, retention and late callbacks. No callback/global counter substitutes for Connection/generation/socket lifetime evidence.

## 6. ED11 — complete accounting model, incomplete numerical qualification
All units below bytes; counts refer to live PLUS retained owners. The known controlled wire/storage component per one-operation C2 Session endpoint is:
- Four payload queues:4*65536=262144.
- Two role channels, each one RX and one TX maximum frame:2*2*(16+16384)=65600.
- Session ordinary CONTROL writer819 plus input803=1622.
- Per-operation reserved CANCEL56 plus FINAL92=148.
- Known subtotal=329514 bytes/endpoint before any metadata/PST/kernel/shared baseline.
For two operation/Session endpoints=659028 controlled bytes; the512KiB global payload cap counts ONLY the eight queues (524288), not the process total. Do not double-charge a staging buffer if it is actually a borrowed span already charged; actual allocation ledger must show the alias/lifetime.

Proposed metadata envelope, for owner decision rather than observed sizeof:≤16384 bytes per operation including immutable tuple/ALPN/token record, role/queue/fence counters, owner references, timers, callback/tombstone/retained record and audit scratch. Proposed fixed callback capacity≤16/operation, with no queue growth. These are new candidate implementation budgets, NOT accepted ADR29 limits. Their feasibility needs compiled object-size/allocation audit; if cap insufficient deny/return for decision, never evict retained ownership.

Complete total equation per process:
T_peak = B_existing + S_shared + N_session*(1622 + M_session + P_CONTROL)
       + N_op*(327892 + M_op + P_DATA_APP + P_DATA_RECORD + P_SITE_if_Accelerator)
       + N_candidate*(84 + M_candidate + P_candidate)
       + K_all + A_overhead.
B_existing includes executable/loaded images/stack/heaps/ordinary sessions and pre-existing Mode B resources; use same-target baseline, not zero.
S_shared includes incremental lazily initialized runtime/provider/trust/credential leases/config/cert snapshots/WinSock/window/event state, counted once by owning allocation and preserved while borrowed.
M categories include every parser/manager/owner/notification/callback/tombstone/retained allocation, aligned actual sizes, fixed logs.
P categories include normal package connection/provider TLS state, handshake scratch, encrypted/decrypted buffering, peer-info/certificate copies and any per-thread stack.
K_all includes all sockets' actual kernel send/receive/TCP state/listeners/pending/retained handles plus window/event/driver bookkeeping; caller requested SO_SNDBUF/SO_RCVBUF is not a certified kernel bound.
A_overhead includes allocator headers/alignment/fragmentation and measurement margin. Do not count both a baseline allocation and its shared delta.

Peak inventory for one active C2 operation: Browser C2 CONTROL+two DATA+site socket,3 secure PST first-hop contexts; Accelerator CONTROL+two DATA+private pair+transient listener (six sockets before adding candidates),4 PST contexts including Site. Existing-service contexts count in B_existing; unbound candidates add their own sockets and Secure TLS contexts. Set finite per-target Session/listener/candidate/credential/peer-chain limits, including retained slots, before admission. Counts above are this operation's incremental inventory, not universal process limits.

Public PST API has no hard allocator-budget/usage cap or kernel accounting query. Source malloc/calloc/private provider allocations cannot be bounded from sizeof(public opaque handles). C1 source explicitly says requested socket buffers do NOT equal OS protocol bookkeeping. Complete total MAX/old-machine fit cannot be honestly supplied from current documentary evidence. No target RAM/required headroom table or same-package worst-case provider/kernel measurements was found in the qualified C0/C1 evidence.

Owner must decide per-target RAM/headroom and fail-closed cap policy, then authorize later measurements independently from E1-E12. Derive B_existing/S_shared/P/K peaks with exact normal SDK/CRT/OS/architecture and bounded credential sizes; measure8/16/32/64KiB variants on actual NT4 and modern Accelerator. Reserve worst-case measured/capped allocations BEFORE activation, monitor high-water, deny when unavailable. If the normal SDK cannot enforce a required provider hard cap, report that limitation for owner disposition; do not change PST, extrapolate averages or treat process termination/retained eviction as cap enforcement.

TOTAL_RESOURCE_BUDGET=COMPLETE_LEDGER_FORMULA_NUMERICAL_PROVIDER_KERNEL_AND_TARGET_LIMITS_PENDING.
No numeric production/NT4 safety claim. Smaller queue measurements remain future evidence and cannot silently replace ADR29's approved experimental64KiB configuration.

## 7. Revocation, FINAL, compatibility and consistency
ED09/10 preserve check→bounded submission→actual count→local revoke linearization. Browser stopping after authenticated notice differs from server's earlier local revoke; already accepted prefixes may escape. No new unauthorized payload completes a partial frame. Narrow FINAL/CANCEL authority cannot reopen DATA.

Server FINAL reports its owned resource aggregate worst disposition only. Browser must separately confirm native site/DATA destruction and its own retained owner state before local clean completion. First cause remains unchanged, destruction count never inferred from release attempts. CONTROL loss/partial FINAL remains peer unknown/non-clean; no implicit retry/reconnect.

Old-peer wire proof at documentary level: unchanged Envelope/CONTROL/old IDs and capability key omission prevent sending C2 to old servers. Required future goldens include exact28 offer/two-grant14 result, omitted-key result with no effects, new server old client, incompatible major/flags and post-negotiation unknown message. No runtime compatibility test executed.

ED14: immutable input hashes, schema fields/offsets and example header lengths match all12 types/14 vectors. They are public illustrative frames, not a runtime transcript. Later approved codec/test goldens must come from that accepted revision with independent encode/decode and all min/max/malformed cases; never use a candidate fixture as a normative oracle before approval. Progressive ADR→spec→diagram/state→conformance→codec/tests→hash manifest remains the qualification gate.

## 8. Closeout and next gate
Reconciled documentary points have explicit recommendations and separately labeled implementation proof gates. Not every final point is closed: ED08 producer-quiescence/BA seal order and ED11 enforceable total/target cap policy remain material. ED06 utility pin/target proof remains a pre-implementation gate. Therefore the exact wire ADR is NOT ready for unconditional acceptance and wire is NOT ready for final approval.

C2_SEMANTIC_DIRECTION=CONDITIONALLY_APPROVED
C2_WIRE_ENCODING=PROPOSED_PENDING_FINAL_GATE
ED01_TO_ED14=RECOMMENDATIONS_RECORDED_ED08_ED11_HOLD
PST_PUBLIC_TLS_FACTS=PUBLIC_API_AVAILABLE_STRICT_PREDICATE_RUNTIME_PROOF_PENDING
TOKEN_ENTROPY_POLICY=MODERN_SERVER_BCRYPT_32_BYTES_PORTABLE_COMPARE_ERASE_GATES
TOTAL_RESOURCE_BUDGET=COMPLETE_ACCOUNTING_NUMERICAL_PST_KERNEL_TARGET_CAPS_PENDING
FENCE_SHUTDOWN_FEASIBILITY=CONDITIONAL_PRODUCER_DRAIN_AND_BA_SEAL_DECISION_PENDING
OLD_PEER_COMPATIBILITY=DOCUMENTARY_COMPATIBLE_RUNTIME_NOT_EXECUTED
ADR_READY_FOR_ACCEPTANCE=NO
WIRE_READY_FOR_APPROVAL=NO
FIRST_UNRESOLVED_DIVERGENCE=ED08_PST_COMPLETE_IS_NOT_RECORD_PRODUCER_DRAIN_OR_BA_SEAL_AUTHORITY
NEXT=OWNER_RESOLVE_ED08_CLOSE_PARTIAL_ORDER_AND_ED11_TOTAL_CAP_POLICY_BEFORE_EXACT_WIRE_GATE

No functional Accelerator/Browser/PST/PACC change, no E1-E12, no promotion/commit/push/tag/release. Production pin0.6.4. Current review/audit supplements old evidence, preserves WIP and records complete SHA256; build/native/runtime not launched.


## Current disposition 2026-10-12

Historical review/handoff consumed by PapinhoAccelerator/ADR-0034. Current normative contract: docs/protocol/pacc-c2-specification-revision-1.0.md; live ED01-ED14 and independent implementation/target gates: docs/capabilities/pacc-c2-normative-approval-and-gates-20261012.md. Earlier HOLD/approval-required statuses retain their as-of-date historical meaning. No runtime gate becomes PASS, implementation is NOT authorized, and numeric total envelopes remain candidates.
