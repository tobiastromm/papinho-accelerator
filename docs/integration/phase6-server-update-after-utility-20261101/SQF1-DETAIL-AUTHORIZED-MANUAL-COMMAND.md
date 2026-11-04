# Sessão manual única — pendente

Revisar o pedido completo e cada plano antes de confirmar. Duas cópias DATA não são E/Q/S. Sem execução automática/retry/kill.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-sqf1-detail-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'cc0bded057b1f10125f091b24e71f92a12241a37ba740c041ccb309d4dee1950' -ExpectedOperationalRequestSha256 'd1dee4fa8bdc00768c27c9448f6da99955b9b616c1e1523f0f778b65c2ab5e54' -ExpectedDecisionSha256 '82957fe0833f4e1f8b7496e9b925100a6dc1c63986ca20dccaeb67c40219eabb'
```

Se retiver após falha, enviar saída e manter UNKNOWN, sem fechar console ou repetir.
