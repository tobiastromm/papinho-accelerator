# Pedido — uma consulta read-only dos participantes da sessão SQB3

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma única execução MANUAL, sem retry.
Não é novo ensaio E/Q/S, disposição, diagnóstico interno ou autorização por PID.

## Fatos e lacuna

Sessão e7627530423dd073c96e836ba22032862b4da44f965302fff87e11fe7569c0d2,
decisão d3c4ce1dc22b8c9b01ec8c0db2dc453c98a8d489e7556407dc41ccb74305b8af,
tentativa771a9ad8cd0e90d4ae1f35a627ef5f228ca078b031032d84c557847fef039ecb.
Duas cópias criadas/readback/close conhecidos: pedido417614100:917504:409120 e
decisão417614100:589824:409139. Três confirmações nativas observadas e Start submetido.
Wait original retornou1460, step5/etapa212/prestart_close0/service_close5023/retained1.
Sessão utilizada, sem retry. A operação pendente do transporte e a fase do receptor
não foram impressas. Não é prova de falha da antiga reabertura administrativa nem
prova de que SQB3, bilateral, receipt/close ou RUNNING foram alcançados.

Controlador original demonstrado: PID4184/criação134381373154465967,
TokenPrimary/grupos17/User S-1-5-21-785699550-1440458426-1682014700-1108,
token191041869:0/auth570063:0/modified191041540:0. PID do runtime não foi impresso.
Os zeros do plano RUNTIME_VIRTUAL são pré-Start, não amostras do processo real.
Nenhum PID de sessão antiga será reutilizado como runtime desta.

## Decisão solicitada e escopo mínimo

Autorizar UMA observação MANUAL do estado atual SCM e dos metadados de processo/
token desse controlador fixo e, somente se SCM START_PENDING consistente, do runtime
candidato que o próprio serviço fixo indicar. Reutiliza observador transitório de
participantes já existente. Não cria owner, canal, serviço, root ou infraestrutura.
Nenhuma autorização anterior de observação ou término é reutilizada.

Host TCORP-TOBIAS, PowerShell5.1Desktop AMD64 e console original, operador original
elevado sem impersonação. Confirmação manual exata OBSERVE_SQB3_CURRENT_PARTICIPANTS_ONCE.
Consumir esta consulta uma vez antes da primeira aquisição. Sem resposta automática.

Abrir SCM somente CONNECT1 e PapinhoAccelerator somente QUERY_STATUS4. QueryServiceStatusEx
retorna estado/PID/códigos atuais. Imprimir os escalares somente após sucesso e
comprimento36 validado. Se não for OWN_PROCESS16/START_PENDING2/PIDnãozero distinto
do controlador/códigos0: interromper sem escolher outro processo. Estado observado
não constitui lease ou fonte original de autorização.

Somente nessa condição, reter o mesmo handle do serviço e adquirir processos com
PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE=0x00101000, sem herança, e tokens com
TOKEN_QUERY8. Validar controlador por PID/criação/User/Statistics originais acima,
processo vivo e caminho C:\Program Files\Papinho\Accelerator\papacc_server.exe.
Para o runtime candidato, observar criação positiva, mesmo caminho, TokenPrimary e
User virtual exato S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099;
exigir criação não anterior à do controlador. PID é localizador, não identidade.

Registrar SID/TokenId/AuthenticationId/ModifiedId/GroupCount/criação de ambos.
Revalidar os mesmos handles, criação e estatísticas; consultar SCM outra vez e
exigir o MESMO PID do candidato. Mudança implica DENY, sem adoção, enumeração ou
busca por substituto. Não existem parâmetros CLI de alvo. Runtime candidato atual
não é atestação original da sessão nem prova de autenticação bilateral histórica.

Fechar uma vez apenas as seis referências explícitas do observador: SCM manager,
serviço,2processos,2tokens. Não tocar referências da aplicação. Falha parcial,
peer ausente, token divergente, timeout ou close incerto preserva UNKNOWN e impede
retry. Observer close não é application/SCQ close nem quiescência OS.

## Limites e utilidade da evidência

A consulta serve para estabelecer a disposição atual e os objetos candidatos,
necessária para governar próximos passos. Não consegue recuperar qual chamada
ou leitura/escrita estava pendente no wait histórico. Essa limitação é explícita;
não repetir observações de DACL, elevação, recursos ou Shell sem necessidade.
Não concluir causa histórica a partir de snapshots atuais. A barreira DACL antiga
não será reutilizada como explicação desta nova sessão sem evidência.

Sem GetKernelObjectSecurity, SACL, enumeração de grupos/privilégios, resource sampling,
impersonação, duplicação/ajuste de token, SeDebug, leitura/escrita de memória,
pipe/endpoint, Start/Stop, TerminateProcess, UPDATE, ACL/SCM edit, arquivos protegidos,
CA/CURRENT, rede, TPM/NV/Q4, limpeza/refund ou autoridades produtivas.
Não consultar outro serviço, conta ou processo. Hash/caminho/TokenUser não autorizam ação.

Seis referências e buffer512bytes sequencial para token, strings limitadas1024chars.
.NET/compilação/loader/alocador/kernel têm custos opacos; não importar quotas G3/B/R/U
ou declarar máximos nativos. Native source somente compilada, modelo sem P/Invoke.
30cenários offline PASS,12fronteiras nativas simuladas, core equivalente ao modelo
(exceto confirmação), ASTwrapper PASS. Inclui troca de PID SCM entre consultas,
PIDzero/do próprio controlador, criação/token/path divergentes, replay/cancelamento
e close UNKNOWN. Nenhum teste/build de produto repetido ou coleta real executada.

## Artefatos imutáveis

- sqb3-participants-readonly.CANDIDATE.cs: 78876a9ae381bb57139805730d1df808eb24a7b89a9b392a9d822768521b3dc9.
- observe-sqb3-participants.MANUAL.CANDIDATE.ps1: 565cb617452edbd22f750d9b6c8cb41b8dfdbc6d145eb128cd45e594bc7499c6.

O wrapper verifica seus bytes, este pedido e fonte, exige contexto original e chama
uma vez o observador após confirmação. Aprovação só registra decisão real e prepara
comando com hashes reais; não executa automaticamente. Pedido não concede término,
UPDATE ou nova sessão. Depois da saída integral, reconciliar primeira falha/consumo/
SCM/identidades/close do observador, mantendo bilateral/application close/retidos e
quiescência OS UNKNOWN. Qualquer disposição futura exige autoridade específica G5.
Preservar históricos/pins/EXEs/autorizações consumidas/DENY/PST0.6.4.
