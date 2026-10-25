# B+D resource reconciliation — bounded code facts, not native operational PASS

Compiler-only sizeof comparison uses exact preimage layouts and current types, same MSVC x64; program performs no process resource queries. layout-only.c/.cmd/.log kept externally. Same sizes across four model variants:

| Storage | Before | After | Increment |
|---|---:|---:|---:|
| PS_ADMISSION instrumented LAB |88|1024|936|
| IG_SAMPLE nested in existing observer |232|1112|880|
| IG_NATIVE_OBSERVER INCLUDING that sample |66248|67128|880|
| Added qualifier owner+observer storage, no double-count of sample | | |1816 bytes|

PS_BD_DESCRIPTOR680, inventory200, BD state936 bytes. Descriptor/inventory are nested once in owner and copied once as bounded observation in the sample; both copies are charged above. Additional bd_borrowed field consumes existing layout padding. Product/service PS_ADMISSION without the instrumented/model define remains88; no service runtime state increase. Code size/CRT/native provider allocations are not represented as these structure bytes.

Native BD binding owns at most process duplicate + control-thread duplicate + token (three persistent context references); during confirmation, two console handles bring this path to maximum five owned handles. Unexpected impersonation-token output is retained for cleanup on rejection, before confirmation, not added on top of the confirmation peak. Existing canonical mutex/intent/launch refs remain independently accounted; no operational intent/effect is created by this slice's read-only component. Configuration/image references are borrowed from canonical protected preparation, not new file copies/handles. Approved SID allocation is28 payload bytes for the fixed operator, released with LocalFree; allocator/provider overhead UNKNOWN, not28 total bytes. No password, file recorder, worker, synchronization object or diagnostic service added.

Original handles are not closed by sampler; cleanup invalidates bounded sample references and then closes owner refs only with quiescence/owner-phase proof. UNKNOWN retains owned state and prevents reuse. Fake retained handles in model fixtures are explicitly synthetic; natural cross-process fixture exit is not a retroactive native cleanup receipt.

Program-owned confirmation arrays:1024 WCHAR plan and two192 WCHAR reply/expected arrays =2816 payload bytes, plus compiler frame/local overhead; token/process revalidation uses fixed1024 WCHAR path and256-byte user buffer. These calls are sequential under bind, not independent simultaneous stack reservations. No complete transitive native/CRT/callback stack maximum is established. No address-space reserve is summed as physical stack use. Caller context-validator transitive stack/cost remains a future qualified-source responsibility. Bounded function frames are not process Private Bytes, total kernel cost or observable stack PASS.

Kernel object bytes, file/console provider internals, heap metadata/cache, transitive stack and between-sample peaks remain UNKNOWN. Live native resource eligibility was NOT measured. Existing administrative256MiB/1024/64/32MiB and native64MiB/128/16/8MiB monitors, global ceilings, smaller quotas, freshness/headroom and NONE_GRANTED remain; no margin or unused slot is credit. Exact1816 static increment is not an assertion that all ED11 gates close or that the128 live guard now passes. New native candidate hashes are recorded but not activated; old launcher/native pins remain historical and unchanged.

ED11_TOTAL_STATUS=PARTIAL
CONSOLE_ADMISSION=DENY
NATIVE_RESOURCE_MEASUREMENT=NOT_RUN
QUOTA_INCREASE=NO
HOST_CREDITS=NONE_GRANTED
