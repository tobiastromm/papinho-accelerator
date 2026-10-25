# Manual read-only qualification and future native LAB lifecycle

The executor observed OPERATOR_ELIGIBILITY_OR_CAPTURE denial, exit2 and factual closure. It did not elevate or spoof an operator. The new LAB directory is a build artifact with actual file-system identity to be observed; it is NOT a productive installation/root. Existing .conf denied settings fixture is copied byte-exact. No account, service, ACL or CA created.

Run only in an existing eligible administrative console; no automatic elevation, passwords or native mutation are in this script:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\scm-protected-binding-readonly.ps1'
```

This changes execution policy only for that child process. Windows handles elevation outside this script. Qualification script requires its existing modern PowerShell/Get-FileHash host; it does not prove Windows7 compatibility. It pins the fixed executable and denied LAB configuration and rejects extra arguments. No arbitrary command/credentials are accepted.

Executable: C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server.exe
SHA256: 3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb
Configuration SHA256: a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44
Script SHA256: da6464af8f022bd2e9667a7032cb288dea82f31232b1c9e0ddcb3872a8d884fd

Expected eligible result: SCM_LAB_PREPARE=BOUND_READ_ONLY, AUTHORIZATION=NOT_ACQUIRED, CREATE_SERVICE=DENY, RESOURCES=CLOSED, SCM_READONLY_BINDING_EXIT=0. Any2 is denial/uncertainty, not permission to relax eligibility/ACLs. Do not retry after unknown cleanup. A different elevated identity is captured factually in a NEW attempt; no previous identity or confirmation is assumed equivalent. No confirmation from read-only F10/NEW_INSTALL is reused.

## Future lifecycle, not authorized/executed here

1. Approve the exact isolated service operation, binary/config/installation/actor and one-shot effectful authorization; retain all native resources through the effect. Read-only completion is not that authorization. Same canonical service name and virtual account; no parallel LAB manager.
2. INSTALL: existing-object/collision denies; Create without fallback and NULL password, SID config/readback, exact disposition; partial/UNKNOWN retains, no auto delete/retry.
3. START: only after actual token, ACL/private CA denial, configuration/runtime/admission and resource requirements are qualified. Current gates must deny; a guarded denial is NOT positive lifecycle PASS. No listener or authority enabled just to get RUNNING.
4. STATUS: distinguish config observation, live token/readiness, pending/error and factual PID/binding.
5. STOP: handler signals; existing runtime teardown emits pending/terminal receipts; wait for factual quiescence. Timeout is not completion, no kill-as-cleanup proof.
6. REMOVE: after exact stopped/quiescent authorized binding; deletion request and confirmed absence are separate. Preserve CA/root/config/data/history. Unknown retains and stops sequence.

Real SCM mutations still require specific official approval and isolated host conditions. No runnable positive mutation launcher was enabled. No service account is created or password requested. OLD LAB keys (including UNKNOWN) remain untouched.
