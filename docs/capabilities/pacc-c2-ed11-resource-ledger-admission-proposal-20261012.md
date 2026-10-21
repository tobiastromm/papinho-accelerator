# ED11 — total resource ledger and candidate admission policy
Date2026-10-12. Status: COMPLETE_ACCOUNTING_DESIGN, NOT_MEASURED/NOT_QUALIFIED.
[ED08 close order](pacc-c2-ed08-record-close-order-proposal-20261012.md), [previous ED review](pacc-c2-final-encoding-gate-review-20261012.md). Existing ED01-07/09-10/12-14 recommendations preserved, not accepted here.

## Ledger rules and dimensions
Every allocation/resource receives owning process/Session/op/generation/lifetime, category, logical capacity, actual allocation/handle, peak and retained status. Borrowed aliases contribute references, not duplicate byte allocations. Child, provider-global and runtime lifetime are different. Retained tokens are invalid for new admission but dependencies and accounting survive. External Site/provider-owned remote resources are not local RAM.

Class U=user-owned allocation/virtual commitment, K=kernel-owned local resource, X=remote/external/unattributable. Byte and object-count quotas are separate, never substitute for each other. Requested buffer sizes are not kernel allocation maxima. Working set, private commit, heap allocation and shared image mappings are complementary measurements, not summands.

| Resource | Scope/owner | U/K/X | Known bound or candidate reservation | Measurement needed |
| --- | --- | --- | --- | --- |
| Four APPLICATION/RECORD directional queues | Per op in each process | U | Approved lab4*65536=262144; high48/low24KiB | Actual allocation + occupancy + retained ownership |
| Two DATA role RX/TX frame staging | Per op | U | Candidate2*2*(16+16384)=65600 | Exact allocated versus borrowed spans; no extra frame queue |
| CONTROL normal RX/TX staging | Per Session | U |803+819=1622 | Allocations and partial-frame lifetime |
| CANCEL/FINAL reserves | Per admitted op, serialized Session owner | U |56+92=148; not borrowed from saturated DATA | Full/partial write and first terminal ownership |
| Role ACK/fence/event staging | Per op | U | Candidate≤8 pending fixed48-byte role-control slots inside op cap | Offset/state; no queue-growth on duplicate intents |
| C3 consumer peer I/O scratch/processor | Accelerator per op | U | Candidate bounded caller scratch, charged inside op cap; no second hidden payload queue | Exact buffer aliases/sizes after implementation |
| C3 pair + setup listener | Accelerator per op | K |2 live sockets plus transient one-accept listener | Kernel send/recv/TCP/handles; listener closes through own owner |
| Browser site socket/C1 pump | Browser per op | U+K | C1 zero payload queues; one pump/event/window; requested send/recv16KiB each | Pump sizeof, kernel overhead, owner thread/stack and retained refs |
| C2 CONTROL transport/PST wrapper | Per Session | U+K | One Connection and socket/secure PST context | Core opaque object + provider handshake/record/state + kernel |
| Two DATA transport/PST wrappers | Per op | U+K | Two Connection/socket contexts, Secure uses two PST connections | Provider queues, decrypted/encrypted buffers and release retention |
| Site TLS PST Connection | Accelerator per op | U | One public CLIENT object using C3 PST-side socket | Provider/native queues, opaque state, copied config |
| Unbound candidates before role claim | Accelerator global/Session | U+K | Candidate4 global/2 Session, one BIND staging≤84 each | Pre-auth TLS/credential parsing work and retained candidate ownership |
| Runtime/provider global state | Process/runtime | U | Shared once; lazy init charged before use | Normal package lazy initialization/global allocations and images |
| Credentials/trust/lease/config | Process/Session/op as actual owner | U | No alias duplication; borrowed owners retained | DER/key copies, anchors, chain/config/ALPN/name copies; secrets wiped only after borrowers |
| Public peer-info snapshot | Per querying op | U | Bounded snapshot lifetime, release after facts copied | Leaf/cert copies + internal provider allocations, not merely summary sizeof |
| Wait-set/external sources/readiness lists | Runtime/process/Session | U+K | One integration owner, finite assigned slots | Allocations, membership, wake event, FD/readiness arrays and platform handles |
| Callbacks/late event/tombstones | Per op + Session | U | Candidate≤16 outstanding/op, fixed reservation before async submission | Struct/alignment/queue entries, old generation owners and borrow refs |
| Timers | Per op + Session | U | Candidate fixed≤8/op within metadata; absolute budgets | Timer records/native timer handles if any; wake does not create timers |
| Tokens/operation metadata | Per op | U |32 bytes/role plus copies; proposed total metadata≤16KiB/op | All copies/writer scratch/erase; owner/counters/generation/tuple |
| Logs/trace ring | Process | U | Fixed secret-safe ring inside shared managed cap | No arbitrary payload/token string or unbounded test trace in normal admission |
| Thread/stack/module/allocator overhead | Process | U+K | No extra parallel C2 runtime; inherited owner/C1 topology | Committed vs reserved stack, DLL shared/private pages, allocator fragmentation |
| WinSock/window/events/handles | Process/owner | U+K | Owned ref/event/window/socket ledger | WSA references and module/driver state, process and system handle deltas |
| Model E/C1 retained resources | Original scope, indefinitely held | U+K | Consume full original quota/leases until confirmed destruction; no eviction | Actual retained child/kernel/credential state; not zero after release ATTEMPT |
| Site peer TCP/TLS/UI/HTTP consumer | Remote or app-owned | X/U | Site remote not local RAM; Browser consumer storage charged in baseline/op actual owner | Separate application bound; no Site TCP delivery claim from C1 send |

