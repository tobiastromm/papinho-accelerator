# Consulta V2 manual única autorizada

Executar em outro PowerShell elevado; manter janelas/processos retidos. Não executar a versão anterior ou repetir em falha.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-runtime-elevation-shape-v2.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '451377ab83a5460ea8c57f4c39f5ed2aaee5976f4bed1143b68e14a287447b6b' -ExpectedRequestSha256 '1b7f61c15019748cdab5b1e021ca39d2faa5c03e2d1a33b065cff55f57ed9fd7'
```

Confirmação: `OBSERVE_RUNTIME_ELEVATION_SHAPE_V2_ONCE`. Enviar saída integral. Nenhuma permissão de Stop/kill/UPDATE/nova sessão ou alteração de guard.
