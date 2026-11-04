# UPDATE do diagnóstico da espera — execução manual autorizada pendente

Pedido `4ca95920467042d7a9eeb1d462cf83584247ed6d1773e026e07bfbb59fb7757b` e wrapper/candidato congelado conferidos.
Decisão real OWNER-WAIT-STATE-UPDATE-APPROVAL.txt, SHA256 `495da88d6b6029d54c4d8b5251bb84f7d0118d586cccdce703b8a3d6a4c1651d`.
Nenhum UPDATE/coleta/Start executado; nenhum build/teste inalterado repetido.

Executar UMA vez no Windows PowerShell administrativo original:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-wait-state-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'b436ee39a4d675fd0a8470117e9cbd0a048a25319858deab835c2f3a71713e6d' -ExpectedRequestSha256 '4ca95920467042d7a9eeb1d462cf83584247ed6d1773e026e07bfbb59fb7757b' -ExpectedDecisionSha256 '495da88d6b6029d54c4d8b5251bb84f7d0118d586cccdce703b8a3d6a4c1651d'
```

Conferir o plano nativo, os IDs/pins e leaves exclusivos. Somente se coincidirem,
confirmar manualmente a linha OWNER_BINARY_UPDATE exibida. Nenhum nonce/resposta
automático. Em erro/DENY/cancelamento, não repetir. Enviar saída integral.
Não permite Start/Stop/limpeza; primeira causa histórica e quiescência OS UNKNOWN.
