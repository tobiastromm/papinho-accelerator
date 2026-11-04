# Uma disposição manual autorizada

Pedido original e hashes conferidos; autorização registrada, execução pendente. Não executar automaticamente/repetir.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\dispose-read-firstcause-retained-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '77b59ed6fb8c706c4cfdaeae444c48429387788a08b983734b933018b51ff0a8' -ExpectedRequestSha256 'd24cdcc1d5df458c4d0e33673d13f849f44619a5675daa2630d0b08e3d1144ba' -ExpectedDecisionSha256 '04af76c8f4761764c0bc884093d05f6bc5dd8edb4e7e6f52406b25bc335a5cbb'
```

Confirmação: `DISPOSE_READFIRSTCAUSE_9004_17856_ONCE`. Runtime17856 primeiro, controlador9004 depois; guards frescos obrigatórios. Enviar saída integral, preservar UNKNOWN; não inferir sucesso READ ou fechamento SCQ.
