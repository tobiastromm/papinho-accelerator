# Comando manual — sessão única aprovada

Windows PowerShell 5.1 x64 elevado, operador original, console interativo.
Não executado pelo agente. O driver verifica pins antes dos efeitos.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-g3-p1-3gib-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'f8c7892f1aaf797cc0c23b4143569da3bb3fdb059d076a44874cb4ed68694f1c' -ExpectedOperationalRequestSha256 '2a34a063563cd9fdf12ec5742b6c6d45ede377fa4501b104779c33205e31cfc0' -ExpectedDecisionSha256 '59f2e58d663bea8646f40d8e3a08c0ec5bddea9e5bfe48774464771658c792a5'
```

Conferir cada plano e IDs. Digitar DATA P1 somente no prompt correspondente;
confirmar E/Q/S separadamente apenas quando corresponderem ao pedido.
Divergência, falha, cancelamento ou UNKNOWN: parar, preservar saída/objetos e
não repetir. Não enviar respostas automaticamente. Enviar saída completa.
