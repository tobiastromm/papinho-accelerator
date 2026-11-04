# R0–R9 accepted, prospective integration and remaining ED11 target closure

2026-10-25. TASK PACC-PHASE6-ED11-R0R9-ACCEPTANCE-AND-INTEGRATION, including the parallel R/Q4/target review. Active main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469. All technical work authorized here is concluded at its actual normative/operational boundaries. ED11 and Phase6 remain PARTIAL. No protected Q4 Verify, LAB/Prepare/preflight/Start/Stop, new intent/identity, SCM/ACL mutation, cleanup/refund, CURRENT/signature/recovery, commit/push/tag/worktree/clone was performed.

## Normative decision and source integration

Approved complete revision1 candidate SHA256 d19ad5922874569af416baab7e86b196b916eaead057418a97dd4226bfc8d9ef matches the unchanged proposal and approved copy byte-for-byte. New accepted record docs/adr/DECISION-service-virtual-user-identity-and-q4-documentary-disposition-20261025.md SHA256 **5bd7e99d48c46490e6c7a3d95765c01093e51cb85b277d7cb83eeac40603e331** incorporates the complete original bytes. OWNER-NORMATIVE-DECISION.json and ACCEPTANCE-VERIFICATION.json retain the explicit owner decision and bounds. Existing107 authority files revalidated unchanged. R3 is the expressly approved prospective exception to Q6/A3; historical denial is not reinterpreted. R6–R8 permits evaluation of a documentary Q4 disposition, not reading or applying it.

The existing service owner now has an opt-in PAPACC_ENABLE_R0R9_VIRTUAL_USER_LAB branch, OFF by default and restricted by the existing canonical/private positive LAB build composition. Production/default, legacy predicate, qualifier owner/mutex, wire and PowerShell remain unchanged. The new branch requires the fixed exact virtual service SID and independent existing SCM readback; original primary TokenUser, full bounded groups, enabled generic SERVICE, no enabled Administrators, absent specific group OR exactly one ENABLED/non-DENY matching group. Disabled/default-only/deny-only/contradictory/duplicate entries deny. Restrictions and restricting-SID queries must be complete/unfiltered/empty. No TokenUser attributes are converted to group ENABLED.

The original owned impersonation duplicate is retained and checked through native CheckTokenMembership and the original per-resource MAXIMUM_ALLOWED AccessCheck/no-write/no-delete/no-WRITE_DAC/no-WRITE_OWNER guards. Duplicate user/AuthId/generation and original/current primary TokenId/AuthId/ModifiedId are checked around observations and at subsequent use. Original identity/provenance/RAW1/file guards, privilege/elevation snapshots and downstream runtime gates remain. No group/privilege adjustment, thread impersonation, SID/account mutation or new token owner exists. Actual process/census PID/creation guards remain in the existing qualifier.

New r4_busy protects the synchronous owner borrow against reentrant runtime entry; it is not a new general multithread token API. Reentry freezes/retains before another acquisition. Stop/retention is checked before new runtime observations and token acquisitions. A failed close preserves the original check_token and error, freezes further acquisitions and prevents retry/double-close. Existing owner-thread/lifecycle and Stop/UNKNOWN contracts remain required. No claim that arbitrary simultaneous unsynchronized callers are now permitted.

## Qualification and corrections

Four Selected/Legacy Debug/Release builds PASS, zero warnings; same MSVC/Ninja/SDK/backend/profile flags retained. BUILD-RESULTS.json and four logs are authoritative. No operational executable entry was invoked.

- Actual source-linked R caller:35 scenarios/144 assertions per configuration,140 scenarios/576 assertions total. Covers no-group and enabled group; wrong identity; invalid/truncated/duplicate/disabled/default/deny-only groups; duplicate identity/AuthId/ModifiedId; incomplete/restricted/filtered queries; failed membership; Stop; actual caller reentry; close uncertainty/no retry; current-token changes and mandatory effective-access rejection.
- Actual owner plus existing recorder with all file APIs modeled:8 cases per configuration,32 total; successful no-group/valid-group prepared frame and specific first-error frames verified. No protected files created or read. Existing2048-byte protocol and historical frame checks preserved.
- Six affected regression executables per configuration:24 runs PASS (default token/config/access, canonical file recorder, SCM contract/readback mismatches, lab/product host lifecycle, census/freshness/identity/Stop). Census runner441 assertions each. Total new R plus affected runs:32 executable runs PASS. Existing76-regression campaign and unchanged native own-token access evidence are reused, not re-run or relabeled new service proof.
- E19/E20 actual extracted consumer boundaries:40 checks Debug and40 Release PASS/zero warnings. This is source-linked non-operational evidence, not Browser VC6 native runtime or full pair admission.
- Prior four native own-token/in-memory SD fixtures remain applicable:32 checks each, confirming kernel membership/read-execute/write/owner-right semantics. They are NOT the virtual service token and do not supply missing future effective-access/service readiness proof.

