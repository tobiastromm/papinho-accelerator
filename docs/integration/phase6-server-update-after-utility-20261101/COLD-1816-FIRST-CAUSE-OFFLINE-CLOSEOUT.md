#1816 no controlador frio — diagnóstico focal qualificado offline

Data2026-11-01. HEAD main/f341985924d29870d8ff627ed37c8a0874ad6469 preservado.
Nenhuma nova sessão, coleta nativa, endpoint, Start/Stop, UPDATE ou SCM/ACL.

## Conclusão factual: UNKNOWN precisamente delimitado

A tentativa histórica não identifica o primeiro predicado. O processo terminou
sem guardar contagem/erro/índice/stage; a transcrição não contém esses campos.
Isso não pode ser recuperado retroativamente por um modelo ou por medir outro
processo. Não há evidência de132 handles nessa tentativa, nem de memória
insuficiente, freshness vencida ou outro recurso G3 concreto.

Pelo retorno preservado1816 e pelo caminho pinado, a falha ocorre em scqc_plan,
antes de perfil/acquisição completa/confirmadores. Três origens exaustivas:

1. scqc_open_raw, em qualquer um dos três índices: falha de
   GetProcessHandleCount OU count>=124, ambos mapeados para1816 no binário antigo.
2. GetSecurityInfo, em qualquer índice, retornando1816 diretamente.
3. GetProcessTimes ou GetExitCodeProcess retornando FALSE/GetLastError1816.

São11 combinações de API/predicado/índice, não11 medições. Contagem consultada
é do PRÓPRIO controller papacc_server.exe, incluindo herdados, não soma com
PowerShell/materializador. Não se sabe qual índice ou se a consulta teve sucesso.
O restante do fluxo converte suas falhas em outros códigos; não inventar causa
por similaridade com staging ou substituir a primeira falha pelo throw do script.

## Matriz de admissão e estágio

| Classe | Pode explicar o1816 preservado antes dos planos? | Evidência/código |
| --- | --- | --- |
| Consulta de handles falha | Sim, binário antigo a chamava de quota | OR combinado, sem sucesso/erro/count guardados |
| Handles>=124 | Sim | Mesma expressão; cap128/guard124 SCQ obrigatórios |
| GetSecurityInfo/tempos/estado do processo | Sim, erro nativo propagado | DWORD/LastError originais, sem stage impresso |
| Amostragem G3 inválida/incompleta | Não nessa fase | scqg_sample usa INVALID_DATA; amostragem após atos confirmados |
| Piso físico/commit G3 | Não nessa fase | NOT_ENOUGH_MEMORY8, não sampleado antes das confirmações |
| Crescimento privado/working set | Não nessa fase | g.ready/phase2 e scqg_admit; NOT_ENOUGH_MEMORY8 |
| Freshness100ms | Não nessa fase | INVALID_DATA13 de scqg_sample |
| Duração/janela/global deadline | Não como1816 direto nesse retorno | TIMEOUT1460/INVALID_DATA/erro de scope, conforme etapa |
| Retenção/close incerto | Não como1816 original | IO_INCOMPLETE996/unresolved/park; não é capacidade |

Excluir o gate G3 de memória como ponto de origem NÃO prova memória do OS
suficiente: um erro nativo de alocação/quota dentro de uma API continua possível.
GetProcessTimes/ExitCode são metadados do controller já existente, não PID/token
futuros do runtime. Não houve runtime, namespace ou três atos nativos nessa tentativa.

## Defeitos demonstrados e correção

Falha de GetProcessHandleCount era incorretamente traduzida em insuficiência de
quota, sem contagem válida. Agora a consulta ocorre UMA vez no mesmo ponto:
FALSE captura imediatamente GetLastError e devolve esse erro; se vier0,
ERROR_GEN_FAILURE31 mantém falha fechada. TRUE/count>=124 continua1816, sem
alterar limite ou direitos. Não se descontam handles de console/herdados/provider.

O if combinado de GetProcessTimes/ExitCode também podia retornar0 após FALSE
se o erro não estivesse definido, fazendo o plano aparentar sucesso incompleto.
As duas chamadas foram separadas, com erro imediato e fallback31, sempre negando
falha. O caso foi reproduzido em modelo, não afirmado como ocorrido historicamente.

GetSecurityInfo mantém seus direitos, um único call, propagação DWORD,
tratamento de ponteiro/retention e falha em SD ausente. Só captura a primeira causa.
Nenhum handle foi fechado antes, removido ou reordenado para alcançar PASS.
Nenhuma quota, política de recursos, timestamp ou autoridade foi relaxada.

## Instrumentação no owner e tradução do wrapper

