# Bootstrap D3 commit: implementation and remaining gate

The existing pending worker now commits initial D3 through a relative NtCreateFile under its retained namespace container handle and the existing TxF engine. It validates held object identities before and after write, flushes and reads back the frozen candidate through the same file handle. No pathname reopen is used for commit. GetFinalPathNameByHandle records observed metadata only. A known successful commit populates committed bytes/path as COMMITTED_PENDING with authority_lost; it does not select CURRENT, publish an epoch, or enable admission.

The existing packet owns parent/container, token, transaction and file handles. Stop is atomic and does not wait for the native worker; polling is nonblocking. A pending or uncertain result retains handles, buffers and credits. Close failure is not retried destructively. The caller must retain the owner until factual worker quiescence and close. Tests do not interpret process exit as DESTROYED.

The initial resolver requires a scoped native proof rather than a readiness boolean. Its native producer checks enabled administrative membership, rejects impersonation, captures actual SID and TokenStatistics (TokenId, AuthenticationId, ModifiedId), and binds scope, root, container identity and candidate digest. The worker revalidates before effects and before final commit. Stop invalidates the operation. This is token-local validation, not proof of external account-directory revocation. Existing update resolver remains unchanged. Actual initial resolution still requires canonical prepared security/policy references; no fixtures supply operational authority.

The root wrapper checks exact prepared BODY/listener correspondence and initial envelope identity, denies issuer staged/snapshot content, enforces reservation and separates INITIAL_BOOTSTRAP from update. Existing p.observed scratch is reused only after custody worker completion and before commit worker exposure. No pinned or retained snapshot is reused. All guards for absent CURRENT on update, unresolved provider and SELECTED topology remain closed.

## Validation and limits

Debug/Release builds pass with /WX and zero compiler warnings. Final image: 14 diagnostic commit invocations pass (success, Stop rollback, injected UNKNOWN, conflict, object identity mismatch, missing authority, Stop-revoked operation), plus 18 selected CTest regressions. Native success verifies bytes, file identity and actual committed path; Stop rollback verifies absence. Native Win32/TxF IO is real; root candidate/security model and administrative bypass are isolated test fixtures. The diagnostic macro is private to test targets and cannot create native administrative proof. A Boolean admin flag alone is rejected.

Earlier same-task evidence includes 16 namespace cases, 24 historical ROOT06/D3 invocations, 50 offline CLI cases and two layout runs. These preceded the final phase-10 metadata supplement and are identified separately; final metadata does not change historical phase-6/8 handling. No network or operational bootstrap ran.

Official read-only executor identity inspection: tromm\tobias; SID S-1-5-21-785699550-1440458426-1682014700-1108; Administrators S-1-5-32-544 deny-only; medium integrity S-1-16-8192; IsInRole(Administrator)=False. No elevation, ACL or token mutation was attempted. Consequently positive real administrative authorization and end-to-end initial resolver qualification remain unavailable; bootstrap remains DENY. No new normative decision or fictional provider is substituted for that factual prerequisite.

## ED11

Managed conservative composition: 2,093,415 / 2,097,152 bytes; remaining 3,737. The extra retained token handle adds eight bytes to prior fixed layout. Root layout is 182,768 bytes; component owner 105,736; pending packet 51,120. Existing caller reservation includes input/AST/output, codec pair, owner stack, receipt and 4,096 bytes native scratch; private begin additionally checks 10,532 bytes. Native proof and observed path reuse existing owned packet scratch. No quota reduction or unpriced allocation is used. Provider/kernel, native peak and operational headroom/freshness remain unqualified. ED11 total is PARTIAL.

## Implementable handoff

Use a separately authorized factual administrative execution context with enabled role and canonical approved references to qualify native proof capture, token invalidation and full initial resolver positive path. Do not elevate this run to force PASS. Keep CURRENT, epochs, admission, named provider, SELECTED topology, carrier issuer and authoritative recovery closed until their own gates. Existing operational update, Sessions/pins and production 0.6.4 are preserved. CNG remains unresolved; no real CA or certificate was created.
