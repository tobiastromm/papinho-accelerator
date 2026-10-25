# First native LAB diagnostic boundary — decision package

TASK=PACC-PHASE6-FIRST-NATIVE-LAB-DIAGNOSTIC-BOUNDARY-1
2026-10-24; branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91.

NORMATIVE_BOUNDARY=PROPOSED. Effective console/admission remains DENY. No native operation or implementation is authorized. The prepared decision is complete as a narrowly specified policy choice; it is NOT a source/native qualification result.

## Exact artifact for future owner decision

[Same boundary candidate, revision2 N0-N9](../../adr/ADR-CANDIDATE-bd-r2-r6-console-infrastructure-boundary-20261024.md)

SHA256: 1cc81dbfdc1a96cf16a223e0eef2446cde7b9628f34da9643a4ee1ddc103b9ab

This revision2 is of the 2026-10-24 boundary candidate, not an edit of the accepted 2026-10-23 B+D revision2. [Candidate revision1 original](candidate-revision1-original.md) remains byte-exact, SHA256ebb39ec0c9db0c70e7f38363e0fb352d7a36f5c0f6d900222efd1d77603bb5e6. The existing accepted records and approved original proposals remain untouched. Acceptance would require a designated normative record with exact precedence; no accepted registration in this task.

Proposed owner choice: accept or reject N0-N9 ONLY as normative policy, explicitly including N7's reduced auxiliary-classification assurance. Normative acceptance would NOT authorize implementation, launcher execution, preflight, file/ACL preparation, SCM rebind, Start/Stop, ETW, CA, certificates, bootstrap or CURRENT. It would permit a later scoped implementation request, reviewed offline qualification and a separately approved exact manual operation. Refusal of N7 leaves DENY; no automatic fallback or wider threat exemption.

The first unresolved decision is precise: may ONE native-identity-bound, context/file/resource-observed auxiliary coexist for this exact diagnostic without claiming its actual console-server association/mapped-image provenance is authenticated? If the answer requires authenticated host ownership, supported source remains unqualified and this route stays STOP. This is not an invitation to call an arbitrary unknown process Windows infrastructure; it is a named limited coexistence risk, with all specified observation facts still mandatory.

## Compared alternatives

| Issue | Boundary revision1 | Selected revision2 single-LAB boundary |
|---|---|---|
| Scope | Existing LAB but classifier profile/provenance rules left for later definition | Exact accepted host/boot/service/account/operator/candidate/config/attempt in N0; no other operation/production |
| Eligibility | General proposed 'proved Windows auxiliary' with unknown association and incomplete launch/token/provenance rules | Exact observed tuple: original handle+birth+liveness, fixed same-operator primary token/session, parent consistency, complete approved client set, process name/pinned opened-file facts, protected exact plan and required fresh metrics |
| Claim | Did not authenticate mapped image but still called classifier conditionally safe/needed fuller profile | Explicit IDENTIFIED_AUXILIARY_CANDIDATE; relationship/mapped provenance UNPROVEN, never 'legitimate host proved' |
| Risk decision | Deferred acceptability and profile before implementation | N7 exactly states trusted-OS/approved-operator scope, indistinguishable genuine-component/lookalike/injection risk, explicit owner acceptance or DENY |
| Resource controls | Four roles and unchanged guards | Same, with complete fixed numeric guards/sink/freshness/headroom/no credit; no baseline exemption |
| Minimal implementation | Potential wider provenance/classifier project | Same owner/intent/census/observer/Stop/sink only; no required new inline trust/PSS/recorder/nonce/Job/service mechanism |
| Operational status | PROPOSED/DENY | PROPOSED/DENY until normative acceptance, scoped implementation qualification and exact approval |

Why selected: it exposes the actual assurance reduction instead of continuing to demand an unsupported authentication proof or repeatedly measuring conhost. It preserves the purpose of the resource/census controls and the canonical authority mechanism. A broader baseline-delta exemption would hide preexisting project/auxiliary costs and trust processes by age; ARM/service admission and console-independent handoff require more architecture and lifetime work. Neither is the minimum path to existing psr_prepare first-failure capture.

## Clause reconciliation and precedence

| Existing authority | Proposed focal specialization | Preserved obligations |
|---|---|---|
| B+D R2 and proof-oriented narrative | N2/N3 replace only auxiliary authentication eligibility with explicit observed candidate tuple | Original process native identity, opened-file facts, session/context, lifetime, metrics, confirmation, no fifth slot and no name whitelist |
| B+D R3 | N7 is an explicit NEW classification/security exception for N2 tuple, not inherited B4.D permission | Actor/qualifier/service native identity/authorization, metrics, ACL isolation, Stop/lifetime remain fail-closed |
| R1 role4 wording | N1/N3: resource candidate with unproved host legitimacy, not a new administrative actor | One elevated PS, one qualifier, future factual service, max one auxiliary/four actual processes |
| R4 / B4.B.4 | N6 retains separate auxiliary metrics/shared admin budget; only eligibility changes | 256MiB/1024/64/32MiB admin pair, each native64MiB/128/16/8MiB, original caps and smaller controlled quotas |
| R5 / B4.B.5 | No authority/exclusivity change | Exact operator/token/plan/confirmation, same canonical mutex/intent/replay, NONE_GRANTED and retained charges |
| R6 | N5 distinguishes control loss, request, acceptance, completion, quiescence and reference close | No unattended continuation, one legal safe Stop when possible, failure/unknown retained, no kill/timeout/retry cleanup |
| B4.B.6 | Pin this exact policy revision; invalidate on all original changes plus changed auxiliary tuple | Same host/boot/candidate/config/identity, oldest metric freshness and single consume before effect |
| B4.D | N7 adds explicit auxiliary-risk decision SEPARATE from original opaque-resource exception | UNKNOWN kernel/cache/transitive stack/sampling facts remain UNKNOWN; no production or universal bound |
| R0/R7/B0-B3/B4.A/B4.B.1-3/B4.C/B5-B6 | No broader amendment | Exact one attempt, protected object/readback, readiness, Stop, sink, later separate approvals and authorities DENY |