PAPACC_SCQ_COLD_DIAGNOSTIC:32bytes no packet privado existente. Campos:
stage/reference/mapped/native_error e query_attempted/query_succeeded/
query_reference/own_count. Primeira falha não é sobrescrita. Query fields vêm
exclusivamente da consulta original antes de FILE_OPEN, sem query adicional.
Em falha, count_known=0: own_count0 não significa contagem zero. Em erro de API
posterior, a contagem continua rotulada LAST/PRE_OPEN, não amostra no instante
do erro. Não há SID, token, nonce, caminho, HANDLE/pointer ou SD no diagnóstico.

Estágios1=count-query falha,2=guard124,3=descriptor-query,
4=process-times,5=process-exit. Reference0/1/2=request/decision/image;
reference3=não é referência de arquivo. Stage0 significa não coberto/não
alcançado, não PASS. O CLI imprime SCQ_COLD_FIRST_CAUSE antes do cleanup;
assim, causa/erro sobrevivem a APIs de fechamento que mudem LastError.
Não altera SCQ1/SQB1/SQB2, receipt/close, wire/API públicos ou dados persistidos.

O launcher PowerShell aprovado propagou exit2 como exceção na linha85. Essa
guarda é correta, permanece byte-exata e não é suprimida. A nova causa deve
ser obtida pelo código nativo, não por mascarar exitcode/erro no wrapper.

## Qualificação afetada

Selected/Legacy Debug/Release: QUATRO builds PASS, zero warnings finais,
mesma toolchain MSVC18/SDK/target existentes. Cinco testes por configuração,
20 pares PASS: cold_open, cold_controller, start_assurance, query_cold_chain,
memory_census. Sem campanhas históricas gerais ou operações protegidas reais.

cold_open:25cenários/81checks, incluindo consulta falha com erros5/1816/0,
123 permitido para aquisição unitária,124/128 negados, uma query por tentativa,
primeira causa preservada, SD nativo1816 e ponteiro incerto retido, ausência de
SD, tempos/exit com1816/0, Stop/replay/unresolved. APIs de arquivo/processo
substituídas por mocks; não prova contagem do host ou autenticação nativa.
cold_controller17/153; G3 assurance33casos+15 query-resource/555checks;
QUERY chain34/1755 e census size-only PASS. Teste de Stop/close UNKNOWN do
owner permanece no cold_controller; sem provocar fechamento real incerto.

Dois problemas de fixture/compilação na preparação foram corrigidos: comparação
signed/unsigned de WinError sob /WX e fixture que combinava erro0 com ponteiro
não nulo enquanto esperava SD ausente. Não se enfraqueceu predicado de produção
ou assertion para aceitar capacidade não demonstrada. Logs finais preservados.

Censo Selected KNOWN2001440bytes, antes2001408: +32, limite3145728 inalterado.
Legacy KNOWN1832872bytes. São armazenamento conhecido/sizeof, não memória
privada, working set ou headroom nativo. A instrumentação não adquire handles.

## Artefato e primeiro gate restante

Installed esperado permanece762880bytes/pin
3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b,
ID417614100:1900544:539611; não foi atualizado ou executado. O novo candidato
Selected/Release é763904bytes (+1024), SHA256
7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d.
DLLs mantêm pins3fb3cd78…d7ed/09eec573…3044. Não tratar o candidato como imagem
instalada, selecionada ou atestada. Não executar o workspace como controller.

A menor evidência factual que distinguirá as origens é UMA linha first-cause
do ponto original (stage/ref/erro/query_success/count_known/count). Não precisa
série temporal, tracing ou enumeração de handles de outros processos. O binário
instalado antigo não emite essa linha; autorização consumida não permite retry.
Uma aquisição futura diagnosticada demonstrará a causa DESSE novo evento, não
provará retroativamente qual dos11 ramos ocorreu na tentativa histórica.
Logo o primeiro gate operacional é disponibilizar legitimamente uma imagem
qualificada com o diagnóstico por UPDATE governado/autorização própria, antes
de formular eventual prova factual específica. Não se emite aqui comando de
UPDATE/sessão nem nova coleta. Avaliar esse caminho reutilizando U1–U7/U2 e o
updater protegido, sem outra infraestrutura ou re-materializar decisão consumida.

Se o dado futuro for stage2, só então será demonstrada a contagem na guarda;
se stage1, tratar a falha da consulta; stage3/4/5, tratar o erro nativo dessa API.
Não criar exceção normativa, aumentar124/128/G3 ou recomendar campanha genérica.
Caso retenção/histórico impeça qualquer operação futura, STOP e preservar UNKNOWN.

Decisão operacional C6 e original417614100:6225920:542803 preservados;
autorização25ea…dff4 utilizada, sem retry. Retenção histórica/OS_QUIESCENCE UNKNOWN;
E21 PENDING; ED11/Phase6 PARTIAL; produção DENY/PST0.6.4. Nenhum refund,
limpeza, recuperação, alteração SCM/ACL, chave, CA/CURRENT, rede ou TPM/NV/Q4.
