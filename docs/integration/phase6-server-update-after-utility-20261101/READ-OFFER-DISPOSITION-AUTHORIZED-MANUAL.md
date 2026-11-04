# One specific G5 authorized MANUAL disposition

Pins verified. Not executed. Validate both fixed original objects and plan. Runtime first/controller second, at most two calls, no retries. Cancel on mismatch. Send full output including other session window; exits are not protocol close or READ success.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\dispose-read-offer-retained-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '9b055fe09dc6f48587376c1a457f4c12e4c2560328c77f8c2211499f432bc528' -ExpectedRequestSha256 'f11d20c0dcdc74d1a6a06aa23e8eb026d2a81f266ce401cc692e782e68a7a2a0' -ExpectedDecisionSha256 '84caa1a36a9473a7d407e48285683a311269bdb84f36fdd61b76d5780a223b5b'
```
