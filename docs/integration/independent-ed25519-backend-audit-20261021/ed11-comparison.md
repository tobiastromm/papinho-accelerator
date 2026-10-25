# ED11 comparison: estimates, previous measurements and UNKNOWN

No new backend was compiled, linked or run in this audit. No operational
capacity is qualified. Windows3145728 only explicit process profile;
Legacy2097152 and individual quotas unchanged. No disk-size or source-line
comparison proves a memory saving.

| Cost | B: source-derived OpenSSL math | A: small-library candidates | C: own | D: independent full OpenSSL |
|---|---|---|---|---|
| Source footprint |33 function bodies34871 UTF8 bytes/1221 lines measured, plus types/constants/glue, roughly35-45KiB source planning envelope, not built code | Monocypher complete core102580/header12175 source bytes; libsodium closure depends on build/link stripping, not assumed whole library or one function | Order-of-magnitude1000-2000 arithmetic LOC comparable to reference, plus tests; uncertain, no implementation | Original curve unit207666 source bytes/5879 lines plus upstream build tree |
| Code resident/text | New subset UNKNOWN; prior TEST code42496 Debug/28160 Release includes harness and other code, not a delta/bound | UNKNOWN for selected build; static link not zero | UNKNOWN | UNKNOWN, linkage/loader isolation must be included |
| Static data | Declared generic table Bi=960; d/d2/sqrtm1=120; four64-bit masks32; wrapper constants96; fe zero32: gross1240 bytes before pooling/alignment, not linked measurement | UNKNOWN selected closure/tables/CPU data | UNKNOWN | Existing tables plus new96 source bytes, final image/init UNKNOWN |
| Owned heap | Allocation-free proposed math topology; prior point path observed0 OpenSSL allocator calls, not proof for extraction/CRT | Function/closure/init must be measured; whole-library services not implicitly free | Unknown until design/proof | Hook can be pure math, but module/provider/runtime/init costs separately count |
| Stack | Prior internal subtotal4032 Debug/3376 Release, external frames unbounded there; not a new-component bound | UNKNOWN target/compiler/helper depth | UNKNOWN | Prior hook listing evidence is not whole-stack peak |
| Buffers | Borrow32 input; existing candidate local point160/result120/re-encode32 are source objects, not complete frame | At least input32; no assumed scratch reduction | Types/coordinates/scratch newly required | Same local math plus opaque runtime graph |
| Initialization | Proposed subset no runtime provider/RNG init; compiler/CRT init still count | Explicit library-init/CPU/CRT/OS closure audit required | Must define and measure | Independent loader/config/provider/CRT coexistence may add costs |
| Retention | Synchronous borrow, no stored key/ref; owner retains caller graph until factual closure | Same required contract, no implicit release | Same required contract | Module lifetime plus allocator ownership cannot cross copies |

The source subset still needs final types/constants/include ordering and symbol
closure review. The numeric source estimate is inventory-based, not an ED11
reservation or accepted source layout. The1240 figure sums declarations;
coalescing must be proved before discounting; alignment/metadata can add bytes.

Previous known fixed1952624 Selected/1790872 Legacy and analytical explicit
coexistence2172856 remain unchanged. The earlier isolated Roughtime graph52904
plus instrumented provider peak353396 gave406300 in that TEST; it does NOT
bound a new library, independent OpenSSL instance or complete root-v4 process.
Do not add every historical test maximum and call it a proved simultaneous
peak, nor subtract it as a reuse credit. Retain root/current/candidate/issuer,
snapshots/pins, buffers, package crypto, caller stack and retained multiplicities
as applicable. No lifetime separation is assumed.

Mandatory ledger for implementation: compiled code/rodata/data, loader/module
and CRT charges, initialization, maximum nested stack including external calls,
any heap/scratch/buffers, owner-held inputs/status/source graphs, retained work,
concurrency and real root/issuer/Roughtime coexistence. Provider/kernel costs
UNKNOWN remain UNKNOWN. Choose a target only after measured reservation/gates;
no favorable source/host-memory assumption. Every candidate remains fail-closed
if incomplete or over budget. ED11_STATUS=PARTIAL.
