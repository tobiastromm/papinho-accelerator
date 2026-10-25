# P3 histórico exato — revalidação parcial e STOP

## Resultado

P3.1–P3.4 têm suporte no alcance histórico delimitado dos recibos, fontes e pins reconciliados. P3.5 **não foi comprovado integralmente**: os efeitos intent/imagem/sink/configuração/SCM foram conferidos por leitura, mas a consulta da cadeia do dossiê protegido parou na primeira abertura, `C:\ProgramData\PapinhoAccelerator`, com `PaccFileAcl.Open(..., false, true): Acesso negado`. Nenhuma identidade ou descriptor desse diretório foi obtido nesta consulta. Não houve consulta subsequente aos inputs nem retry, remediação ou bypass.

Não foi preparado pedido exato P3.6 nem registro da transição como se todos os requisitos estivessem satisfeitos. O pacote preparado na tarefa anterior continua histórico/PREPARED e não se torna elegível por esta revisão. O destino P3-EXACT-DISPOSITION-RECORDED.md permanece ausente. A regra normativa accepted não foi alterada.

## P3.1 — origem, integridade e identidade histórica

O manifesto baseline-manifest.json reconcilia a origem do recibo fornecido pelo owner, seu hash30889b7ab36e08e5c8ad57ee7d0ac2c7ec118566d6972aaa504de12e9017ed26, a aprovação histórica556167c630bf870797dbd2b970c6d5a5f2177cc06d731b1986c4aa30dbe18456, o request9f7443f233c4c08a052186a779e7e60f51b79d68594edab7f4d964ccd8a48f65, o qualifierb482b959400b41e6e64ecb86858dfd00522a0aa988b36300dc7d70f4d3c33eae, launcher histórico573bf1ca9a95dc052c992a0893b47f06f9dd96a92f53d3aa41cdf5bcc25951c2 e as fontes correspondentes. Todos os pins do manifesto foram conferidos. Não confundir com o launcher atualdeb0ab304110e5675057261df09e2057199db11884be5718c77a86ca1d382cec. A alteração posterior de console não é aplicada retroativamente à execução.

Norma de disposiçãoSHA8fcdea4ae9c639bef55a5a9ef97eeb93044a1ccf441b62a6347253602f596c6e e regras N0/ED11 foram confrontadas com o escopo exato. Host/boot do recibo permanecem identidade histórica, não observação atual ou permissão para rearmar. A prova depende de recibos de origem humana declarada e artefatos íntegros; não se fabrica assinatura/atestado nem auditoria universal do OS.

P3_1_ORIGIN=PROVEN_WITHIN_DECLARED_HISTORICAL_EVIDENCE_SCOPE.

## P3.2 — nenhuma chamada StartService

Recibo: first1816 em EXPLICIT_INSTRUMENTED_REBIND, starts0/stops0, intent_consumed1/effect2/diagnostic_created1. No código pinado, readback/recovery/files/status/censo e métricas vêm antes de cycle_stage=START_ONCE; ig_start incrementa starts antes de cycle_start_requested=1 e StartServiceW. O ramo que retorna1816 nessa observação termina antes desses pontos. Não há contagem0 interpretada isoladamente ou STOPPED como prova de não chamada. Chamadas fora deste caminho por outros administradores não são atestadas.

P3_2_NO_START=PROVEN_FOR_THIS_ATTEMPT. Stop também não foi chamado pelo caminho instrumentado. Nenhum PID de serviço ou token de processo não iniciado foi inventado.

## P3.3 — fechamento de referências próprias e borrows

ig_cleanup exige flags não retidas, fecha snapshot e aposenta a amostra/borrows B+D antes de bd_close; fecha eventos/guard, desbloqueia configuração, fecha aprovação/contexto N0, executa cycle_cleanup/ps_close e finish. bd_close fecha suas12 referências possíveis, zera binding/inventário e propaga erro. finish exige contexto sem pending/owner correto e fecha intent/lançadores, libera e fecha mutex com retorno conferido. O registro intent não é apagado. Um erro nesses passos leva a retenção e não ao IG_FINAL close0 observado.

