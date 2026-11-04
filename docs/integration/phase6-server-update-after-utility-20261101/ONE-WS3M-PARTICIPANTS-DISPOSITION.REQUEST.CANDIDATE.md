# Pedido — disposição excepcional dos participantes WS3M antes do UPDATE SQB3

Estado CANDIDATE; aguarda decisão expressa do proprietário. A aprovação da consulta read-only NÃO autoriza esta operação. Não executar automaticamente.

## Decisão solicitada e fundamento

Solicita-se uma única execução manual excepcional para encerrar exclusivamente os dois objetos de processo identificados abaixo. Reutiliza-se o adaptador transitório de disposição já existente, agora com dois alvos fixos e gates SCM pós-runtime. Não cria serviço, owner, updater, root, canal, protocolo ou mecanismo permanente.

G0–G9 accepted, G5 exige decisão posterior específica para terminar processo e não a concede implicitamente. G7 mantém referências/reservas e close/quiescência UNKNOWN; G9 separa operações. Esta é a decisão operacional excepcional posterior prevista, não mudança desses contratos nem prova de sucesso diagnóstico. SSO-B/OHI/QUERY/G3/P0–P7 e U1–U7/U2 continuam inalterados. Não se pede nova ADR nem aprovação de UPDATE.

A sessão WS3M9249509a3a942554defa8142bb6b78252e1c5ff9cc39db7bb11082548867653b/
decisão2161c0f085438c1e217d30063df9b5856d3e01733b2d33a8e29a3c1f6891dd69
foi utilizada, com três confirmações E/Q/S e submissão de Start. O wait original
retornou1460 em212, CLOSE5023/RETAINED1. Não repetir sessão ou originais.
A consulta read-only c51072d516532ac5b452ec4e4fa8e898697f275bae29010d12bdb069ad4ce036
foi concluída com first0/completeTrue/observer_close_knownTrue e está consumida.
Sua saída preservada OWNER-WS3M-CONTROLLER-QUERY-ACCESS-OUTPUT.txt tem SHA256
296c8d6f26b1ad8c78d6435583035e4df6d18c55cdfd462c2797a3b97a1a6ddd.
Correlacionou os objetos e tokens abaixo, sem autorizá-los ou encerrá-los.
Esses fatos não afirmam que os processos ainda estão vivos nem que o SCM permanece
START_PENDING: os gates desta execução devem demonstrá-lo antes de qualquer efeito.

SQB3 com errata Q6 foi aprovado e qualificado offline (quatro builds/76pares), mas
não está instalado. Candidato804352bytes/pin11cbef6508b1b06fb56031b22b93dccc6ae3e05df22528190f3cf7215636d815;
último instalado798720bytes/pin6ad6e378ea5ec074ad6af44327b6fd1a22880a8cf18a8f02a3ecc28baf0edb9e.
A nova imagem não pode ser usada como pin destes participantes históricos.
A barreira DACL atual observada não comprova o erro nativo histórico da sessão.
A aprovação normativa SQB3 e o “siga” de preparação não autorizam este término.


Os códigos de retenção não têm saída normal demonstrada que produza close/STOPPED. Não há caminho normal de Stop autorizado/qualificado para esta sessão; não enviar ControlService. A aprovação solicitada é somente para disposição excepcional, não UPDATE ou novo Start. Se os objetos já tiverem terminado ou divergirem nos gates, interromper sem procurar substitutos.

## Dois alvos exclusivos e evidência

Controlador: PID19860, criação134381311017119087, TokenUser S-1-5-21-785699550-1440458426-1682014700-1108; TokenPrimary, groups17, token164979794:0, authentication570063:0, modified164979462:0.

Runtime: PID19836, criação134381311235391902, TokenUser S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099; TokenPrimary, groups10, token165008246:0, authentication165007967:0, modified165008015:0.

Os dois nomes de imagem devem corresponder a C:\Program Files\Papinho\Accelerator\papacc_server.exe. Uma referência de leitura retida do EXE canônico exige identidade417614100:4128768:327703,798720bytes, link único, arquivo não diretório/reparse e SHA2566ad6e378ea5ec074ad6af44327b6fd1a22880a8cf18a8f02a3ecc28baf0edb9e. Não é atestado de imagem mapeada nem aprovação por hash. Reutilizam-se as primitivas de referência/identidade/hash do adaptador anterior, sem permissões ou confiança adicionais por workspace.

## Artefatos fixos

