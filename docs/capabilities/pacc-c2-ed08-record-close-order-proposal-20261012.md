# ED08 — proposed RECORD close-order contract
Date2026-10-12. Review slice: ED08/ED11 focused closeout.
Status PROPOSED; semantic direction conditionally approved, exact wire not approved.
Authority: Accelerator ADR-0022/0029; PST accepted ADR-0008/0009; preserved C2 semantic3/spec0.3 and [final ED review](pacc-c2-final-encoding-gate-review-20261012.md). Does not supersede accepted ADRs or silently change candidate1 fields/messages.

## Factual authority table
| Fact | Legitimate source | What it proves | What it does not prove |
| --- | --- | --- | --- |
| PST_TLS_COMPLETE | Public shutdown call OK, operation COMPLETE, operation-result OK, same CLIENT identity | Reciprocal TLS operation completion | Provider output drained, C3 receive count, PACC delivery, native close |
| PST_RECORD_OUTPUT_DRAINED | Conditional external C3 observer: known producer ceased, full ordered receive through actual EOF, prefix accounted | All bytes arriving on that attached bridge stream before orderly EOF observed | Hidden unsubmitted provider records unless upstream completion contract confirms them |
| C3_BYTES_ACCEPTED_BA | Actual send return on consumer-owned C3 peer | That exact prefix accepted by native bridge transport | PST internal read/record processing |
| PACC_RECORD_PREFIX_ACCEPTED | Actual full framed-write completion/count on corresponding RECORD DATA | Prefix submitted to protected/plain PACC transport | Browser frame receipt, C1 acceptance or Site TCP delivery |
| BROWSER_RECORD_PREFIX_RECEIVED | Full validated frames/offsets on Browser owner | Exact admitted ordered payload received | Native send or remote site receipt |
| BROWSER_NATIVE_SEND_ACCEPTED | Actual C1 send result under matching token/generation | That prefix accepted by Browser site socket | Site TCP delivery or TLS peer processing |
| SITE_TCP_DELIVERED | Independent authorized fixture observations when available | Fixture-specific actual peer receipt | Guaranteed by a PACC fence, C1 send count or generic normal peer |
| FENCE_ACK | Full matching role/direction/count after stated downstream accepted total | Only declared downstream prefix boundary | Global socket order, PST consumption, TLS completion or destruction |
| Model E DESTROYED | API call/record OK plus DESTROYED | PST child destruction confirmed; pointer invalid | Whole operation finalization, preserved provider-output delivery |
| Retained | Observed PRECONDITION_RETAINED or CLOSE_UNRESOLVED_RETAINED | Same references/dependencies remain owned | Safe close retry, clean terminal or freed quota |

Native readiness/WOULD_BLOCK is not EOF/drain. No private backend signal, guessed TLS packet size, sleep or cross-process timestamp becomes authority. Names above are documentary event identities, not new public APIs/wire fields.

## Discovery: previous AB-first plan is incomplete
The preceding final-review proposal listed PST release→AB drain→Browser BA seal. This cannot cover a nonempty admitted BA tail: its downstream is native send into the PST-side bridge, which may already be closed after DESTROYED. Waiting to ACK BA then is impossible or falsely counts destruction as delivery. Existing proposal remains historical evidence; this document refines it without altering those bytes.

## Proposed order using existing twelve candidate messages
The following additional STATE GUARDS require explicit owner approval. They allocate no thirteenth message/enum, but MUST NOT be implemented as if they were already accepted.

1. Finish admitted APPLICATION BA writes while public PST state permits writes. Its BA fence ACK requires the entire actual public-write-accepted prefix; a fence is not HTTP request-end/TLS half-close. Required plaintext reads/delivery must complete BEFORE local shutdown starts, or outcome is non-clean. Peer-initiated read CLOSED preserves already obtained plaintext/facts, then reciprocal shutdown proceeds. User cancel/Stop is a different non-clean branch.
2. Perform bounded reciprocal shutdown through public PST, continuing opaque RECORD transport on both directions. No APPLICATION read after C_SHUTTING, no synthetic COMPLETE. Failure, loss or deadline terminates without clean success.
3. After factual COMPLETE and after all admitted APPLICATION AB bytes are submitted, emit APPLICATION AB FENCE. **Proposed normal-close guard:** this particular fence is eligible only after reciprocal completion and plaintext-source quiescence. It still attests only its APPLICATION prefix; the shared phase guard supplies closure coordination, not a new TLS verification mask.
4. Browser observes full valid APP AB fence for exact operation/generation. Its one owner freezes NEW site recv/admission for RECORD BA (read-interest selection, not C1-wide cancel or native close). Finish any already-entered bounded recv and capture its admitted tail. This is an intentional producer stop, NOT raw EOF/close_notify or site delivery. C1 SEND remains permitted for pending AB records. No timestamp/global ordering inference.
5. Browser submits all its already admitted BA prefix while the PST child socket still exists, then emits RECORD BA FENCE on that same ordered RECORD stream. Accelerator confirms complete native C3-peer send acceptance of that prefix and emits BA FENCE_ACK. If those writes cannot complete without further forbidden PST I/O, fail boundedly, no fake ACK.
6. Accelerator requires validated APPLICATION fences/ACKs and RECORD BA fence/downstream accounting, with BA ACK fully written, before removing PST wait-set membership and making eligible try_release. No later native BA send or new PST I/O is permitted after this point. Keep manager, C3 consumer peer, PACC DATA/CONTROL and borrowed shared dependencies alive. Full local ACK write is distinct from Browser receipt.
7. Only OK+DESTROYED invalidates that PST child. Then drain the independently consumer-owned C3 peer through factual ordered EOF within existing bounded budgets, preserve every returned AB byte, and retain the manager. A reset, timeout, missing EOF or late violating BA bytes yields non-clean result. Never close/retry the PST-owned socket. PRECONDITION allows only governed membership correction; CLOSE_UNRESOLVED freezes affected operation I/O and keeps dependencies/quota.
8. After child-producer quiescence AND all C3 AB bytes before EOF are admitted/submitted, emit RECORD AB FENCE. Browser completes actual C1 native-send accounting before ACK. If close_notify bytes were queued on AB they must not be discarded to produce EOF/clean PASS. C1 send-direction end follows accounting; its read producer may already have stopped intentionally in step4.
9. Full four-direction confirmations plus confirmed eligible finalization permit server FINAL. Browser separately requires its own native site/DATA finalization before local clean completion. Remote FINAL is not Browser native destruction authority. A retained child/owner is never clean and cannot be evicted for quota.

