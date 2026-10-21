# ROOT08 worker storage strategy — implemented scratch optimization, target still DENY

2026-10-20; task PACC-PHASE6-ROOT08-WORKER-STORAGE-STRATEGY-1. main HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Prior capacity manifest matched before edits. Baseline3928 files; exact pre/post hashes and hunks in source-operations.json/exact-source.patch. Accepted ROOT08/v2/root/C8/issuer/Model E/ED08/ED11 contracts were not modified. Functional patch officially approved before application.

## Implemented A/C slice

The full observed16896 array was redundant with the immutable expected packet for byte comparison. Replace ONLY that array by observed512 and use pending_compare_file at BOTH consumers: pending_worker pre-write full CAS comparison, and bootstrap_commit_worker post-flush read-back. Previous16896, next16896 and committed16896 retain their capacities and complete contents. No format/version, CAS, root identity, issuer, callback, pin, grant, publication, quota or worker/admission rule changed.

pending_compare_file reads exactly min(remaining,512) bytes synchronously, compares each byte at the matching expected offset, advances only after a full matching chunk and never retries short/error reads. It returns match only after every expected byte matches. Mismatch remains CAS conflict in update; read failure remains cleanup/uncertain handling. Bootstrap requires full match and otherwise follows its original uncertain path. File position on complete match is unchanged versus the old full read. On mismatch the cursor may stop earlier, but neither consumer performs a subsequent write/commit from that branch. Native rollback, Stop revalidation, file/transaction close results and retained UNKNOWN disposition remain unchanged.

The existing native transaction/exclusive file handle protects the selected object across chunks. Bootstrap compares through its same retained file reference and revalidates identity/authorization afterward. No reopen or alias is introduced. This does not extend namespace/history/fencing guarantees beyond the accepted contracts.

ReadFile handles are synchronous, with no OVERLAPPED. On success the OS borrow ends at return, and memcmp finishes before the chunk is reused. On failure there is no next chunk/read retry; existing packet/handle retention paths apply. No output API, source, callback, slot or pin exposes observed. Search of all app/src/tests finds only these two buffer consumers; both were changed. This is eliminated scratch storage, not reclaimed evidence, a snapshot, a credit or a legitimate reserve. The packet itself remains alive through existing thread signal/close/retention rules.

No heap/stack/process relocation: the512-byte scratch stays embedded in the SAME packet/owner. Three DWORD helper locals and ordinary call frames use the already reserved65536-byte worker stack; its reservation is unchanged. The helper allocates no handle, owner or memory, calls no callback and receives no root pointer. Opaque kernel/provider resources are still UNKNOWN. Number of reads increases (up to33 for current16896 capacity); this is not a latency qualification or cancellation mechanism. Owner Stop remains independent of pending native I/O; no new wait, retry or forced termination was introduced.

## Per-buffer lifetime / coexistence

|Buffer|Owner/size|Begin and factual end|Coexistence/retention/reuse|
|---|---|---|---|
|committed|Existing root16896|Factual committed installation; replaced only by existing result integration after required native facts|Old state remains through CAS/rollback/UNKNOWN/Stop. Never overwritten by this patch.|
|previous|Existing pending16896|Frozen before worker launch; also bootstrap witness/path scratch under its existing phase|Retained packet, CAS comparison/checkpoint/export history; not shared with committed or caller.|
|next|Existing pending16896|Frozen before launch through result/export dependencies|Cannot recycle on timeout/UNKNOWN; preserved verbatim.|
|observed|Same pending512|Each synchronous read then memcmp; packet allocated/retained as before|Only current chunk matters; no caller/ref/pin. Reused after comparison, never as expected image.|
|encode prefix/output|Existing caller9376/9408 in known no-carrier scenario|Encode input/output call and preparation handoff, subject to caller ownership|Both can coexist with root/packet/source. No credit omitted. Future v2 requires28928/28960 unless separately proven staging.|
|decode workspace/hash|Caller/codec synchronous buffers and existing reservations|Validation/copy/callback completion, with retained caller ownership where required|No full-copy elimination or workspace sharing inferred here.|
|slots, pins, issuer snapshots|Existing3456/1024/24576 plus issuer stage8192|Acquired before exposure, retired only after factual finalization|All remain charged; old Sessions and uncertain operations keep dependencies.|
|worker stack / owner hash scratch|Existing65536/8192 reservations|Existing worker/owner lifetime|No reduction, no overlay with images or retained bytes.|
|root/source/AST/material caller|Existing graph plus source caller85576, unitemized caller allowance131072 and material32768|Existing preparation and lease contracts|Kept in the known composition. Unitemized allowance is not a spare pool.|

