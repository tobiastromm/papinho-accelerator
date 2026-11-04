# Comando manual — único UPDATE PDP autorizado

Pedido SHA256: `f11d942d0d9c0553ff8197d67e19d838f80dfdf0adc9d7df02c5c861f33298a2`. Decisão real SHA256: `d50dd4e8346c3f99b6b8ca54cc2986a6c0929ca63be3c571f91459fd240bc687`.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-pdp-parent-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'a347968f996f8f133d122c55c9e89b0bf2a778e37038c149ac4ca85df0485069' -ExpectedRequestSha256 'f11d942d0d9c0553ff8197d67e19d838f80dfdf0adc9d7df02c5c861f33298a2' -ExpectedDecisionSha256 'd50dd4e8346c3f99b6b8ca54cc2986a6c0929ca63be3c571f91459fd240bc687'
```

Conferir o plano nativo e digitar somente a linha OWNER_BINARY_UPDATE com a tentativa exibida. Não repetir após falha/consumo. Enviar saída integral. Não há criação PDP/source/FCA autorizada por este comando.

SUPERSEDED_BEFORE_EXECUTION: NÃO EXECUTAR ESTE COMANDO. Único ativo: PDP-PARENT-UPDATE-REV3-MANUAL-COMMAND.md.

