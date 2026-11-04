# Comando manual — uma sessão P1 SD autorizada

P1-SD-THREE-ACTS-AUTHORIZATION.md registra autorização efetiva do proprietário.
Registro da decisão OWNER-P1-SD-THREE-ACTS-APPROVAL.txt/hash
019222b9ea2c4e2e0889639098c63ff5187b04de96b8ddc768a0b9fcd437b28b.
Execução e originais protegidos ainda pendentes. Nenhuma operação executada
pelo agente. No PowerShell como administrador, executar UMA vez:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-p1-sd-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '86a21d84647e00bce9654d699b95aadcbe8f2e737890ec36967194deee704e93' -ExpectedOperationalRequestSha256 '16e0693541eed115886c59092ebac8f5b32dfe68ef2a893b5fda5d49fd7fb99a' -ExpectedDecisionSha256 '019222b9ea2c4e2e0889639098c63ff5187b04de96b8ddc768a0b9fcd437b28b'
```

O driver confere hashes/contexto e exige primeiro a confirmação documental
para as duas cópias NOVAS. Compare os IDs emitidos e os planos seguintes;
não reutilize IDs dos originais históricos. Apenas se o materializador
concluir com ambos verificados e close conhecido o driver avança à imagem
instalada. Não modificar arquivos/ACLs ou repetir para superar uma falha.

Somente quando aparecer o prompt nativo P1 de DATA, digitar a linha selecionada
no pedido (o driver também a mostra):

```text
P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:8716288:38019 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87
```

Essa linha não é comando nem confirmação E/Q/S. Confira separadamente cada plano
e responda manualmente ao respectivo desafio OWNER_SCQ_PRIVATE_ENDPOINT,
OWNER_SCQ e OWNER_SCQ_DIAGNOSTIC_START que for apresentado. Não enviar respostas
automáticas ou supor que todos serão alcançados. Cancele em divergência.
Guard124/cap128/G3 podem negar; não elevar quotas ou fechar refs por conveniência.

Envie saída COMPLETA: materialização, IDs, diagnóstico nativo e estado terminal.
Em falha/DENY/UNKNOWN, NÃO repetir, criar outra decisão, limpar ou restaurar.
Se estacionar em retenção, não fechar janela/kill/Stop sob esta autorização;
informar última saída. Não executar Start/Stop manual fora deste owner.
Bypass só no processo filho; sem mudança persistente de política/GPO.
