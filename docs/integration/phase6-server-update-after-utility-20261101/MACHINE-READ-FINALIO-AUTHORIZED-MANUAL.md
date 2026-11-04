# Sessão READ final-I/O — uma execução manual autorizada

Decisão específica registrada; execução pendente. Não executar automaticamente. Conferir todos os planos e IDs novos e fornecer cada confirmação independentemente, sem texto extra. Em divergência, cancelar. Sem retry/Stop/kill/limpeza.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-machine-read-finalio-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'e980398f1cd9186d15770bd2390c6f49a937517a32cd7b756e457f6462ac27c7' -ExpectedOperationalRequestSha256 '55ffe9a68db126733d5aa3b85e805c035e57f4a12505055b901838b265be8ce9' -ExpectedDecisionSha256 '241f3179c9ffe5ae92d4916706536d80b81244b8e6f8bcb0c88fdd268fbf6354'
```

DATA de seleção (não confirmação E/Q/S/P):

```text
P1 MACHINE_READ AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:1966080:412413 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87
```

Enviar saída completa. CLI3 inconclusivo não é PASS; código0 também requer reconciliação de todas as provas. Se reter sem retornar, preservar janela e saída, não repetir ou terminar processos por conta desta autorização. UNKNOWN histórico preservado.
