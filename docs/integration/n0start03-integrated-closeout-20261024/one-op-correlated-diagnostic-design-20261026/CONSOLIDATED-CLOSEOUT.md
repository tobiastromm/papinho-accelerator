# One-operation correlated diagnostic — consolidated design and qualification

TASK=PACC-PHASE6-ED11-ONE-OP-CORRELATED-DIAGNOSTIC-DESIGN
Date2026-10-26. Design/non-operational work qualified. **Execution readiness remains DENY**, not an operational approval or capacity proof.

## Result and concrete corrections

[Operation design](OPERATION-DESIGN.md) maps one real logical C2 operation through Browser NSS/x86 CONTROL/APP/RECORD, raw Site TCP, Accelerator OpenSSL/x64 and the existing C3 private socket pair. Setup can temporarily own listener+connector+accepted peer; attachment transfers connector ownership and the listener closes once. This is one logical operation, not one socket or a guaranteed two-thread topology. Existing parent Session, runtime/credentials, workers, modules and retained dependencies remain charged in their actual owners. Browser/Accelerator process clocks and budgets are distinct.

The fixed Browser reservation1,858,020 fits only the existing2,097,152-byte controlled expression; two reservations cannot fit. Outer response, child output+NUL, shared parent/root, allocator, thread/stack, active TLS and kernel costs still require overlap/attribution proof. No complete one-operation capacity claim is made from fixed arithmetic.

Implemented additive offline coverage validation in existing [mode_c_pair.py](../../../../tools/offline/mode_c_pair.py): existing receipt verifier runs first; immutable original prefix facts and identity/backend/root/local-clock/phase/sequence/retention are checked. No caller approval Boolean, live query, policy setter, alternate owner, IPC, wire or positive route is introduced. Output is ALWAYS admissionDENY, ED11_PASSfalse, native_qualifiedfalse, creditfalse and provider/kernelUNKNOWN, even when all phases are reported. Missing observations remain missing; zero values remain zero, not absent. No overlapping process sums.

**Additional demonstrated defect corrected:** old offline artifact aggregate1MiB and CLI per-file262144-byte bounds rejected the actual current Browser1,388,544 plus Accelerator342,528 bytes (total1,731,072). Finite artifact aggregate is now2MiB; receipt/fact262144bytes and16artifact count remain. CLI checks remaining allowance before reading. This is an offline input-byte bound, not a runtime quota increase or a2MiB Python-memory bound. Reproduced old INVALID/ARTIFACT_BYTES, then API and CLI correlation with actual static image bytes and SYNTHETIC protocol references. Native pair qualification remains false/DENY. [Exact regression](ARTIFACT-SIZE-REGRESSION.json).

Qualified an isolated public PST getter prototype: [scope and integration conditions](INSTRUMENTATION-AND-REMAINING-INTEGRATION.md). It queries initialized/available/provider ID and copied release disposition only under an existing serialized borrow guard, stops on first failure/borrow loss and never releases/acquires/logs/retries. Snapshot168bytes, not total stack or provider cost. Tests use simulated APIs/objects; no actual PST Connection, provider initialization or handshake was created.

## Tests and proof limits

- Four native fixture builds: MSVC/x64 and VC6/x86 normal SDK headers, Debug/Release, /MD /WX, zero warnings. Four fixture executions,17 checks each,68 total: initialized/uninitialized, guard/freeze at every query boundary, first API failure, retention, destroyed-state contradiction, malformed/mismatched provider, invalid arguments. Native executable with SIMULATED APIs is not native active-provider evidence.
-33 new offline tests PASS: provider absent/uninitialized, identity/image/SDK/backend/root mismatch, prefix mismatch, replay, wrap/regression, independent clocks, Stop/retention, missing metrics/phase, input bounds, no credits.
-37 affected regressions PASS (19existing receipt tests,18external-observer tests).
- Actual-image API/CLI artifact-size regression PASS, explicitly synthetic references/real static bytes only.
- Existing product builds/typed tests and runtime-core campaign reused without redundant rebuild. No runtime product C, Browser source, launcher, pins or protected inputs changed in this task. Only operational-repository code change is offline Python analysis plus its tests. C prototype is isolated/unintegrated.
- git diff --check in both repositories PASS. Historical authority/artifact tuples and HEAD/WIP preservation are checked in FINAL-VERIFICATION.json. No accepted ADR modified, no historical evidence overwritten.

Provider initialized1, actual connection/socket metrics, mapped-module provenance and kernel allocation are NOT native observations here. Tests qualify recognition/denial, not those facts. Existing native process producers remain partial. Browser first-reached slots do not sample the entire transfer. Accelerator raw refresh can observe ACTIVE outside dispatch, but typed export deliberately rejects network-active/dispatch. That guard was preserved; provider_runtimeUNKNOWN was not forged into evidence. Existing external observer phase/provider/kernel coverage remains incomplete.

## Normative boundary and candidate policy

**GOVERNANCE CONFLICT for executing the proposed diagnostic now:** accepted Mode C validity rules1/4/5 require an applicable evidence-backed quantitative target policy and administrative root/CURRENT/epoch/SiteTLS binding. Those are absent. Existing C8 CONTROL-only supervised diagnostic exceptions do not authorize this composition. The owner's ONE-operation direction authorizes this design, not a bypass or effects.

