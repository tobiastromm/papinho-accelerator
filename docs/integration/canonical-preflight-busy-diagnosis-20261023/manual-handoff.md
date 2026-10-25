# Future manual read-only capture

Requires NEW SPECIFIC OWNER AUTHORIZATION. This task did not execute Check, Prepare, preflight, Start or any launcher entry. Helper/script/policy/candidate pins are in pins.json. Only helper pin and receipt root changed; service candidate preserved.

Prepared command, NOT authorized to run by this task:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1" -Mode Prepare
```

Do not use Launch. No EXACT-OPERATION-APPROVED.json created. Preserve IG_DIAGNOSTIC, IG_CENSUS_REJECT, IG_FINAL and final receipt. Failure/UNKNOWN retains STOP; no automatic retry. Preflight PASS does not authorize rebind/Start/Stop.
