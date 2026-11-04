# Sessão única manual autorizada

Windows PowerShell5.1 x64 como administrador original, console interativo.
O driver verifica hashes e exige confirmações documentais e E/Q/S separadas.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-retained-cause-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'e0a1f49ddb337e86dcbac64e2f0bf091d982dd2f1cb22265d4c29daab44b8737' -ExpectedOperationalRequestSha256 '8ee498d23e5c306b746f1e32e246d37a0ee4e50df5081842a15da2274200ae54' -ExpectedDecisionSha256 'ff573574ea963eaa300f4d3aa4b4699ab76db416753118945f5a1742ea2c3ef4'
```

Conferir planos/IDs/hashes e digitar P1 DATA somente no prompt próprio.
Não fornecer respostas automáticas. Falha/cancelamento/UNKNOWN: não repetir,
preservar saída/objetos. Se ficar retido, não fechar console/kill/Ctrl+C por
esta aprovação. Enviar saída completa.
