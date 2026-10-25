# Factual CA_INITIALIZE producer — focused STOP, 2026-10-22

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-CA-INITIALIZE-PRODUCER-1.
Branch main; HEAD 913c90cf2328f7939d8f5b927640601903ac6e91.

The accepted CA technical profile, docs/adr/DECISION-internal-ca-technical-profile-20261018.md, Owner conditions, requires an approved backup/recovery/protection/rotation procedure BEFORE productive CA initialization. Initial verified backup/restore before issuance (3.5–3.6) is a separate later gate. This does not require backing up a nonexistent key before creation.

Local searches across docs/adr, docs/capabilities and docs/integration identified the requirement and historical copies, but no concrete approved deployment procedure satisfying it. The existing issuer-same-root-checkpoint-linkage-20261019/backup-fencing-and-cli-handoff.md explicitly leaves protected backup/recovery selection and latest-state/fencing proof as future work. This is a bounded finding about available local evidence.

The productive contract exists; its concrete prerequisite is not demonstrated. Task sections 2 and 10 require STOP rather than presumed authority. No accepted contract was changed or LAB G1–G5 promoted to production.

Technical profile 3.3 also requires explicit issuer-operator and distinct operational-service SID selection. The CLI captures actual operator identity, but no productive service-selection/provenance producer was identified. The LAB LocalService exclusion cannot become a productive default.

## Existing components preserved

- papacc_ca_key_create in apps/server/server_ca_key_win32.c already owns creation/properties/finalization/public validation; no new manager is needed.
- server_launcher_personal_win32.inc still explicitly requests no CA_INITIALIZE. Its read-only confirmation cannot authorize key creation.
- personal_native_ceremony_ready in server_configuration_personal_data_win32.inc remains false; DATA matching is not native admission.
- server_configuration_personal_ca_win32.inc supplies exact effect-plan matching, protected borrow/current/release and both LAB-identity exclusions, not a factual initializer. Caller-owned same-thread CA storage must survive all borrows and uncertain results.

Minimum next input: approved concrete procedure under technical profile 3.5–3.7/Owner conditions, identifying protected recovery mechanism/artifacts, custodians, consistency/latest-state/fencing, rotation/retirement and uncertain-result retention; explicit productive operator/service selection under 3.3. No backup tool or policy is selected here. Actual restore/token isolation qualification remains separate.

After these sources exist, exact source hunks can bind them to the effectful plan before presentation, consume CA_INITIALIZE once, create the child exclusively, invoke the existing CA owner and acquire the existing protected borrow. This is a future handoff, not implemented behavior. Create/Finalize are separate effects; closing handles does not prove persistent deletion. No code patch was prepared and no functional approval requested or rejected.

Root preparation remains NOT_RUN. A finalized key supplies neither CA certificate/issuer correspondence nor qualified UTC. Do not fabricate these inputs. Initial bootstrap does not require previous CURRENT and is not replaced by UPDATE.

No resource delta. Previous measured layout subtotals: Legacy1799624/2097152, explicitly Selected1965120/3145728. Nominal remainders297528/1180608 are not complete margins. Separate caller-owned CA storage480 bytes must be charged once if not already reserved. Full nested stack, provider/kernel, runtime, retention and host admission remain unqualified; ED11 PARTIAL. Prior builds/tests are historical evidence, not new executions.

Only documentary artifacts added; existing-file hashes checked in evidence.json. No functional source, accepted ADR, receipt, LAB artifact or build directory altered. No native key query/create/delete, bootstrap, certificates, CURRENT, network, PST, Browser, wire, quota, production0.6.4, commit, push, tag, release, clone or worktree operation.

```text
CURRENT_TASK_COMPLETED=NO
CA_INITIALIZE_PRODUCER=STOP_PREINITIALIZATION_PROCEDURE_NOT_DEMONSTRATED
EFFECTFUL_CLI_BINDING=NOT_IMPLEMENTED
NEW_INSTALL_AUTHORIZATION=EXISTING_DATA_ONLY_NOT_PROMOTED
TARGET_EXCLUSIVE_BINDING=EXISTING_GUARDED_PATH_PRESERVED
CA_KEY_FINALIZATION=NOT_RUN
CA_FACTUAL_REFERENCE=NOT_ACQUIRED
CA_CUSTODY_LIFETIME=EXISTING_BORROW_RETENTION_PRESERVED
INITIAL_ROOT_PREPARATION=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
BUILDS=NOT_RUN_STOP_BEFORE_PATCH
WARNINGS=NOT_ASSESSED_THIS_TASK
OFFLINE_TESTS=NOT_RUN_STOP_BEFORE_PATCH
NATIVE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
NEW_PRODUCTIVE_KEY_GENERATION=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
LAB_KEY_PRESERVED=YES_NO_KEY_OPERATION
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_VERIFIED_IN_EVIDENCE
FIRST_UNRESOLVED_DIVERGENCE=APPROVED_PRODUCTIVE_CA_PREINITIALIZATION_PROCEDURE_NOT_IDENTIFIED
NEXT=PRODUCTIVE_CA_PREINITIALIZATION_PROCEDURE_AND_SERVICE_IDENTITY_THEN_CA_INITIALIZE_PRODUCER
REQUESTED_DOWNSTREAM_NEXT=PERSONAL_FIRST_INSTALL_INITIAL_ROOT_PREPARATION
```
