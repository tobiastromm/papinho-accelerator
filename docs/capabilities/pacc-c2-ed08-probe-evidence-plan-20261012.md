# ED08 evidence matrix and non-invasive probe plan
Date2026-10-12. PREPARED_NOT_EXECUTED. No C2/C3 implementation or E1-E12.
[Proposed close-order](pacc-c2-ed08-record-close-order-proposal-20261012.md).
Independent raw socket/Public PST probes are not integrated C2 qualification.

## Evidence already available, preserved
| Evidence | Class | Relevance | Limit |
| --- | --- | --- | --- |
| Public API3 header and normal libraries (OpenSSL x64, NSS VC6) | Normal immutable SDK identity and public symbol inspection | Shutdown/interest/try_release/peer-info exist | Not a C3 drain or C2 runtime run |
| PST ADR0008/0009 | Accepted contract | Provider-exclusive native close, observed dispositions | Does not promise record-output flush or a retained memory bound |
| C1 qualification20261011-222128 | Existing native current-host test/compiled VC6 | Actual partial send/recv, backpressure resume, prefix/site-send counts, drained EOF, late events | No PST/C3/PACC role integration; NT4 not run |
| Existing Browser Model E native-retention package evidence | Separate governed TEST_ONLY evidence | Retained owner/provider/credential dependencies, no unsafe retry | Never normal-package native failure or fresh ED08 PASS |
| Source audit PST read/shutdown state | Read-only source evidence | Public read forbidden after shutdown start | Source logic is not a normal binary execution |
| Close-order dependency graph | Proposed analytical ordering | Removes identified BA-after-destruction cycle | Owner approval/upstream output guarantee and execution absent |

## Probe definitions and exact oracles
Each standalone probe uses exact normal SDK/CRT hashes, one durable owner, public CLIENT APIs, controlled TLS1.3 fixture certificates, fixed payload and bounded queue sizes. No consumer-faked operation results, private symbols, native failure injection, C2 proposed wire sent to a real peer or automatic Browser route activation. A standalone socket-pair fixture is observation topology only, not a shipped C3 implementation.

| ID | Trigger/barrier | Positive observation | Relevant negative | Status |
| --- | --- | --- | --- | --- |
| EP01 | Factual handshake/TLS facts; controlled peer close_notify; actual bridge backlog before shutdown complete | COMPLETE and separately recorded remaining C3 bytes; EOF count kept distinct | Queue-empty/WOULD_BLOCK cannot substitute for drain | PREPARED |
| EP02 | Actual C3 native send returns partial/WOULD_BLOCK, fixture reading paused by explicit coordination | Partial actual count retained, readiness resumes same prefix | No fabricate NEED_WRITE or count requested length | PREPARED |
| EP03 | All queues reach high watermark; actual native/PACC downstream stalled | Interest removed, bounded held bytes, resume below low | No CONTROL/ACK starvation or new allocation growth | PREPARED; PACC portion requires later approved integration |
| EP04 | Cancel after recorded nonzero accepted prefix and BEFORE later drain completion | No new unauthorized submit; first cause/escaped bytes preserved | No normal-close tail flush after security revoke | PREPARED |
| EP05 | BA tail admitted before producer freeze; same owner serialized freeze | BA prefix native accepted and ACK completed before PST release | BA ACK after producer DESTROYED is invalid | PREPARED; role ACK requires approved C2 |
| EP06 | PST DESTROYED then real C3 peer EOF after all bytes returned | AB prefix retained and C1 accepted before ACK | RST/missing EOF/timeout blocks clean | PREPARED; upstream U08 confirmation required |
| EP07 | Attempt FENCE before captured tail or producer end | Identity/offset/state check rejects; zero false clean | APP AB pre-COMPLETE coordination is forbidden | PREPARED; NOT a fabricated live wire event |
| EP08 | Full FENCE but partial downstream accepted total | ACK stays pending until exact actual prefix | Queued/received bytes are not downstream send acceptance | PREPARED |
| EP09 | Old generation callback queued; owner generation revoked | No dereference/new I/O/publication; dependencies remain alive | No resurrected channel or global-count inference | EXISTING_C1_SCOPED + integrated PREPARED |
| EP10 | Actual peer close_notify versus raw recv0/reset | Public clean/truncated facts remain separate | Raw EOF never manufactured as close_notify | PREPARED |
| EP11 | Active public wait-set membership at release | Observe INVALID_STATE/PRECONDITION_RETAINED; no native close; correct membership then eligible release | No discard on release ATTEMPT or assume disposition | PREPARED_SAFE_PUBLIC_PRECONDITION |
| EP12 | Retained disposition from existing governed evidence | Same owner/leases preserved, no new I/O/release/fallback | No newly induced unsafe native close failure | HISTORICAL_TEST_ONLY_REVIEW_ONLY |
| EP13 | Reverse initiation and EOF-less site | Normal phase can stop BA without raw EOF only under approved guard | No unbounded waiting or loss of AB send permission | PREPARED; owner guard approval required |

## Trace recipe and ownership controls
Per event: run_id, local monotonically increasing event sequence, Connection token, operation ID/generation, socket-lifetime token, owner, role/direction, requested length, actual accepted/received length, payload offset, queue total, PST call return/operation/result, wait-set membership, release-info/result/disposition and observer state. Runtime IDs remain trace-local, never wire or authority. Do not log tokens/keys/arbitrary payload.

Observe public call boundaries and native operations on **fixture/consumer-owned** sockets only. No wrapper may intercept/change production PST results or native close mechanics. Order comes from same-owner sequence and explicit barrier acknowledgements, not cross-process timestamps. Need-write/queue barriers must be factual actual I/O outcomes; if not established mark INCONCLUSIVE, never timeout PASS.

Tail probes preserve buffers until completed/abandoned writes quiescent. Any release-unresolved path stops affected operation work, retains manager/provider/runtime and occupancy, and ends probe with truthful non-clean retention evidence. Never retry PST/provider native close, terminate process as clean evidence or use last-known socket handle after ownership transfer.

## Why no native probe was executed in this slice
Normal C2/C3 does not exist and candidate wire/phase guard is unapproved. A raw socket experiment could establish only TCP behavior, not U08 provider output semantics or integrated four-fence authority. Existing C1 native facts already cover that limited boundary and are not rerun. U08 clarifications are missing; constructing a live full record-drain harness now would embed that unapproved lifecycle. Prepared probes satisfy the requested preparation branch and avoid repeating old evidence or implementing C3 under a probe name.

Later standalone public-package probes may execute within a separately accepted recipe; integrated PACC cases require implementation authority. E1-E12 remains a distinct, unauthorized execution gate. No real/synthetic native failure is newly induced.
