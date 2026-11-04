# ONE authorized manual D1+F session

Hashes checked offline. Not executed. No retry. Check each live plan and answer the two independent confirmations only if correct. If D1 succeeds but F denies, preserve everything and do not rerun. Send full output for reconciliation.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\prepare-machine-fixture-ga-readback.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'beafa99737f32cd071997d6fd054e092e97e131cdd20c06c04b14b251938d932' -ExpectedOperationalRequestSha256 '71e1d1a3eb460e43ea1a1960c09587aaff408039d84a1b5b25ef2b881055ac5e' -ExpectedDecisionSha256 'f431b5e9c4c68371649d9ee4277d6b8f6b76568c7c8805dbb6f972e8c3b5e579'
```
