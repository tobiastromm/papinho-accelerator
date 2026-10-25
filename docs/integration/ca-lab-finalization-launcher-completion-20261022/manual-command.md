# Manual owner execution — single independent LAB attempt

The executor has NOT run Launch or key generation. Only the owner runs this command after checking the scope and pins. Use a real interactive Windows console. Native UAC handles elevation/credentials; the script never requests a password. No persistent execution-policy/UAC setting changes.

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\ca-custody-elevated-lab-uac.ps1" -Mode Launch
```

The elevated window displays the actual expected Tobias SID, exact new LAB name, MACHINE/provider, binary hash and effects. Type the ENTIRE newly displayed `LAB_CA_CREATE_FINALIZE <attempt>` line, then Enter. Empty Enter is denial; Esc cancels before consumption. Prior read-only confirmations do not apply. The attempt is consumed once immediately before submitting the child. F10 closes the result window after the result is saved. Do not close the window early, retry or delete receipts.

Effects: one persistent LAB-only RSA2048 key under the exact accepted new name, required properties/Finalize/public validation, then handle closure. **The key is NOT deleted automatically, including on success.** No private export, signing, certificate, root, bootstrap, CURRENT, network or clock change. Any collision/native error/uncertainty stops; previous key stays UNKNOWN. A successful LAB result is not productive custody/ED11/authority qualification.

Receipts use the existing fixed paths under docs/integration/ca-custody-elevated-lab-finalization-20261022/manual-results. A started/result file from any earlier run blocks this attempt. Never remove/reset one to rerun. Missing final receipt after interruption is UNKNOWN, not rollback. Preserve stdout/screenshot and receipt for reconciliation; no further creation is authorized on failure.

Optional safe check (no KSP query or creation):

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\ca-custody-elevated-lab-uac.ps1" -Mode Check
```

Exact launcher and executable SHA-256 pins are in artifacts.json; Check verifies the binary and the elevated child verifies the script pin handed off by the parent. The old raw binary/command from earlier preparatory reports must not be used; only the updated pinned launcher represents this one approved LAB scope.
