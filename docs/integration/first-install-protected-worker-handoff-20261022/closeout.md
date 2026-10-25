# Protected first-install parent-to-worker handoff

Task PACC-PHASE6-FIRST-INSTALL-PROTECTED-WORKER-HANDOFF-1, 2026-10-22.
Branch main; HEAD 913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
CURRENT_TASK_COMPLETED=NO: protected read-only handoff implementation and negative/DATA regressions complete; positive native worker qualification remains unavailable on this host.

## What changed

The same pending_worker now dispatches a private read-only packet discriminator12 before its historical initialization can overwrite the retained source handle. This discriminator is internal metadata, not a persisted root format, protocol or wire change. Historical9/10/11 dispatch and the child-creation gate remain unchanged. No second worker, root, queue, registry, journal, pin table or resource owner is introduced.

The existing personal native context gains104 bytes: one handoff state, expected attempt32 and frozen relative leaf65 with alignment. The leaf is captured BEFORE plan presentation. Existing context already holds the observed process-token identity/statistics/SID/session, native parent identity/security descriptor and source identity/handle. Existing DATA plan remains an owned copy. All belong to the same pending packet, never a path-only descriptor or a copied handle owner. No child identity exists or is fabricated.

Owner begin requires same writer, confirmed unconsumed DATA, exact attempt, same frozen leaf, existing parent/token/source handles, live context, no Stop/unresolved/pending/conflicting effect and inclusive reservation. It revalidates token/parent/security descriptor and source bytes. It consumes once immediately before submitting this read-only operation. Thread-creation failure preserves consumption and sets terminal denial, never rearms it. No caller pointer escapes. Begin does not clear pending storage, reopen the parent or duplicate handles.

The same worker receives only the packet, never the root pointer. It checks Stop, expected attempt and frozen leaf; revalidates the same token/parent/security descriptor and held source bytes; checks Stop again and records VERIFIED/DENIED/RETAINED. It never reads owner-only DATA phase/consumed, which may be invalidated by Stop; plan bytes, attempt and native inputs stay frozen until quiescence. VERIFIED means read-only revalidation only, not authorization, CAS, child creation, custody or CURRENT.

Poll is nonblocking and writer-only. WAIT_TIMEOUT preserves all references. Only thread signal proves quiescence; successful CloseHandle is separately required before clearing active/thread. Close failure or uncertain native cleanup retains state and references, preventing close/retire/replay. A retained handoff is not destructively retried. Native current/source wrappers refuse owner-side revalidation during worker access. Native close checks active/thread before inspecting worker-mutated context, avoiding concurrent resource access.

CLI now submits this read-only handoff instead of locally consuming alone. It polls the same worker, observes Stop at bounded loop intervals, requests Stop atomically and retains the graph if still pending. Final cleanup cannot retire the plan or free parent/token/source while active/thread/retained state exists. Successful read-only handoff still ends with BOOTSTRAP=DENY and CURRENT_PUBLICATION=DENY. No FILE_CREATE, CA/key operation, certificate, commit or publication was added to this path.

## Target and CA boundary

The worker now has access to the original protected parent and frozen relative name without reopening a path; that is the prepared input boundary for a future relative exclusive creation. This task does NOT connect packet12 to historical FILE_CREATE, clear the native effect gate or call custody_begin. Actual child creation, effect-time admissibility, qualified host resources and further custody remain separately denied. The old bootstrap path cannot be used as a shortcut around this retained graph.

No factual PAPACC_CA_KEY reference is available in the current invocation. The existing metadata query still receives NULL; no key was looked up, opened, generated, borrowed or copied. Minimal remaining CA operation: distinct explicit CA_INITIALIZE plus factual identity/generation/operator/service binding and protected reference through the existing CA owner, under its own accepted gates and authorization. NEW_INSTALL/read-only verification does not supply that action or reference.

## Review and validation

All changes were submitted as exact staged hunks and officially approved before application: operations, tests, leaf, capacity and warnings manifests. Preimages/postimages and merged final hashes were verified. A test-only strcpy warning was caught by /WX and corrected using bounded literal memcpy; failed logs are preserved rather than omitted.

After clean rebuild of the owner layout, final MSVC x64 Debug/Release pass ten suites each with zero warnings. DATA tests cover absent/unconfirmed parent context; divergent attempt invalidation; divergent/coherent modeled leaf without invented parent; one-shot replay; Stop; retained handoff blocking poll/close/retire; existing DATA consumption/replay/quiescence guards; UNKNOWN metadata and absent CA. No synthetic handle or positive native authority is used. Fixture-only metadata reset is explicitly separate from product lifetime.

Thirty negative CLI cases pass plus two explicit no-console route assertions. Read-only native preflight still reports NATIVE_READ_ONLY_PREFLIGHT_DENIED_NO_ELIGIBLE_CONTEXT_OR_PARENT; POSITIVE_NOT_QUALIFIED in both builds. Existing factual host probe recorded limited token, disabled administrator membership and no console handles. No automatic elevation, pseudo-console, eligible-token simulation or positive worker submission was attempted. Actual valid/divergent-parent revalidation in the worker, real thread-close failure and full native retention path remain NOT_QUALIFIED. Models are not evidence of native custody.

## ED11

Census in both final builds: root175,112; pending43,400; known managed1,799,600 bytes. Fixed owner increment104 bytes, no quota change or3MiB selection. SAME serialized worker count1, reservation64KiB. That stack is already in PAPACC_C8_KNOWN_MANAGED_BYTES; it is charged once, not added again. Inclusive caller reservation includes the full fixed root and additional live CLI document/diagnostic/confirmation resources. Begin checks inclusive limit and minimum owner/stack lower bound; this check is not an operational capacity proof.

At most one new active/retained thread handle on this serialized path. Existing parent/token/source/security descriptors remain in their original owner, no duplication or additional CA borrow. Stop may retain entire root, plan and references while worker pending/cleanup uncertain. User/kernel thread internals, descriptor sizes, full nested SDK/CRT/console/RNG/provider costs, actual CA/effect coexistence and host capacity remain UNKNOWN. A64KiB reservation does not prove nested stack use fits. ED11_TOTAL_STATUS=PARTIAL; no operational PASS or nominal-margin authority. See ed11.json.

## Result

PROTECTED_PARENT_HANDOFF=IMPLEMENTED_READ_ONLY_POSITIVE_NOT_QUALIFIED
WORKER_OWNER_BINDING=SAME_FROZEN_PENDING_PACKET_NO_REOPEN
TARGET_EXCLUSIVE_CREATION=NOT_RUN
CA_CUSTODY_REFERENCE=ABSENT_DENY
ED11_TOTAL_STATUS=PARTIAL
BUILDS=MSVC_DEBUG_RELEASE_PASS
WARNINGS=0_FINAL_BUILDS
NATIVE_TESTS=NEGATIVE_AND_DATA_PASS_POSITIVE_NOT_QUALIFIED
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
FIRST_UNRESOLVED_DIVERGENCE=POSITIVE_NATIVE_HANDOFF_QUALIFICATION_REQUIRES_ELIGIBLE_OPERATOR_AND_REAL_CONSOLE
NEXT=PERSONAL_FIRST_INSTALL_NATIVE_CEREMONY_QUALIFICATION_ON_ELIGIBLE_HOST

Accepted F0–F9/P1/issuer/ED11 contracts remain unchanged; original WIP and four LAB directories preserved. No commit/push/tag/release, clone/worktree or production effect. Native qualification and CA/capacity gates must be satisfied before any later persistent effect.