Application AB fence must not be emitted early just because a response queue is empty. Without owner acceptance of the proposed phase guard, current generic END_PREFIX does not supply BA seal authority. A failed/canceled early application seal does not drive steps4-9 as clean closure. No new interpretation of old NETWORK_HALF_CLOSE occurs.

## Direction and state invariants
| Role/direction | Producer end authority | Downstream ACK | Required phase |
| --- | --- | --- | --- |
| APP BA | Authorized end of admitted plaintext, not directional TLS close | Public PST plaintext accepted | Prefix drained before shutdown |
| APP AB | Source stopped; all admitted plaintext submitted; proposed post-COMPLETE normal guard | Authorized Browser-owned storage | Coordinating fence, no UI/HTTP attestation |
| RECORD BA | Browser-owner producer freeze after validated normal-phase APP AB fence, plus admitted tail | Actual C3-peer native send accepted | Must confirm BEFORE PST child destruction |
| RECORD AB | Destroyed child + ordered C3 EOF/drain + upstream guarantee for provider-held output | Actual C1 native send accepted | Last RECORD seal AFTER child destruction |

All counters are checked U64 payload-only offsets; local received/submitted/downstream/escaped totals differ. Directional seal forbids future payload, not unrelated reverse role-control ACK. Every event requires exact op/generation/channel/owner and current normal-close authority. Revocation prevents new payload submissions; do not use the normal bounded tail plan to submit unauthorized bytes after revoke. Borrowed in-flight buffers survive until owner quiescence.

## Deadlock analysis, not runtime qualification
BA admission-freeze precedes BA fence, which precedes child release, which precedes AB EOF/seal. This removes the identified release-before-BA cycle. Browser does not wait for final AB fence before freezing BA, and does not disable site send while AB drains. CONTROL retains CANCEL/FINAL reserves; ACKs use bounded role-control capacity, not a full payload queue. No success depends on remote TCP EOF when normal-phase producer-stop is authorized.

This graph is acyclic at contract level. It does NOT guarantee peer progress: filled native buffers, missing peer close_notify, frozen downstream storage or lost channels fail under fixed deadlines. It does not prove provider COMPLETE has no hidden unsent record output. No sleeps, arbitrary drain, alternate destruction or retained retry.

~~~mermaid
flowchart TD
    A[Application delivery guards] --> B[Public reciprocal COMPLETE]
    B --> C[APP AB fence received by Browser]
    C --> D[Browser stops new BA reads; sends admitted BA tail]
    D --> E[BA fence: C3 accepted prefix and full ACK write]
    E --> F[Governed membership removal and try_release]
    F --> G{Disposition}
    G -->|DESTROYED| H[C3 peer ordered EOF; preserve AB prefix]
    G -->|Retained| R[Retained non-clean; no fallback]
    H --> I[AB fence and C1 send-accepted ACK]
    I --> J[Four confirmations and owner finalization]
~~~

## Minimum upstream dependency
Request normative clarification/test evidence for unchanged normal0.6.5 public socket transport:
U08-1: after successful reciprocal COMPLETE, are all required TLS records submitted to attached transport, with no unsubmitted provider-held TLS output that could be dropped by subsequent eligible release?
U08-2: does normal eligible OK+DESTROYED preserve ordered transport output so consumer peer may observe all submitted bytes before orderly EOF; what outcomes mean reset/non-preservation?
No public provider byte-counter/API extension is requested initially. External EOF can delimit native stream, but cannot prove absent provider-held output by itself. If owner cannot affirm U08-1/2, do not assert drain authority; request the smallest documented public completion/transport-drain contract or supported observer mechanism upstream. No PST change/rebuild is authorized.

Upstream question is prepared in [dependency handoff](../integration/pacc-c2-ed08-ed11-upstream-and-owner-handoff-20261012.md), not sent. Runtime recipe/evidence matrix in [probe plan](pacc-c2-ed08-probe-evidence-plan-20261012.md). ED08 is a defined candidate close-order, NOT_QUALIFIED and pending owner/upstream confirmation.
