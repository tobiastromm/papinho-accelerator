# Personal first-install bootstrap completion — partial implementation and native STOP

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-BOOTSTRAP-COMPLETION-1, 2026-10-22.
Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
CURRENT_TASK_COMPLETED=NO. The dependent CA/bootstrap path stopped on a factual native finalization failure. Independent approved DATA/owner integration and regressions remain preserved. No productive bootstrap or CURRENT publication.

## Independent implementation

The same PAPACC_PERSONAL_PLAN_DATA component now distinguishes an effectful scope from the existing read-only scope. Private bounded presentation bytes explicitly name NEW_INSTALL, CREATE_EXCLUSIVE_TARGET, CA_INITIALIZE, INITIAL_ROOT_PREPARE, COMMIT=DENY and CURRENT=DENY, followed by operator identity, retained-parent identity, exact relative name, requested CA authority/generation/service identities and exact configuration. These are ephemeral DATA bytes, not a persisted/wire format, root, journal or approval service. The constructor is portable C with no native types, crypto, allocation or callbacks; hex encoding is presentation only, not a signature/digest or authority proof.

The maximum remains8192 total plan bytes. Metadata reduces the available configuration space; oversize requests deny instead of increasing buffers/quotas. Inputs, bounds and aliases are checked before writing output. Conversion from a captured configuration to the effectful plan is allowed only before presentation, with unconsumed DATA and matching exact configuration. A presented/confirmed/consumed F10 read-only attempt cannot be promoted. Normal confirmation/consumption APIs reject effect scope. The distinct exact declaration is OWNER_NEW_INSTALL_CA_INITIALIZE followed by this attempt's64 hex digits; wrong statement/plan invalidates, and consumption is irrevocable and single use. Stop invalidation does not erase consumed state or resources.

papacc_configuration_personal_effect_plan_data prepares this scope in the SAME owner from existing protected native operator/parent, frozen relative leaf and already held source bytes. It requires same writer, no Stop/unresolved/worker/created child, captured-not-presented state, and revalidates native context/source. Full expected plan is staged at previous+1024 within existing bounded scratch and copied into the existing owned plan. No new owner or heap. Requested CA identifiers/service bytes remain DATA; the helper does not authenticate them as CA custody or service configuration. The source revalidator compares the exact configuration segment while retaining the same file identity. Read-only worker submission rejects effect scope; effect start uses separate consumption against the frozen full plan. The existing factual/material/capacity gate remains closed.

The production CLI/UI and its read-only F10 flow were deliberately not activated for effects. Actual effect-plan console presentation/confirmation, factual configured service/CA source, protected CA reference acquisition, initial root material resolution and commit integration are NOT qualified/completed by the DATA APIs. No read-only result is reused as CA authorization. No accepted contract, historical codec/format, pin table, root or worker architecture changed.

## Exact isolated CA operation and STOP

A new, separately reviewed ca-lab case was compiled only into the existing isolated native component diagnostic. It is not the removed historical --native-key fixture and does not run automatically in CTest or product startup. The official executor separately approved the exact command run-ca-lab.ps1 and its effects before execution. The command pinned Selected/Release diagnostic SHA256610a083df2ee0d29da3f36adf08de23b4a083ccf0c529eba9d8793fcdf5a9006. It used a fresh parent/child under build/personal-bootstrap-completion-20261022/effects/ca-lab, actual process TokenUser and explicitly selected WinLocalServiceSid as a LAB-only exclusion identity, not a guessed product service identity. No automatic elevation or credential prompt.

The one exact disposable LAB key request was:

- Provider: Microsoft Software Key Storage Provider; scope MACHINE.
- Authority ID: cb87632463bb48dd8a34fa8ea6f3911b.
- Generation ID: 030fa34dd44f41e8b143b9da09532cf2.
- Name: PapinhoAccelerator.CA.cb87632463bb48dd8a34fa8ea6f3911b.030fa34dd44f41e8b143b9da09532cf2.
- Existing papacc_ca_key_create, RSA2048, export policy0, signing-purpose property, existing protected DACL; no overwrite flag, certificate, signature or private key copied to CLI/logs.

The planned test would validate custody, require NTE_PERM on private export, test borrow/return, then delete ONLY the exact owned disposable key via NCryptDeleteKey after successful qualification. Delete failure would retain, with no retry. This discard path was separately included in official approval. It was NOT reached.

Observed result: wrapper exit1; CA status0x80090010 (NTE_PERM); created=1; finalization=1 (attempted, not confirmed); state=3 (CLOSED); key_owned=0/provider_owned=0. Sticky first failure: function1 BEGIN_CREATE, stage20 FINALIZE, native API7 FINALIZE, original0x80090010. This identifies NCryptFinalizeKey failure, not a diagnosed reason for the provider's refusal. Do not infer that elevation alone would repair it.

