# Instrumented CNG native diagnostic — precondition STOP

2026-10-19. Task PACC-PHASE6-CNG-INSTRUMENTED-NATIVE-DIAGNOSTIC-1.

No native diagnostic attempt was submitted or executed. The user STOP conditions apply before key access: exact new diagnostic authority/generation/name and designated administrative operator context have not been established. Existing fixture derives operator from any current TokenUser and service from LocalService; these are observable identities, not proof of administrative appointment or actual service-token isolation.

Observed executor: TCORP-TOBIAS\CodexSandboxOffline, SID S-1-5-21-896877160-3262526087-2538948522-1003, medium integrity S-1-16-8192. Group query contains no Administrators membership. This does not prove NTE_PERM cause or that elevation is required; no token/security changes attempted. Process query found no ca_key/ca_failure diagnostic process.

HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, WIP513 entries at start. Selected source/header/test/preimages and Debug executable match prior final manifest. Debug executable SHA256 24E0314226A08493F029DBCF39948348126FABD0F6082D517E64D24C2E823C07. Backend SHA256 E75190B8071FCDD6E834F512D69FB4E51E50290BCB7497B92C597345B2600C9C; header CA0742B6B69E5E24627938FF265E915DD21790F6416D2663A456122C38D8CD74; fixture A83AFDD3FCF93CE6177D06402361F27EA6723C6BD25BA0BF4518F033DCC3D9BF.

The existing --native-key flow generates an unreported name immediately before create, attempts private export after success, then reopens and deletes its new key. Current task forbids private export and deletion; therefore that binary command cannot be submitted unchanged even if creation is expected to fail. No assumption that failure will avoid prohibited branches. Backend creation itself sets security descriptor for the new key as accepted contract requires; this must not be confused with authorization to modify existing key ACLs.

Minimum next preparation: separately reviewed diagnostic entry point with explicit predeclared fresh authority/generation/name, designated operator/service SIDs, read-only exact-name preflight, one create invocation, complete failure/close receipts and no private export/delete/retry. Preserve old UNKNOWN key untouched. Read-only open failure must not be represented as global absence; uncertainty denies proceeding. No backend guard or KSP change is justified. Submit exact executable/arguments and scope to executor before native use. If owner requires zero security-descriptor writes even on the newly created key, existing backend cannot satisfy that restriction unchanged; resolve before invocation.

No query of ambiguous/new/old key was made. Prior key remains UNKNOWN; no new key exists by an action in this task. No evidence retroactively identifies historical API/stage.

```ini
CURRENT_TASK_COMPLETED=NO
CNG_NATIVE_ATTEMPT=NOT_RUN_PRECONDITION_STOP
OFFICIAL_NATIVE_EXECUTOR_REVIEW=NOT_SUBMITTED
FIRST_FAILURE_STAGE=NOT_OBSERVED
FIRST_FAILURE_ORIGIN=NOT_OBSERVED
FIRST_FAILURE_API=NOT_OBSERVED
FIRST_FAILURE_CODE=NOT_OBSERVED
KEY_STATE=UNKNOWN
KEY_FINALIZATION=NOT_OBSERVED
KEY_CLOSE_RESULT=NOT_RUN
KEY_CLEANUP_REQUIRED=UNDETERMINED
KEY_CLEANUP_AUTHORIZED=NO
KEY_CREATED=NO
KEY_DELETED=NO
ACL_CHANGED=NO
REAL_CA_CREATED=NO
REAL_CERTIFICATES_ISSUED=NO
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
NETWORK_TESTS=NOT_RUN
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=EXACT_DIAGNOSTIC_IDENTITY_AND_ADMIN_CONTEXT_NOT_ESTABLISHED
NEXT=EXACT_NONDELETING_DIAGNOSTIC_ENTRYPOINT_AND_OPERATOR_IDENTITY_REVIEW
```