Known wire/storage subtotal for one op/Session endpoint=262144+65600+1622+148=329514 bytes. Separate op component=327892 (queues/staging/notices), Session component1622. This is NOT total memory and excludes metadata/TLS/kernel/baseline. Two such op/Session endpoints659028 controlled bytes, not512KiB total. Payload-only aggregate512KiB remains accepted experimental limit.

Browser application-consumer storage MUST be reserved too: candidate up to1MiB/op, preserving the existing response bound rather than lowering it to fit queues. It is separate from the512KiB core op cap but inside the2MiB managed process cap; maximum managed per Browser op is therefore1536KiB before Session/shared reservations. Already allocated baseline storage is charged once; new/growing C2 storage cannot be hidden in baseline. This refines the prior provisional Browser1MiB/16MiB envelopes: the proposed process/user caps here are2MiB/20MiB, not measured results.

## Total equations (no invented measurements)
U_total = U_baseline(existing code/ModeB/UI/etc)
 + U_shared_delta(provider/runtime/config/images/owner)
 + N_Session*(1622 + metadata_S + CONTROL_PST/wrapper)
 + N_op*(327892 + metadata_O + bridge/C1 + role_PST/wrappers + Site_PST_if_Accel)
 + N_candidate*(84 + metadata_C + secure_PST/wrapper)
 + allocator/stack/snapshot overhead not already charged.
K_total = all actual socket/TCP/buffer/driver, wait-set/event/window/timer/handle resources, baseline and retained included.
X = remote Site/network behavior and unobservable attribution, recorded separately; never set to0 by lack of API.

All N include live/closing/retained occupants. CONTROL connection persists when N_op=0 and is charged per Session. Lazy runtime/provider and credential aliases counted once by actual owning allocation. Caller-owned application storage outside queue cap must be charged; legacy ModeB1MiB response maximum is NOT a C2 buffer exemption. No opaque PST usage is inferred from sizeof(pointer).

## Target candidate caps — not measured, not product defaults
These are concrete owner-review envelopes, not runtime facts or enforceable opaque-PST guarantees. They can be revised only by documented owner gate; do not silently change toolchain/backend/queue defaults to meet them.

| Target/scope | Candidate user-managed cap | Candidate total incremental user envelope | Candidate kernel envelope | Status |
| --- | --- | --- | --- | --- |
| Browser VC6 x86, one configured C2 Session | Core op512KiB + app-consumer≤1MiB; Session16KiB; process C2-managed2MiB |20MiB beyond same-target baseline |2MiB beyond baseline | PROPOSED_MEASUREMENT_REQUIRED |
| Accelerator modern x64 | Op512KiB; Session16KiB; candidate16KiB; process C2-managed2MiB |32MiB beyond same-target baseline |8MiB beyond baseline | PROPOSED_MEASUREMENT_REQUIRED |
| Real NT4 Browser | Same VC6 candidate initially; smaller queue8/16/32KiB are measurement variants only |20MiB provisional, not claimed to fit any NT4 RAM size |2MiB provisional, not certified kernel limit | NO_VERIFIED_NT4_RUN/TARGET_IN_THIS_SLICE |

Candidate opaque TLS-context reservation≤2MiB/context and shared runtime/provider user delta≤8MiB Browser/≤4MiB Accelerator are **measurement thresholds**, not public API-enforced hard caps. Browser one C2 CONTROL+two DATA=3 contexts:≤6MiB thresholds; managed2+shared8+contexts6 leaves4MiB proposed allocator/stack margin inside20MiB. Accelerator two C2 Sessions+two ops*(two DATA+Site)+four candidates=12 contexts:24MiB; managed2+shared4 leaves2MiB margin inside32MiB. Preexisting ModeB/PST shared allocation in baseline must not be counted twice; still include its total in machine fit.

