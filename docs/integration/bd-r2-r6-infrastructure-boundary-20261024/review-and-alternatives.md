# R2/R6 console infrastructure boundary review

TASK=PACC-PHASE6-BD-R2-R6-INFRASTRUCTURE-BOUNDARY-1
Date2026-10-24; main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91.

Conclusion: separation of administrative authority from console transport is sound, but does not alone prove which auxiliary process is eligible. Prepare a focused candidate for A; operational result remains D (DENY) while classification assurance/profile and explicit owner decision remain absent. No supported proof of the stronger relation was found by the prior feasibility slice; no universal impossibility is asserted. No existing fact is discarded or promoted into authentication.

Authorities: accepted B+D revision2 R0-R7; accepted B0-B6 revision4/B4.B.1-6/B4.D; Model E and specification1.0 sections11.1-11.4; Stop/lifetime/pre-close ADR0030/0031/0032; independent gates ADR0034. Applicable continuous-session organization/family governance preserved; organization taxonomy0003 and external scratch0006 reviewed. Relevant future personal-listener direction remains unchanged (this task changes neither direction nor implementation). Audited ps_bd_console/inventory code: native source still fails closed, synthetic positives test-only. Candidate/capability are documentation, not runtime facts.

## Authority versus infrastructure

The elevated PowerShell's held creation/token/operator/confirmed plan and the canonical owner are the administrative chain. Console host identity is not a credential; it cannot grant a second admission or make Start authorized. The current contract nevertheless requires proof of host relationship and mapped-image provenance as a census security guard. Reclassifying it as a bounded auxiliary changes that guard. It cannot be sold as an ED11 accounting clarification or covered by original B4.D's kernel-cost risk acceptance. R3 must change expressly with R2; otherwise the proposal would conflict with the existing accepted decision if implemented. No conflict between two currently accepted ADRs is asserted; current code remains DENY and accepted text unchanged.

## Alternatives A-D

| Alternative | Security and census | Stop/resources | Disposition |
|---|---|---|---|
| A supported auxiliary classification | Native held process birth/liveness, token/session, protected opened OS file identity/provenance, actor/client/birth correlation and exact confirmed tuple together; neither names alone nor false authenticated relation. Must confront arbitrary genuine OS binary/spoofing cases | Same interactive transport/owner, four roles, full shared admin charge, metrics/freshness and UNKNOWN retained | Smallest proposed boundary change; conditional only, classifier/profile NOT qualified; missing facts still DENY |
| B console only during authorization, protected handoff | Single canonical owner could hold confirmed plan; exact handoff/lifetime/anti-replay still needs proof. Existing console process may remain present, so handoff alone does not remove accounting/classification | Requires a usable independently qualified post-handoff Stop/control path and changes R5/R6; not presently implemented/authorized | No smaller proven fix; do not introduce a second owner/channel by assumption |
| C qualifier independent after confirmation | Detaching qualifier leaves elevated PowerShell host; no-window is not authorization. Removing all console dependence entails a different operator/control topology | Console loss cannot terminate governance; independent Stop/retention path would require reviewed source/lifetime/resource contract | Alternative for a later explicit design slice if A's limited assurance rejected; not a drop-in flag fix |
| D preserve DENY | Current accepted R2/R3 plus native gate unchanged; arbitrary/unknown children remain rejected | Existing Stop and retained charges unchanged | CURRENT effective outcome until a proposal and implementation actually qualify |

A is not presented as a proven safe classification. Its proposed predicate and residual inability to distinguish some deliberately launched/substituted candidates are explicitly exposed in the candidate. The preservation requirement 'reject arbitrary process' forbids implementing a heuristic then calling it authenticated. If owner demands detecting those adversarial cases at the auxiliary boundary, choose D and develop B/C only under a new scoped authorization. No assurance can be invented merely to choose A.

## Supported observations and limits

[GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist) enumerates attached clients, not a documented native host handle. [QueryFullProcessImageName](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamew) supplies the process image name; it does not return publisher or mapped file identity. File identity/hash/trust applies to the opened object, not automatically that mapped image. Parent/session/client correlations remain labeled as such. Native handle+birth checks address reused process instances, not proof of console ownership. No new collection is needed to establish these documented limitations.

[SetConsoleCtrlHandler](https://learn.microsoft.com/en-us/windows/console/setconsolectrlhandler) is per-process notification handling; registration or receipt is not a completed service Stop. Closing console/system teardown may terminate processes before factual cleanup. [FreeConsole](https://learn.microsoft.com/en-us/windows/console/freeconsole) detaches caller; another sharing process can keep console alive. These documented properties rule out declaring cleanup or topology simplification by detachment alone. No API/capture/trace/native process query was executed here.

## Stop distinctions to preserve

| Fact | Meaning / forbidden inference |
|---|---|
| Interactive confirmation | Authorized actor accepts bound exact plan; host is transport, not authority |
| Authorized operation | Canonical owner holds fresh valid bound authorization; acceptance does not execute it |
| Console/actor loss | Suppress new effects; record failure; no implicit continuing detached authority |
| Stop requested | Intent only, at most existing single safe service request when already authorized/legal |
| Stop accepted | Native/runtime acknowledgement, still not quiescence |
| Quiescence proved / STOPPED factual | Separate runtime/process/SCM evidence; no timeout/window inference |
| Own refs closed | Explicit close results for original owned refs; not host termination, universal kernel release or historical cleanup |
| UNKNOWN | Retain evidence/charges/references as factual capability permits; no fictitious cleanup when original process itself dies |

## Exact review package

[Candidate R2.N1-N5 / R3.N / R6.N1-N4](../../adr/ADR-CANDIDATE-bd-r2-r6-console-infrastructure-boundary-20261024.md). R3 is a necessary consistency amendment; R0/R1/R4/R5/R7 are not replaced. This is a separate focal candidate referencing the byte-exact accepted revision2, not editing or overwriting its original proposal/history. Normative acceptance, scoped implementation, offline qualification, native preflight and exact operational approval remain separate.

No code/launcher/pin edits, builds/tests or new native evidence. User requested normative review only, so unchanged code does not require four repeated builds. Historical106/128 remains NOT_REACHED, no new handle sample, trace or preflight. Verification covers preexisting WIP byte preservation except the intended live documentation append. PST/Browser/wire/production0.6.4 and four historical LABs preserved. No service/SCM/ACL/provider/boot/key/certificate/bootstrap/CURRENT/git publication effects.