Fonte ws3m-participants-disposition.CANDIDATE.cs, SHA256 8facc8b30715f13374f0fc06b5b34d46f780db8127b7e3ed33e3241a68e9a839.
Procedimento dispose-ws3m-participants.MANUAL.CANDIDATE.ps1, SHA256 472c31542b8da80f33b7bce3238a51f63cc17395f70cc04e96d8f6edc36be233.
Pasta canônica docs/integration/phase6-server-update-after-utility-20261101. O wrapper verifica pedido, script, fonte e arquivo da decisão operacional efetivamente emitida, com hash real informado no comando posterior. Arquivo previsto OWNER-WS3M-PARTICIPANTS-DISPOSITION-APPROVAL.txt, que ainda NÃO existe. Sem ele, DENY. Não há placeholders para execução nem comando prematuro.

## Contexto, direitos e gates antes de efeitos

Host TCORP-TOBIAS, PowerShell Desktop5.1 AMD64, console sem redirecionamento; operador original elevado, sem impersonação. Nenhum alvo pode ser selecionado por CLI. Abre SCM com CONNECT1 e serviço PapinhoAccelerator somente QUERY_CONFIG|QUERY_STATUS5; não pede direitos de alteração/Stop/Start/Delete.

Antes da confirmação e novamente antes da primeira submissão, QueryServiceConfigW deve corresponder ao comando exato "C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed, OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1, conta virtual NT SERVICE\PapinhoAccelerator; SID type UNRESTRICTED1. QueryServiceConfig2W exige zero ações de recuperação configuradas. Existência de ação, consulta falha/incompleta ou configuração divergente => DENY, sem alterar configuração. A ausência de ações evita solicitar deliberadamente recuperação SCM; não garante ausência de agentes externos ou mudança concorrente.

SCM exige START_PENDING/PID19836 e códigos de saída zero imediatamente antes da submissão. Abre somente os dois processos com QUERY_LIMITED_INFORMATION|SYNCHRONIZE|TERMINATE(0x00101001), sem herança. Mantém os mesmos handles; valida PID/criação/vivacidade/path e TokenUser/Statistics originais por TOKEN_QUERY8; revalida os mesmos objetos/tokens antes de efeitos. Nenhum debug privilege, impersonação, ACL override, enumeração, leitura de memória ou fallback.

Abre e verifica o EXE somente leitura, FileShare.Read, retendo a referência até finalizar. Não cria/copia/atualiza/renomeia/exclui arquivos da instalação. Valida identidade da referência antes dos efeitos e após os marcos. Retorno do path do processo não é prova de fonte administrativa, de bilateral ou dos bytes mapeados; a decisão excepcional é o fundamento operativo separado.

## Confirmação e sequência indivisível sem retry

Exibir os dois alvos e exigir manualmente DISPOSE_WS3M_19860_19836_ONCE após a aprovação específica dos bytes deste pedido. Nenhuma resposta automática. Cancelamento não autoriza repetir; registrar tentativa utilizada sem retry.

1. Revalidar configuração/ausência de recuperação, SCM START_PENDING/PID19836, ambos processos/tokens e referência de imagem. Consumir a decisão uma vez antes da primeira mutação.
2. Uma chamada TerminateProcess ao runtime19836, código de saída excepcional0xe0000001. Marcar submissão antes da chamada; resultado ambíguo não autoriza repetir. Uma espera de no máximo60000ms pelo mesmo handle, seguida de GetExitCodeProcess exigindo o código exato. Timeout/erro/divergência => parar, manter controlador e UNKNOWN.
3. Uma consulta SCM deve demonstrar STOPPED/PID0. Códigos de saída do serviço são registrados, não exigidos como zero: término excepcional não é diagnóstico bem-sucedido. Não há polling/retry para fazer esse gate passar. SCM pode ainda estar atualizando estado; nesse caso a operação fica parcial e o controlador é preservado.
4. Revalidar configuração/ausência de recuperação, controlador original e referência de imagem. Somente então uma chamada TerminateProcess ao controlador19860 com o mesmo código excepcional; uma espera60000ms e uma consulta do exit code exato. Não tocar outro controlador ou eventual novo runtime.
5. Confirmar SCM STOPPED/PID0 e imagem original pela referência. Fechar uma vez os recursos explícitos do adaptador. Falhas de fechamento permanecem UNKNOWN; não compensar ou repetir.

São no máximo DUAS submissões, uma por objeto fixo; uma única execução manual.60000ms por espera é uma escolha explícita de tolerância operacional solicitada, não máximo nativo nem empréstimo do deadline G3. No máximo120000ms de esperas explícitas sequenciais; chamadas Win32/RPC/provider podem ultrapassar qualquer previsão. Não há garantia de duração global.

