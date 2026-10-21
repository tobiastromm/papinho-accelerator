# ED11 complete ledger: incremental envelopes and measured scope

2026-10-12. Current accounting reconciliation; numerical total caps remain PROPOSED / NOT_QUALIFIED. [Accepted interpretation](../adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md), [earlier ledger](pacc-c2-ed11-resource-ledger-admission-proposal-20261012.md), [measurements](ed11-total-resource-measurements-20261012.json), [final conformance](ed11-total-resource-final-conformance-20261012.json).

## Four resource planes

MANAGED_MEMORY means controlled requested allocation capacities with actual owner/lifetime IDs, fixed metadata and borrowed aliases charged once. TOTAL_USER_MODE_MEMORY includes baseline, private allocation/commit, copied config/certificate/TLS/allocator state and shared mappings with stated attribution. KERNEL_RESOURCE_USAGE includes sockets/TCP, handles, events, windows, threads, wait-set/platform and driver state. EXTERNAL_OR_UNMEASURABLE_RESOURCES includes remote Site behavior and local categories lacking a supported attribution/bound; unknown is not zero.

PrivateUsage, working set, peak/pagefile commitment and classified virtual views are complementary, overlapping metrics. Do not sum them into a fictional user-memory total. VirtualQuery classifies addresses into MEM_PRIVATE/MEM_IMAGE/MEM_MAPPED; image copy-on-write remains IMAGE in that view, and physical aliases can back multiple virtual views. MEM_RESERVE is address-space reservation, not committed RAM. Neither mapped image size nor reserved virtual size determines minimum hardware.

The owner accepted incremental meaning only: Browser managed2MiB/incremental user20MiB/incremental kernel2MiB; Accelerator managed2MiB/incremental user32MiB/incremental kernel8MiB. Total process and available host margin must still be evaluated. Caps remain candidates, never automatically qualified or RAM requirements for a486.

## Allocation ledger and attribution

| Category | Scope / plane / owning allocation | Known or measured fact | Outstanding total gate |
| --- | --- | --- | --- |
| APPLICATION BA/AB queues | op / U-managed, two distinct buffers | Proposed2*65536; high48/low24KiB | Full production queues absent |
| RECORD BA/AB queues | op / U-managed, two distinct buffers | Proposed2*65536; normal observer two queues measured | Combined four-role runtime absent |
| DATA framing/staging | op / U-managed |2 roles*RX/TX*(16+16384)=65600; aliases inside fixed slab not recharged | Actual codecs/channel wrappers not implemented |
| CONTROL normal input/output | Session / U-managed |803+819=1622, inside16KiB Session slab; filled in policy saturation test | Real secure CONTROL/PACC Session not measured |
| CANCEL/FINAL | op / U-managed |56+92=148, fixed reserve outside saturated DATA | Real partial framed writer pending |
| Role FENCE/ACK staging | op / U-managed |Candidate8*48=384 inside core reservation | Production dispatch/fences pending |
| C3 peer scratch | op / U-managed+K |Candidate65536 inside core; actual normal bridge counters/lifetime | Production C3 forbidden here |
| C3 socket pair / transient listener | op / K |2 sockets per operation plus owned listener during setup; successful transfer/remove/release checked | Socket/TCP kernel allocation unmeasurable per object |
| C1 owned object | Browser op / U-managed+K |VC6 sizeof pump412 bytes; status344 includes endpoint288/token8; critical section24 already inside pump | Allocator/kernel/window/event/thread costs separate |
| C1 application queues | caller / U-managed |C1 itself has zero payload queues; proposed app consumer1MiB/op separately charged | Actual Browser navigation storage/baseline not measured |
| C1 native socket/WinSock/window/event | owner / K+U |Qualified raw component; buffers requested16KiB each, not kernel maxima | Full Browser/C2 and real NT4 absent |
| Public PST core Connection | connection / U-opaque |Normal public lifecycle, exact API3/SPI4 package | No allocator cap/usage query |
| Provider Connection / SSL_CTX / SSL | connection / U-opaque |OpenSSL public normal fixture process deltas | Per-input maximum/attribution unknown |
| Runtime/provider shared state | process/runtime / U-shared |Two logical operations share one actual runtime; record owner once | Shared caches/unload attribution unknown |
| CONTROL/DATA/Site secure contexts | Session/op / U-opaque+K |Known target topology hypothesis: Accelerator up to12 contexts including candidates | Observer at most4 contexts, different composition |
| Credentials/trust/config/ALPN | actual owner / U-opaque |Exact public test PKI, copied/borrowed source boundaries | Worst allowed peer/config complexity unknown |
| Peer-info summaries/certificate copies | connection/query / U-opaque |Summary obtained/released via public API | Internal snapshot memory not individually bounded |
| Wait-set/readiness registration | runtime/op / U+K |Active membership PRECONDITION; successful governed removal/destroy | Opaque lists/platform-event memory unknown |
| Callback/timer/owner metadata | op/Session / U-managed |Candidate16KiB metadata/op,16 callbacks/8 timers; final controller sizeof248 x64/172 VC6 | Future full structs/callback implementation absent |
| Principal/grants/policy/immutable tuple | Session/op / U-managed |Must remain in Session/op metadata with generations and borrower references | Actual C2 structs absent; no secret token runtime implemented |
| Tokens/tombstones | op/process / U-managed |64 role-secret bytes plus bounded writer copies inside already charged spans; high-water identity instead of unbounded retired list | Exact CSPRNG/copy/wipe implementation separate ED06 |
| Retained Model E | original lifetime / U+K |Actual PRECONDITION measured; candidate terminal retention holds op+Session+host credits | Real OpenSSL R4 resources separate owner |
| Modules/images/CRT/stack/heap overhead | process / U-baseline/shared |Virtual view classification plus private/working-set/commit counters | Allocation/physical ownership attribution incomplete |
| Handles/threads/driver bookkeeping | process/owner / K |Normal loops handles/thread samples; external C1 samples | Quota charges are not all kernel usage |
| Logs and measurement observer | diagnostic process / U+K |Bounded run counts; trace files identify diagnostic overhead | Not production trace ring |
| Remote Site / network / other processes | X |Fixture-specific peer receipt separate from local native acceptance | No generic Internet delivery or memory guarantee |

