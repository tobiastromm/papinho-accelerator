# Issuer live-root and recovery — partial closeout 2026-10-19

Implemented the safe prerequisite: immutable issuer snapshots tied to the existing root's three slots and exact pin tickets. Native component tests preserve three simultaneous generations, reject stale pins and full capacity, and retain dependencies through Stop. Application dormant/publication/admission guards remain intact.

Dependent live publication was stopped at the unproven operational-cohort lifetime boundary. Existing Session validation uses the listener's single CURRENT revision/epoch. Advancing it without a proven cohort transition would invalidate Sessions; rewriting their admitted facts would reinterpret authority. Authoritative recovery also cannot use integrity-only history in place of the independent checkpoint/fencing required by accepted technical profile2.4. See lifetime-and-recovery-proof.md for exact functions, constraints and next source work. No claim of completed live issuer is made.

Official escalated writes/builds/offline runs were approved. No executor denial occurred. Source changes are limited to existing root snapshot storage/copy/slot selection, preserving the application guard; tests and live documentation. No accepted decision changed. Prior WIP preimages and baseline are preserved. HEAD remains main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Browser, PST, public wire and production pin were untouched.

CURRENT_TASK_COMPLETED=NO
ISSUER_ROOT_LIVE=NOT_ENABLED_IMMUTABLE_GENERATION_PREREQUISITE_IMPLEMENTED
CURRENT_PUBLICATION=DENY_PENDING_COHORT_CONTINUITY
EPOCH_FACTUAL=EXISTING_INITIAL_ONLY_NO_NEW_ISSUER_EPOCH
AUTHORITATIVE_RECOVERY=DENY_PENDING_APPROVED_CHECKPOINT_AND_FENCING
SERIAL_DURABILITY=NATIVE_STORAGE_VALIDATED_SIGN_AUTHORITY_DENIED
REVOCATION_PERSISTENCE=SYNTHETIC_HISTORY_NATIVE_FILE_VALIDATED
SESSION_COEXISTENCE=ISSUER_BYTE_SNAPSHOTS_VALIDATED_OPERATIONAL_SESSIONS_NOT_PROVEN
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=PASS_SCOPED_COMPONENT_AND_APPLICATION_GUARDS
NETWORK_TESTS=NOT_RUN
CNG_NATIVE_STATUS=UNRESOLVED
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_WITH_PREIMAGES
FIRST_UNRESOLVED_DIVERGENCE=OPERATIONAL_POLICY_COHORT_CONTINUITY_ACROSS_ISSUER_COMMIT_NOT_PROVEN
NEXT=PROVE_SAME_ROOT_POLICY_COHORT_CONTINUITY_AND_BIND_APPROVED_RECOVERY_CHECKPOINT_BEFORE_CNG