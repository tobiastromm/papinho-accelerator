# Disposição manual autorizada — pendente

Uma execução, sem retry. Revalidar todos os alvos/gates apresentados.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\dispose-waitstate-retained-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'cc2151609167b51585e919d9db525547da32207243db25ba3b65c2f9508b54e5' -ExpectedRequestSha256 '9bd9caf5b87b515d8380958cdcab02e3efad381647a69768853be15eb6feff8c' -ExpectedDecisionSha256 '2bd42b0e6a7f9b743dce62620ffbe37519940eb84bf4dbd2712a932e5d7a4790'
```

Confirmar somente o plano exato com DISPOSE_WAITSTATE_12976_8760_ONCE.
Se houver divergência, cancelar. Não repetir em falha. Fornecer saída integral
das duas janelas. Não executar UPDATE ou nova sessão com esta autorização.
