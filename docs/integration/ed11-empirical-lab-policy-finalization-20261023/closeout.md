# Single diagnostic LAB policy — complete proposal awaiting exact owner risk acceptance

TASK=PACC-PHASE6-ED11-EMPIRICAL-LAB-POLICY-FINALIZATION-1

The SAME [B0–B6 candidate revision4](../../adr/ADR-CANDIDATE-ed11-single-lab-diagnostic-native-boundary-20261023.md) now contains explicit numeric freshness/headroom/participant/retention/credit choices. [Decision package](decision-package.md) separates facts, proved controls, finite observations, selected operational risk reserves and residual UNKNOWN. No parallel ADR or new quota. [Revision3 original](original-candidate-revision3.md) preserved SHA25665bbe1361cd83551a3e7b7bb630e54f6ceb857eaf02ba708ff1f31bfb485750a; all older proposals remain. B0–B3/B4.A/B4.C/B4.D/B5–B6 retain their normative text.

## Concrete decisions ready for review

Preflight expiry60000ms; final oldest capacity metric age<=1000ms; nominal100ms sampling/max1000ms gap; no more than two pre-effect metric generations. Host admission physical>=5GiB/commit>=9GiB; monitoring floors4GiB/8GiB with1GiB chosen operational allowance. Max4 admitted processes; each PowerShell memory guard256MiB/handles1024/threads64/observable committed stack32MiB, each native process64MiB/128/16/8MiB. Sum640MiB/2304 handles/160 threads/80MiB committed stack, stack nested in commit; no double count. [Arithmetic check](numeric-policy-checks.json) is document consistency, NOT a runtime test or measured resource ceiling.

These are now explicit PROPOSED risk choices as requested, not falsely derived maxima. Existing Selected3145728/Legacy2097152 controlled bytes and all other quotas unchanged. HOST_CREDITS=NONE_GRANTED; lifetime/exclusivity proof remains mandatory, unknown costs not free. Future canonical owner must exclude competing project admissions and preserve retained full charge; if not qualified, exact operation STOP. The real service/concurrent peak is an outcome of the eventual approved measurement, never a prior service-peak prerequisite. Missing pre-effect process metrics concern live participants only; no service PID/token is invented before Start.

Kernel/cache/CRT/native/callback stack, external concurrency, between-sample overshoot and synchronous I/O uncertainty remain explicit and require exact owner risk acceptance. Limits are monitoring/STOP guards, not fail-before-all-native-allocation OS guarantees. No destructive timeout cleanup, retry, extra Stop, overwrite or automatic deletion. If safe Stop is not accepted by SCM, preserve query-only retention/UNKNOWN.

## SCM1783 resolved

[Root-cause report](scm1783-root-cause.md) and [paired native result](scm-readback-size-result.json): the temporary collector used cbBufSize16384 where the documented/RPC contract maximum is8192. On the SAME handle, aligned buffer, ABI and token context,16384 =>1783; changing ONLY declared size to8192 =>success,528 bytes. Configuration type16/startup3/correct virtual account/old image and SID type1 observed; status STOPPED/PID0/1077. Handles closed, no SCM/ACL mutation. The canonical adapter ALREADY uses8192 and needs NO functional patch. The faulty historical agent collector receipt remains unchanged; the minimum correction is only its query-size constant. No assumption of RPC transport/trust/ACL/service-account failure or incorrect structure layout remains necessary. No second service/lifecycle experiment.

[Microsoft API limit](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryserviceconfigw) and [MS-SCMR RPC range](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-scmr/89e2d5b1-19cf-44ca-969f-38eea9fe7f3c) support the argument-range diagnosis. Research lookup is not an application-network test. Existing historical capacity20:29Z RAM6285791232/commit17306345472 remains stale background evidence, not current capacity or admission. No equivalent fresh memory/boot query repeated.

## Acceptance and next exact operation

The proposal is now numerically concrete, but the owner has not approved THESE selected numbers/residual risk. AGENTS.md requires proposed durable decisions to receive human approval. Submission is this exact package/proposal hash, not a fabricated official acceptance or a claim that tool execution approval accepts architectural risk. Keep PROPOSED until owner decision and official exact acceptance record. Needed decision: approve or revise B4.B.2–B4.B.6 values and B4.D residual uncertainty for ONE B0–B6 diagnostic profile. No request for new facts or a circular premeasurement replaces that explicit choice.

Only after normative acceptance: freeze/qualify successor launcher and observer, pin executable/config/native identity, prove canonical owner exclusivity, prepare protected exclusive image/diagnostic file and governed SCM rebind/readback, then obtain separate manual elevated exact-operation approval. Existing SCM image is still OLD; no placement/rebind/diagnostic file or live one-shot intent was prepared/armed here. Normative approval alone grants NO Start.

```text
CURRENT_TASK_COMPLETED=NO
ED11_EMPIRICAL_LAB_PROFILE=PROPOSED
FRESHNESS_POLICY=PROPOSED_PREFLIGHT60000MS_FINAL1000MS_SAMPLING100MS_MAX_GAP1000MS
HEADROOM_POLICY=PROPOSED_ADMISSION_RAM5GIB_COMMIT9GIB_MONITOR_FLOORS4_8GIB_ALLOWANCE1GIB
HOST_CREDITS=NONE_GRANTED
RESIDUAL_UNKNOWN=EXPLICIT
SCM_READBACK_1783=RESOLVED_COLLECTOR_CBBUFSIZE16384_EXCEEDS8192_CANONICAL_ADAPTER_ALREADY_CORRECT
SINGLE_ATTEMPT_ADMISSION=STOP_UNTIL_APPROVED
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_8415_PRIOR_PREIMAGES_TWO_FOCAL_DOCUMENTARY_UPDATES_ONLY
FIRST_UNRESOLVED_DIVERGENCE=EXACT_OWNER_ACCEPTANCE_OF_SELECTED_NUMERIC_RISK_POLICY_PENDING
NEXT=OWNER_POLICY_ACCEPTANCE_THEN_SUCCESSOR_QUALIFICATION_AND_SEPARATE_EXACT_OPERATION_APPROVAL
```

main HEAD913c90cf2328f7939d8f5b927640601903ac6e91;8415 preimages verified before changes. One focused read-only paired SCM test exited0; no production build/functional tests needed or claimed. PST/Browser/wire/production0.6.4/quotas/four historical LABs and protected files/keys unchanged. No clone/worktree/commit/push/tag/release, bootstrap, keys/certificates/CURRENT, clock/reboot, account/ACL/service mutation or operational network. Prior orphan remains UNKNOWN, no retrospective cleanup/refund.
