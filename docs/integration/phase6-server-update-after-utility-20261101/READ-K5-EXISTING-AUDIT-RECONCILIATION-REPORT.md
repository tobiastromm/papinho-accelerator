# Auditoria existente KSP — reconciliação

2026-11-03. READ_COMPLETE / AUTHORIZATION_CONSUMED / ISOLATION_DENY.

Leitura manual: firstNONE/consumedTrue/records5/query_end_observedTrue/observer_close_knownTrue. Nenhum trace ou auditoria foi habilitado pelo procedimento, nenhuma nova sessão executada. A autorização está consumida, sem retry. Projeções recebidas foram transcritas como tal, não declaradas exportaçãoEVTX ou XML bruto byte-exato.

## Evidência adicional

No mesmo filtro da fixture/provider/janela/sujeitos aprovados:

| RecordID | Evento | UTC (21:13:47) | Sujeito | Operação bruta | Retorno |
|---|---|---|---|---|---|
|1170235|5058|.1195576|operador|%%2458|0x0|
|1170236|5061|.2633206|operador|%%2480|0x0|
|1170240|5058|.2949584|operador|%%2458|0x0|
|1170241|5061|.2951667|operador|%%2480|0x0|
|1170245|5061|.3425783|SID runtime|%%2480|0x80090016|

As duas operações administrativas auditadas foram bem-sucedidas; a operação auditada sob SID do runtime devolveu NTE_BAD_KEYSET. Isso corrobora o resultado nativo fora do mapeamentoWin321168/controlador: não há evidência de que o Papinho tenha inventado BADKEYSET ao traduzir o status. A correlação usa mesma chave/provider/SID/janela, mas não transforma o evento em token original/handle de chave original ou prova exclusiva do PID desta tentativa. Não contém novo identificador de token, logon ou clientPID.

O padrão de duas leituras/aberturas positivas é compatível com aquisição e reopen administrativo positivo do código existente. Não vincular individualmente cada evento àquela chamada só pela ordem, nem supor identidade de handle ou continuidade provada pelo log. Essas provas pertencem ao caminho original da sessão. Operação/machine-key aparecem como %%2458/%%2480/%%2499; preservar esses valores, sem tradução baseada em mensagem não exportada.

## O que permanece desconhecido

Não apareceu5058 do runtime neste conjunto. Isso não demonstra que o KSP deixou de tentar arquivo, falhou antes da camada de arquivo, escondeu a chave por ACL, recebeuNAME_NOT_FOUND/ACCESS_DENIED interno ou usou outro armazenamento. O filtro não incluiSYSTEM/outrosSIDs e a auditoria pode não registrar todo subpasso. O booleanoKeyFilePathFalse no5061 significa campo ausente/vazio nessa projeção, não inexistência de arquivo.

O motivo internoKSP paraBADKEYSET permaneceUNKNOWN. Não há correção comprovada de nome/flags/token/ACL ou quota a aplicar. Uma nova sessão/UPDATE com a mesma pergunta não se justifica. O diagnóstico de lifecycle já alcançou fechamento controlado; não confundir sua conclusão com isolamentoK5 qualificado.

## Primeiro gate e caminho mínimo

K5 accepted exigeNTE_PERM+NULL com mesmas provas; a observação continua fora desse critério. Para avançar, precisa-se de evidência específica que demonstre um defeito corrigível ou de uma decisão normativa focal sobre o requisito de produto, sem promover BADKEYSET aPASS por conveniência. Este relatório não aprova alteração deK5. Não há comando operacional adicional preparado ou autorizado.

Próximo trabalho offline: confrontar formalmente o critério de retorno exatoK5 com a semântica públicaNCrypt e esta evidência, separando ausência de capacidade observada, negação qualificada e readiness. Se houver incompatibilidade que exija decisão do proprietário, apresentar somente proposta indispensável e riscos; nenhum patch dependente antes de aprovação. PreservarK5 vigente/DENY até lá. Não habilitar logs ou tracing por fallback.

Sem mudança de código/binário ou repetição de builds/testes. Mesma imagem882688/ID417614100:1966080:412413/fixture/originais/autorizações consumidas/PST0.6.4. HistóricoUNKNOWN/quiescênciaOS e retenção preservados; fechamento conhecido da última sessão não foi rebaixado ou ampliado retrospectivamente.
