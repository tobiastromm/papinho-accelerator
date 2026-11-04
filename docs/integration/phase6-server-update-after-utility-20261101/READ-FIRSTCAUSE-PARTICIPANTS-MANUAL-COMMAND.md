# Uma consulta manual read-only

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-read-firstcause-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '30f59e14837b9b1067771659d82f4e7b626319b26ccb97b87b9f5eb9b4ba6adf' -ExpectedRequestSha256 '95e8a2df49bf1b49f2adb1e0ea77bf2dfae8655f0c3e50e6035956cbbe1116af'
```

Confirmação exata: `OBSERVE_READFIRSTCAUSE_CURRENT_PARTICIPANTS_ONCE`. Sem retry; não encerra processos.
