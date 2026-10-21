# Issuer live publication and Session coexistence — executor STOP

CURRENT_TASK_COMPLETED=NO
OFFICIAL_EXECUTOR_REVIEW=DENIED_FOR_SECOND_SOURCE_OPERATION
ISSUER_LIVE_PUBLICATION=PARTIAL_UNVALIDATED_NOT_INTEGRATED
SESSION_SNAPSHOT_BINDING=NOT_IMPLEMENTED
OLD_REVISION_RETENTION=PREVIOUS_THREE_SLOT_IMPLEMENTATION_PRESERVED
CURRENT_PUBLICATION=NEW_COMPONENT_HELPER_UNVALIDATED_NO_APPLICATION_CALLER
EPOCH_FACTUAL=NEW_POST_COMMIT_HELPER_NOT_EXECUTED
CHECKPOINT_FENCING=NOT_PROVEN
AUTHORITATIVE_RECOVERY=DENY
BUILDS=NOT_RUN_CURRENT_PARTIAL_SOURCE
WARNINGS=NOT_ASSESSED_CURRENT_PARTIAL_SOURCE
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
CNG_NATIVE_STATUS=UNRESOLVED
ED11_TOTAL_STATUS=PARTIAL_CURRENT_DELTA_NOT_COMPILED
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_BASELINE_AND_BYTE_EXACT_PREIMAGES
FIRST_UNRESOLVED_DIVERGENCE=OFFICIAL_REVIEW_REJECTED_CURRENT_EPOCH_SESSION_REAPER_PATCH
NEXT=OFFICIAL_EXACT_PATCH_SECURITY_BOUNDARY_REVIEW_BEFORE_FURTHER_SOURCE_WORK

## Operations and exact disposition

1. Officially approved documentary operation preserved baseline/preimages and registered the exact owner-approved coexistence direction as docs/adr/DECISION-issuer-live-session-coexistence-20261019.md, ACCEPTED. This normative record does not qualify implementation or waive security/ED11 gates.
2. Officially approved first source operation partially executed. It added per-slot context_epoch, same-root pin_read and post-commit issuer_publish06 helpers, initial slot-epoch binding, root_snapshot_held, four reserved Session-pin containers, and the first held-snapshot condition in root_cohort_preflight. Its unique-preimage check then failed before the second epoch condition: source newlines differed from the expected hunk. That was a local preimage mismatch, not executor approval denial. No broad newline rewrite was performed. Five source files differ from this task baseline.
3. A second exact source operation proposed the remaining epoch/cohort checks, listener compatibility, Site TLS held-pin validation, factual Session pin acquisition during adoption, per-Session validation, and release only at the existing quiescent reaper boundary. The executor rejected it BEFORE execution. Those changes were not applied. No alternative tool, shell, split submission, privilege/token change or indirect write was used to apply them.

## Exact executor objection

This action was rejected due to unacceptable risk.
Reason: These hunks alter core admission, epoch, Session-binding, and reaper authorization paths; despite the scoped coexistence request, the exact changes are not yet proven to preserve the security boundary and may weaken current-epoch validation while enabling live authority.
Do not bypass this rejection through a workaround or indirect execution.

## Scope of partial WIP and risks

Changed sources: server_configuration_commit_win32.h, server_configuration_issuer06_win32.inc, server_configuration_root_transaction_win32.inc, server_launcher_owner_win32.h, server_admission_preflight_win32.inc. Baseline and exact preimages are preserved; focused diff accompanies this closeout. server_c2_root_binding_win32.inc, server_c2_owner_win32.c/.h, server_listener_composition_win32.inc and server_launcher_owner_win32.c remain byte-exact to the task baseline. The Session pin arrays are not populated by adoption. The new component publication helper has no application caller. Its existence is not a normative application CURRENT/epoch acknowledgement or safe live composition. Source is partial and uncompiled: do not run old executables as evidence of these edits or claim previous PASS for this state.

No attempt was made to call the new helper, start a product, create a Session, emit CNG signatures, generate certificates or operate network. Generic production publication denial and all admission configuration were not changed. Authoritative recovery remains observational/denied; no checkpoint/fencing proof was supplied. Current storage/stack/coexistence census must be recompiled before any conformity claim. Prior numerical ED11 census belongs to the previous source composition.

## Required official review evidence before continuation

Review exact accepted-direction record, baseline and focal diff together with the denied operation's function list. Prove that an old Session retains its admitted revision/epoch/Principal/grants, while current policy still enforces contextual revocation; no arbitrary valid old pin may substitute for a factual admitted Session. Prove new Sessions acquire only actual CURRENT after a confirmed same-root commit and post-commit epoch issuance. Prove held snapshot validation cannot replace authentication/admission/grants, and that failed unpin/close preserves ownership through quiescence. Review reaper call placement against every native Session/Connection/channel/processor/operation/writer/callback/retained predicate. Inspect aliasing and forged/stale token behavior before claiming a pure-copy API safe. No positive recovery without fencing/latest checkpoint.

Submit any future exact operation through official review; this closeout is not approval to retry the rejected operation. Existing owner authorization does not override the executor denial. No build/test is declared PASS for partial source.