These envelopes are policy candidates precisely because measurements are missing. Public PST has no allocator reservation/cap API. An input-dependent provider allocation can exceed an assumed allowance; observing private bytes afterward is NOT fail-before-effects hard enforcement. If owner requires a hard worst-case total, normal package contract/allocator/input-limit evidence is an upstream dependency. No C2 admission should be enabled on estimates alone.

Machine fit criterion must include baseline user/kernel, incremental envelopes, shared physical mappings without duplicate accounting, OS/driver background reserve and owner-approved safety/headroom. Unknown installed RAM/headroom or unmeasured provider/kernel category denies target qualification. No universal NT4 minimum RAM or compatibility is asserted. Browser tools/VC6 native current-host proof is not an NT4 run.

## Candidate admission counts and fixed reservations
Accepted experiment:≤2 global operations,≤1/Session; C1≤2 live owners including observer-retained. Proposed tighter Browser configuration:one C2 Session AND one independent existing-service Session per single selected endpoint/profile, including retained old Session; one C2 op within that Session. Server:≤2 eligible C2 Sessions,≤2 ops globally,≤4 unbound candidates total/≤2 Session. Count all retained Session/candidate/op owners against these caps; no reconnect to bypass retention. Existing server pre-auth/listener capacity remains separately required; C2 subset quotas cannot pretend to bound all unrelated Sessions.

Before accepting/opening:
1. Check owner authority, exact grant/profile/tuple, fixed slots, counters/generation and same-target validated allocation envelope.
2. Reserve op/Session managed bytes, callbacks≤16/op, timers≤8/op, role-control slots, CANCEL/FINAL buffers, future native/provider/credential budgets and kernel headroom. Increment one ledger atomically under owner.
3. Only then perform new socket/TLS/bridge/role effects. Failure rolls back only confirmed uncreated/destroyed resources; ownership_accepted or retained disposition keeps reservation.
4. At any unknown hard-cap category, deny enablement. At measured high-water stop new admission. A runtime overrun is truthful cap failure/containment, not evidence that admission was hard-bounded.
5. Release credits only after each lifetime DESTROYED/own native close confirmed AND callbacks/borrowers quiescent. PRECONDITION/UNRESOLVED or C1 failed close retains full slot/dependencies. Do not reclaim based on Stop, timeout, writer reset or release ATTEMPT.
6. Fair scheduler:CONTROL opportunity before additional DATA,≤8 opportunities/wake,≤16KiB/flow, round-robin operation/role/direction. Reads pause at approved high watermark and resume below low. ACK/control reserved slots remain schedulable when payload queues saturate. Partial frames never spliced.
7. No destructive eviction, victim closure on wrong token, implicit fallback or process exit as clean capacity recovery. Process containment requires separate operational policy and never qualifies clean/exactly-once.

## Measurements and decision boundary
[Target measurement plan](pacc-c2-ed08-ed11-target-probe-plan-20261012.md) defines accounting/run barriers and normal/test-only separation. [Upstream/owner handoff](../integration/pacc-c2-ed08-ed11-upstream-and-owner-handoff-20261012.md) requests only missing contract/limit authority, not a PST rebuild.

ED11 design ledger and candidate policy are complete as a review deliverable. Numerical total limits, enforceability, current-host/NT4 fit and runtime admission remain NOT_QUALIFIED. ED11 cannot be approved as a measured resource guarantee until every U/K category is bounded and the target gate accepts residual X uncertainty.

## 2026-10-12 governed execution supplement

The preceding proposal records its original NOT_MEASURED state and is preserved byte-for-byte. Current evidence is now in [total resource closeout](ed11-total-resource-closeout-20261012.md) and [reconciled ledger](ed11-total-resource-ledger-reconciled-20261012.md).

The owner accepted [incremental-envelope meaning](../adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md), not numerical caps or encoding. Final controlled policy50 checks/build PASS in x64 Debug/Release and VC6/C89/current Windows, including Session lifetime independent of its final operation and original retained host credits. Native loop/counter/mapping and immutable C1 component observations are separately classified. Full provider/kernel/Browser/NT4 total limits remain NOT_QUALIFIED.

[Final conformance](ed11-total-resource-final-conformance-20261012.json), [RFC supplement0.3.4](../protocol/pacc-c2-ed11-total-budget-supplement-draft-0.3.4.md), [owner handoff](../integration/ed11-total-resource-final-owner-handoff-20261012.md). Earlier evidence/values and specification0.3/0.3.2/0.3.3 are preserved. No PST/Browser/wire/pin change or C2/C3 production implementation.