## Efeitos e riscos residuais que a decisão deve reconhecer

TerminateProcess é abrupto, assíncrono e não executa o protocolo SCQ/close/destrutores da aplicação; solicita cancelamento de I/O e pode não concluir dentro da espera. Estado de DLLs/dados, referências externas, borrows/reservas e quiescência OS podem continuar UNKNOWN após a saída dos dois processos. Não chamar isso de SCQ close ou recuperação autoritativa.

SCM observará término anormal e poderá atualizar estado/códigos/contabilidade de falhas como consequência nativa; não se altera sua configuração e não se reseta essa contabilidade. Ausência de ações configuradas é gate obrigatório, não garantia contra interferência externa. Se o SCM ou um terceiro recriar processo/mudar estado, interromper; nunca perseguir outro PID.

Pode haver falha parcial: runtime encerrado com controlador ainda retido, controlador encerrado mas readback/close desconhecido, referência que não fecha ou saída não observada. Não há rollback, retry, limpeza, exclusão, refund ou liberação presumida de reservas. A sessão diagnóstica fica abandonada/FAILED, nunca PASS. A sessão WS3M teve timeout sem primeira causa nativa completa demonstrada; bilateral original, receipt/close, retenção histórica e quiescência permanecem UNKNOWN, e readiness produtiva DENY. Disposição não corrige retroativamente o protocolo.

Até7 referências explícitas simultâneas (SCM manager/serviço,2processos,2tokens,1arquivo), buffer8192bytes para configuração e512bytes sequenciais para token. Cópias gerenciadas, hash/provider/RPC/kernel/temporários do compilador têm custos opacos; sem importar quotas G3/B/R/U ou alegar máximos demonstrados. Apenas recursos adquiridos pelo próprio adaptador serão fechados, uma vez. O fechamento do adaptador não atesta fechamento das aplicações.

## Proibições e evidência futura

Não autoriza UPDATE, Start/Stop por SCM, endpoint, SCQ, limpeza/exclusão/rename/restauração/rollback, alterações ACL/SCM, CA/CURRENT, rede operacional, TPM/NV/Q4, refund ou autoridades produtivas. Não alterar DLLs, EXEs anteriores, dados ProgramData, originais ou pins. Autorizações anteriores continuam consumidas. Preservar PST0.6.4 e todos os UNKNOWN históricos.

Após execução manual, exigir saída integral, primeira falha/consumo/submissão de cada alvo, saída observada/exitcodes por caminho qualificado, SCM e fechamento. Mesmo resultado positivo não permite UPDATE/Start: preparar pedidos separados somente após confrontar a evidência e os gates frescos.

## Qualificação offline

Fonte real compilada no Windows PowerShell5.1 sem invocação do RunFixed. Modelo substitui17 funções nativas e a fronteira de aquisição/hash da imagem;52 cenários PASS. Incluem configuração/recuperação, identidade/token/path/SCM divergentes, replay/cancelamento, falhas antes/depois da submissão de cada processo, timeout, saída divergente, readback SCM não parado, identidade alterada após término do runtime e fechamento UNKNOWN. Guarda máxima de uma chamada por alvo e ordem runtime antes do controlador verificadas. Wrapper AST e equivalência do core estão registrados em WS3M-DISPOSITION-OFFLINE-QUALIFICATION.json. O core é idêntico ao anterior após remapear somente constantes e nomes, e o core do modelo é equivalente ao fonte real exceto a entrada de confirmação simulada.

## Fontes primárias

- https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminateprocess — término abrupto/assíncrono, I/O e referências do objeto.
- https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_failure_actionsw — recuperação SCM quando o processo termina sem reportar STOPPED.
- G0–G9 G5/G7/G9 e SSO-B accepted do projeto, preservados sem alteração.


## Próximo passo estritamente separado

Aprovar os bytes deste pedido autoriza somente registrar a decisão efetivamente
recebida e preparar o comando manual vinculado aos hashes reais. Não haverá execução
automática. Exigir saída integral para reconciliação, mesmo em falha parcial.

Somente depois de disposição suficiente e readback fresco STOPPED/PID0 pode-se
avançar ao pedido condicional ONE-SQB3-IMAGE-UPDATE-REQUEST.CANDIDATE.md, SHA256
f9a85a2f700d3cae1e18ca2b6fded91843791e2e9203a0028c30099379b98986.
Este pedido de disposição NÃO concede UPDATE nem nova sessão E/Q/S. Nenhuma
interpretação de UNKNOWN como PASS, close SCQ ou liberação de reservas.
