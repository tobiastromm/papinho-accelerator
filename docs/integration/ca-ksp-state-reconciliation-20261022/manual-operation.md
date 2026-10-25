# Prepared manual LAB operation — NOT AUTHORIZED TO EXECUTE

This task prepares the operation only. A later explicit owner/executor approval must cover the exact binary hash, new MACHINE key creation, public-only validation, borrow negative test and deletion of the same new successfully validated key. Do not run the effectful command below now. The prior read-only/F10 confirmation does not authorize it.

## Fixed identity

Provider: Microsoft Software Key Storage Provider; scope MACHINE; RSA2048; signing-only; export policy0; existing operator/SYSTEM DACL and LocalService exclusion unchanged. LocalService is LAB-only, not a product service identity.

New authority: `2345e07adf714045a20d1c8b6fdf5aaf`.
New generation: `a2bcd6d2ed3043d083e7e50342d6fab0`.
New name: `PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0`.
Expected actual operator: `S-1-5-21-785699550-1440458426-1682014700-1108`.
The old UNKNOWN name is reserved indefinitely pending factual reconciliation and must never be reused, overwritten, deleted or queried by this experiment.

## Pre-effect gates, after separate approval

1. Verify artifacts.json hashes from trusted source and keep binary/source immutable during use. Manual UAC must display a real elevated interactive console; no automatic executor elevation, passwords, services, tasks or fallback. Cancellation means DENY. Verify actual operator SID, full elevation, enabled Administrators and no impersonation; a different credentialed operator means STOP and a separately reviewed binding.
2. Execute the existing pinned read-only diagnostic ONLY for the new name, in that same eligible context, preserving its receipt. A successful open means collision/STOP. Any other error, uncertain cleanup or mismatch means STOP. Exact BAD_KEYSET at OpenKey with confirmed provider closure is only a provisional not-found observation, never absolute namespace absence. No-overwrite on CreatePersistedKey remains the decisive collision barrier. Do not requery the old name.
3. Preserve the old receipt and create a new exclusive diagnostic attempt receipt before launching the effectful binary. Record hashes/operator/context/new name/approval/preflight and mark the attempt consumed. An existing or uncertain receipt blocks reexecution. This is LAB evidence in this integration directory, not a product journal/authorization store. Do not erase/rearm it automatically. No invocation of the raw binary before that gate.
4. Confirm one attempt only and explicit approval for its cleanup. No certificate/signature/bootstrap/CURRENT/network/clock operation is included. The result window/transcript must remain available for manual capture, with no secrets. Save return code and stdout exactly; do not equate window/launcher success with key success.

Exact preflight child arguments, after the above approval:

```powershell
& 'C:\Projetos\PapinhoAccelerator\build\ca-finalize-readonly-diagnosis-20261022\readonly_diagnostic.exe' --observe-existing 2345e07adf714045a20d1c8b6fdf5aaf a2bcd6d2ed3043d083e7e50342d6fab0 PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0 S-1-5-21-785699550-1440458426-1682014700-1108 S-1-5-19
```

Candidate native UAC command for the single effectful child, only after the separately approved preflight and consumed attempt receipt (do not execute now):

```powershell
$p = Start-Process -FilePath 'C:\Projetos\PapinhoAccelerator\build\ca-ksp-next-lab-20261022\Release\next-lab.exe' -ArgumentList '--execute-new-lab' -Verb RunAs -WindowStyle Normal -Wait -PassThru
$p.ExitCode
```

This command alone is not the complete ceremony: a future reviewed capture wrapper or manual elevated session must preserve output and enforce the preflight/receipt/hash gates. No armed automatic launcher is delivered in this preparation. The --describe option is safe and was tested; it establishes neither key absence nor authorization.

## PASS / STOP and disposition

PASS requires successful creation/finalization/validation for exactly the new name, expected operator, correct public properties, protected borrow and return, confirmed deletion of that same new handle, confirmed provider close and exit0. Cleanup uses NCryptDeleteKey only on the newly created and validated owned key; success consumes that handle. It does not reopen/delete any existing or unconfirmed object. No private export probe is permitted.

Any failure/Stop/unknown state gives STOP. Keep the new name quarantined, record first failing call/status and current owner fields, preserve references while live and retain evidence. No retry, timeout release, automatic close/delete of an uncertain object, name change or presumed rollback. Process termination cannot prove persistent deletion. Interrupted evidence is UNKNOWN even if the window closed. Separate follow-up approval is required for any additional observation/remediation.

Successful new execution would qualify this particular elevated LAB path only. It would neither prove the unique cause of the old failure nor qualify product custody, signatures, UTC or ED11. Previous UNKNOWN debt remains in the resource census.
