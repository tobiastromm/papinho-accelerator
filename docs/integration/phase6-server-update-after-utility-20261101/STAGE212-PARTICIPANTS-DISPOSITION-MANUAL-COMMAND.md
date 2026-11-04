# Disposição excepcional manual única autorizada

Executar em outro PowerShell elevado; não repetir em falha. Não fechar manualmente as janelas retidas antes da operação.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\dispose-stage212-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '8c7e0f6b81ccb4ff2618e671a0c3d99f60a8fdf3f8b19bd227d42a8e59014790' -ExpectedRequestSha256 '75ce698ed1846c25d40390f01fa89c93b3ace0e2e199b6ab9bd6b5807f728494' -ExpectedDecisionSha256 'b4bab5face7f7bf72ddb3896545a7190b96fca2aabc3d5c3b7afe6bda24c4881'
```

Confirmação interativa: `DISPOSE_STAGE212_7580_18000_ONCE`. Requer alvos/identidades exatos e gates do pedido aprovado. Enviar saída integral, inclusive resultado da outra janela. Nenhum UPDATE ou novo Start autorizado.