O launcher histórico faz WaitForExit e obtém exit2; reconhece IG_FINAL com starts0, fecha reader/process/pins/locks e só então emite N0_CYCLE_EXIT2. Não há CLOSE_UNKNOWN ou IG_RETAINED terminal no recibo. A prova é agregada aos recursos efetivamente adquiridos, não inventário de todos os handles internos do OS. Nenhum CloseHandle histórico foi repetido nesta tarefa.

P3_3_LOCAL_CLOSE=CLOSED_EVIDENCED_IN_ORIGINAL_OWNER_LAUNCHER_SCOPE. Fechar uma referência ao auxiliar não prova término do auxiliar; close0 não vira quiescent1.

## P3.4 — nenhuma pendência conhecida nos owners

Os sources reconciliados mostram leitura/escrita/cópia/flush síncronos, operações SCM que retornaram, LockFileEx FAIL_IMMEDIATELY e unlock conferido, guards de pending_sd/retained/borrow e fechamento de snapshot antes do retorno. OVERLAPPED do lock não é prova de callback/I/O assíncrona pendente. Nenhum borrower, callback, IO, snapshot, lock ou operação SCM conhecido permanece sob esses owners no caminho de retorno observado.

Isso comprova apenas ausência de pendência **conhecida e controlada** nesse escopo, como exige P3.4. Kernel/cache/writeback/console e lifetime global continuam UNKNOWN, categoria E. Não inferir sua liberação pela saída do processo.

P3_4_NO_KNOWN_PENDING=PROVEN_IN_CONTROLLED_OWNER_SCOPE; GLOBAL_QUIESCENCE=UNPROVEN.

## P3.5 — inventário factual atualizado

Consultas somente por leitura reutilizaram o helper pinado scm-file-acl-native.ps1SHAf52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75, sem novas declarações nativas/instrumentação. Os arquivos read-*-query.ps1 são apenas orquestração documental das APIs existentes Open(false)/Identity/Read/GetSecurityInfo mask7/FileDigest/CloseHandle. Nenhum Apply/SetSecurityInfo, launcher LAB ou preflight foi invocado. Os próprios dados de evidência foram escritos somente nesta pasta de integração.

### Objetos conferidos

`current-file-observations.json` registra IDs/volume/tipo/ausência de reparse e hash do conteúdo pelo mesmo handle original, owner/group/DACL nativos RAW e repetição da identidade/descriptor antes/depois. O helper conferiu volume417614100, tamanho e LastWriteTime, e as referências próprias da consulta foram fechadas. Não são uma nova admissão nem uma transação atômica de todo o host.

| Objeto | Identidade nativa | Bytes/hash de conteúdo | Resultado |
|---|---|---|---|
| Pai de instalação |417614100:1966080:79426|RAW owner/group/DACL6483a0d3acb883e2af96b684fcaa5e39d0aeed696f2bb117fb4b388ddce70868|Mesmo pai observado; ACEs herdáveis AU históricas preservadas, sem remediação. Não é qualificação de destino sucessor.|
| Intent |417614100:2097152:2358097|103 /9e54d105d484c2201b5b280f9354038e9373f355f137c7e8a0debbca1ca45895|Mesmo objeto e conteúdo consumido; owneroperador/BA; DACL protegida operador/SYSTEM/BA FA.|
| Imagem |417614100:655360:2416599|434688 /eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b|Mesmo objeto/conteúdo; owneroperador, DACL protegida FA somenteSYSTEM/BA, demais ACEs0x1200a9 leitura/execução.|
| Sink/arm |417614100:1048576:2416813|1024 /240d658c0cd8fdc2fdd6980f81c3a6419b1c8427876b766a664d686ebdd661e0|Mesmo arm anteriormente validado; sem append; DACL protegida operador/SYSTEM/BA FA e serviço0x120083.|
| Configuração |417614100:393216:1120240|116 /a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44|Mesmo objeto/conteúdo, descriptor observado e preservado.|

Hashes RAW atuais de intent59c82694..., imagem3c7d08b9..., sinkd31c844a... e configuração76b04d3d... estão completos no JSON com bytesbase64. São observações atuais, não alegação de comparação RAW com baseline histórico inexistente. Relatos Get-Acl anteriores são descrições e podem serializar/ordenar ACEs diferentemente; não são substitutos RAW. Não normalizamos, reordenamos ou regravamos nada. SACL/auditoria/mandatory labels não consultados permanecem UNKNOWN.

