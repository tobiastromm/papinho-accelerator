# BCrypt ownership ED11 requalification

Task PACC-PHASE6-BCrypt-OWNERSHIP-ED11-REQUALIFICATION-1, 2026-10-21.
Result: STOP. No functional patch, new architecture or quota change.

## Contract finding: reservation is not an exception for UNKNOWN

[ADR-0034](../../adr/ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md),
ED11 paragraph, accepts checked reservations and retained quotas, while keeping
provider/kernel/full target evidence mandatory. The frozen
[specification section 11.3](../../protocol/pacc-c2-specification-revision-1.0.md)
requires separately charged metadata, staging, allocator/controller overhead;
total/baseline/increment/peak/shared/retained/host evidence; and denial before
effects for unknown mandatory provider/kernel/resource bounds. Its last paragraph
requires separate evidence-backed target approval for any empirical uncertainty
exception. No such exception is supplied by this task.

[Incremental envelope decision](../../adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md)
accepts interpretation beyond factual baseline, not numeric adequacy. It explicitly
does not accept a guessed opaque provider/kernel bound or host threshold.
The 32 MiB user and 8 MiB kernel Accelerator figures remain candidates, not
approved per-hash reserves. They cannot absorb UNKNOWN by declaration.

[Windows profile W2/W4](../../adr/DECISION-ed11-windows-3mib-process-profile-20261020.md)
sets one explicitly selected process managed cap, not a per-operation allowance.
W4 preserves all fixed/transient/caller/heap/stack/native/retained charges with
real multiplicity and target evidence. Provider/kernel UNKNOWN remains nonzero
and unqualified. [TS0-TS8 ED11](../../adr/DECISION-time-source-ts0-ts8-20261021.md)
also distinguishes serialization capacities from reservations and requires the
full v3/v4 composition before dependent worker integration.

Consequently a conservative reserve is usable for a known finite capacity with
proven ownership/multiplicity/lifetime, charged once with checked admission.
Existing fixed arrays, three G7 slots and already charged stack/scratch reserves
fit that accounting pattern. It does not license inventing per-provider bytes,
calling a sampled peak a maximum, or assigning all remaining managed space to an
opaque allocator without proof/enforcement that it cannot exceed that reserve.
No numeric opaque reserve is proposed: there is no qualifying bound to justify it.

## Ownership recheck

The unchanged server_store_hash_win32 context has two opaque references and
40 measured x64 bytes. Two contexts reside in the existing root/pending owner:
maximum four new references, +80 fixed bytes. The previous issuer pair remains
separate; conservative named hash-path count is six references. BCrypt references
are NOT proven kernel handles; neither their value size nor count bounds provider
or kernel bytes. Algorithm implementations may share backing; no sharing credit
is assumed and no sum treats phases as independent objects.

An active context excludes reentry; successful Destroy/Close alone clears its
references. Failed cleanup retains reference/status, suppresses digest success,
and denies subsequent hashing without another cleanup attempt. Stop preserves
references. Owner finish and pending reset deny blocked contexts. Quiescence of
the caller/worker does not establish native destruction after an uncertain result.
The same fixed contexts cannot accumulate a new pair per failed attempt.
No timeout release, second cleanup, new owner or fallback exists.

The isolated owner test holds both pairs simultaneously and verifies Stop and
finish retention. The helper test checks reentry, refused reuse and cleanup call
counts, including no duplicate destruction. These are controlled fault branches,
not arbitrary threaded stress or proof of native provider failure disposition.
Real CNG succeeds on the normal path. Failure shims withhold native cleanup;
fixture-only teardown therefore has factual provenance. That teardown does not
authorize production retry after an actually uncertain cleanup result.

## Capacity and resource classification

See reconciled-census.json for current compiled Selected/Legacy layouts. Current
Selected known managed total is 1,952,704; cap 3,145,728; arithmetic difference
1,193,024. Legacy is 1,790,952; cap 2,097,152; difference 306,200. Debug/Release
Selected census values match. No memory is transferred between profiles.