Two fixture defects were corrected without changing authority: the access fixture had hashed an absolute descriptor as though contiguous serialized bytes; the R fixture now serializes a self-relative descriptor before immutable hashing. The file mock still hardcoded historical03 diagnostic leaf while global Q4 binding selected10584; it now derives the exact modeled leaf from the same binding header, preserving the historical-name rejection case. Initial local test failures were not operational effects. New recorder fixture initially lacked modeled readback and its initial wrapper macro was corrected before final qualification. FIRST-ACTUAL-R4-TEST.log retains the initial source-linked failure; final logs are all PASS.

## Ownership and storage ledger

The seven existing runtime HANDLE slots and canonical owner are unchanged. R uses the existing original token, owned duplicate and existing transient check_token slot. Borrow is synchronous; no additional persistent handle, observer, owner, service or authority. More queries/revalidations are a real cost and may still cause freshness DENY. No handle or host credit is claimed.

Compiled full-file-runtime layout in Selected Debug AND Release: PS_RUNTIME12080→12152, PS_NATIVE24992→25064, existing service-storage reporter25208→25280 bytes: **+72 bytes**. This is the added duplicate TOKEN_STATISTICS and bounded scalar state/alignment, not total memory cost. The smaller no-file caller test reports7752/20664 and is explicitly not substituted for full product layout. A fixed32-byte expected SID constant, predicate/record strings, code, stack and native/provider costs remain separate. No ledger adds storage to overlapping process private/commit/WS or counts a closed file as a live handle.

Selected Release emitted own-frame sub-rsp values: r4_current328, psr_token344, psr_current304 bytes; existing pt_token_body704, psrt_return1192, psrt_token_emit680. These are compiler frame allocations, excluding register pushes, caller chains, Windows calls, reserve/commit and PSS/kernel. They are NOT a total stack ceiling or guaranteed headroom. See COMPILED-STORAGE.json, COMPILED-FRAMES.json and emitted assembly. Kernel/cache/provider and unsampled peaks remain UNKNOWN.

## Artifacts and compiled pins

Selected Release prospective service candidate SHA256 **01ea58a57d2513ac682a5a1edbae0a7b8463335bf59ac4c14e970aeab0388287**.
Selected Release newly compiled qualifier SHA256 **05e8d00722b4bdce54a2dc4d1909e62bf7f4764a6645ab04094d7be93d0e62f7**.
All four services contain the exact accepted decision hash tag. FINAL-SOURCE-HASHES.json and DERIVED-CANDIDATE-PINS.NOT-ACTIVATED.json record every current artifact and source hash.

The operational launcher remains SHA531a749789892e8356de00870085c2d110b2f80a5caa4a4c9918e2424e7a6bb1. The historical qualifier remains76a3d760f3ee468be41e7e98064edbdcaa3209fac108a76810635a22dafd2e04 and service128834681905d861eb3252c1e0079e1d06a02b46c2f6d34ddb3612bcc85b3dc8. Nineteen historical artifact/policy tuples and107 authority files match unchanged hashes. Existing header/launcher compiled image pin still binds historical128834...; every new qualifier was verified to retain it. It intentionally DOES NOT admit the new R candidate under a consumed Q4 identity/approval. Prospective image hashes are prepared separately, not activated into an unauthorized new scope. No new operational bundle or executable positive command exists. Replacing that header with a new service under old Q4 would violate R3/R8; a future independent owner-approved identity/binding must establish its own pins.

## Q4 state and documentary gate

Consumed N0TOKEN-20261025-01 /10584f02e11f92c92b43cddf0d9d316f remains Start1/Stop0, intent consumed, image/sink/rebind preserved, historical first13/service5. Original process terminated/local references checked closed in their historical scope; global quiescence UNPROVEN. No current protected readback occurred here. Historical unprinted intent/image native IDs remain UNKNOWN. Real reservations stay unavailable; H0–H9 is not refund. Q2 of historical03 does not dispose Q4.

Existing separate protected readback request SHA256 **2aa9e0b81352b0efe28e23135fa4644b70d17241565e1fb6e6a6d191312c663b** is unchanged and **NOT_AUTHORIZED**. A single manual readback could provide current original-object/native-ID/RAW1/hash/lifetime evidence, not invent historical creation IDs. Only after its result may an exact documentary disposition be prepared for separate approval. No application, release, cleanup, Stop, retry or successor follows acceptance automatically.

## E19/E20: concrete new evidence and technical boundary

Read e19-e20/TARGET-BOUNDARIES-AND-MINIMUM-CLOSURE.md, topology CSV, extracted unchanged C bodies, source/SDK inventory and logs. The real Accelerator papacc_c8_target_admission returns RESOURCE_UNKNOWN even with valid raw and CURRENT/root binding: no provider/kernel recipe or approved empirical target policy is installed. Browser papinho_c2_budget.inc has staticzero c2_target and no writer in current src. Its required provider_known/kernel_known/host_approved/metric lifetime/credit cannot become true from valid source today. Forcing flags or copying LAB limits would be a normative/security change, not a defect correction.