SCM: todas as sete consultas sc qc/queryex/qfailure/qfailureflag/qtriggerinfo/qsidtype/sdshow retornaram0. Mantém ImagePath instrumentado antigo, conta virtual correta, DEMAND_START, SIDUNRESTRICTED, STOPPED/PID0/1077/specific0; nenhuma recovery action listada, reset0/comando e mensagem vazios, non-crashFALSE e nenhum trigger registrado. Arquivos sc-* preservam saídas brutas. Nenhum readback prova quiescência histórica.

### Primeira divergência e STOP

Na consulta independente dos inputs protegidos, o primeiro objeto era `C:\ProgramData\PapinhoAccelerator`, identidade esperada417614100:786432:1087874. Get-Item retornou, mas a aquisição nativa read-only Open(false,true) retornou Acesso negado. A exceção é Win32Exception capturada pelo helper CreateFileW/GetLastError; o registro conserva o texto de acesso negado, sem campo numérico NativeErrorCode separado. Nenhum handle desse grupo foi adquirido; o resultado contém observations vazio, close_errors vazio e descriptor_retainedfalse. Não houve reconsulta nem prosseguimento para Protected/N0/evidence/approved.

O fato não demonstra que a ACL foi alterada, que o arquivo sumiu ou que o dossiê foi substituído. Não permite distinguir sozinho token/privilégio do executor e proteção do alvo. Execução fora do sandbox não implica token UAC elevado suficiente. Não foi tentada elevação adicional/remediação para obter PASS. As identidades, hashes e RAW1 históricos do dossiê permanecem evidência antiga, insuficiente como verificação atual P3.5.

P3_5_PERSISTENT_EFFECTS=PARTIAL_NOT_PROVEN_DOSSIER_ROOT_ACCESS_DENIED.

## P2 e obrigações preservadas

O inventário p2-inventory-REVALIDATION-PARTIAL.json mantém A–F e as evidências/origem/identidades/owner/lifetime/obrigações/limites por categoria. A fechado apenas no alcance provado; B não iniciado; C lifetimes compartilhados não presumidos; D consumido/imagem/arm/SCM preservados, inputs protegidos sem revalidação concluída; E UNKNOWN_RETAINED; F allowance sem refund e sem transição de marcador aplicada. Sem nova tentativa, reserva, crédito, cleanup ou liberação de exclusividade.

## Próximo passo mínimo, sem execução

É necessária consulta read-only efetivamente habilitada a abrir a cadeia protegida, com operador/token apropriados, reaproveitando o leitor existente e seus pins. Antes de qualquer execução posterior, revisar o contexto/token de acesso; não concluir automaticamente defeito de ACL nem alterá-la. Esta tarefa terminou no STOP, sem retry ou comando positivo. Reapresentar todos os efeitos necessários se houver nova autorização/contexto de consulta e, somente após P3.5 completo, preparar pedido exato P3.6. O pacote anterior não autoriza registro neste estado.

CURRENT_TASK_COMPLETED=NO
P3_1_ORIGIN=PROVEN_HISTORICAL_SCOPE
P3_2_NO_START=PROVEN_FOR_THIS_ATTEMPT
P3_3_LOCAL_CLOSE=CLOSED_EVIDENCED
P3_4_NO_KNOWN_PENDING=PROVEN_CONTROLLED_OWNER_SCOPE
P3_5_PERSISTENT_EFFECTS=PARTIAL_STOP_ACCESS_DENIED_PROTECTED_ROOT
ADMINISTRATIVE_TRANSITION_APPLIED=NO
REFUND_AUTHORIZED=NO
SUCCESSOR_AUTHORITY=DENY
UNKNOWN_CHARGES=RETAINED
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=P3_5_NATIVE_OPEN_ACCESS_DENIED_C_PROGRAMDATA_PAPINHOACCELERATOR
NEXT=READ_ONLY_PROTECTED_DOSSIER_ACCESS_QUALIFICATION_BEFORE_P3_6
OWNER_REQUESTED_NEXT=OWNER_APPROVES_P3_EXACT_ADMINISTRATIVE_DISPOSITION (não atingido)
