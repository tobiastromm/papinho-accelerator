# Pedido operacional — UPDATE do piso de commit P1 TRANSPORT_ONLY

2026-11-02. PERSONAL_LOCAL / BINARY_UPDATE_CLOSED / uma tentativa manual.
CANDIDATE / NOT_OPERATIONALLY_AUTHORIZED / NOT_EXECUTED.
A tarefa atual autoriza exclusivamente norma/integração offline. Este pedido
solicita autorização específica futura; não reutiliza aprovação operacional
anterior. Hash/DATA/presença de arquivo não são autoridade.

## Contratos, finalidade e qualificação

Reutilizar U1–U7/U2 accepted, mesmo owner e updater independente protegido.
DECISION-personal-binary-update-u1-u7-20261101.md incorpora
9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234;
DECISION-personal-update-executor-data-u2-20261101.md incorpora
58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Não alterar esses contratos. Disponibilizar a especialização B de piso de
commit P1 aprovada em DECISION-personal-scq-p1-commit-floor-20261102.md, proposta
807ab4645cea56bad200e032e02bfa29c9de994f4eea91e2b13612948ec3a220.
Somente P1 PERSONAL_LOCAL TRANSPORT_ONLY passa a8589934592bytes/8GiB.
READ/NEW_INSTALL/TO sem P1 e demais perfis mantêm o piso anterior. Valor é
risco administrativo, não provisão ou máximo nativo demonstrado. Não ampliar
G3-P1 para UPDATE: este continua U1–U7/U2 com K/R próprios.

Quatro builds Selected/Legacy Debug/Release PASS sem warnings,68 pares PASS;
fronteiras8GiB, isolamento, policy divergente, seleção ausente, overflow,
consulta indisponível, Stop/UNKNOWN/readiness prematura. Fontes e hashes em
G3-P1-COMMIT-QUALIFICATION.json e relatório G3-P1-COMMIT-INTEGRATION-REPORT.md.
Mantidos128/188/192, físico7239172096, private/WS1994944, freshness100ms,
janela240000ms/global60000ms e todos os demais contratos/gates. Policy G3 novo
256fe6ab14dcc5554994b589017b1c8e7d7817bcdac89982d44d73048a31cafd nas duas
pontas. Sem mudança wire/persistido/owner/canal/serviço. A instalação da nova
imagem não prova admissão ou autenticação do Start. Não executar teste nesta operação.

## Seleção concreta e contexto original

Operador esperado S-1-5-21-785699550-1440458426-1682014700-1108,
host TCORP-TOBIAS; contexto administrativo legítimo original e confirmação
atribuída OWNER_BINARY_UPDATE do plano imutável são obrigatórios.

Parent C:\Program Files\Papinho\Accelerator, ID417614100:8257536:71209.
ProgramFiles x64 histórico ID417614100:131072:226316. Adquirir/reter/revalidar
identidade, proteção owner/DACL estritos, ancestrais, ausência de reparse e
continuidade pelos mecanismos existentes. Não modificar ACLs.

Executor C:\Program Files\Papinho\Accelerator\papacc_update.exe,
104448 bytes, ID417614100:30670848:57859, SHA256
084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
Não executar outro updater nem permitir que DATA seja executor.

Canônico anterior C:\Program Files\Papinho\Accelerator\papacc_server.exe,
786944 bytes, ID417614100:917504:303885, SHA256
a975ec0303c6c04d7d6e48ae1ebf803976b47a4f42c1996affc1eab21d8e0ce0.
Origem histórica: G3-P1-ADMISSION-IMAGE-UPDATE-RESULT.json, status
G3_P1_ADMISSION_BINARY_UPDATE_VERIFIED_CLOSED. Estado não consultado nesta tarefa.

Novo Selected/Release exclusivamente DATA:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,
786944 bytes, SHA256
40b708daadae4342e5cd90090d99189234437ab84f47b2d00f76aeaab05eba76.
Congelamento workspace papacc_server.g3-p1-commit-786944.exe, mesmos bytes.
Delta de tamanho 0 bytes; conteúdo diferente. Sem migração silenciosa de artefato. Capturar identidade da
DATA na referência original retida, inspecionar SD sem conceder autoridade à
origem, impedir escrita/substituição, conferir hash/conteúdo/continuidade.
Rejeitar alias/hardlink entre DATA/executor/canônico. Não inventar ID futuro.

DLLs somente verificar, não substituir:
- libssl-3-x64.dll:1321472 bytes, SHA256 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
- libcrypto-3-x64.dll:7217664 bytes, SHA256 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.

SCM PapinhoAccelerator: OWN_PROCESS16, DEMAND_START3, ERROR_NORMAL1,
NT SERVICE\PapinhoAccelerator, SID UNRESTRICTED1; comando exato
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Readback fresco SERVICE_STOPPED/PID0/configuração antes dos efeitos e após
substituição. Expectativas históricas não são lease. Divergência => STOP.

## Efeitos delimitados U1/U3/U5/U6

Uma única invocação do updater protegido existente. Tentativa CSPRNG32 gerada
pelo owner real, dois leaves concretos exclusivos mostrados no plano:
papacc_server.<hex64>.candidate e papacc_server.<hex64>.previous.
Conferir plano/IDs/pins/K/R/nome/escopo antes da confirmação independente.

1. Revalidar executor/DATA/antigo/DLLs/parent/ancestrais/token/SCM/recursos;
   declaração e confirmação atribuída, consumo único antes de qualquer efeito.
2. Criar candidato FILE_CREATE exclusivo com DACL protegida BA/SY full/AU RX;
   copiar786944 bytes da referência DATA retida, conferir hash, flush,
   identidade/proteção e revalidar originais antes dos renames.
