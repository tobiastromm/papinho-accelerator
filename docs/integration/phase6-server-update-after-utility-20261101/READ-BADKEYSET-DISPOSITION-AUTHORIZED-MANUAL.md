# Uma disposição manual autorizada

Pedido/hash/fonte conferidos. Decisão registrada, execução pendente. Não executar automaticamente nem repetir.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\dispose-read-badkeyset-retained-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'a0c9016907d80ac5e24943552b506b331ad30a871ecc97b9024168e7d2d39334' -ExpectedRequestSha256 '5b2b0ae47c88426a3d84d8545e0c386a433b177e8d6c16f602cfdb7c2dae994e' -ExpectedDecisionSha256 'e1101e8037fe003407dbce48a1726870bf059b9d685b8907c7834b4ab88dfb71'
```

Confirmação exata `DISPOSE_READBADKEYSET_12612_19168_ONCE`. Runtime19168 primeiro, controlador12612 depois; guards frescos obrigatórios. Enviar saída integral, sem inferir READsuccess/SCQclose/quiescência.
