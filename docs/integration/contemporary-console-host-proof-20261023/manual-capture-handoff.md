# Captura manual futura — exige autorização específica

Não executar nesta tarefa. Launcher e helper estão pinados em pins.json; nenhuma aprovação positiva existe. Preparação para UMA futura execução manual read-only:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1" -Mode Prepare -ExpectedScriptSha256 60a6dd761fee00143cdf88d7fcf629ad1a47986f41875ab1e8020c641b10bca0
```

Confirmar UAC somente depois da autorização específica. Não usar Launch. A coleta ocorre UMA vez após a rejeição contemporânea no censo, com dados da instância atual, sem PID histórico de argumento. No mesmo processo, mede relação/identidade/recursos e preserva a rejeição170. Se o censo não rejeitar, esse collector não é executado: não declarar prova ausente como sucesso.

Esperar IG_CONSOLE_CAPTURE/IG_ACTOR, identidade/imagem/sessão, membros/janela, recursos e IG_CONSOLE_RELATION, com fechamento ou retenção explícitos. Mesmo uma captura útil termina STOP170, pois não altera o censo. O campo antigo DIAGNOSTIC_CAPTURED=False refere-se ao recibo do arquivo operacional NTSTATUS e continuará esperado neste modo: NÃO é indicador de presença das linhas de prova de console. Arquivo diagnóstico operacional não é criado; apenas os logs/recibos administrativos já existentes.

Não há rebind/Start/StopService, mudança ACL, autorização de quinto processo, assinatura/CA/CURRENT ou consumo da tentativa. Nenhum overwrite/retry/kill. Se aparecer IG_RETAINED, preservar janela/owner e recibos; F10 somente após retorno factual conforme mensagem. Falha de observabilidade permanece UNKNOWN. Coleção não é autorização de classificação nem de operação positiva.
