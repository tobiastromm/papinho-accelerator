# UPDATE único manual autorizado — diagnóstico207

Windows PowerShell5.1 x64 como administrador original. Conferir plano/IDs/
pins/K-R/SCM/dois leaves e efeitos de retenção antes da confirmação.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-take207-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '6347380e008035a24ef8c82450ac5dd500b44799f4f66b806e7619ad5ce9f1d9' -ExpectedRequestSha256 '44b7df5e18c8d82b252a805414b3beaf6ab0a392941c76f6af01fc950671d3d5' -ExpectedDecisionSha256 'b59ec9781a3bafcb3dbae094493bbdfe0a5549b647dec1e61a822029243b008f'
```

Digitar confirmação somente no prompt nativo. Não fornecer respostas
automaticamente. Falha/cancelamento/UNKNOWN: preservar saída e objetos, sem retry,
rollback, limpeza ou Start. Enviar saída integral.
