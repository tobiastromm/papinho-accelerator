# Pedido focal — metadados da cadeia existente do destino

Estado: CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Escopo delimitado; procedimento executável e seus pins ainda não qualificados. Não autoriza execução nem reutiliza o pedido de staging consumido.

## Escopo exato

Uma consulta manual no host de planejamento TCORP-TOBIAS. Comparar a identidade nativa do controlador com o SID esperado `S-1-5-21-785699550-1440458426-1682014700-1108`, sem aceitar esse texto como autoridade. Divergência implica STOP. Observar boot/contexto atuais; não preencher com história.

Resolver FOLDERID_ProgramFiles pelo mecanismo existente. Esperado neste pedido: `C:\Program Files`; raiz `C:\`; componentes relativos `Papinho` e `Accelerator`. Divergência implica STOP, sem fallback. Consultar somente raiz, componentes do caminho do known folder e os dois componentes exatos se existirem. Parar no primeiro ausente; distinguir ausência de acesso negado/erro. Não enumerar irmãos, arquivos, ProgramData, CNG ou árvores. Existência não permite adoção; ausência não permite criação.

## Aquisição e direitos

Usar o adapter/owner existente, abrir somente objetos existentes, raiz sem seguir reparse e componentes relativamente ao handle original do parent. Reter referências até a revalidação final. Registrar volume/file ID, atributos, relação parent/nome e ausência de reparse; não substituir identidade por reabertura de path.

Pedir READ_CONTROL | FILE_READ_ATTRIBUTES; SYNCHRONIZE somente se exigido pelo modo síncrono qualificado. Se o backend exigir FILE_TRAVERSE explícito, congelar esse direito no procedimento antes da aprovação, sem adição silenciosa. Não ajustar privilégios. Proibidos listagem, ADD_FILE/ADD_SUBDIRECTORY, DELETE, WRITE_DAC, WRITE_OWNER, SACL e ACCESS_SYSTEM_SECURITY. Não utilizar o preflight completo de staging, que solicita criação.

Quando necessário, consultar apenas token próprio com TOKEN_QUERY: SID, grupos/atributos, restrições, privilégios relevantes e presença de impersonação. Não impersonar, habilitar privilégio, exportar token ou testar outro usuário. O token administrativo não representa todos os potenciais terceiros.

## Evidência permitida

Por objeto: OWNER/DACL via handle original; validade, presença/nulidade, controle/proteção; ACEs na ordem original com tipo, flags, máscara e SID. Separar OI/CI, INHERIT_ONLY e INHERITED. Não resolver nomes por rede nem imprimir SACL/blobs desconhecidos/conteúdo de arquivos. Tipos não compreendidos mantêm análise UNKNOWN.

Registrar comprimento e SHA256 da representação OWNER+DACL retornada, sem normalização/reordenação, declarando método de serialização. Mostrar motivo/índice/máscara do classificador atual somente se efetivamente executado por ele; replay documental não é classificador C nativo. Distinguir bits explícitos, mapeamento genérico e avaliação estrutural de AccessCheck.

Revalidar identidade, atributos e descritor nos mesmos handles e continuidade das relações observadas. Igualdade entre amostras não prova ausência de mudança intermediária, handles preexistentes de terceiros ou proteção futura universal.

## STOP e encerramento

Uma passagem de aquisição e uma revalidação final, sem retries. STOP em reparse, divergência, falha de consulta, tipo não suportado para análise, insuficiência de recursos ou Stop do operador. Ausência legítima encerra a cadeia sem criação. Preservar primeira falha e UNKNOWN.

Fechar uma vez cada handle próprio e liberar descritores após último uso; reportar fechamento/retidos incertos. Não fechar emprestados ou presumir quiescência OS. Custos internos opacos continuam UNKNOWN, sem ceiling inventado; não importar envelopes LAB/TPM/SCQ nem autorização B3.

Saída apenas no console existente. Sem arquivo/sink de coleta, mudança de execution policy, escrita/criação/flush, ACL, registry, SCM, Start/Stop, endpoint, CNG/RNG, CA/CURRENT, TPM/NV/Q4, staging, cleanup/refund ou recuperação. Não testar escrita/rename/delete para provar proteção.

## Gate anterior à execução

Vincular procedimento mínimo implementado/qualificado e seus hashes reais, direitos efetivos e testes offline. Esta versão fixa escopo, não fornece comando nem está pronta para execução. A autorização operacional posterior deve identificar pedido e procedimento congelados; não usar aprovação anterior ou pins futuros inventados. Uma captura atual não reconstrói o descritor não salvo da tentativa consumida.
