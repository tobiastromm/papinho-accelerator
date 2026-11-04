# Pedido operacional — leitura de auditoria KSP já existente

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. 2026-11-03.

Solicita UMA execução MANUAL somente leitura do canal local Security, sem nova sessão READ/Start, sem habilitar auditoria ou tracing. Objetivo: verificar se eventos5058/5061 JÁ registrados acrescentam operação/retorno/contexto à tentativa final-I/O encerrada2a2db773415e5b78cf4b165e5bbf91ae78f918ee8bb64acf030d0692aab5095b. Não promete revelar primeiro predicado interno do KSP ou mudarK5. Autorizações de sessões/UPDATE/coletas anteriores estão consumidas e não autorizam esta consulta.

## Escopo exato

HostTCORP-TOBIAS; operadorSID S-1-5-21-785699550-1440458426-1682014700-1108, administração legítima, WindowsPowerShell5.1DesktopAMD64/console original sem redirecionamento ou impersonação. Contexto nativo fresco verificado pelo procedimento antes da leitura; nenhuma identidade presumida de string/hash.

Uma EventLogQuery/EventLogReader em canalSecurity, direção cronológica, BatchSize1, filtroXPath imutável: providerMicrosoft-Windows-Security-Auditing; EventID5058OU5061; KeyName exatamentePapinhoAccelerator.CA.f3f7579f3e90851845264abbb7991500.9b845d6b6c8e909e5f226c65b321987c; ProviderName exatamenteMicrosoft Software Key Storage Provider; SubjectUserSid operador acima OU runtimeS-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099. ExcluiSYSTEM/outros sujeitos: se a emissão usar outra atribuição, ausência deste filtro não prova ausência global nem autoriza ampliar automaticamente.

JanelaUTC fechada2026-11-03T21:12:54.438685Z até2026-11-03T21:20:59.438685Z. Derivada offline da criaçãoFILETIME do controlador19300: high31288280/low446295973 =>2026-11-03T21:12:59.438685Z, menos5segundos/mais8minutos para janela histórica focal. Não é deadlineG3 nem prova de que todo evento nessa janela pertence à sessão; chave/provider/sujeito/operação/tempo exigem correlação.

No máximo16registros; ReadEvent recebe timeout5s, verificação de tempo decorrido30s antes de cada chamada, sem retry/resize/fallback. São limites configurados do observador, NÃO teto nativo de latência/memória/kernel ou limite globalREAD. A última chamada pode exceder o tempo desejado; custos de consulta/servidorEventLog/rendering são UNKNOWN. BatchSize1 não limita o custo total interno da busca no canal. Ao atingir16 sem observar fim, resultado incompleto, não ampliar. Erro/timeout/acesso negado/XML inválido/campo ausente ou dúvida de close => UNKNOWN, sem habilitar logs, mudar permissões ou reconsultar.

Cada XML deevento, já obtido peloWindows, é limitado na projeção a65536caracteres; isso NÃO limita alocação anterior deToXml. Parser comDTDproibido/resolverNULL, campos/sujeitos/tempo/provider/IDs reconferidos. Saída somente eventID, recordID, UTC, papelOPERATOR ouRUNTIME_SID_ONLY_NOT_ORIGINAL_TOKEN, operation, ReturnCode, KeyType e booleano de presença deKeyFilePath. Nunca imprime XML/rawmessage/caminho de arquivo da chave/accountname/blob/conteúdo privado; não abre o caminho eventualmente mencionado. SID/papel/log não é prova original do token nem autoridade.

## Efeitos e gates

Aprovação específica deste pedido e conferência byte-exata do pedido/procedimento antes da chamada. Procedimento observe-read-k5-existing-audit.MANUAL.CANDIDATE.ps1, SHA256 f7b7e96fd59e65f2782c1bb957c7d0ab981283f11db547c4af837c2f9fca3bd7. Sua presença não autoriza execução. Exige confirmação manual exata READ_K5_EXISTING_5058_5061_ONCE; consumo da única consulta marcado antes de criar oreader. Fecha cada EventRecord e reader uma vez; close incerto mantémUNKNOWN. Sem novo serviço/owner/canal/collector residente ou dados produtivos.

Efeito autorizado: consulta de registros existentes e projeção na console. Não cria arquivo de evidências no sistema protegido, não escreve/limpa/exporta/configura logs. Histórico da própria execução doPowerShell/OS e custos opacos não são garantidos zero. Não usarStartTrace/EnableTrace/auditpol/wevtutilset/install/clear, alterarG3/K5 ou tocarprovider/chave/ACL/SCM. Nenhum EXE instalado atualizado ou executado por este procedimento.

A consulta não é nova aberturaNCrypt, não coleta token/processo/SCM atuais, não executaREAD/E/Q/S/P/StartStop. Não há replay da sessão. Eventos podem não ter sido habilitados, estar ausentes/retidos/sobrescritos ou omitir detalhes internos; nenhuma configuração será alterada para obter dados. Zero registros, fim observado, exit0 ou close do observador não são isolamentoPASS, causalidade ou quiescência global.

## Evidência documental e qualificação

Microsoft documenta Security5058 como operação no arquivo de chaveKSP e5061 como operação criptográfica, incluindo sujeito/provider/key/operação/retorno. Eventos poderão acrescentar a fronteira observada, não são garantidos como explicação interna completa. Fontes: https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-5058 e https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-5061 . NenhumEventIDCrypto-NCrypt/VBS, GUID/keywordparticular não qualificado ou eventoCAPI2 assumido como suficiente.

ASTPASS/12casos de projeçãoXML em memóriaPASS (2positivos/10negativos, incluindo campos/caminho omitidos, sujeito/provider/chave/tempo/retorno divergentes, DTD/duplicação/tamanho/campo ausente). Não invocadoEventLogReader/consulta real; suporteXPath/política/disponibilidade do canal host são gates pendentes, nãoPASSoffline. Produto inalterado; builds88pares existentes reutilizados, sem repetição. Evidência READ-K5-EXISTING-AUDIT-OFFLINE-QUALIFICATION.json.

## Limites e reconciliação

Aprovação solicitada somente para uma leitura manual acima, sem retry. Após saída: reconciliar primeira falha/consumo/contagem/fim/close, eventos/retornos/tempos/sujeitos e capacidade real de correlação. Não inferir equivalênciaACL/NTE_PERM ou causalidade deBADKEYSET sem prova. Nenhum token de runtime futuro/histórico inventado; não promover evento auditado a autoridade original.

Preservar sessão final fechada inconclusiva, originais417614100:2555904:107563 e417614100:3866624:108666, imagem882688/ID417614100:1966080:412413, fixture e históricos/pins/autorizações consumidas/EXEs anteriores/HEAD-WIP/PST0.6.4. Histórico de retenção/reservas/applicationclose/quiescênciaOS continuaUNKNOWN, isolamento/produtivoDENY. Não autorizaUPDATE/coletaoutra/endpoint/SCQ/StartStop/KSPreal/SCM-ACL/CA-CURRENT/rede/TPM-NV-Q4/limpeza/refund/kill/produção.

Somente após decisão efetiva deste pedido preparar comando manual com hashes reais, sem placeholders; não executar automaticamente.
