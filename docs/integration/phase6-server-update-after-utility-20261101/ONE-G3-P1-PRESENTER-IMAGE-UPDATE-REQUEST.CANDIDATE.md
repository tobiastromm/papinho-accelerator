# Pedido operacional — UPDATE focal do presenter P1 TRANSPORT_ONLY

2026-11-02. PERSONAL_LOCAL / BINARY_UPDATE_CLOSED / uma tentativa manual.
Este pedido materializa a autorização contextual do proprietário desta tarefa;
nenhum efeito foi executado. Hash/DATA/presença de arquivo não são autoridade.

## Contratos, finalidade e qualificação

Reutilizar U1–U7/U2 accepted, mesmo owner e updater independente protegido.
DECISION-personal-binary-update-u1-u7-20261101.md incorpora
9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234;
DECISION-personal-update-executor-data-u2-20261101.md incorpora
58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Não alterar esses contratos. A correção é de apresentação: P1 TRANSPORT_ONLY
mantém campos ProgramData vazios; presenter exibe não aplicável em vez de exigir
strings inexistentes. Não descobre ProgramData, não fabrica campos e não concede
confirmação. READ/demais modos preservados. Ver G3-P1-PRESENTER-QUALIFICATION.md
/json: quatro builds sem warnings, 56 pares PASS, 12 casos de projeção e143
falhas de escrita, APIs/console de teste simulados. Sem nova ADR/owner/canal.

G3-P1 permanece entry128/guard188/cap192 somente no seu domínio; nenhum limite
G3 transportado para UPDATE. Todos os demais parâmetros e formatos preservados.

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
784896 bytes, ID417614100:1835008:74889, SHA256
201b0f01d0d6b33f73d653bd492567ff663f14e183170ced4d18a6a6f0ddb752.
Origem histórica: G3-P1-RUNTIME-SID-IMAGE-UPDATE-RESULT.json, status
G3_P1_RUNTIME_SID_BINARY_UPDATE_VERIFIED_CLOSED. Estado não consultado nesta tarefa.

Novo Selected/Release exclusivamente DATA:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,
785408 bytes, SHA256
f572949bb6b88acc396b81f48b1e527d3b3e680ba1d36c0a9689c5d49134262d.
Congelamento workspace papacc_server.g3-p1-presenter-785408.exe, mesmos bytes.
Delta +512 bytes. Sem migração silenciosa de artefato. Capturar identidade da
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
   copiar785408 bytes da referência DATA retida, conferir hash, flush,
   identidade/proteção e revalidar originais antes dos renames.
3. Renomear antigo para .previous exclusivo sem overwrite, RootDirectory=NULL.
4. Renomear candidato para canônico sem overwrite; readback de novo objeto,
   conteúdo/hash/SD/identidade, anterior preservado e SCM STOPPED/PID0.
5. Fechar uma vez quando demonstrado. Falha parcial/Stop/close incerto preserva
   objetos, coorte, reservas e retenção UNKNOWN. Sem rollback/cleanup/retry.

Preservar absolutamente todos os históricos, candidatos/.previous, decisões e
originais anteriores; inclusive candidato742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b,
ID417614100:1769472:537456, e os originais da sessão consumida
cce1bb4ce0a0f47ab19d2506cbf5908564949bb9a1107cba61f085017b37cbaa:
pedido417614100:11599872:11740 e decisão417614100:1048576:290736.
Novo fechamento conhecido não resolve retenção anterior por inferência.
UNKNOWN material/conflito/lock impeditivo => STOP; não refund/retry.
Pode existir intervalo sem canônico ou substituição parcial. Não presumir
atomicidade, durabilidade de metadata ou recuperação automática.
DLLs/updater/configuração SCM/ProgramData/CA/CURRENT não alterados.

## Recursos U4, Stop e riscos

K do MESMO updater protegido2027314 bytes; Selected3145728 inalterado.
Não substituir por censo do servidor novo. Físico/commit disponíveis>=K,
com margem fresca antes de cada submissão mutante. R=ceil(785408/U)*U com
geometria real congelada/revalidada e overflow checado; exemplo U4096 =>786432
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

## Procedimento manual e evidência futura

update-g3-p1-presenter-image.MANUAL.CANDIDATE.ps1, SHA256 9c031a0da99aded2f60bb57d939f55fff05db4708ba7be74f9506e8f5b3dcae8.
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
reconciliação deste UPDATE. A autorização contextual de teste será registrada
sem fabricar confirmação/ID antecipados. Não reutilizar sessão consumida nem
inferir autenticação bilateral/receipt/close/RUNNING diagnóstico ou produtivo.

Proibidos UPDATE adicional, coleta, endpoint/SCQ/Start/Stop nesta operação,
Start produtivo, SCM/ACL, CA/CURRENT, rede operacional, TPM/NV/Q4,
limpeza/exclusão/restauração/rollback/refund/recuperação. Preservar HEAD/WIP,
históricos/pins/EXEs/retidos/reservas, DENY, UNKNOWN e PST0.6.4.
