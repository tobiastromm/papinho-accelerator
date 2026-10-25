# Alternatives: causal comparison and approval evidence

Relative work S/M/L is an engineering estimate, not a promised duration. It includes
review/tests, not only line changes. No alternative authorizes implementation.

Common invariant baseline for ALL viable rows: unchanged ROOT06/v2/v3/v4 bytes,
CP/hash domains, one canonical root/worker; full predecessor/native identity CAS;
UNKNOWN preserves resources/generations/credits; Stop never proves cancellation;
no CURRENT/issuer/status/UTC authority from DATA; no timeout release. A row that
cannot preserve this baseline is rejected. VC6 compatibility of an interface is
not NT4/box/ISA runtime qualification. No current quota increases are proposed.

| Option | Cause addressed; affected files/functions | Benefit and ED11 effect | Risk, portability, dependencies, upkeep, relative work | Evidence / decision |
| --- | --- | --- | --- | --- |
| A Keep BCrypt, lifetime only | server_store_hash/helper and issuer/store02 cleanup | Contains reference-loss/accumulation; already scoped store03 fix. Opaque bytes/latency remain | Low algorithm risk, Windows native dependence, retained resources can close admission indefinitely. Low maintenance/S; NT4 not made BCrypt-capable | Existing93+17 checks pass; still requires provider/kernel target policy/bounds. Containment, not complete solution |
| B Deterministic storage SHA | private storage math/helper, store03/digest storage and hash callback uses | Removes hash-object/provider allocation from exact migrated math closure; bounded arrays make stack proof tractable | M; new pinned subset/license/header/VC6 qualification, byte-equivalence and overflow/Stop tests. No CNG/Win32 core requirement; other targets unqualified | Source no-provider closure exists in BearSSL candidate; compiled closure/KAT/stack NOT yet proved. Necessary component of primary, not whole-worker cure |
| C Split storage/issuer callbacks | backing_capture/borrow, cohort/act storage hashes, root codec storage bindings | Stops correlation math from implicitly entering issuer native hashing; preserves issuer API | M; missing a call site leaves BCrypt reachable. Same portable callback contract; explicit owner/context gates must remain. Small long-term boundary | Static67-symbol prior inventory plus current source graph. Test both callbacks independently; never replace issuer/signature API globally |
| D Reorder phase scratch lifetimes | capture/workspace, prepared descriptor, worker handoff | Can release scratch after synchronous native borrows end while keeping protected evidence and immutable material | M/L if redesign broad; TOCTOU/UAF risk. Same owner; do not drop status protection or native revalidation. No new library | Existing decode arena reuse is proved; further savings are hypotheses requiring borrower/last-use proof. Start by measuring phases, not deleting buffers |
| E Precompute hashes/revalidate minimum | borrow/checkpoint_match/act_match, per-generation metadata | Reduces repeated math/provider work; possibly fewer transient calls, not automatically smaller fixed graph | M; corruption-detection contract, stale cache and callback-version risks. Portability neutral, extra metadata/maintenance | Same exact bytes/domain/serial/owner immutable scope needed. Current mutation tests reject changed backing; removing rehash requires preserving or revising that invariant. Defer initially |
| F Rewrite backing validation block | v3_backings + root8_data/signer + validation call structure | Can consolidate redundant traversal and make candidate/published access explicit | L; large regression surface in corruption/history/Stop. No intrinsic format change, but a rewrite can accidentally weaken semantics. High upkeep transition | No objective need for full rewrite: narrow callback/version/view refactor reaches same seam. Reject broad rewrite as first move |
| G Existing minimum math module | BearSSL3 source units, or independently pinned OpenSSL pure-C SHA subset if BearSSL fails | B with concrete provenance/symbol boundary, no provider state in math | M; OpenSSL alternative has more config/header/helper closure; preserve upstream arithmetic/license, no whole TLS/EVP library. Actual VC6 qualification mandatory | Primary uses BearSSL candidate. Backup only for failed provenance/equivalence/VC6/stack gate; not another implementation search |
| H Reduce buffers/copies/retention | committed/previous/next, G7 slots/CA, issuer snapshots | Upper-bound opportunity: image copy or duplicate CA could be removed only if same-owner frozen lifetime spans worker/UNKNOWN | L/risky until closed graph. Worker currently forbids root pointer and copies isolate races. Retention cannot be evicted. Portable concept, backend proof required | Previous observed-image reduction saved16,384 with bytewise read proof; repeat that standard. No current full-cap excess justifies risky storage sharing |
| I Keep code, revise qualification only | per-phase ledger/target metrics and tests | Corrects category double-counting and separates bounded DATA proof from operational activation; no byte saving | S/M; cannot fix v3-only code, missing factual resolution or lost handles. Any empirical exception needs exact approved target policy | Accepted ED11 permits implementation exercising deny. Useful mandatory method, insufficient alone because functional gaps are directly proved |
| J Same-owner typed prepared-storage boundary (PRIMARY) | narrow B+C plus explicit v3/v4 candidate/published validation and phase ledger; same worker/CAS | Closes architectural cause: bytes/math, protected provenance and native effect no longer collapsed into issuer hash callbacks. Leaves known native residuals explicit instead of moving them | M staged; no new owner/store/protocol. Reuse existing codecs/slots; keep rehash/CAS initially. Best maintenance boundary with existing code; target qualification still mandatory | Capsule experiment first, then canonical D>0 kind8-v4/candidate access and native resource/authority gates. No whole-path PASS is claimed from local math |