Real topology is mapped: Accelerator C2 two operation slots/four Session entries, four64KiB flows/operation; C3 two additional64KiB queues plus setup listener/connector/peer/PST-owned references. Shared runtime/trust/scheduler/managers are borrowed and counted once per owner; retained references remain charged. Browser is a separate process with CONTROL bridge thread and serialized DATA; UI/render/DNS and native multiplicities remain unobserved. No process-per-object, socket option ceiling or universal kernel cost is invented. Browser2MiB and Accelerator selected3MiB managed profiles are not the LAB128/1024 handle budget.

Browser normal SDK91 contained entries match; three changed consumer doc/build files in the historical95-entry manifest are not SDK corruption. Accelerator OpenSSLx64 and Browser VC6/x86 NSS are distinct artifact/backend/allocator envelopes. Current Browser HEAD96f6cd2f9ec4f834999828fa13bb7d170db66a98 is separately recorded; no Browser files changed here. A matched current pair/module provenance and active lifecycle still require real qualification.

Accepted DECISION-mode-c-evidence-validity-semantics-20261018 explicitly leaves product target metric/max-age/headroom PENDING_EVIDENCE_AND_SEPARATE_NORMATIVE_DECISION and forbids silently inheriting C8 freshness. LAB60s/1s and5/9GiB do not fill that gap. The minimum implementation is one exact evidence-backed target profile/recipe per supported pair/backend and a real owner-bound Browser producer/import, then freshness/replay/lifetime/UNKNOWN validation in both actual consumers. No additional product owner/daemon/registry is needed. Until guarantee choice and quantitative policy have evidence/approval, implementing force-pass producers or inventing ceilings is outside authority. The report provides the concrete technical design and alternatives without altering accepted policy.

## Full gate result and minimum critical path

CURRENT-ED11-22-GATE-MATRIX.csv and live ED11_GATE_MATRIX.csv: **4 demonstrated,12 limited,6 pending**. Mechanisms already proved are retained. E17's normative blocker is resolved and actual source integration qualified offline; actual service token/access/readiness/resources still needs a future separately authorized independent scope. E21's evaluation authority is accepted, while protected readback and subsequent documentary application remain independently gated. E19/E20 now identify policy/producer/recipe/pair-composition gaps instead of recommending a redundant service observation. E18 safe Stop/lifecycle remains operationally unproved. E22 is dependent and cannot pass first.

Minimum parallel path, with no blanket operational approval:

1. Owner may authorize only the existing exact Q4 readback request; after factual result, review exact documentary application separately. Keep all physical effects/reserves/UNKNOWN and consumed identity throughout.
2. Decide product ModeC target guarantee and evidence policy (hard provider/kernel envelope OR explicitly delimited empirical residual coverage), including metric/freshness/headroom per actual backend/pair. Then implement the already identified producer/recipe and source-bound negatives in actual consumers, and qualify the real composed pair under its own approved run. No invented quantitative candidate is asserted approved.
3. If still needed for E17/E18 after the Q4 obligations are compatible, prepare ONE new owner-selected independent service identity/scope, specific N7, fresh protected inputs and exact candidate hashes/binding/admission approval. Only a real eligible cycle can demonstrate service resources/readiness/Stop; current R builds alone do not. Do not reuse Q4, old approvals or diagnostic intent. No new identity is generated here.
4. Reconcile all mandatory actual target/lifecycle gates and controlled closures, then evaluate E22. No universal proof that opaque historical Windows residue vanished is required by fiat, and no UNKNOWN is converted to zero.

Phase6 separation remains independent: ED08/ModelE actual composition, product C2/C3/Browser, personal installation/security/CA/time/status/CURRENT and revocation/lifecycle have their own evidence gates. Deferred personal-MVP recovery availability is not invented as a new mandatory blocker; authoritative recovery remains DENY. No ED11-only claim closes Phase6.

CURRENT_TASK_COMPLETED=YES
R0_R9=ACCEPTED
R0_R9_FUNCTIONAL_INTEGRATION=QUALIFIED_OFFLINE
E17=NORMATIVE_RESOLVED_OFFLINE_READY_OPERATIONAL_PENDING
E21=DOCUMENTARY_EVALUATION_ALLOWED_READBACK_AND_APPLICATION_NOT_AUTHORIZED
Q4_READBACK=NOT_EXECUTED
Q4_DISPOSITION=NOT_APPLIED
E19_E20=ACTUAL_BOUNDARIES_QUALIFIED_POLICY_PRODUCER_RECIPE_AND_COMPOSITION_PENDING
OPERATIONAL_PINS=HISTORICAL_PRESERVED_CANDIDATES_NOT_ACTIVATED
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
HOST_CREDITS=NONE_GRANTED
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_WITH_AUTHORIZED_FOCAL_CHANGES
FIRST_UNRESOLVED_GATE=Q4_PROTECTED_READBACK_AUTHORITY_AND_TARGET_POLICY_SOURCE_COVERAGE
NEXT=EXACT_Q4_READBACK_DECISION_IN_PARALLEL_WITH_TARGET_POLICY_AND_RECIPE_DECISION
