# Manual receipts — current gate only

No manual query or key generation has been executed by the executor. The fixed `.started.json` receipt is created exclusively BEFORE the new-name diagnostic query; the fixed `.result.json` is written once after it. Do not remove/reset receipts or retry after interruption. No credentials/private material may be recorded here.

The launcher is read-only and fail-closed; it cannot finalize or create a key. Check can be run without UAC:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\ca-custody-elevated-lab-uac.ps1" -Mode Check
```

Only if the owner chooses to execute the single new-name read-only collision observation manually, this command uses native UAC (not run by the executor):

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\ca-custody-elevated-lab-uac.ps1" -Mode Launch
```

Expected final gate exit2 is STOP regardless of F10. This is not the requested positive generation operation. Do not invoke the standalone harness effectful argument separately. See ../closeout.md for the unresolved supported-absence gate.
