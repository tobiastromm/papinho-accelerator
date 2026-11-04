# Pedido G5 — disposição excepcional dos dois participantes READ retidos

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. 2026-11-03. UMA execução MANUAL, sem retries. G0–G9 G5 exige decisão específica para TerminateProcess; autorização de UPDATE, teste, debug ou nova sessão não substitui essa decisão. Mesmo adaptador transitório já qualificado, sem serviço/owner/root/canal/protocolo/norma novos. Preparar comando somente após decisão efetivamente emitida, sem placeholders; não executar automaticamente.

## Disposição factual

Consulta V2 consumida/concluída: first0/completeTrue/observer_close_knownTrue; duas consultas SCM START_PENDING2/PID18532/win320/specific0. READ-OFFER-PARTICIPANTS-V2-RESULT.json registra saída colada pelo proprietário, não consulta nova pelo agente ou lease futuro. Correlação atual não prova autenticação bilateral original do runtime, receipt/close ou quiescência global. Sessão anterior40bc2ee4db303e0bf00075f4a1db7125149276dc44fe678d2a3ae9829b2ea361 confirmou E/Q/S/P, Startsubmission1 e falhou no retorno de offer_data/step5/result4/mapped5; stage212/close5023/retained1. Não retomar/repetir sessão.

Objetos exclusivos deste pedido:

- CONTROLADOR PID15896, criação134382037035307596, TokenPrimary/grupos17, token40848941:0, authentication620529:0, modified40848611:0, User S-1-5-21-785699550-1440458426-1682014700-1108.
- RUNTIME PID18532, criação134382037367289066, TokenPrimary/grupos10, token40862347:0, authentication40861824:0, modified40861885:0, User S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099.

PID é somente locator. Adquirir e reter referências nativas, validar criação/liveness/path/token/estatísticas/SIDs/tipos e revalidar continuidade antes de cada efeito. Não atingir processo substituto por PID reuse, localizar outro runtime ou aplicar fallback se um objeto já saiu.

## Imagem e SCM frescos

Ambos path C:\Program Files\Papinho\Accelerator\papacc_server.exe. Reter leitura original on-disk872960bytes/ID417614100:1310720:606807, SHA2563eb8e497a05294f03bc82e61bf68c8741f712502edc872b6d367cf679e9f9d79, link único/sem diretório/reparse. Pin on-disk não atesta páginas mapeadas. Não substituir EXE durante a disposição.

Host TCORP-TOBIAS, WindowsPowerShell5.1 Desktop AMD64/console original sem redirecionamento, operadorSID1108 administrador legítimo sem impersonação/autoelevação/SeDebug. SCMCONNECT1 e serviçoQUERY_CONFIG|QUERY_STATUS5. Conferir OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1, conta NT SERVICE\PapinhoAccelerator, SID_UNRESTRICTED1, comando exato "C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed; recovery actions count0. Antes da primeira submissão exigir START_PENDING2/PID18532/códigos0; divergência nega, sem editar configuração ou limpar erro.

## Efeitos máximos e ordem

Processos abertos sem herança com QUERY_LIMITED_INFORMATION|SYNCHRONIZE|TERMINATE0x00101001; tokensQUERY8. Nenhum token privado/reutilizável entregue, mudança de token/privilégio ou impersonação. Plano fixo apresentado e confirmação independente exata DISPOSE_READOFFER_15896_18532_ONCE; presença de arquivo/hash não confirma.

Após plano, revalidar SCM/configuração/ambos processos/tokens/imagem, consumir uma vez ANTES dos efeitos. No máximo:

1. UMA TerminateProcess no handle retido do runtime18532/criação134382037367289066, exitcode0xe0000001. Uma espera60000ms e GetExitCodeProcess para observar saída desse handle/código. Não retry se erro/timeout.
2. Conferir SCM STOPPED/PID0, configuração preservada, controlador original ainda válido e imagem original retida. Só então UMA TerminateProcess no handle retido do controlador15896/criação134382037035307596, mesmo exitcode. Uma espera60000ms e exitcode desse handle.
3. Conferir SCM STOPPED/PID0/configuração/imagem e fechar uma vez as próprias referências.

As esperas60000ms são limites escolhidos de observação, não teto nativo/duração garantida ou nova autorização de consulta repetida. Dois TerminateProcess fixos são o máximo, não obrigação de prosseguir após primeira falha. Se runtime encerra e etapa posterior falha, preservar resultado parcial/primeira causa/consumo e UNKNOWN; não repetir ou completar por comando alternativo. Se um objeto não corresponde, não encerrar qualquer substituto. Fechamento de observador não prova application-close.

SCM pode reportar1067 após término abrupto; é consequência esperada a registrar, não sucesso READ, Stop normal ou motivo para limpar códigos. Não emular SCQ close/receipt ou revogar/restaurar autorizações/reservas. Original-runtime/bilateral/probe/receipt-close não alcançados permanecem não provados; historical application-close/retenção/reservas/quiescência OS continuam UNKNOWN mesmo com exits observados.

## Riscos e limites

O proprietário deve reconhecer término abrupto dos dois objetos fixos, possível estado parcial, interrupção de operações/fechamentos internos, liberação OS sem close protocolar, erro SCM e retenção histórica persistente. Nenhuma garantia de durabilidade, close limpo, restituição de reservas, quiescência global ou recuperação produtiva. Sem rollback/retry/refund/cleanup/delete, sem novo Start/Stop/UPDATE/fixture/READ/E/Q/S/P ou canal. Não alterar ACL/SCM/configuração/recovery/DLLs/EXEs/chaves/originais. Sem CA/CURRENT/rede operacional/TPM/NV/Q4 ou produção. Preservar chave qualificada e chave parcial antiga, todos os .previous/originais/decisões consumidas/históricos/pins/HEAD/WIP/PST0.6.4.

## Qualificação e decisão solicitada

read-offer-retained-participants-disposition.CANDIDATE.cs SHA2562c9e8790e83fb90cbd932d52f5bfa450a18e81be414cd92832d6b366a37823c3. dispose-read-offer-retained-participants.MANUAL.CANDIDATE.ps1 SHA2569b055fe09dc6f48587376c1a457f4c12e4c2560328c77f8c2211499f432bc528. Mesmo adaptador/duas chamadas fixas/17APIs nativas/guards e lifetime anteriores; atualizados todos os pins de ambos processos/tokens/imagem, sem copiar valores históricos não vinculados. Predicados reais de token extraídos em métodos privados puros e16casos adicionais PASS (cada campolow/high/grupos mutado por participante). Candidato compilado offline, nunca RunFixed; AST PASS/sem resposta automática.52casos de fluxo/modelo anteriores reutilizados sem campanha inalterada repetida. Nenhuma coleta, abertura de handle com término ou TerminateProcess pelo agente na qualificação.

Solicita autorização específica de UMA execução MANUAL dessa disposição excepcional, sem retry e sem execução automática, incluindo os riscos delimitados. Depois registrar decisão real em OWNER-READ-OFFER-RETAINED-PARTICIPANTS-DISPOSITION-APPROVAL.txt, congelar hash e fornecer comando manual. Não fabricar aprovação agora.

Após saída: reconciliar primeira falha/consumo/runtime_submitted/runtime_exit/controller_submitted/controller_exit/SCM/observerclose/efeitos parciais e UNKNOWN. Somente se estado compatível, preparar UPDATE governado da correção READ já qualificada offline, com seus gates frescos e autorização prospectiva existente; nova sessão e provas ainda independentes. Não instalar antes de resolver este gate operacional. Nenhum G5 é deduzido das autorizações anteriores.
