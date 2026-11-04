# Consulta SQB3 autorizada — única execução manual pendente

Pedido aprovado `029503adb6642e1c7cffce9892c8ee1ce469051c0681fec16b82f4e069d4ba2c`; fonte/wrapper/modelo/qualificação conferidos.
Decisão efetivamente recebida em SQB3-PARTICIPANTS-READONLY-AUTHORIZATION.md.
Nenhum método nativo, coleta, build/teste inalterado ou mutação executado pelo agente.

Executar UMA vez no Windows PowerShell administrativo original:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-sqb3-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '565cb617452edbd22f750d9b6c8cb41b8dfdbc6d145eb128cd45e594bc7499c6' -ExpectedRequestSha256 '029503adb6642e1c7cffce9892c8ee1ce469051c0681fec16b82f4e069d4ba2c'
```

Quando solicitado, digitar manualmente SOMENTE:

```text
OBSERVE_SQB3_CURRENT_PARTICIPANTS_ONCE
```

Enviar saída integral; se negar/cancelar/falhar, não repetir. Estado/identidades
atuais não comprovam a operação pendente do timeout histórico. Nenhuma autoridade
de TerminateProcess/Stop/UPDATE/Start/limpeza é concedida. Observer close não é
application/SCQ close; histórico e quiescência OS continuam UNKNOWN.
