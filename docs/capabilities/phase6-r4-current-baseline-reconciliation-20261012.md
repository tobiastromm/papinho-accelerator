# Phase 6: current PST baseline, BIO and R4 reconciliation

2026-10-12. Evidence reconciliation only. No integration execution or code change.

## Artifact identities and limitations

PST homologacao/pst-0.6.5 HEAD 4dbd2783ebaf135114da2b7247e36c0ad6bc550a; Accelerator main c50b8fb9a7e3293b5e6355fc08459bf85b56857a, existing dirty WIP preserved. Both requested ZIP hashes match. All 18 BIO and four R4 checksum entries verify; full hashes and inventories are in the adjacent JSON.

BIO manifest identifies API 3.0.0/SPI 4.0, current commit/tree, persistent BIO gate and OFF/STOP/RESUME PASS. Packaged traces support genuine blocked shutdown writes with WRITE present, including READ|WRITE. Qualification is upstream test-only precondition evidence, not new Accelerator D/E/F execution. Its README still claims old commit da719093; request an upstream documentation correction without replacing the frozen ZIP or silently resolving that discrepancy. The archive supplies no public SDK headers or OpenSSL import libraries; standalone SDK completeness is not established. No staging or library mixing occurred.

R4 decision bundle contains only assessment, two handoffs, manifest and checksums. It contains no library, patch, harness, trace or Connection-scoped release result. CONTROLLED_PROVIDER_BOUNDARY_FAILURE is the assessed mechanism; execution of retention predicates is not demonstrated by this bundle. Native Winsock failure is explicitly NO. ORACLE_SATISFIED remains PARTIAL.

## Original R4 authority and owner decision

Current PST docs/model-e-consumer-conformance-matrix.md R4 says RESOURCE_FAILURE plus query-only retention, minimum class "falha governada". Its footer prohibits substituting synthetic, normal, test-only, real-native and platform classes. ADR-0008 requires provider-exclusive classification, truthful native fields, preserved dependencies and no retry/fallback; every native failure must be retained. Neither source expressly states that the R4 row itself requires both a controlled provider failure and a real Winsock failure.

The returned assessment explicitly calls its outstanding requirement the "original native-Winsock R4 oracle". That requirement is not located as a separate normative R4 clause in the verified matrix/ADR. This is an unresolved evidence-class/authority distinction, not permission to downgrade a gate. Native-failure conformance remains separate and unproved for OpenSSL. No full R4 qualification is declared.

Owner decision requested: identify the exact native-Winsock R4 normative reference, or separately approve a governed-provider R4 conformance class while retaining native-failure coverage as an explicit independent obligation. Any accepted change must be recorded normatively before qualification; do not edit accepted ADRs retroactively. A decision document alone cannot replace execution evidence.

Safe next proof: owner-governed current-commit test-only package with provider-boundary interception before native close, truthful non-native diagnostics, factual release attempt/result/disposition, per-Connection/operation/generation retention and negative traces. Such a lane cannot prove closesocket failure. A real-native lane requires a separately approved safe ownership-preserving recipe; none is authorized here. Do not corrupt handles, set consumer linger, hook Winsock, retry close or infer native failure from peer reset. No external message was sent.

## ED08 reconciliation

All 28 historical trace hashes and PASS identities verify without rerun. They remain scoped to the original normal OpenSSL and Combined artifacts, not the new current-baseline normal SDK. Current BIO qualification does not invalidate those historical results and does not migrate them to a different artifact.

| Boundary | Preserved authority | Remaining limit |
| --- | --- | --- |
| Reciprocal COMPLETE | Public PST operation | Not OUTPUT_DRAINED |
| BA fence/ACK | Frozen producer and exact C3 accepted prefix | Candidate harness, not production PACC/Browser |
| Child release | Membership removed, public result plus DESTROYED | No destructive inference from attempt |
| C3 EOF | Ordered external EOF after DESTROYED | EOF alone never proves provider output submitted |
| AB fence/ACK | Exact prefix and downstream native acceptance after EOF | Native acceptance is not Site TCP delivery |
| Model E | Real PRECONDITION_RETAINED and DESTROYED | OpenSSL R4 full failure lane remains open |
| Cancel/reset/late byte | Truthful non-clean outcome and generation guards | No false clean or premature seal |

ED08 scoped historical prefix/EOF probes remain PASS; full gate incomplete. Remaining dependencies: R4 evidence-class decision and actual governed retention execution; current normal artifact impact analysis before claims about that artifact; real Browser/C3/PACC integration where required. ED11 is preserved as a separate task and is not updated here. Specification 0.3 and supplement 0.3.2 remain proposed; encoding approval and runtime qualification remain distinct.

CURRENT_TASK_COMPLETED=YES
SLICE_COMPLETED=YES
R4_QUALIFIED=PARTIAL
R4_NATIVE_WINSOCK_FAILURE=NO
ED11_STATUS=PRESERVED_SEPARATE_TASK
WIRE_READY_FOR_APPROVAL=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
FIRST_UNRESOLVED_DIVERGENCE=R4_NATIVE_ORACLE_AUTHORITY_AND_EXECUTION_EVIDENCE_UNRESOLVED
NEXT=PST_OWNER_CLASS_DECISION_AND_GOVERNED_EXECUTION_RETURN; BIO_README_PROVENANCE_CORRECTION

## Subsequent governed return and execution

The new R4 package provides oracle-authority.md accepting the controlled provider class and withdrawing the unlocated native-only assertion. [Integrated component closeout](ed08-r4-integrated-closeout-20261012.md) records 20 new runs with real Accelerator API3 components and actual provider retention. Earlier documentary assessment remains historical; native Winsock remains unproved and production C2 publication remains outside the executed fixture scope.
