# V4 immutable backings and canonical pins — scoped DATA proof

Task PACC-PHASE6-V4-BACKINGS-PINS-ED11-1,2026-10-21. TS0-TS8,F/G,AT/PT,Q/U/E,root/C8,Model E and ED08/ED11 retain authority; no accepted clause or wire changed. [Prepatch proof](prepatch-capacity-lifetime.md) was compiled/documented before source mutation and submitted with the exact source operation. This proof is memory/lifetime DATA, not source authorization, CURRENT or full operational qualification.

## Same owner and exact capacities

The existing PAPACC_CONFIG_V3_BACKING type now reserves packet31840 instead of30592 ONLY in the explicit Selected Windows profile. Its legacy private name and the same3 v3_backings slots are retained; no parallel v4 table, storage owner or pin system is added. Each slot is40120 bytes (packet31840,public CA8192,metadata88); all3 reserve120360 even when holding shorter v3 or retained data. Compiled fixed managed composition1952624 is3744 above1948880. Legacy1790872 unchanged. Canonical tickets remain32. Committed/pending.previous/pending.next and packet11/update cohort guards remain v3-sized30592. Native v4 worker/commit/publication is NOT enabled.

Capture and borrow share the existing private implementation, parameterized by exact version3 or4. Existing v3 APIs still reject v4. New v4 wrappers validate via the v4 DATA codec and expose TSC slices only from the owned checked packet. Neither wrapper converts versions or grants authority. Private source key component9 remains mandatory for genuine mathematical validation; no production adapter was invented.

## Owner and lifecycle graph

| Object | Owner/storage/protection | Start/end and coexistence |
|---|---|---|
|All three packet/CA/metadata backings|existing root owner by value, exact slot serial|allocated for whole owner lifetime, fully counted pinned/retained; never reused while readers/retention exist|
|Candidate capture inputs|caller immutable packet+CA and exclusive bounded arena, synchronous DATA validators|kept disjoint/protected through return; only after validation/final same-slot check are owned bytes sealed|
|CA decode then BODY decode|same caller arena32768 max for v4|native decode borrows end before BODY overwrites arena; no decoded pointer/handle escapes; unsupported capacity rejects|
|SNT/PDG/TSC/TSR/CP4/issuer|slices inside the same checked immutable packet|same root/revision/serial/generation, no separate source registry or duplicated authority|
|Canonical borrow|same existing ticket and slot/readers/serial, const pointers|caller keeps ticket until all uses/children return; stale/missing/version-mismatched ticket denies without changing output|
|External source evidence/key backend|not created/acquired here|must be independently protected/accounted before any factual source use; root pin alone is insufficient|
|Stop/uncertain/retained data|existing owner and credits|Stop rejects new captures/borrows and effects, preserves old bytes; only factual quiescence/closure permits unpin/reuse/finish|

Capture requires the existing prepared non-current free slot, nonzero matching generation, no readers/retained/busy/update/Stop/uncertainty and no previous seal for that serial. It rechecks root/view/projection/revision/predecessor/issuer/CA/snapshot identity before sealing. Caller inputs and callbacks are serialized with owner/Stop; no asynchronous work, extra I/O, lease, allocator or worker is introduced. Stop/retention/finish functions themselves are unchanged. No claim is made that arbitrary native callback latency is preempted by Stop; actual native adapter/latency/operational qualification remains pending.

Borrow rechecks the canonical ticket, expected version, complete packet and CA hashes, slot/root/revision/predecessor/issuer and existing issuer snapshot where present. The v4 borrow contains the existing88-byte view plus TSC pointer/size,104 caller-owned bytes, not a new pin. History and revoked source bytes remain stored DATA. A historical pin proves memory lifetime, not latest CURRENT/status or UTC. Missing, altered, stale or uncertain context fails closed. E0-E8 RELEASE_INVALID and all authorities remain unchanged.

## Capacity reconciliation

The actual after-layout is compiled in all relevant Selected objects and census; default Legacy is unchanged. [Census](ed11-census.json) charges all3 slots at their new maximum with no reuse discount. The independent caller scenario167328 plus full PT explicit52904 and fixed1952624 equals2172856. The arena reuse is proven only between synchronous CA and BODY decodes; no old/new generation, pinned slot, worker or crypto storage is assumed exclusive. TSC/SNT/PDG/carrier bytes already counted in each image are not added twice.

This is known managed DATA storage evidence, not an operational peak, reservation or host credit. Native key/provider/kernel/CRT/transitive stack, source/evidence and future worker/Roughtime/issuer/package coexistence remain UNKNOWN. No prior isolated provider peak is promoted to a bound. Direct assembly reservations are archived and explicitly do not qualify whole call-chain stack. Worker65536/checkpoint8192 reservations remain in the existing ledger; actual native stack needs qualification. No memory was shifted to heap/caller to hide charges. A caller must count physical allocations even when passing slices. Windows3145728 only explicit selection; quotas and Legacy2097152 intact. ED11 total PARTIAL and dependent activation DENY.

## Validation and handoff

Final clean Selected Debug/Release passed16/16 suites per configuration; Legacy13/13, zero warnings. The maximum follow-up passed250 backing checks in each Selected configuration, including v3 bytes30592 and v4 bytes31840 together, third slot occupancy, key/payload mutation, absent/stale/wrong-version pins, source REVOKED DATA, caller-copy independence, Stop, clean quiescent release and retained finish denial. Real canonical ticket/hash/CA parsing functions run; publication bits and key validators are explicitly synthetic DATA models, not native CURRENT/source authority. The retained cycle intentionally does not clean the owner; process exit is not presented as factual closure. Existing16 historical native CAS success/conflict/UNKNOWN/Stop cases passed again after recompilation, with their explicit synthetic contexts.

An initial test-local shadowing warning was corrected without suppression. Three incremental historical failures were traced to stale layout objects: ninja -t deps showed #deps0 for affected test objects. The SAME functional source passed after clean-first builds; no guard/test expectation was weakened. Layout changes require a clean build in these cached trees until the existing dependency-tracking issue is separately repaired. Initial logs remain history.

Next: qualify a genuine Ed25519 mathematical key-validation primitive and its resources, then separately prove full worker/candidate/source operational capacity/lifetime before native v4 persistence. Do not enlarge the other three packet stores or relax packet11/CURRENT guards by inference. No signature verification success, public-key import, source record, pin or calibration assumption substitutes for canonical administrative authority/PAL/UTC qualification.