No borrowed buffer is charged again as a new allocation. A fixed core slab contains queues/staging/metadata/scratch; do not add both slab capacity and its component spans. Nested C1 status/endpoint/token/lock are already included in412 bytes. Process counters are cross-checks, not extra allocations added to the ledger.

## Controlled reservations

Proposed core/op512KiB; Browser additional app1MiB; Session16KiB; managed process2MiB. Span plan:262144 queues+65600 DATA staging+148 notices+384 ACK slots+16384 op metadata+65536 C3 scratch=410196 inside524288, leaving114092 unassigned margin. Session803+819=1622 leaves14762 for metadata/other bounded Session storage.

Final candidate controller/entry sizes: x64248/48 bytes, VC6 x86172/32. Controller is charged once. Two Accelerator core ops/two Sessions plus controller=1081592 bytes x64. One Browser core+app+Session+controller=1589420 bytes VC6. These are controlled-model reservations, not full application allocations or malloc-overhead guarantees.

Session lifetime is independent: closing the final operation refunds its core/app only. CONTROL/Session16KiB, callbacks, policy/grants and dependency credit remain until confirmed Session destruction and borrower quiescence. Old Session generations cannot free a new incarnation. Retained child blocks both its op credit and parent Session credit; no eviction/reconnect bypass.

Opaque context2MiB and shared Browser8MiB/Accelerator4MiB thresholds from the earlier ledger are hypotheses, not measured/enforceable PST limits. Do not extrapolate a4-context fixture into the12-context target or call a virtual-map delta a provider-owned shared allocation.

## Candidate-cap decision

Managed2MiB is feasible for the tested fixed reservation layout and denial policy, subject to future full structs fitting their slabs. Preserve user/kernel numerical envelopes initially; no factual basis proves or revises them globally. Full Browser process, hostile peer/provider maxima and per-socket kernel totals remain unknown. The normal public SDK does not enforce those hard totals.

Current rule: no full C2 admission where a required hard category is unknown. An empirical lab policy requires an explicit owner decision, bounded input scope, host/process reserve and freshness policy, finite topology and stated residual risk. A fitting incremental quota never overrides exhausted host commit/physical/process capacity.

NT4_REAL=NOT_RUN; C12=PENDING. VC6 build on Windows10 does not qualify NT4/2000/XP runtime.
