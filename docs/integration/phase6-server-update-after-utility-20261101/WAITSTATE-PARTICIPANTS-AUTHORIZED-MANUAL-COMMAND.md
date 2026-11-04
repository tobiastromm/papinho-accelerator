# Consulta manual read-only autorizada — pendente

Uma chamada, sem retry; confirmação digitada pelo proprietário.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-waitstate-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'f89b5d9b223169821252f0fffd555571297516d263e2040d5419864832a30083' -ExpectedRequestSha256 'e6e06c85e50c4b1f0cf53422022b5de5f73cf192e7cb67c232ab53e205ec4bb2'
```

Confirmar OBSERVE_WAITSTATE_CURRENT_PARTICIPANTS_ONCE. Fornecer saída integral.
Não executar UPDATE/Stop/kill/nova sessão. Divergência implica DENY sem fallback.