No GOVERNANCE CONFLICT between currently accepted decisions was found. Implementing the proposed waiver before its explicit acceptance WOULD conflict with current R2/R3; it is prohibited. It is not an editorial reinterpretation of the previously rejected revision1 waiver.

## Supported evidence boundary

GetProcessTimes supplies creation on a held process handle; QueryFullProcessImageName supplies process image name; GetConsoleProcessList supplies attached client IDs, not host identity. Candidate N2 uses these component observations with native file/token/session/resource facts and explicit limitations; it never combines them into authenticated ownership. All supported mandatory facts must be implemented/qualified later, rather than imported from historical PIDs. No new API probe, file/open-process metric observation or signature check was performed now.

Official sources reviewed: [GetProcessTimes](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesstimes), [QueryFullProcessImageName](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-queryfullprocessimagenamew), [GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist), [SetConsoleCtrlHandler](https://learn.microsoft.com/en-us/windows/console/setconsolectrlhandler). SetConsoleCtrlHandler is notification handling, not completed runtime/SCM Stop or a guarantee cleanup executes before process death. No Win7 execution qualification inferred from supported component documentation; exact approved target remains Windows10.

## Threat and denial review

| Scenario | Required policy result |
|---|---|
| Wrong/unelevated operator, impersonation in mandatory administrative context, changed bound token, wrong plan/confirmation/hash/service/config | DENY; auxiliary cannot repair authority |
| Second admission/replay/consumed intent/abandoned canonical mutex | Existing canonical DENY/UNKNOWN; no replacement owner or rearm |
| Same-name executable in another path, unknown file identity/hash/SD or unsupported required query | DENY; N7 never waives required facts |
| Reused PID, original handle lost, exited auxiliary or changed token/session/file evidence | DENY/invalidate; after effect follow safe Stop/retention, no automatic replacement |
| Arbitrary child/compiler/receiver/new shell/second auxiliary/extra client/fifth role | DENY when outside the single exact eligible tuple; preserve first ERROR_BUSY/predicate |
| Deliberately launched genuine component/injected same tuple meeting every observable N2 fact, yet not real console server | NOT DETECTABLE AS SUCH by selected evidence; exactly N7 risk decision, not guaranteed rejection. If required to reject this hidden case, proposal cannot authorize this route |
| Required metric missing/stale,129 qualifier/service handles, admin sum over guard, host below floor | DENY before effects; after effects suppress/request only allowed safe Stop; no quota/credit substitution |
| Old process/baseline, same session/parent/name only | No trust, admission or free cost implied; use full N2 or existing unknown-process rule |
| Partial/empty sink/record-binding error, wrong native-valid domain | Failed/incomplete diagnostic; first failure/UNKNOWN, no success or retry |
| Console/owner loss, timeout or STOP_REQUESTED without acceptance/quiescence | No cleanup inference; suppress new effects, safe Stop only while legal/live, retained evidence/charges |

## Factual status, evidence and next work

Read both static/admission reports, revision1 candidate, accepted B+D/B0-B6 in full, Model E/ED08/ED11 specification sections and current core census/policy gates. Applicable governance remains from continuous-session review; relevant future listener direction reviewed and unchanged. Current ps_bd_console still ERROR_NOT_SUPPORTED for native proof; synthetic source test-only. No source/build/launcher/pin change. Four prior warning-free builds remain historical; no repeated tests required for documentation-only work. Old106 remains sample with HANDLE_GATE NOT_REACHED.

Historic boot pins and old capacity observations are not fresh current admission evidence. Future native/file/process IDs are not filled from text or reused old PIDs. No new recorder/trace/preflight/Start/rebind/Stop/SCM/ACL/bootstrap/key/certificate/CURRENT action occurred. Preserved WIP and accepted/historical hashes checked by verification.json; only candidate revision2 and live documentation append intentionally change existing files. Production0.6.4/PST/Browser/wire and historical LABs unchanged.

Next only AFTER an explicit normative decision: authorized minimal integration into existing owner/descriptor/census/observer, scoped four-build/offline qualification, fully pinned single elevated launcher, then separately approved read-only native qualification and exact manual diagnostic operation. Acceptance cannot certify an already-passing preflight or produce native proof retroactively.
