# Windows administrative qualification handoff

Status: procedure prepared, NOT EXECUTED. This document does not authorize elevation, bootstrap, CURRENT or network execution. Use only a separately approved local Windows administrative session, initiated by the designated human through normal Windows controls. Do not change ACLs, groups, privileges or borrow another process token to force a result.

## 1. Freeze the target

Use C:\Projetos\PapinhoAccelerator, its active ref and WIP, not a clone. Verify HEAD and the SHA-256 manifests of the preceding bootstrap implementation. Preserve original evidence. Record exact executable/configuration hashes and normal PST SDK references. Read the project's target build instructions before any new build. Keep all admission gates closed. No keys, real certificates or network are needed for this qualification.

## 2. Read-only environment preflight

From the exact session intended to launch the qualification process:

```powershell
whoami /user
whoami /groups
whoami /priv
[Security.Principal.WindowsPrincipal]::new([Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
```

Require the designated identity, enabled Administrators membership (not deny-only), and factual elevated context. The commands above alone do not report TokenElevation/TokenElevationType or prove the qualification process has the identical token. The native qualification entry must query its OWN effective context: reject impersonation as the current adapter requires; capture TokenUser, TokenType, TokenElevation, TokenElevationType, TokenIntegrityLevel, TokenGroups, TokenPrivileges and TokenStatistics from its own token using existing Windows token facilities. Record observed values and errors without secrets. No enabling privileges or using another process token. A missing query/unsupported target is INCOMPLETE, not inferred elevation. Enabled local authority must satisfy accepted ceremony P1; merely belonging historically to Administrators is insufficient.

## 3. Entry-point gate

The existing production binding is bootstrap_admin -> bootstrap_auth_native -> authority_valid -> initial resolver -> initial commit worker. Use that path, the same owner and retained packet. The existing papacc_bootstrap_d3_commit_test target uses a compile-isolated diagnostic bypass and synthetic candidate/model: its success is NOT positive native administrative qualification. There is currently no approved CLI bootstrap command or general provider to invoke instead. Before execution, identify a reviewed native qualification entry that calls the real path without the diagnostic macro. If no such executable entry is available, STOP and prepare an exact minimal test-harness patch for official review; do not invent a command or run the diagnostic target as proof. This is a qualification prerequisite, not a new architecture or authority.

## 4. Scoped positive operation

After that entry is reviewed and explicitly authorized, the identified administrator declares a new LOCAL installation, not restore/reset/adoption. Use a dedicated approved local test parent and a new container created exclusively by the existing custody worker. Do not treat an absent file or empty directory as proof. Preserve the same parent/container handles and their observed object identities. Existing/uncertain history or loss of custody denies the operation. Do not retry with a new name to bypass an uncertain attempt.

The real native producer must capture its SID/TokenId/AuthenticationId/ModifiedId into the existing scoped packet and bind the same owner invocation, prepared candidate and retained container. Record the factual prepared revision and candidate digest; these are not CURRENT or epochs. Supply only references accepted for initial resolution; no TEST_ONLY security authority, named provider or unqualified SELECTED interface. Verify MODEL_BOUND remains distinct from RESOLVED. The first resolver must work without synthetic CURRENT; the update resolver must still reject missing CURRENT.

Exercise on separate authorized disposable test operations: valid proof; absent/disabled/filtered authority; identity/token change; candidate mutation; scope reuse; container mismatch; Stop before/during work; and uncertain/close-failure retention. Do not manufacture a native positive outcome from mocked SID/flags. Where token variants cannot be safely provided, mark NOT_RUN and retain that qualification gap. Changes to token/group membership are outside this procedure. Token-local checks do not prove immediate external-directory revocation.

## 5. Result and Stop

Use the existing Stop path; do not wait for native IO before requesting Stop. Keep ownership, packet, handles and credits while the worker is pending/unknown. Inspect completion and close outcomes separately. Commit must use the retained object, flush/readback and factual transaction result. File presence or process exit does not prove CURRENT or DESTROYED. No automatic deletion, destructive retry, TerminateProcess or rollback assumption. If the reviewed qualification scope stops before commit, report commit NOT_RUN; do not expand execution implicitly.

Positive proof requires observed native role, identity, operation/candidate/container binding, protected lifetime, resolver outcome and factual disposition. Save original native results and all negative/NOT_RUN results using existing evidence/logging mechanisms. No secrets. CURRENT publication, A/P/I, operational admission, issuer/CNG and recovery remain denied irrespective of qualification success.

## ED11 and independent gates

No product storage is added by this handoff. Managed budget remains 2,093,415 / 2,097,152; margin 3,737 is not permission for buffers. Any qualification instrumentation needs an exact storage/lifetime census and official approval before implementation. Provider/kernel UNKNOWN.

Independent work completed in this task: 50 real candidate CLI invocations (25 each Debug/Release) passed without normal launch or sockets. Validate is usable only within its stated offline scope. Diff/show-effective positive paths require factual CURRENT, which is unavailable; their negative guards pass. Codec/root8/source/binding regression evidence and implementation hashes are preserved. No new issuer/CNG source work is opened; existing issuer carrier, native CNG disposition and authoritative recovery handoffs remain separate gates. No source patch or recompilation was necessary.
