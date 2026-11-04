# Viabilidade de diagnóstico provider K5 — conclusão

2026-11-03. Pesquisa documental e qualificação offline apenas.

Não foi demonstrado um schema público de Crypto-NCrypt que garanta revelar o predicado interno de SoftwareKSP/BADKEYSET nesteWindows10. A existência geral deETW não qualifica provider/eventID/keyword/cobertura/custos. A orientação oficial deCrypto-NCrypt eventos13/14/15 encontrada trataVBS Key Isolation, não esta fixtureSoftwareKSP; não transportar essa especialização. CAPI2/Operational não foi demonstrado como prova suficiente deste caso. Não foram inventadosGUIDs ou ativações, nem iniciado tracing. A recomendação local de ferramentasWindows existentes foi lida; não há novo collector/residente.

Foi encontrada alternativa suportada mais restrita: Security5058/5061 já existentes. 5058 registra operação de arquivoKSP;5061 registra operação criptográfica. Poderão acrescentar sujeito/operação/retorno e correlação histórica, sem repetirStart ou ativarlog. Não garantem causa interna; nenhuma existência/disponibilidade/política do host foi consultada. Ausência não prova nada sobre proteção/isolamento e não permite configurarauditoria porfallback.

Preparado pedidoUMAleituraSecurity com filtro fixo da fixture, provider, doisSIDs e janelaUTC histórica da tentativa encerrada, até16registros e saída whitelist sem caminho de chave/XML/secreto. Procedimento só futuro após decisão específica. CustosEventLog/rendering sãoUNKNOWN; limites configurados não são teto nativo. Não transferequotas/autoridadeREAD nem exigePID/token futuro. Query pode falhar ou não acrescentar causalidade; isso é um resultado delimitado, não justificativa para repetir automaticamente.

Pedido ONE-READ-K5-EXISTING-AUDIT-REQUEST.CANDIDATE.md SHA256 de878e960d5ad49ce60cfd1041e69aa693be3bc6e1b6cd071be7a1cca626a0af. Procedimento observe-read-k5-existing-audit.MANUAL.CANDIDATE.ps1 SHA256 f7b7e96fd59e65f2782c1bb957c7d0ab981283f11db547c4af837c2f9fca3bd7. AST/12casos purosXMLPASS, zero consultasEventLog/ETW/KSP/SCM. Código/binários do produto inalterados; sembuilds/repetição de campanhas.

Primeiro gate: aprovação específica para essa leitura existente, não nova sessão. Após evidência, distinguir retorno auditado de causa interna; manterK5 estrito eDENY até qualificação suficiente. Não há pedido deUPDATE ouStartjustificado por esta pesquisa. HistóricosUNKNOWN/fechamento conhecido da sessão presente preservados.

## Fontes primárias consultadas

- [5058 — Key file operation](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-5058): schema de auditoria do arquivoKSP.
- [5061 — Cryptographic operation](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-5061): schema de auditoria de operaçõesKSP.
- [wevtutil](https://learn.microsoft.com/en-us/windows-server/administration/windows-commands/wevtutil): distingue consultar registros/metadados de configurar/exportar/limpar; nenhum comando executado.
- [EnableTraceEx2](https://learn.microsoft.com/en-us/windows/win32/api/evntrace/nf-evntrace-enabletraceex2): provider controla filtros/eventos; metadata local pode ser necessária para nível/keywords, não há cobertura universal inferida.
- [VBS key protection](https://techcommunity.microsoft.com/blog/windows-itpro-blog/advancing-key-protection-in-windows-using-vbs/4050988): orientação de eventosVBS não qualifica automaticamenteSoftwareKSPWindows10.

Exemplos de terceiros encontrados não foram usados para fixarGUID/flags/eventos ou definir causalidade. Semcontatoexterno, downloads ou alteração de autoridades.
