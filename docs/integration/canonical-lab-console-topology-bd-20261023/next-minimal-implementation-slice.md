# Next minimal B+D implementation slice — proposal only, NOT authorized or executed

NEXT=CANONICAL_LAB_BD_TOPOLOGY_IMPLEMENTATION. The [accepted decision](../../adr/DECISION-canonical-lab-console-topology-bd-20261023.md) separates normative approval from functional work. IMPLEMENTATION_AUTHORIZED=NO remains. This file prepares a concrete future scope; it does not authorize any item below.

## First vertical slice: canonical single-actor binding, four-role inventory and fail-closed console-proof gate

Problem: the current qualifier requires two distinct PowerShell identities and its census rejects any extra child. Its current observed console evidence cannot prove R2. Changing process flags, passing one PID twice, marking console as PowerShell or removing child rejection would violate the accepted amendment.

1. Reuse the SAME canonical admission owner. Define a bounded private operation descriptor binding one elevated administrative process HANDLE+creation+approved SID/token/no impersonation to exact host/boot/attempt/policy/plan/configuration/candidate identities, console/confirmation context and one-shot owner state. Keep existing mutex acquisition/consumption/Stop/retirement. No second owner, registry, service or unverified boolean confirmation. Old two-actor entry must not silently accept a duplicated identity or become an authority bypass.
2. In the existing census/observer, enumerate four distinct roles: one elevated actor, qualifier, future service when it actually exists, and zero/one infrastructure instance only with complete R2 evidence. Count actual overlapping process identities exactly once; retain separate original handles/lifetimes. A console role value without independently qualified factual relationship/provenance/session/lifetime/metrics remains UNKNOWN => DENY. Existing arbitrary-child/project-concurrency checks remain for every unproved process. No global name exception.
3. Represent required console evidence in the existing bounded owner state, with explicit query result/source/identity/freshness rather than path/name/parent trust. Before adding any new collection code, assess already available Windows/WPT evidence/tools and supported native APIs for the specific required relationship and mapped-image/provenance facts. Existing GetConsoleProcessList/window/current-file observations are NOT a sufficient implementation. If no sufficient source is qualified, implement/test only the fail-closed gate and report the exact missing fact; do not expose the positive launcher or expand instrumentation merely to label UNKNOWN as proven. No new recorder or ETW capture is part of this proposed slice.
4. Reuse policy/storage: separately observe the elevated PowerShell and proved console and check their combined administrative budget256MiB/1024 handles/64 threads/32MiB observable stack. Independently check qualifier and future service64MiB/128 handles/16 threads/8MiB each. Keep overall caps, freshness/headroom, mandatory metrics and NONE_GRANTED. Do not treat console as an uncounted baseline process, alias an IG_PROCESS to PowerShell to bypass128, duplicate memory/stack accounting, invent a service PID or grant credits from an unused role. Reconcile every added state byte, buffer, handle and lifetime before qualification.
5. Preserve native confirmation and Stop bridge, token continuity, immutable first error and UNKNOWN retention. Successful contract/model checks are not authority to run Start or admit an unproved console.

## Expected source boundary, for later exact patch review

- tools/qualification/scm-instrumented-cycle-lab.inc: private argument/actor binding, existing census role matching and operation context.
- tools/qualification/scm-instrumented-policy.h and scm-instrumented-observer.inc: explicit role/accounting and required-proof validation, within bounded existing owner.
- apps/server/server_service_admission_win32.h/.inc: only a demonstrably necessary minimal extension of existing owner/bind state; avoid duplicating checks/state when the qualifier already owns them.
- Existing busy/policy/cross-admission model suites and focused four-role/binding tests.

Audit before selecting exact files/bytes; this scope is not a preapproved patch. Source is unchanged in the current decision. Launcher remains pinned to its old qualified binary until the native core and factual proof/accounting qualify. Do not modify or execute it now. A later direct-in-session already-elevated launcher integration must use the accepted single actor, explicit confirmation and separate operation approval, never spawn another PowerShell to fill an old role.

## Qualification and stop criteria for that future authorization

Build Selected/Legacy Debug/Release with qualified toolchain, /W4 /WX, external scratch, no quota/backend change. Run isolated model/process fixtures WITHOUT preflight/SCM/network. Required cases: exact single actor versus duplicate/changed token/creation/confirmation/attempt; four actual roles versus fifth; missing/contradictory console relationship/provenance/session/lifetime/metrics; same-named/unrelated/reused/arbitrary child; real concurrent and duplicate canonical admissions; combined administrative budget crossing; native129 handles independently denied; Stop before effect, failed close, retained UNKNOWN, replay, quiescence and factual release. All R2 facts must be independently testable as UNKNOWN/DENY. Synthetic positive evidence tests only contract logic, never real Windows console proof.

Exit for this slice: core binding/role/accounting/negative semantics qualified offline, costs reconciled, factual evidence source explicitly qualified OR named as blocked. If R2 proof remains unavailable, mark POSITIVE_ADMISSION=DENY and retain the old launcher; a passing injected model must not arm native admission.

Only after that slice and a separately authorized launcher integration can a new read-only operation dossier/pins/manual command be prepared for separate approval. Historical ED11H claim/receipts and orphan UNKNOWN must not be reused, rearmed, refunded or silently replaced. No native handle remeasurement or preflight is authorized here.

## Current state

IMPLEMENTATION_AUTHORIZED=NO
IMPLEMENTATION_EXECUTED=NO
LAUNCHER_CHANGED=NO
PREFLIGHT_EXECUTED=NO
START_STOP_EXECUTED=NO
SCM_ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
