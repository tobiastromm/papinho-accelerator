# Sessão manual P1 READ — autorizada, pendente

Uma sessão, sem retry/respostas automáticas. Conferir plano e identidades antes de cada confirmação. Duas cópias DATA não são E/Q/S.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-p1-read-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '7c67c8aca915d0149e087b81383f1474ebcc6a7d085dfd794c4f7353298a97b6' -ExpectedOperationalRequestSha256 'd8d62c7444ddef14c85ed138ec5fb2218a954415129ea9cf95caba6b5a9e9153' -ExpectedDecisionSha256 'aee3282767c34c072f93c3cb40566a079859242fe7bd7ca07bf30335aea5ae59'
```

Enviar saída integral. Se falhar ou retiver, não repetir/fechar console/terminar processos. UNKNOWN histórico permanece.
