# Consulta manual única autorizada

Executar em outro PowerShell5.1 elevado, mantendo os processos/janelas retidos. Sem retry.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-runtime-elevation-shape.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '2e0b15a3babc06c1d7f505a97ef1429b5f7470fca14411a3b449dd43c9351bd0' -ExpectedRequestSha256 'c9fe877d57b0d4d8fcb47cbe3e2518dfd0ef63f4036f9abb081298c9a921db4b'
```

Confirmação interativa: `OBSERVE_RUNTIME_ELEVATION_SHAPE_ONCE`. Enviar saída completa. Não admite token no produto nem autoriza Stop/kill/UPDATE/nova sessão.
