# Factual sources: STOP and initial-bootstrap readiness

Task PACC-PHASE6-V4-FACTUAL-SOURCE-ED11-1;2026-10-22.
Active main/HEAD913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
Owner explicitly confirms that no qualified canonical installation/CURRENT,
operational UPDATE approval or qualified UTC can be supplied. No nonexistent
path/installation was inferred, created or searched for as a replacement.

## Exact new bootstrap defect (independent of missing authority)

server_config_source_binding_win32.c, initial_bootstrap_commit, passes
pending_commit.observed to body05_encode with claimed capacity1024, then compares
1024 bytes. server_configuration_pending_win32.h declares observed[512], followed
by path and native transaction metadata. body05_encode accepts n==1024 and calls
BODY04 which writes that span, then writes at offsets512..1023. When its preceding
valid-input gates succeed, the caller can write512 bytes beyond observed and
read512 bytes beyond it. This is a source-level extent mismatch, not a claimed
observed exploit/crash. The path was NOT executed to demonstrate the problem.

The earlier compare-scratch reduction is safe for chunked full-file CAS but the
bootstrap BODY05 reuse was not adjusted. GOVERNANCE CONFLICT (implementation versus accepted safety requirements): this
is a concrete implementation nonconformance to D2.3/Model E/lifetime and ED11 span/capacity obligations; changing
an accepted contract would not repair it. Bootstrap positive qualification must
STOP before this path. Packet11 LAB success never exercised this initial resolver.

The adjacent exact proposal uses a1024-byte local expected_body, sizeof-based
calls and an explicit additional1024 in the minimum reservation. No owner/packet
layout growth, pin, worker, fallback, grant or authority is proposed. The local
borrow ends before worker handoff. The patch remains PROPOSED/UNAPPLIED: current
owner scope requests readiness and forbids bootstrap execution, not automatic
acceptance of a bootstrap functional correction. It needs exact official source
review, Debug/Release span/canary/Stop regressions and compiled nested-stack/caller
budget verification before any ceremony. The extra source array is not itself a
full stack or capacity proof. Do not enlarge observed or erase retained state.

## Bootstrap initial is distinct from UPDATE

Already implemented: separate initial resolver that rejects prior published/
committed/recovery/retained state; native admin/token/candidate/container binding;
exclusive new-container creation via retained native parent/container identities;
nonreparse checks; same worker packet9 custody and packet10 commit; v1 no-issuer
R1/predecessor0 and absent checkpoint; participant/source/policy checks and Stop/
retention. Initial resolver deliberately leaves CURRENT-oriented resolved/epoch
fields unset. It does NOT require a fabricated predecessor/current pin.

Still required BEFORE executing initial bootstrap:

1. Fix/qualify the512-vs1024 scratch defect above.
2. Review a real qualification entry through the existing native binding without
   PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST/checkpoint91 bypass. Existing diagnostic
   bootstrap successes remain LAB. server_config_cli.c apply remains a deny gate.
3. Explicitly designate operator, authorized NEW installation operation and
   parent/container target; prove its own effective token, no impersonation and
   same invocation/candidate/custody. Role/authentication alone is not approval.
   No such operation or target is authorized by the current task.
4. Provide existing canonical Secure material-loader references, policy and
   participant evidence with their own origin/validity/borrows; the owner still
   supports one logical listener and ALL, not inferred SELECTED topology. A
   participant's published Secure source is not a previous root CURRENT.
5. Prove reservations/lifetimes/native closure for that actual ceremony. Its
   execution, commit and any later publication need their separate authorizations.

Bootstrap creates no v4 root directly and is not UPDATE. Issuer initialization
and later version transitions remain their own accepted implementation/authority
gates; no new CA/certificate or shortcut is authorized. Restart/historical bytes
cannot restore CURRENT or UPDATE authorization. Existing historical campaign
numbers/version limitations are not reused as current implementation evidence.

## V4 source inventory: where execution stops

CURRENT/predecessor: canonical current_pin/current functions enforce same owner,
normative_committed/administrative_published, epochs/revision and real canonical
tickets. Protected bytes/G7 and packet11 CAS exist, but no factual installation
is available. Candidate/model/historical snapshot is never substituted.

UPDATE/operator: U0-U7 act code captures/revalidates native identity and correlates
candidate/predecessor. It sets PAPACC_V3_STATUS_SOURCE_ABSENT. Native token success
alone does not authorize an UPDATE, TIME_SOURCE or source status.

Issuer/policy/SNT/PDG: codecs, protected DATA and current-source guard exist.
The same source transaction must supply actual validated published references;
DATA rows/certificate validity/digests cannot supply their administrative status,
revocation, authority, validity or source lifetime. Existing UPDATE hold returns
NOT_SUPPORTED, approval/validate remains NOT_SUPPORTED without sources.

TIME_SOURCE/UTC: TS0-TS8 persistence/strict point validation and offline Roughtime
verification exist. No authorized factual TIME_SOURCE endpoint/key/status/pin
was supplied. papacc_issuer_qualified_utc_read still returns NOT_SUPPORTED; Q0-Q7
math is not an implemented factual source. No rho/E/freshness/continuity defaults,
local-clock shortcut, LAB key or source authority is introduced. E0-E8 remains
source-absent; no lease or positive trust is fabricated.

Thus operational source binding cannot be completed by wiring DATA callbacks.
No functional source changed in this task. No generic registry/resolver proposal
is needed or created. Missing factual inputs, missing producer integration and
native capacity evidence are separate limitations, not all environment failures.

## Independent ED11 evidence, without installing anything

Inspected the existing exact Selected MSVC Debug/Release canonical commit COFF
objects with dumpbin; hashes and full disassembly are adjacent. Observed explicit
prologue RSP deltas (subtraction plus pushes) in bytes:

| Function | Debug | Release |
|---|---:|---:|
| pending_worker |344|216|
| pending_packet11_valid |312|152|
| pending_packet11_image_guard |184|136|
| pending_compare_file |104|120|

These observations narrow own-frame evidence ONLY. They exclude entry return,
caller-owned inbound home/spills, nested hash/CRT/WinAPI calls, native startup,
OS pages/guard/reservation/commit. They are NOT a nested stack bound; do not sum
all rows as simultaneous use or claim a64KiB operational proof. No code was
instrumented or rebuilt and no worker/bootstrap/fixture was executed here.

Existing known fixed Selected1956448 and Legacy1790952 remain previous qualified
layout values, not a new total. Full source graph, nested native/provider/CRT/
stack/user/kernel/host attribution and fresh concurrency/retention credits remain
unqualified. No observation of the agent process would price the real operation.
ED11 operational STOP/PARTIAL; quota and nominal headroom grant nothing.

## Minimum continuation

Review/qualify the proposed scratch correction and real initial preflight entry
before requesting a specifically targeted bootstrap ceremony. Preserve initial
mode with no prior CURRENT; keep UPDATE and all publication/signature effects
DENY. Qualify the actual source graph and operational ED11 only when independently
sourced administrative evidence exists. No further checksum slice replaces this.