Why J over an entire replacement engine: current full-byte transactional CAS and
UNKNOWN tests already support its safety structure; none of the observed facts
requires another journal, helper process, root or worker. Those would introduce
new persistence/recovery protocols and move opaque native costs rather than
close them. Native TxF target availability itself remains a platform/backend
qualification gate; no NT4 support is inferred from the modern Windows backend.

Definitive versus interim: A is interim containment; E/H are optional optimizations;
I is a necessary correction to proof methodology; J is the durable responsibility
boundary with B/C math separation. G is the single concrete backup implementation
of J's math part, not a competing architecture. Unsupported or unavailable factual
sources still deny; no architecture can legitimately remove that security gate.

## Discriminating experiments, including what was actually done

| Hypotheses distinguished | Minimal experiment and observable PASS/FAIL | Status in this audit |
| --- | --- | --- |
| v4 blocked only by memory vs functional version gates | Inspect hold_data image version, act bounds, worker guard, kind8 header/borrow; show paths reject4 before positive effects | DONE: independent code blockers found; not an environment hypothesis |
| hash-only provider vs native worker dependency | dumpbin imports/symbols plus static call sites. Dedicated capsule later must show no reachable provider; whole binary import absence/presence alone insufficient | DONE: current fixture imports BCrypt, BCryptGenRandom and Ktmw32; static hash edges corroborate; capsule not built |
| DATA fixtures qualify canonical components vs modeled kind8 | Trace component4 -> component3 and inspect canonical signer header3 guard | DONE: fixtures model kind8, so their positive v4 coverage is appropriately limited |
| unsafe hash retention vs successful native cleanup | Existing isolated helper/owner tests and source reference count; controlled withheld cleanup is distinguished from native failure | DONE: Debug/Release5 suites each pass; no new native failure forced |
| actual managed overrun vs scenario arithmetic | Current sizeof ledger with nested attribution; separate user/kernel counters, scenario lifetimes and overlapping measures | DONE by source/evidence reconciliation: no full operational maximum or actual overrun proven |
| deterministic math preserves storage byte semantics | Exact pinned capsule with independent KAT, streamed/large boundaries and old/new codec differential output | NOT RUN: one next implementation slice, separate authorization/executor review required |
| repeated hash safely cacheable vs corruption requirement | Approved test preserves immutable identity then deliberately mutates backing; cache cannot return prior success on disallowed mutation | Existing tests demonstrate rejection; proposed cache experiment not run and no cache added |
| Stop has bounded signalling vs bounded synchronous return | Instrument lock/provider intervals in isolated target; stall provider shim outside production, request Stop and distinguish atomic signal from return | NOT RUN; source lock edge proves missing latency proof, not actual hang. Capsule reduces storage-native edge; full owner proof remains |
| phase separation yields real savings vs late borrows | Trace each arena last borrow with retain/UNKNOWN paths; reuse only after factual release; measure exact nested frames/runtime categories | Only existing decode->BODY arena reuse and512 compare scratch established; further reuse NOT claimed |

No new fixtures, credentials, CURRENT, system clock change or network were
needed to discriminate the principal root-cause hypotheses. Further experiments
are concrete and narrow, not a request to repeat a general readiness matrix.
