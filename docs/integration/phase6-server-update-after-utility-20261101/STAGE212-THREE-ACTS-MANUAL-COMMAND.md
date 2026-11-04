# Sessão manual única E/Q/S autorizada

PowerShell5.1 elevado, console original. Conferir planos/IDs; cópias documentais são DATA e não confirmam E/Q/S. Não repetir em falha ou preencher confirmações automaticamente.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-stage212-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '5e21ac2ad8116bdb8626df407e1630272e86dd6269a4658c3b8f29bd566b8817' -ExpectedOperationalRequestSha256 'b275975034b33477a90157a0f55bc67e5bd9482abdf21c1c967f384e0cbfb896' -ExpectedDecisionSha256 'af606af323918f1bc9362a1de6ad5eff22fe515315b5b5a0ea45aa524abf5223'
```

No prompt P1 DATA, copiar somente a tupla aprovada:

```text
P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:3014656:330305 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87
```

Confirmar cada ato nativo separadamente somente se corresponder ao pedido. Enviar saída integral. Retenção/UNKNOWN não permite Stop/kill/limpeza; não reutilizar disposição anterior.
