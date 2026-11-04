# Pedido G5 — disposição excepcional dos dois participantes READ retidos

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. 2026-11-03. UMA execução MANUAL, sem retries. G0–G9 G5 exige decisão específica para TerminateProcess; autorização de UPDATE, teste, debug ou nova sessão não substitui essa decisão. Mesmo adaptador transitório já qualificado, sem serviço/owner/root/canal/protocolo/norma novos. Preparar comando somente após decisão efetivamente emitida, sem placeholders; não executar automaticamente.

## Disposição factual

Consulta POSTOFFER consumida/concluída: first0/completeTrue/observer_close_knownTrue; duas consultas SCM START_PENDING2/PID4792/win320/specific0. READ-POSTOFFER-PARTICIPANTS-RESULT.json registra saída colada pelo proprietário, não consulta nova pelo agente ou lease futuro. Correlação atual não prova autenticação bilateral original do runtime, receipt/close ou quiescência global. Sessão anterior7cf71cb4a6caf77fe500b457ca016710980ebb4e75f786a9b04de1619b149c9c confirmou E/Q/S/P, Startsubmission1 e falhou no retorno de transport_continue/step10/result_known0/mapped5; stage212/close5023/retained1. Não retomar/repetir sessão.

Objetos exclusivos deste pedido:

- CONTROLADOR PID5364, criação134382052521544528, TokenPrimary/grupos17, token48429409:0, authentication620529:0, modified48428675:0, User S-1-5-21-785699550-1440458426-1682014700-1108.
- RUNTIME PID4792, criação134382052843849000, TokenPrimary/grupos10, token48451502:0, authentication48451224:0, modified48451271:0, User S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099.

PID é somente locator. Adquirir e reter referências nativas, validar criação/liveness/path/token/estatísticas/SIDs/tipos e revalidar continuidade antes de cada efeito. Não atingir processo substituto por PID reuse, localizar outro runtime ou aplicar fallback se um objeto já saiu.

## Imagem e SCM frescos

Ambos path C:\Program Files\Papinho\Accelerator\papacc_server.exe. Reter leitura original on-disk872960bytes/ID417614100:3080192:866236, SHA256486d7971097087cf31dbe8ff43b7edc0b05ad2efdf5c06a6975dc56d0fb61932, link único/sem diretório/reparse. Pin on-disk não atesta páginas mapeadas. Não substituir EXE durante a disposição.

Host TCORP-TOBIAS, WindowsPowerShell5.1 Desktop AMD64/console original sem redirecionamento, operadorSID1108 administrador legítimo sem impersonação/autoelevação/SeDebug. SCMCONNECT1 e serviçoQUERY_CONFIG|QUERY_STATUS5. Conferir OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1, conta NT SERVICE\PapinhoAccelerator, SID_UNRESTRICTED1, comando exato "C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed; recovery actions count0. Antes da primeira submissão exigir START_PENDING2/PID4792/códigos0; divergência nega, sem editar configuração ou limpar erro.

## Efeitos máximos e ordem

Processos abertos sem herança com QUERY_LIMITED_INFORMATION|SYNCHRONIZE|TERMINATE0x00101001; tokensQUERY8. Nenhum token privado/reutilizável entregue, mudança de token/privilégio ou impersonação. Plano fixo apresentado e confirmação independente exata DISPOSE_READPOSTOFFER_5364_4792_ONCE; presença de arquivo/hash não confirma.

Após plano, revalidar SCM/configuração/ambos processos/tokens/imagem, consumir uma vez ANTES dos efeitos. No máximo:

1. UMA TerminateProcess no handle retido do runtime4792/criação134382052843849000, exitcode0xe0000001. Uma espera60000ms e GetExitCodeProcess para observar saída desse handle/código. Não retry se erro/timeout.
2. Conferir SCM STOPPED/PID0, configuração preservada, controlador original ainda válido e imagem original retida. Só então UMA TerminateProcess no handle retido do controlador5364/criação134382052521544528, mesmo exitcode. Uma espera60000ms e exitcode desse handle.
3. Conferir SCM STOPPED/PID0/configuração/imagem e fechar uma vez as próprias referências.