[Candidate D0–D9](../../../adr/ADR-CANDIDATE-mode-c-one-operation-diagnostic-authority-20261026.md) explicitly identifies the limited diagnostic-policy question and preserved invariants. StatusPROPOSED; SHA256 **84e2c9a7e3922247ee5ca8901594d29a2e1418eef68f91e58cd69ff53cc2a2bf**. No approval registered. Alternatives: complete hard coverage, expressly limited diagnostic policy with justified annex and separate manual approval, or continued DENY. No automatic selection of empirical residual risk.

[Hybrid annex candidate](HYBRID-POLICY-CANDIDATE.json) supplies the complete field structure, exact controlled ingredients and required unknown categories. SHA256 **943c07a6cc6aadc42687f287db2a354dd437440a50c122e4fbad4b78669f9e84**. Active empirical coverage, target max-age/collection validity, physical/commit requirements, exact host/root/module/destination binding and retained-charge recipe are null/PENDING, not guessed. Therefore it is NOT complete for normative acceptance. Actual sample maxima cannot fill those missing guarantees. Earlier quantitative/hybrid candidates and accepted decisions remain unchanged.

Read-only prototype/normal owner integration plan is concrete, but further integration of active snapshots must respect current owner/typed-export boundaries, not remove guard checks or introduce a second producer/logger. After a valid scope decision, planned source integration and affected rebuilds/tests can proceed autonomously under existing technical authorization; operational effects still require their exact separate approval. No executable command is fabricated while these prerequisites are missing.

## Artifacts and identities

[Static binding](STATIC-ARTIFACT-BINDINGS.json) records verified normal Release candidate bytes:

| Artifact | SHA256 |
|---|---|
| Accelerator Selected server |676d7c827ba9c993066d00b89100f9389e6dafccb530770a01f742459a309034|
| Browser candidate VC6/NSS |7d60405498bc55ce442529efd2d834ef6a714023ebfe45bb47983bff7d7215c4|
| Updated offline analyzer |5c9e268b604b85cd2af732b2d5a5e21635971596f2074ca84891f5011747ddfe|

These images are pre-integration static bindings, not mapped provider authentication or future final operational pins. [Full new source/probe hashes](FINAL-SOURCE-AND-PROBE-HASHES.json). Actual original PID/creation, boot/operator/token, operation identity, administrative root/CURRENT/epoch/SiteTLS/destination and mapped provider original objects remain unbound; none invented. No new operational identity, intent or protected object created.

## Q4, ED11 and minimum remaining path

Q4 readback is reported by the owner as independently authorized in a prior decision. This task did NOT perform it, apply disposition, register replacement authority or reuse Q4. Exact prior executable approval binding was not reconciled here; only the earlier inert candidate was found during unprotected documentary search. See Q4-INDEPENDENT-STATUS.json. This does not revoke the reported decision. Original effects/consumed intent/reservations/UNKNOWN and global quiescenceUNPROVEN remain intact.

ED11 current22gates retain4demonstrated/12limited/6pending; all14historical matrix rows preserved. E19/E20 gained actual offline implementation, source topology, simulated collector qualification and a real artifact-size compatibility fix; no active composition proof. E17/E18 independent service readiness/Stop remain pending. E21 needs separately conducted authorized readback and later own documentary disposition. E22 remains derived/pending. Phase6PARTIAL, independent of other capability gates.

Minimum path: resolve the explicit limited diagnostic policy versus hard-envelope/STOP decision; supply justified quantitative/administrative bindings; complete bounded passive snapshot integration through existing owners with affected qualification; issue one exact operation request and obtain separate manual approval; execute once manually; reconcile initialized provider, resource overlap, prefixes, lifecycle, Stop and retained UNKNOWN; only then complete/reject the quantitative empirical annex and E19/E20. No repeated runtime-core/service-only observation is proposed.

[Prepared next owner prompt and deliberately blocked operational-request template](NEXT-OWNER-PROMPT.md). It explicitly marks missing values and does not invite an approval of effects with placeholders. No micro-approval per technical edit is requested; the remaining decision changes normative authority and is reserved to the owner.

CURRENT_TASK_COMPLETED=YES_FOR_DESIGN_AND_NONOP_QUALIFICATION
ONE_OPERATION_DESIGN=CONDITIONAL_DOCUMENTED
OFFLINE_CORRELATION=IMPLEMENTED_QUALIFIED_DENY
PROVIDER_PROBE=QUALIFIED_SIMULATED_UNINTEGRATED
ACTIVE_PROVIDER_NATIVE=NOT_OBSERVED
KERNEL_SOCKET_CAPACITY=UNKNOWN
QUANTITATIVE_POLICY=PROPOSED_INCOMPLETE
EXECUTION_READINESS=DENY_POLICY_AND_NATIVE_BINDINGS_MISSING
Q4_READBACK=OWNER_REPORTED_PRIOR_AUTHORIZATION_NOT_EXECUTED
Q4_DISPOSITION=NOT_APPLIED
NEW_OPERATIONAL_IDENTITY=NOT_CREATED
INTENT_CONSUMED=NO_NEW_CONSUMPTION
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
QUOTAS_CHANGED=NO_RUNTIME_QUOTAS_CHANGED
HOST_CREDITS=NONE_GRANTED
HISTORICAL_UNKNOWN=RETAINED
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=EXPLICIT_LIMITED_DIAGNOSTIC_POLICY_DECISION_WITH_JUSTIFIED_ANNEX_OR_HARD_ENVELOPE

No Prepare, network execution, LAB operation, new service/owner, protected creation/readback, cleanup/refund, clone/worktree, commit/push/tag/release.
