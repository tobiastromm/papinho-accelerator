# Consulta manual autorizada — pendente

Uma chamada, sem retry ou resposta automática.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-sqf1-stage10-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '5c5d3e53c6f252cf45e76ad0baf31673ab3cafb60a674fdba38aa59074056dd8' -ExpectedRequestSha256 '5ed67e7c2b5c98005d8a451b00e67b19165687910cc14310bc5abe3d4b4f5add'
```

Confirmar OBSERVE_SQF1STAGE10_CURRENT_PARTICIPANTS_ONCE. Fornecer saída integral.
Não concede término/UPDATE/Start. Divergência nega sem fallback.