As esperas60000ms são limites escolhidos de observação, não teto nativo/duração garantida ou nova autorização de consulta repetida. Dois TerminateProcess fixos são o máximo, não obrigação de prosseguir após primeira falha. Se runtime encerra e etapa posterior falha, preservar resultado parcial/primeira causa/consumo e UNKNOWN; não repetir ou completar por comando alternativo. Se um objeto não corresponde, não encerrar qualquer substituto. Fechamento de observador não prova application-close.

SCM pode reportar1067 após término abrupto; é consequência esperada a registrar, não sucesso READ, Stop normal ou motivo para limpar códigos. Não emular SCQ close/receipt ou revogar/restaurar autorizações/reservas. Original-runtime/bilateral/probe/receipt-close não alcançados permanecem não provados; historical application-close/retenção/reservas/quiescência OS continuam UNKNOWN mesmo com exits observados.

## Riscos e limites

O proprietário deve reconhecer término abrupto dos dois objetos fixos, possível estado parcial, interrupção de operações/fechamentos internos, liberação OS sem close protocolar, erro SCM e retenção histórica persistente. Nenhuma garantia de durabilidade, close limpo, restituição de reservas, quiescência global ou recuperação produtiva. Sem rollback/retry/refund/cleanup/delete, sem novo Start/Stop/UPDATE/fixture/READ/E/Q/S/P ou canal. Não alterar ACL/SCM/configuração/recovery/DLLs/EXEs/chaves/originais. Sem CA/CURRENT/rede operacional/TPM/NV/Q4 ou produção. Preservar chave qualificada e chave parcial antiga, todos os .previous/originais/decisões consumidas/históricos/pins/HEAD/WIP/PST0.6.4.

## Qualificação e decisão solicitada

read-postoffer-retained-participants-disposition.CANDIDATE.cs SHA2560f3561d23f75cd0c26693a61aea368d0185d9bc4181e3f74a48535348651d49f. dispose-read-postoffer-retained-participants.MANUAL.CANDIDATE.ps1 SHA2560a9af3740cd68f40dc91f8b9e0daaf014ca8ad624c188883f7337b8783629976. Mesmo adaptador/duas chamadas fixas/17APIs nativas/guards e lifetime anteriores; atualizados todos os pins de ambos processos/tokens/imagem, sem copiar valores históricos não vinculados. Predicados reais de token extraídos em métodos privados puros e16casos adicionais PASS (cada campolow/high/grupos mutado por participante). Candidato compilado offline, nunca RunFixed; AST PASS/sem resposta automática.52casos de fluxo/modelo anteriores reutilizados sem campanha inalterada repetida. Nenhuma coleta, abertura de handle com término ou TerminateProcess pelo agente na qualificação.

Solicita autorização específica de UMA execução MANUAL dessa disposição excepcional, sem retry e sem execução automática, incluindo os riscos delimitados. Depois registrar decisão real em OWNER-READ-POSTOFFER-RETAINED-PARTICIPANTS-DISPOSITION-APPROVAL.txt, congelar hash e fornecer comando manual. Não fabricar aprovação agora.

Após saída: reconciliar primeira falha/consumo/runtime_submitted/runtime_exit/controller_submitted/controller_exit/SCM/observerclose/efeitos parciais e UNKNOWN. Somente se estado compatível, preparar UPDATE governado da diagnóstico focal de primeira causa do transporte, se qualificado offline, com seus gates frescos e autorização prospectiva existente; nova sessão e provas ainda independentes. Não instalar antes de resolver este gate operacional. Nenhum G5 é deduzido das autorizações anteriores.

Aprovação G5 anterior f11d20c0dcdc74d1a6a06aa23e8eb026d2a81f266ce401cc692e782e68a7a2a0 foi consumida pelos objetos anteriores15896/18532, ambos encerrados, e NÃO autoriza estes5364/4792. Decisão específica NOVA é indispensável. O ramo interno que produziu5 segue UNKNOWN; instrumentação focal pode distinguir retorno original sem consulta nova ou relaxamento de guard, não provar causa retroativa.
