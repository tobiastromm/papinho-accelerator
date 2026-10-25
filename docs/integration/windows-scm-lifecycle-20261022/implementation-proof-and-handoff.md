# Windows SCM — implementação independente e fronteira factual

Task PACC-PHASE6-WINDOWS-SCM-LIFECYCLE-1. Reutiliza executável API3 candidate, console Stop bridge e main/runtime existentes. Fontes funcionais e extensão do teste foram oficialmente revisadas antes de aplicar. Não há segunda CLI/produto, root/worker/pins/registro de clientes ou serviço paralelo.

## Comandos concretos

No executable candidate papacc_server.exe:

```text
service install --account .\NomeLocal
service start
service stop
service status
service startup automatic
service startup manual
service remove
```

NomeSCM privado fixo PapinhoAccelerator. Parser C89 copia account bounded128 e aceita somente prefixo local .\ e subset ASCII alfanumérico/_/-, sem senha/expansão/comando. Não comprova que a conta exista, seja padrão ou esteja autorizada. Comando malformado retorna1; negação/erro2; consulta factual concluída0. Não há defaults SYSTEM/LocalService. O internal service host só é encaminhado ao dispatcherSCM; não faz fallback console.

server_service_data.c/.h implementa apenas sintaxe, admissibilidade DATA por estado e lifecycle replay-safe: EMPTY→START_PENDING→RUNNING→STOP_PENDING→STOPPED, com RETAINED terminal não limpo. Stop antes de RUNNING impede avanço; finish sem fato de quiescência não fecha; retenção nunca reabre nem vira sucesso após timeout. Isso não é fonte de autoridade.

server_service_win32.c consulta OpenSCManager/Service/QueryServiceStatusEx somente com SC_MANAGER_CONNECT/SERVICE_QUERY_STATUS. O resultado de serviço ausente é limitado ao nomeSCM consultado, não prova instalação nova, CA ausente ou autorização. Observação de PID/status não é posse do objeto/autoridade. CloseServiceHandle é verificado uma vez por referência: falha retém service+manager ou sómanager, bloqueia reentrada/reuso por atomic gate, guarda erro e nunca tenta novamente. No máximo dois handles de caller; custosWindows/RPC opacos não se tornam0.

A fonte factual de binding de instalação/conta/SID/ACL/executable/credencial não existe: ps_binding_ready retorna0 fixo, sem setter/argv/env de habilitação. Todos os comandos mutantes negam ANTES de adquirir SCM. Existem branches nativos StartService/ControlService/ChangeServiceConfig/DeleteService atrás desse gate, NÃO qualificados/executados. install tem apenas parse/planejamento DATA e recusa nativa; NÃO existe CreateService nem envio de senha ou nomeNULL que escolheriaSYSTEM. Não alegar instalação completa. Remove nunca apaga dado/CA/CURRENT/config; sua futura resposta seria DELETE_REQUESTED, não ausência confirmada.

## Host e Stop

O mesmo executable tem StartServiceCtrlDispatcher e ServiceMain/HandlerEx. Callback só sinaliza atomicStop e o bridgeStop existente, sem liberar/retry/bloquear. ServiceMain possui status/lifetime; nenhum statuspositivo de RUNNING é anunciado sem fontes. Hoje registra START_PENDING e nega antes de runtime/root/token/worker; só pode reportarSTOPPED porque o conjunto de recursos do servidor é factualmente vazio. Serviço fora deSCM retorna erro sem rodarconsole.

Falha/incerteza de status mantém metadata e contenção query-only, sem falsoSTOPPED. Sleep(INFINITE) nessa contenção é espera sem retry de cleanup, não liberação por prazo ou tentativa de prolongar timeoutSCM com checkpoints fictícios. Não é promessa de sobrevivência contra encerramento forçado do OS; nenhuma política de timeoutUAC/SCM é alterada. Se um futuro produtor permitir invocar o callback do main existente, inteiro de retorno NÃO prova quiescência: código conserva RETAINED e não anunciaSTOPPED. Falta integrar fatos canônicos de configuração/prontidão/finalização antes de admitir um host operacional; não transformar o guard em1 para testá-lo. Nenhum loop/runtime de servidor foi duplicado.

## Qualificação real versus modelos

Selected/Legacy MSVC x64 Debug/Release: quatro builds finais sem warnings, servidor incluído. Sete suítes/configuração=28 combinações PASS (3SCM + consoleStop + effectplan + memorycensus + issuerproductiondeny). Extensão do MESMO teste modelado ServiceMain exigiu quatro rebuilds/repetições focais adicionais, todosPASS; não repetir suítes inalteradas sem motivo.

Modelo de query substitui somente APIsSCM de observar/fechar; depois ampliado com registro/status/Sleep sintéticos para testar ServiceMain sem runtime, registro negado, statusSTART/STOP falhando, retenção/nenhumterminalrepetido e callbackStop/interrogate/shutdown. Nenhum fakehandle passa a APIreal; reset de metadata simula nova instânciaMODEL, não liberação real. DATAcases cobrem instalação ausente/existente, start/stop/startup/remove, replay, Stop/quiescência/retenção. Isso NÃO testa CreateService real.

36 casos no executable canônico PASS: consultaSCM read-only observouABSENT nas quatroconfigurações; seis mutações negadas, host foraSCM negado e comando inválido. Nenhuma criação/início/parada/remoção real, elevação, conta/ACL/chave/cert/bootstrap/CURRENT/rede. Não há criaçãoLAB autorizada ou comando positivo preparado, pois falta binding qualificado. Não usar conta fictícia dos negativos como fonte produtiva.

## Recursos e handoff mínimo

ed11-census.json e evidence/ed11-measurements.json contêm owners116bytes, RO38, objetosstack132/36, PEdelta e ownstack diagnóstico. Função storage_bytes mede apenas estruturas112; atomic4 é contabilizado separadamente. Não contar hosts/life12 embutido duasvezes. Provider/SCM/kernel/CRT/fullnestedstack e threads nativas do dispatcher/ServiceMain continuamUNKNOWN. Não há worker novo, malloc próprio ou quota maior; isso não prova heaptransitivo0. ED11PARTIAL/ativaçãoDENY.

Primeiro requisito concreto: produtor canônico factual de conta local padrão dedicada e instalação/executable/ACL/autorização, SIDSAM versus SIDSCM separados, logon/credencial SCM protegidos sem segredo em argv/log/conf. Depois ligar configuração/prontidão e conclusão/quiescência do runtime existente ao statusSCM e qualificar budgets/Stop antes do positivo manualLAB. Nenhum novo ADR geral é necessário só para UI: usar M0–M11 e os owners existentes. Sem selecionar conta/criarserviço por inferência. O código parcial concluído não deve ser descartado/reimplementado.
