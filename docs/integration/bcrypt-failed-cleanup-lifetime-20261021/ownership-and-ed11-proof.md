# BCrypt explicit ownership and ED11 proof

Task PACC-PHASE6-BCrypt-FAILED-CLEANUP-LIFETIME-1, 2026-10-21.
Baseline HEAD 913c90cf2328f7939d8f5b927640601903ac6e91.

The owner selected preservation of previous BCrypt APIs with explicit ownership.
No BCryptHash/pseudo-handle or Windows 10 minimum was introduced. Modern Windows
qualification does not qualify NT4, box or ISA; platform backend boundaries remain.

## Implementation

server_store_hash_win32.h/.inc carries algorithm/hash references, operation and
cleanup NTSTATUS, active/retained/stopped state. One context is embedded in the
existing root configuration owner, one in its pending worker: no global orphan
manager, worker, root or pin table. Each measured x64 context is 40 bytes.
Contexts must not be copied, zeroed or freed while active/retained.

store03_hash and nested legacy digest use this same implementation. Private
BODY04/BODY05 codecs and issuer/backing/root callers pass explicit durable
contexts. SHA-256 and codec bytes are preserved. Input/output are synchronous
borrows; BCryptCreateHash receives NULL object buffer, so provider owns its
object allocation.

Exact zero status and nonnull handles are required. Open failure with null
output acquires nothing; nonnull output on failure is retained without further
native calls. Create failure with null hash attempts one algorithm close;
nonnull hash on failure retains both references. HashData/Finish failure records
operation status and attempts cleanup once. Successful Destroy clears hash;
failed Destroy retains both references and skips algorithm close. Successful
Close clears algorithm; failed Close retains algorithm. Unexpected nonzero status
fails closed. The digest is staged in 32 stack bytes and published only after
successful cleanup and absent Stop. Failed cleanup never returns success.

A retained context rejects further hashing without native calls. No retry,
timeout disposition or fallback exists. Retention preserves an uncertain
reference; it does not assert whether the native object remains live or was
released. The adapter has no qualified query for that state after failed
DestroyHash/CloseAlgorithmProvider. Consequently dependent work remains DENY.

Stop sets flags without waiting or releasing native resources. Completion clears
active, not retained. Finish and pending resets reject blocked contexts. The
transaction owner memset requires successful finish; launcher reset requires
FINISHED, and an initialized retained configuration prevents quiescence.
Pending completion preserves retained state and marks uncertainty without
rewriting CAS UNKNOWN as rollback. Existing issuer hash entry is guarded against
blocked hash contexts, while bootstrap leases remain valid. CURRENT unchanged.

## ED11

Selected measured fixed graph 1,529,824; root 328,216; commit 251,160; pending
62,144 bytes. Child structures are nested, not additive. Known managed total
1,952,704 = previous 1,952,624 + 80 bytes. Cap stays 3,145,728; fixed-only
difference 1,193,024. Legacy known 1,790,952; cap 2,097,152; difference 306,200.
Neither difference is an operational margin.

Prior unimplemented v4 three-buffer projection adds 3,744: projected Selected
known 1,956,448; fixed-only difference 1,189,280. No worker v4 authorization.
Worker stack 65,536 and checkpoint scratch 8,192 are already charged, not added
again. Full transitive codec/native hash stack remains unproved.

At most two native references/context, two canonical contexts, four references
in active/cleanup/retained phases; phases do not represent additional objects.
Existing issuer pair remains separate: conservative six named opaque references
for these hash paths together, not six known byte allocations. Retention blocks
new attempts. Test-only contexts are fixture storage, not a product owner.

Provider hash/algorithm allocation bytes, kernel/provider costs, runtime/init,
full nested stack and full allowed retained coexistence remain UNKNOWN. Counts
do not bound their bytes. ED11_TOTAL_STATUS=PARTIAL; V4_WORKER_ED11_GATE=STOP.
No quota change or unproved reuse credit.

## Approval and tests

Exact staged source operations passed official executor review before applying.
Preserved manifests and SHA-256:

- operations.json: 86e9007b1d131f259e175be1b3e0fb3cf73199f1bd8cf752d35ab5d5b98ad0fa
- test-operations.json: e9ad3a3ee7e1cb720c73301a50b997fe1c9ad6539ae480dbe6218fdce433d6a7
- owner-test-operations.json: 60ee0879a4cce162069a14b8f16962c3943db431b2bd946a0e811b92bd9c5975

Initial Debug compilation exposed one missed private call argument, corrected
before final clean builds. Final Selected MSVC Debug/Release have zero warnings,
eight passing CTests each: root/v2/v3/v4, census, v4 backings, hash lifetime,
owner retention. Lifetime: 93 checks; owner: 17 checks. Extra catalog: 31 checks
per configuration. Native v3 success/conflict/UNKNOWN/Stop: four modes each pass.
Legacy Debug configuration library and census build without warnings.

Real CNG success is tested. Controlled shims withhold cleanup to exercise failure
branches; fixture teardown knows cleanup was withheld. This is not a product
retry rule or proof about real provider failed cleanup. Owner test holds four
real fixture references, verifies Stop/finish retain them, then performs known
fixture teardown. No native failed cleanup was forced. No factual CURRENT,
trust, certificate, key or network was produced. VC6/NT4/box/ISA native BCrypt
paths were not newly qualified.

Logs and wip-verification.json are adjacent. Baseline 6,436 files: 28 changed
preexisting files, all authorized; zero unexpected changes before documentation.
PST, Browser, wire, production 0.6.4, accepted ADRs and LAB remain untouched.

Next: establish opaque native byte bounds and full nested stack/retained cohort
bounds without treating uncertain cleanup as success or reclaiming by timeout.