The existing owner performed its failure-close path and reported closed handles. That does NOT prove absence/deletion of the named persistent key object, nor successful RSA generation, private-key protection, CA custody READY, correspondence or rollback. Namespace disposition remains unproved. No key reopen/query, retry, replacement generation/name, explicit deletion, export, borrow, certificate, signing or further dependent operation followed the failure. The created target fact remains; process exit is not treated as rollback/destruction proof. The exact failed name/identities/command/result are retained for later governed reconciliation. This task authorizes no automatic rerun of the key command.

The first factual blocker is therefore CA_INITIALIZE/NCryptFinalizeKey=NTE_PERM before a READY protected CA reference. The private-key/provenance/correspondence/operational resource gates remain unqualified. Product CLI effect activation and initial root/commit were not bypassed to work around this failure.

## Bootstrap and authority boundary

Initial bootstrap still has no prior CURRENT/predecessor requirement and does not manufacture either. Existing papacc_server_config_initial_bootstrap_resolve/commit require the canonical invocation, factual administrative custody, prepared governed participants/policy/source, matching model/security references and reservations. No CA key handle, model-only confirmation, DATA PASS or old F10 receipt supplies those prerequisites. No scope/proof bit, CURRENT pin, certificate or initial root image was fabricated. CA signing lease is not substituted for bootstrap. The sequence stopped before initial root preparation/validation/commit; all publication/admission/signature/recovery gates remain DENY.

## Validation and ED11

Every functional change was approved as exact staged hunks with pre/postimage hashes. Final MSVC19.51 x64 Legacy/Selected Debug/Release builds pass /W4 /WX without warnings. Eleven offline suites pass per build (44 suite runs), including new effect-plan valid/mismatch/read-only reuse/capacity/alias/identity/Stop/replay cases, existing read-only/namespace guards, hash ownership/lifetime, scratch, issuer denial and memory census.

The separately approved44 no-key native cases pass across four builds, covering new/existing target, actual identity, parent/attempt mismatch, Stop before/after creation, injected UNKNOWN/close failure, double submission and metadata-only CA. Native authority in these diagnostics remains MODEL; actual filesystem identity is distinguished from it. One additional separately approved CA LAB case FAILED as above; there is no positive custody qualification. Export/borrow/deletion subtests were NOT_RUN. No network, real bootstrap, operational certificates or CURRENT.

Durable DATA adds8 bytes: Legacy root175120, pending43408, known managed1799608/2097152; Selected root340616, pending73296, known managed1965104/3145728 with explicit profile selection. Nominal remaining297544/1180624 is arithmetic only, not operational margin. Existing scratch/one serialized64KiB worker are reused, not charged twice; no new pool/pins/concurrency or quota. One child handle remains in the existing field. The LAB-only CA owner uses the existing PAPACC_CA_KEY type, actual token query, provider/key and three descriptor/SID allocations; no copied key or second CA service.

Selected own frames observed, excluding return addresses/callees: effect builder Debug264/Release184; effect confirmation120/72; LAB CA helper2552/2360 bytes. The latter reserves2048 bytes for a planned negative export test and wipes before checking its result, but export was not reached. These are compiled frame observations, not total nested stack or a complete resource peak. Full native/CRT/provider/kernel/host admission, key namespace disposition and coexistence remain UNKNOWN. No opaque cost is zeroed and ED11 stays PARTIAL/operational DENY.

F10 UI source, launcher, old qualified binary and receipt48091b1698c841e6a63bb11ee5b0ab30 remain byte-exact; no UAC/F10 repeat. Baseline1261 existing WIP files outside reviewed scope are byte-exact before the appended live-document update. PST, Browser, PACC wire, production0.6.4 and four historical LABs were not modified; new effects have their own previously absent root. No clone/worktree/commit/push/tag/release.

## Result

CURRENT_TASK_COMPLETED=NO
EFFECTFUL_PLAN=OWNER_DATA_IMPLEMENTED_NATIVE_CLI_EFFECTS_NOT_ACTIVATED
PERSONAL_CEREMONY_BINDING=SEPARATE_SCOPE_READ_ONLY_REUSE_DENIED
TARGET_EXCLUSIVE_CREATION=LAB_NATIVE_PASS_PRODUCT_DENY
CA_CUSTODY_INITIALIZATION=LAB_STOP_NCRYPT_FINALIZE_NTE_PERM
CA_FACTUAL_REFERENCE=NO_READY_CUSTODY_REFERENCE
INITIAL_ROOT_PREPARATION=NOT_RUN_GATES_PRESERVED
NATIVE_BOOTSTRAP_QUALIFICATION=STOP
ED11_TOTAL_STATUS=PARTIAL
BUILDS=MSVC_SELECTED_LEGACY_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS
OFFLINE_TESTS=44_SUITE_RUNS_PASS
NATIVE_TESTS=44_NO_KEY_PASS_1_CA_LAB_FAILED_STOP
NETWORK_TESTS=NOT_RUN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=LAB_CA_NCRYPT_FINALIZE_0x80090010_NO_READY_REFERENCE
NEXT=FIRST_PERSONAL_BOOTSTRAP_OPERATIONAL_READINESS
