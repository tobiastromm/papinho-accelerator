# Resource and qualification diagnosis

## Existing authoritative budget

ADR0034/spec11.3 accepts per-process controlled managed allocations and distinct
provider/kernel/host evidence. The accepted Windows W1/W2/W4 decision specializes
only the explicitly selected managed process ceiling to3,145,728. Default/Legacy
remains2,097,152. Per-operation512KiB, per-Session and flow quotas are not raised.
Incremental user32MiB/kernel8MiB remain candidate envelopes, not qualified reserves
or parts to add to managed memory. Unknown required target evidence fails closed.

The incremental-envelope decision requires total, factual pre-C2 baseline,
attributed increment, peak, shared attribution, retained charges and host margin.
A pure sizeof censo does not supply these facts; neither does a process peak.
Spec11.3 permits UNKNOWN represented as denial while implementation proceeds;
a separately approved target uncertainty/freshness/headroom policy is required
before any empirical exception can permit admission. No current exception exists.

## Reconciled known ledger

| Item | Current measured or prior proved layout | Classification |
| --- | ---: | --- |
| Selected fixed graph | 1,529,824 | sizeof graph containing root, pending, G7 and other fixed participants |
| Base/secure/scheduler/bind arrays | 319,488 +25,088 +4,432 +144 | distinct existing arrays, not included in fixed graph |
| Worker stack/checkpoint reserves | 65,536 +8,192 | already charged once; enclosing/full nested proof still required |
| Current known Selected | 1,952,704 | not operational peak |
| Minimum v4 committed/previous/next growth | 3*(31,840-30,592)=3,744 | type-only projected change; worker not implemented |
| Projected known Selected | 1,956,448 | not full maximum; arithmetic difference1,189,280 to cap |
| Legacy current known | 1,790,952 | separate profile, no v4 grant; difference306,200 |
| G7 three slots | 3*40,120=120,360 | already inside fixed graph; packet31,840+CA8,192+metadata88 each |
| Issuer snapshots/stage | 24,576 +8,192 | distinct from G7, already in fixed graph |
| Current three images | 91,776 | committed +previous +next, nested in graph; projected95,520 |
| Pins/slots | 1,024 +3,456 | nested; 32 tickets do not imply32 complete backing copies |
| New BCrypt context storage | 2*40=80 | already included; no automatic saving from future SHA choice |

Three slots bound old/current/candidate/retained material capacity, not three
concurrent administrative transactions. Only one pending worker/cohort is admitted;
busy/unresolved/authority_lost guards stop replacement. Dormant UPDATE forbids
active listener/session coexistence, but fixed pools remain allocated. A prior
retained opaque runtime cannot be discounted solely because flags say dormant.

## Real missing terms and category boundary

1. Hash provider objects: four references in two retained helpers, plus issuer
   pair. They are opaque references, not proof of six kernel objects or bytes.
   The helper bounds repeated admissions, not internals of native allocation.
2. Certificate/DER/signature/status/UTC/operator sources: caller arenas have
   defined spans but complete nested callbacks and native internals must be
   charged. Root pins do not protect or price external sources automatically.
3. Native transaction/file/thread objects: at most the identified single-worker
   references, with UNKNOWN preserving them; commit and close failures remain
   native evidence obligations after mathematical hashes become deterministic.
4. Stack: actual caller/codec/hash/math/native/CRT nested frames, OS-rounded thread
   reservation/commit, guard pages and live owner stack versus existing reserves.
   Local array size is not the whole frame/call peak. Do not add a frame twice if
   it is proven covered by a reserve. The full coverage proof is absent.
5. Code/const data, runtime/compiler helper initialization/shared allocations,
   baseline/attributed incremental user/kernel charges, host physical/commit/
   address-space margin and fresh concurrent credits are not a managed-only sum.

No source-level API used here exposes a universal byte upper bound or safe
post-error disposition query for every BCrypt/TxF/kernel internal allocation.
This is a limit of available evidence, not proof of a leak or proof that native
resource growth is infinite. An arbitrary reserve would not enforce the bound;
more sampling would not turn an empirical maximum into a mathematical one.
Pinned target measurements can qualify a policy only through the explicit accepted
policy route. That is a normative/evidence decision, not changing3MiB to fit data.

## Coexistence, copies and safe optimization

The earlier167,328-byte caller scenario,52,904 Roughtime graph and353,396 observed
provider test peak have different scopes/lifetimes. They cannot be blindly added
or subtracted. Earlier3,813,017 conservative sums were scenarios, not demonstrated
actual simultaneous managed usage. They do not prove quota excess. Conversely,
the1,189,280 nominal difference is not spendable credit for those unknown terms.

Retaining committed/previous/G7 copies isolates the worker from root mutation and
supports full byte CAS. Removing a copy could save up to its capacity only if the
worker receives an equally protected immutable lifetime through UNKNOWN, with
actual refs/credits/Stop proven. It currently intentionally receives no root
pointer. CA dedup across slots similarly requires same-owner identity/refcounts
through old-reader retention. Neither optimization is necessary to explain the
current gate and neither has been credited.

Capture's CA-arena-to-BODY reuse is explicit after native borrows return; the
512-byte compare scratch is overwritten only after synchronous ReadFile releases
its buffer. These are examples of correct lifetime-based optimization, not a
license to release source evidence or transaction preimages after a timeout.

## Impact of primary solution

A pure deterministic storage math capsule can eliminate algorithm/hash provider
creation, cleanup uncertainty and provider stack from EXACTLY that call closure.
Its own state/stack/code/const/CRT costs must be measured. Existing issuer/native
operations and their retained resources remain charged; no global savings are
claimed. Callback split prevents reintroducing issuer-provider work via backing
or predecessor hashes. Version/candidate changes close independent integration
defects, not a byte-budget term. Phase/category accounting makes the remaining
native target gate visible rather than moving UNKNOWN to a hidden module.

Current/proposed totals are unchanged by this read-only audit. No complete maximum,
operational margin or full ED11 PASS can be published. V4 worker remains DENY.
The first correction slice produces actual component evidence; full admission
still needs native target proof and factual administrative sources. This is why
the conclusion is more specific than merely ED11 PARTIAL.

### Recorded implementation nonconformance

GOVERNANCE CONFLICT (implementation versus retained-ownership requirement,
not contradictory accepted ADRs): store02_hash in
server_configuration_store02_win32.inc:10-19 still keeps handles only locally
and returns on failed cleanup without transferring uncertain references. The
accepted D2.3 rule in DECISION-config-bootstrap-update-boundary-20261019.md
requires uncertain results to preserve data/history/resources; the accepted
package-delegation persistence transaction/lifecycle clauses preserve retained
owners/graphs through factual closure. No actual leak is alleged. This historical
path was not patched, imported as fallback or used to justify a global crypto
change. A later correction needs its own explicit scope/executor review.