No active second transaction can reuse this packet: existing active/thread/pending/authority-lost guards and full packet reset boundaries are unchanged. Stop does not free expected images or pinned generations. The only reduction is the inspected observed array; UNKNOWN/close failure still retain the packet and native handles. Process exit is not proof of DESTROYED.

## Strategies A–E and remaining structural block

A: eliminated redundant full observation storage, implemented and tested. No duplicate committed/previous elimination is claimed.
B: previous/committed/issuer snapshots cannot be overlaid without additional proof. Bootstrap mutates previous as namespace witness and final path; CAS/export/checkpoint/UNKNOWN still consume full previous. Pinning a slot does not protect arbitrary mutable committed bytes. No new borrowing or pin system was introduced.
C: incremental read/compare preserves exact comparison and layouts. Incremental serialization directly into next is NOT implemented: current begin resets the packet, so a blanket input/output alias could destroy candidate data. Accepted encode disjointness and byte-exact formats remain intact.
D: caller staging remains charged. Moving bytes to caller heap/stack does not eliminate coexistence, and a returned pointer would need factual lifetime through worker/retention. No uncounted external staging was added.
E: one-worker serialization already exists, but caller input/AST and frozen packet can coexist at begin. No new admission rule, quota reduction or exclusion of retained work is inferred. Future signer/config overlap remains unqualified.

## ED11 before/after

Fresh Debug/Release sizeof results match: pending51096->34712, commit105712->89328, launcher182768->166384, fixed graph1384376->1367992. These are nested containers: subtract the16384 delta ONCE, not four times. Known layout floor1807256->1790872. Slots, tickets, committed, stage, issuer snapshots and stack reservations are unchanged. Component-test hooks add24 bytes separately and are not production sizes.

Known conservative existing composition2084039->2067655, delta-16384, limit2097152, accounting margin29497 (not activation permission). Existing ALL-size variant2084023->2067639 remains lower by16. Source/caller/codec/receipt/native scratch/retained reservations stay included as before. Full process RAM/commit/handles/provider/kernel and future activation peak remain UNKNOWN; ED11 total PARTIAL.

The requested BEFORE2171399 was a hypothetical full ROOT08 widening of four full images plus replacement of codec pair, not actual allocation. With this optimization, future hypothetical ROOT08 max still needs previous/next/committed28960 while observed stays512:

2067655 +3*(28960-16896)+(57888-18784)=2142951.

Target improvement28448 versus2171399 corresponds to a future28960-byte observed image replaced by512; actual implemented current saving is16384 versus current16896. These are different baselines and must not be conflated. Target remains45799 OVER cap BEFORE additional source/carrier/complete callback/X509/native costs. No widening was applied. Even an unproved output-in-next variant would remain2113991, excess16839. No sufficient further optimization/lifetime proof was found within the existing caller contract, so dependent ROOT08 integration stays DENY. This is not a claim that every conceivable future design is impossible.

## Validation

Same MSVC19.51/Ninja/x64/C90 profile and normal SDK0.6.5;46 SDK hashes verified. Independent build directories preserve historical binaries. Debug/Release build server, affected libraries, census and native fixtures with zero warnings.

Per configuration:10 selected offline suites; six native D3 modes (reject/success/first-block conflict/last-block conflict/UNKNOWN/Stop); seven ROOT06 cases (codec-only/history/snapshots/pending success/conflict/UNKNOWN/Stop); seven bootstrap cases (success/conflict/identity/Stop/UNKNOWN/missing/revoked). All pass. Native file/TxF/read/write/CAS and handle facts are real; CA/request/certificate model bytes, policy/CURRENT contexts, diagnostic bootstrap administration and injected faults are synthetic or compile-isolated fixtures. No product bootstrap, actual administrative-positive qualification, CA/key/real certificate, private signature, network or operational CURRENT was exercised. Test files/logs remain preserved.

Last-block conflict flips a byte after the first512 bytes deterministically and must reject; successful variable lengths exercise the final partial chunk. ROOT06 history tests preserve high-water/revocation; pinned snapshot tests and Stop/UNKNOWN cases preserve retention. Short-read branches reject without retry, but no dedicated native short-read or provider-close-fault injection is claimed. No v2 native commit path was enabled by these tests; v2 carrier remains offline data validation only.

## Handoff

Keep the validated scratch optimization. Before enabling ROOT08 worker storage, prove a concrete candidate construction/transfer or previous-image lifetime that removes additional simultaneous storage without packet reset, mutable alias, lost issuer or early release. Price every retained caller/source/worker generation against2097152 and submit exact new hunks. Do not activate ROOT08/TLS/X509 merely from the new29497 accounting margin. CURRENT, signature, recovery and all admission gates remain DENY; no parallel worker/root/journal/authority was added.