3. Renomear antigo para .previous exclusivo sem overwrite, RootDirectory=NULL.
4. Renomear candidato para canônico sem overwrite; readback de novo objeto,
   conteúdo/hash/SD/identidade, anterior preservado e SCM STOPPED/PID0.
5. Fechar uma vez quando demonstrado. Falha parcial/Stop/close incerto preserva
   objetos, coorte, reservas e retenção UNKNOWN. Sem rollback/cleanup/retry.

Preservar absolutamente todos os históricos, candidatos/.previous, decisões e
originais anteriores; inclusive candidato742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b,
ID417614100:1769472:537456, e os originais das sessões consumidas. Última sessão
f87d9281c4f9926fdc04af50006b48ac881d9dc466c392d475e941fe54299d12,
pedido417614100:15925248:65193/decisão417614100:1507328:290169, origem validada,
negou por commit9081241600<piso antigo9823326208, sem efeito de Start. Não
reutilizar. Preservar também59a5a75925b52590d0db90eb0bdb436cc7dd9ffe1a3cb0a81200b21ff53c399a,
pedido417614100:9961472:64054/decisão417614100:11206656:64376, confirmou Q/E/S,
negou pré-namespace/Start com retorno5 sem subpredicado. Não repetir. Preservar
também cce1bb4ce0a0f47ab19d2506cbf5908564949bb9a1107cba61f085017b37cbaa,
pedido417614100:11599872:11740 e decisão417614100:1048576:290736.
Novo fechamento conhecido não resolve retenção anterior por inferência.
UNKNOWN material/conflito/lock impeditivo => STOP; não refund/retry.
Pode existir intervalo sem canônico ou substituição parcial. Não presumir
atomicidade, durabilidade de metadata ou recuperação automática.
DLLs/updater/configuração SCM/ProgramData/CA/CURRENT não alterados.

## Recursos U4, Stop e riscos

K do MESMO updater protegido2027314 bytes; Selected3145728 inalterado.
Não substituir por censo do servidor novo. Físico/commit disponíveis>=K,
com margem fresca antes de cada submissão mutante. R=ceil(786944/U)*U com
geometria real congelada/revalidada e overflow checado; exemplo U4096 =>790528
bytes, não observação atual. Caller-free e actual-free>=R antes de criar e
payload restante arredondado antes de escrever. Não importar2R/B0–B8/R0–R9/G3.

Loader/CRT/wrapper/alocador/SD/hash/filesystem/SCM/RPC/cache/flush/rename/kernel e
metadados permanecem opacos. Margens não demonstram máximos nativos ou latência.
Custos transitórios, retenção persistente e espaço de objetos anteriores não
são zero. Quiescência OS e retenção histórica UNKNOWN.

Revalidar imediatamente antes de cada efeito e após confirmação/outra chamada;
sem TTL inventado. Esc cancela. Ctrl+C/Break sinaliza Stop do owner, não chama
ControlService. Não prometer cancelamento imediato de I/O. Identidade/proteção/
SCM/recursos/Stop/UNKNOWN impeditivo nega próximo efeito; não alterar guards,
ACLs ou sharing para obter PASS. Sem rollback, restauração ou limpeza.

## Decisão operacional solicitada

Solicita-se selecionar exatamente o EXE786944/SHA256
40b708daadae4342e5cd90090d99189234437ab84f47b2d00f76aeaab05eba76 e autorizar
UMA execução manual exclusivamente deste UPDATE, reconhecendo riscos U1–U7/U2
opacos/parcialidade/intervalo sem canônico/retenção persistente/sem rollback.
Após aprovação conferir e registrar ato real/hash, preparar comando manual
byte-exato com pins do pedido/decisão/procedimento/executáveis. Não executar
pelo agente, não fornecer confirmação nativa automaticamente. Ausência de
aprovação específica mantém DENY. Não conceder endpoint/SCQ/Start/Stop.

## Procedimento manual e evidência futura

update-g3-p1-commit-image.MANUAL.CANDIDATE.ps1, SHA256 4a20bbbe618fc0a5696c250c1fd037e36268b30756d9d7c5deeec236917066e0.
Mesmo wrapper qualificado: seis helpers preservados, 16 argumentos e UMA
chamada ao updater protegido; parser AST sem executar definições ou preflight.
Retém DATA/executor/DLLs/cadeias. Libera lock readonly do antigo antes de abrir
DELETE no owner, sem alegar continuidade nesse intervalo; owner readquire e
revalida nativamente. Falha de close nega chamada. Sem resposta automática.
PowerShell5.1 Desktop x64 legítimo, sem autoelevação. Conferir hashes da decisão,
pedido, procedimento e executáveis antes da chamada. Não executar pelo agente.

Após saída integral: primeira falha, consumo/efeitos/dois renames, anterior
retido, canônico ID/bytes/hash/SD/readback, SCM parado, fechamento e retenção.
Não concluir pelo exit isolado. Autorização utilizada não se repete.

Novo pedido E/Q/S deve vincular a identidade criada efetivamente, somente após
reconciliação deste UPDATE. Será necessária autorização operacional específica da sessão,
sem fabricar confirmação/ID antecipados ou converter a aceitação offline em ato real. Não reutilizar sessão consumida nem
inferir autenticação bilateral/receipt/close/RUNNING diagnóstico ou produtivo.

Proibidos UPDATE adicional, coleta, endpoint/SCQ/Start/Stop nesta operação,
Start produtivo, SCM/ACL, CA/CURRENT, rede operacional, TPM/NV/Q4,
limpeza/exclusão/restauração/rollback/refund/recuperação. Preservar HEAD/WIP,
históricos/pins/EXEs/retidos/reservas, DENY, UNKNOWN e PST0.6.4.
