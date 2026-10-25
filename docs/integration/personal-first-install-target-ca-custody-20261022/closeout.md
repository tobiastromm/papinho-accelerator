# Personal first-install target / CA custody — independent scope

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-TARGET-CA-CUSTODY-1, 2026-10-22.
Branch main; HEAD 913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
CURRENT_TASK_COMPLETED=NO. Independent DATA projection and negative integration tests are implemented. Actual exclusive-target handoff and CA custody binding remain denied/unimplemented at the personal entry.

## Existing mechanisms and exact remaining boundary

Target owner: PAPACC_CONFIG_PENDING inside the same PAPACC_CONFIGURATION_COMMIT. `bootstrap_worker` in server_configuration_bootstrap_win32.inc opens the native parent, uses relative NtCreateFile FILE_CREATE for the child, records native identities and preserves CAS_UNKNOWN/handles. Begin/poll/ready/close are in server_configuration_pending_win32.inc. `personal_native_ceremony_ready` remains permanently false outside the diagnostic-only component path. No diagnostic bypass was used.

The old begin routine cannot adopt personal preflight resources safely as written: pending_blocks rejects the live personal graph, and after admission it memset-clears the packet and the worker reopens the parent. Connecting it directly would lose the held-before-confirmation context and violate F3/F5. That handoff is still absent; this task does not relax the guard, duplicate handles, copy a key owner, clear pending storage or claim that metadata satisfies it. The personal CLI does not call begin or create a child.

CA owner: PAPACC_CA_KEY in server_ca_key_win32.h/.c. Native key/provider, thread, borrower, retained token/local allocations, close attempts and first failure remain in that owner. Existing borrow/return preserve a single borrower and factual owner; issuer CNG uses those operations for a protected signing lease, after root/identity/status gates. No existing PAPACC_CA_KEY instance is supplied to the personal CLI. The CNG signing lease is not an initial CA ceremony and requires existing root proofs; it cannot be substituted for bootstrap or CA_INITIALIZE.

F0–F9 F7 and issuer clause6 require a distinct explicit CA_INITIALIZE action, factual owner identity/generation/service/operator binding and correspondence. Current personal UI explicitly does not request CA_INITIALIZE. Generating/opening a guessed key, calling generate_key, signing or fabricating a reference would not complete this binding. All are uncalled. The current absence is reported as absence in this invocation, not a global assertion that no CA key exists on the host.

## Independent integration delivered

`papacc_configuration_personal_custody_data` synchronously projects the existing pending target and optional existing CA owner into four unsigned fields, returned by value. No new owner, stored pointer, handle, allocator, callback, pin, resource manager or borrow is introduced. Caller must serialize on the canonical writer and protect any CA owner for the call. Nothing escapes. A foreign canonical writer yields UNAVAILABLE. A foreign CA owner thread yields UNAVAILABLE without further inspection.

ABSENT means no observation/reference in these owners, never filesystem/key-store absence or permission to initialize. METADATA means existing metadata only, never native custody or approval. RETAINED covers active/unknown cleanup/dependencies and remains visible after Stop. Existing metadata can never change `effect_denied=1`. No function opens, validates, exports, signs, frees or retries a CA object. The existing CLI reports this projection on its completed interactive path, passing NULL for the genuinely absent CA reference. Redirected/no-console entry still denies before acquisition or projection.

This is a preparatory diagnostic connection, NOT an actual CA borrow or an exclusive-target bridge. It must not be described as completed custody integration. Operator/console/plan/consumption/native revalidation gates remain unchanged. Accepted contracts and historical native creation/CAS paths are unchanged.

## Validation and effects

Exact six-file patch, three-file naming correction and one-file negative-test extension were separately approved by official executor review and applied with pre/postimage verification. The first build exposed a constant/type naming collision; historical failed logs are retained. Final Debug/Release builds have zero warnings and ten suites PASS each. The 30 negative CLI regression cases PASS.

The extended model test covers absent target/reference, modeled created/existing-target metadata, UNKNOWN preserved after Stop, absent CA, retained CA, READY metadata without a key, an existing borrower and wrong-thread owners. It asserts byte-exact preservation of modeled CA storage and deny-only results. Canonical custody_begin is invoked only on states denied before native calls: absent personal proof, existing created metadata and stopped/UNKNOWN. It does not start a worker or obtain parent/container handles and preserves created/UNKNOWN. Existing DATA replay/double-consume/retire/Stop regressions also pass. These modeled states do not claim actual native identity, key or authorization.

Read-only native preflight again denies the current context; positive operator/console ceremony remains NOT_QUALIFIED. No namespace component diagnostic, real exclusive creation, CA initialization, key opening, certificate, CURRENT or network was executed. Existing LABs and WIP remain preserved.

## ED11

Fixed census unchanged: root175,008; pending43,296; known managed1,799,496 bytes in Legacy. No 3MiB selection or quota change. Query result16 bytes is caller-local, not durable owner growth. No new handle/heap/borrow/retention pool. Source inspection and object disassembly show only GetCurrentThreadId plus Debug runtime/security helpers. Query own frame excludes return address: Debug120 bytes (104 local+16 pushes), Release40 bytes (32 local+8 push). .text$mn sections730/362 bytes respectively are object code evidence, not total image or process memory. CLI adds its local16-byte report plus existing formatted-output path; complete nested/runtime/GetCurrentThreadId and console/CRT stack costs remain UNKNOWN. Actual future target/CA native/provider/kernel/coexistence resources remain UNKNOWN, not zero. No operative capacity bound is established. See ed11.json and disassemblies.

## Result

TARGET_EXCLUSIVE_BINDING=DATA_OBSERVATION_ONLY_NATIVE_HANDOFF_PENDING
CA_CUSTODY_BINDING=EXISTING_OWNER_QUERY_ONLY_FACTUAL_REFERENCE_ABSENT
PERSONAL_CEREMONY_INTEGRATION=DENY_ONLY_PROJECTION_NATIVE_GATES_PRESERVED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=MSVC_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS
OFFLINE_TESTS=10_SUITES_EACH_PASS
NATIVE_TESTS=30_NEGATIVE_CLI_PASS_READ_ONLY_PREFLIGHT_DENIED
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
FIRST_UNRESOLVED_DIVERGENCE=PERSONAL_PROTECTED_PARENT_TO_WORKER_HANDOFF_ABSENT
NEXT=PERSONAL_FIRST_INSTALL_NATIVE_CEREMONY_QUALIFICATION_ON_ELIGIBLE_HOST

Positive execution still requires a deliberately eligible host/console. CA_INITIALIZE and full lifecycle/resource qualification remain separate gates; NEXT authorizes no persistent effect by itself. No accepted ADR, production0.6.4 or non-Accelerator project changed. Baseline1112 files were hashed; existing changes outside the reviewed six-file scope remained byte-exact before this live-document update.