The existing minimum v4 type projection adds three image deltas of 1,248 bytes,
thus 3,744 and projected known 1,956,448. Nominal difference 1,189,280. It is not
an installed v4 worker or a complete peak. No full capacity margin is available.

CURRENT/predecessor/candidate use the existing three G7 slots, 40,120 each,
120,360 total, already contained in the fixed graph. Packet, issuer/CA and
SNT/PDG/TSC/TSR/CP4 slices inside each slot are not extra allocations. Separate
issuer snapshots 24,576 and staged issuer 8,192 remain charged. Existing root
slots 3,456 and pin tickets 1,024 are also nested. Role names and 32 tickets do
not multiply the material copies. Projected committed/previous/next are three
separate 31,840-byte images, 95,520 total; do not merge them with G7. Comparison
scratch 512 is distinct and already included.

One canonical pending worker/cohort can remain busy/retained; guards prevent a
replacement cohort. The dormant UPDATE path excludes active operational Sessions,
but their fixed pools remain charged. CAS UNKNOWN/Stop retain both generations,
source dependencies and credits; no abort/reuse discount. The worker file,
transaction and thread references are another native resource category; closing
a signaled thread does not prove disposition of an uncertain transaction.
No new v4 CURRENT or predecessor was manufactured for this audit.

Direct adapter-managed memory: embedded contexts and local 32-byte digest.
There is no explicit adapter heap/hash-object buffer. BCryptCreateHash passes
NULL/0 object storage, so an object can be allocated internally by the provider;
absence of caller malloc is not absence of allocation. Provider algorithm/hash
storage, process-private native allocations and kernel categories remain unknown.
A runtime property or one successful process sample alone would not bound all
provider allocations, initialization or uncertain retained lifetimes.

Stack: existing 65,536 worker and 8,192 checkpoint reserves are included exactly
once. CreateThread requests the former with STACK_SIZE_PARAM_IS_A_RESERVATION.
That request is not a measured bound for the complete caller/provider stack,
actual OS-rounded allocation and all external callback frames. Local frame sizes
must not be added twice to a proved enclosing reserve; this enclosing proof is
still incomplete. CRT/runtime/shared initialization, full caller/status/source
arenas and retained concurrency, pre-C2 baseline, attributed increments and
factual host headroom remain separate unqualified obligations. Working set,
private commit, reserved address space and kernel quota counters overlap and
must not be added as disjoint allocations or compared blindly with managed cap.

## Validation and decision

Previous ownership evidence manifest and all approved source hashes were checked:
zero mismatches. HEAD/main remains 913c90cf2328f7939d8f5b927640601903ac6e91.
Local existing Debug/Release CTest filters papacc.store_hash and
papacc.phase6.memory_census each pass 3 tests. Direct runs confirm 93 helper
checks and 17 owner checks per configuration; Legacy census also passes.
No rebuild was necessary: no source change and binary hashes are recorded.
No network, CURRENT, certificate, clock change or operational authority.

The reference-loss/accumulation blocker is resolved in the scope tested. The
first remaining resource blocker is the byte bound for opaque BCrypt provider
allocations across active/cleanup/uncertain retention, with separately attributed
kernel cost. Full nested stack and remaining transitive/host evidence also remain
mandatory. Maximum bytes, complete transient peak and complete margin are UNKNOWN.
Neither actual quota overrun nor complete capacity is established.

ED11_REQUALIFICATION=STOP; V4_WORKER_PERSISTENCE=DENY_UNTIL_ED11_PASS.
The minimum next proof is a pinned target/provider resource bound and complete
transitive call/lifetime accounting, or a separately approved evidence-backed
target uncertainty policy. This report does not propose/accept such a policy.
No worker-v4 integration is licensed by nominal fixed-layout headroom.
