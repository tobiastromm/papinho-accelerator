# UPDATE manual autorizado — execução pendente

Uma execução, sem retry. Conferir o plano nativo e sua confirmação atribuída. Em divergência, cancelar; não executar Stop, rollback ou limpeza. Hashes são conferidos pelo procedimento antes da chamada. Não foi executado pelo agente.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-read-final-io-completion-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'c19121fc8f704947d28a64afa5ddce8ec869a1c04e930b61c7aaa738c34ef6d4' -ExpectedRequestSha256 '5da25da5938aef7eaa289e320393051fd7dfdb1f5fed05db364b6c94487e1d86' -ExpectedDecisionSha256 'cabfc935e93975d9a1a5755ea499a9656a1d033aae05484a9c97fc29f38360ed'
```

Após a saída integral, reconciliar efeitos/identidades/readback/SCM/fechamento. Não executar sessão READ com esta autorização. UNKNOWN histórico preservado.
