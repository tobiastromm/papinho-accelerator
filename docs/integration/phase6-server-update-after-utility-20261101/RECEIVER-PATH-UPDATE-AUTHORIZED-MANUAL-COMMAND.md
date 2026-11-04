# UPDATE manual autorizado — execução pendente

Uma chamada, sem retry. Script confere seus pins/pedido/decisão; gates originais
U1–U7/U2 e confirmação nativa continuam obrigatórios.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-receiver-path-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '3decf073d85867a007bca9084253f161f56c6f633842abec049b96631c481cdf' -ExpectedRequestSha256 '6437fc983b0eb3abb8b4cf6e907efb83bbe3703ea5ae751d557b24aafd9b2484' -ExpectedDecisionSha256 '91a371604e2e2e30a40c30a81113bb542aae0392923b074c37096fa9e76bf29d'
```

Conferir candidato805376/pin174db00a… e antigo805376/pin044f10de…, identidades,
executor, parent, dois leaves exclusivos e K/R no plano. Cancelar em divergência.
Não fornecer resposta automaticamente. Enviar saída integral, sem repetir.
Não executar Start com esta autorização.
