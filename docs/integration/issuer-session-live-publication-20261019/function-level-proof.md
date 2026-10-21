# Exact reconciled operation — not yet applied

Authority: accepted issuer/C2 I/R/A/P relation P1–P4, P6.4/P7; C2 current epoch and negotiated grants; ADR0012; Model E/ED08/ED11. The historical 18 foundation hunks are not accepted or replayed. This operation is reconciled against source after the pending persistence adapter and stack deduplication.

## Function-level proof

- Native snapshot/pin_read: exact owner/ticket/index/serial/readers; conditional I presence; A and P separate. Reject alias of output/input/owner. No new native query or callbacks. Old bytes are data, never CURRENT.
- root_operational_reference_current: require actual selected root model, current source/leases, C2 P exactly equal captured P, full pure semantic operational projection, no Stop/unresolved/authority_lost/active worker. No ordinal/less-than generation fallback. Generic root CURRENT also requires actual selected R/index/A and no commit pending.
- New Session adoption: unchanged real negotiated RESULT/grants/Principal checks; acquire root CURRENT pin before exposing Session; capture its R/I/A/P and actual incarnation. No historical pin manufactures new authority. Failed acquisition denies, failed release retains Session/pin. No policy/grant expansion.
- Existing Session check: exact owned Session address/incarnation and its own admitted reference; actual CURRENT operational P/complete projection/source/leases checked separately. Original authenticated control/negotiation/capability/current-policy checks remain. A former R can protect memory while P remains genuinely current; no stale P accepted.
- Site template/listener: exact Session's reference or existing protected listener reference plus actual CURRENT operational projection. Original strict P, Site TLS profile, trust, revision/time, mTLS/Principal/grants and admission checks remain. Profile/Principal/policy changes deny this issuer-only path.
- Issuer prepare: same root/owner, durable participants/source, bounded reservation, actual CURRENT/P, full operational equality and image provenance. No caller epoch. CAS expected R/high-water guards remain. Flags bind one staged operation to R/P.
- Issuer publish: begin existing worker once, poll existing worker on later owner calls, including Stop completion. No synchronous commit or additional worker. Only factual CAS_SUCCESS plus post-completion revalidation can call root selection/A emitter. Whole selection/model acknowledgement protected by existing lock with no callbacks. A generated after commit; P preserved from factual current operational slot; no grants rewritten. Conflict/UNKNOWN never fall back. Terminal failures cannot restart commit; publication attempt is latched to prevent blind retry after postcommit failure.
- Reaper: every original guard precedes unpin, and exact private helper independently repeats absence of native Session/Connection, operations (even EMPTY refs), writes/denials, channels, processors and matching candidate/PST/security/adapter dependencies. Typed A/P, original R/domain/incarnation/ticket checked. No CURRENT requirement for destruction-only unpin. Failure returns before clearing manager/Session/pin. Exactly-once metadata clearing only after successful factual unpin. No native close, credit refund or alternate cleanup.

No checkpoint/fencing/high-water recovery source is fabricated. Normal PUBLISHED/REVOKED issuer record activation guard remains unchanged, as do signing proof bits and active-certificate-revocation gate. No Browser/PST/wire/network/CA/key change. Administrative metadata publication is not real credential issuance qualification.

## Proposed regression evidence

New normal-library application-root cases use real native root, actual initial P, TxF and existing runtime/wait-set attachment with no listener/Connection. REVOKED Session metadata is explicitly synthetic and nonauthorizing; not native authentication. Pin acquisition/selection and reaper execute actual product functions. Cover old/new revision references, immutable I snapshot, separate A/P, identical/divergent projection, stale R, active worker denial, duplicate begin, native file conflict and native pre-CAS access failure UNKNOWN, Stop before publication, three pinned slots, each borrower class blocking reaper, invalid pin, duplicate release and retained root. Existing pending adapter/core/storage/catalog tests retain native versus injected-fault distinction. Debug/Release fresh outputs + compiled censo are required after approval. No existing PASS is reused for new source.

ED11: per-slot tags and existing Session pin descriptors grow within same root, no new slots or worker. Stack and embedded packet charged once by existing reservation; quotas unchanged. Exact compiled census required. Provider/kernel UNKNOWN; all admission remains DENY.

## Exact files

- `apps/server/server_admission_preflight_win32.inc` before `675f92281ae8a3a2344c28d67376f3b48498dfc5e09437cb97cf6a2e1d16f3fb` after `3e51b06e3970932d943e2466a838cb2f855e5b6f0d8a2a6f9ae03ace40856ec1`.
- `apps/server/server_c2_owner_win32.c` before `30e0b35eb108b426194a94b115b701b7f16068f68c22e6bd0da535e151b77c7f` after `3ec15eaf6997e764e4b754dcade66ec2b67814927f061322adf9ebbdef406c1c`.
- `apps/server/server_c2_owner_win32.h` before `2aa684c2780e72325cffe7b7e35f1f5502a42f25680b380b544716f9333817fa` after `2b1271bbaef840d5859ed3b5c8acd1f57011e476cea90548ac53d7456c9a8e9e`.
- `apps/server/server_c2_root_binding_win32.inc` before `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96` after `813b0e0a6d2429fe27b3971806479849e5c2c5d45cc5f98d8f13948bae5e44bb`.
- `apps/server/server_configuration_commit_win32.h` before `51190785721e758bf0b152756e9d853328c73aa6b52d12f15e6601028db04068` after `d5e7f97e7fc2662386259fcef215f1d05a46292db374fa01ddadcccc5d95d93f`.
- `apps/server/server_configuration_issuer06_win32.inc` before `1dcf71c46f0f96167ed914a43dd495bc31be6c510a6c22ab4d7d1d94a88427fc` after `2fca216c08aa9638826620665f23175e4bc6d920e282d73bb32b8d12b629a395`.
- `apps/server/server_configuration_root_transaction_win32.inc` before `9566553905ddcc75be9eae53d94108c57d197a80f17ca04454317498b002c04f` after `44a5f11c174deac5e21daac929328a54b44f83d4438e9851bea437d42b0a65b9`.
- `apps/server/server_launcher_owner_win32.c` before `6647936749308a8d1002a55d761c972e3bbd487c9c6856926c2883bd39b4ef52` after `5e7f2b97fb0f6fb0035de68f4adbe70961bef1e536793d165cab4a34f11722bc`.
- `apps/server/server_launcher_owner_win32.h` before `fa5b1904176deded02d1d3673f81e9d600923ee30b6c3a923cb880745eb279ca` after `9af6e3cd12906c596cf5197fbfe58ff535ca0a5c614ed7680f33337e153b554a`.
- `apps/server/server_listener_composition_win32.inc` before `144294ca7481797dacbf181bcefdf86d5a717cccae3dd4ceadaf71869c43be4d` after `690dd3f8d732a3777c5b93d0ec479040d810b42370d095c994f3af7fb6e3eb18`.
- `CMakeLists.txt` before `5a2a85f08e7108e96757bbd9f34a42adfe895d451b126e3279d708225832960e` after `b3e8a2022084f17a42a9fb5d760093b3c71d34cb0cb3542bef0e950f57e51087`.
- `tests/canonical_root_transaction_test.c` before `8bc3b61d9704a4940d4db65ee2738ce12e7099ba885aa912d68be22bef1e3980` after `fbf6aa6dcd127b21fab1591aea957934cbd08db9981c38cb52b3132de4b2ec8c`.
