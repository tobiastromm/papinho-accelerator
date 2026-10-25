# Observação read-only autorizada — execução manual única

Pedido SHA256 `67c312608207eaf4e7dcff863a8c1253d4558b9c8e4d2e7d54dc5a416e90db16`. Declaração real do owner: `autorizo`, em resposta a esse pedido. Registro criado exclusivamente e conferido byte-exato; nenhuma assinatura criptográfica é alegada.

Registro aprovado SHA256 `2f3f261d0213b7f6a46a580b447dc24f33ef391fb3e9c9ccf607e6317415f589` (4973 bytes). Launcher/qualificador/DLL/binding/decisões e helper conferidos; nenhum rebuild ou alteração dos pins.

No único Windows PowerShell5.1 x64 já elevado, com console interativo e contexto não reutilizado pelos helpers, executar uma vez:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Prepare -Profile N0START03ReadOnly -ExpectedScriptSha256 b65571abc3b5335b79f60ab0cca055d08c71dff749b1b947c5dcf55ca03cd6b3 -ExactApprovalSha256 2f3f261d0213b7f6a46a580b447dc24f33ef391fb3e9c9ccf607e6317415f589
```

Digitar somente as duas confirmações exatas exibidas nesta execução; não reutilizar PID/criação/desafio antigos. Escape solicita Stop governado. F10 retorna após recibo final e término factual. Não iniciar outro PowerShell pelo procedimento. Não fechar processos históricos para fabricar margem.

Se qualquer gate falhar, preservar primeira falha e retenção, sem retry, kill ou alterações SCM/ACL. Guardar e enviar toda a saída, inclusive os três checkpoints, janelas PSS, métricas e recibos finais. Não executar novamente após erro/timeout/UNKNOWN.

Os inputs protegidos e host/boot/token não foram promovidos a fatos atuais por este registro: serão revalidados pelo owner nativo na execução manual. A autorização inclui somente os recursos preparatórios locais e as consultas descritas no pedido. Não autoriza Start/Stop, intent, imagem, sink, arm ou efeitos positivos.

NATIVE_PREFLIGHT=NOT_EXECUTED
ED11=PARTIAL
INDEPENDENT_BUDGET=NOT_GRANTED
POSITIVE_ROUTE=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
