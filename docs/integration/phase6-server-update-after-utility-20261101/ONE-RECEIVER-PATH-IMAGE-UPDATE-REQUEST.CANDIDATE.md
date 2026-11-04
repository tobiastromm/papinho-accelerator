# Pedido operacional — UPDATE da seleção P1 receptora

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma execução MANUAL
BINARY_UPDATE_CLOSED PERSONAL_LOCAL, sem retry; sem Start ou nova sessão E/Q/S.

## Correção e limites

Ingress receptor exigia profile.pre_readiness antes de first_prepare definir esse
campo. O perfil vazio fazia QUERY P1 cair em paths_new. Corrigida dependência
circular: seleção de aquisição pelo estado fechado P1 pré-preparação; estado
incompleto nega, sem fallback NEW_INSTALL. Identidade/cadeia/proteção/continuidade/
OBJ_DONT_REPARSE/binding/bilateral/G3/receipt-close/Q6 permanecem obrigatórios.
Não prova que esse defeito causou o timeout histórico; primeira rejeição do receptor
continua UNKNOWN. Sem formato/CLI/autoridade/quotas novos. Demais ramos preservados.
Quatro builds sem warnings e76pares afetados PASS. Modelo cold_chain exercita o
predicado real mas modela conteúdo/SD; não prova execução nativa do ingress inteiro.
Ver RECEIVER-PATH-OFFLINE-CLOSURE-REPORT.md e qualificação correspondente.

## Contratos e autoridade

Reutilizar integralmente U1–U7/U2 accepted, mesmo owner e papacc_update.exe protegido.
Propostas incorporadas9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234
e58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Nenhuma nova decisão de arquitetura ou exceção de autoridade. A correção recompõe a seleção P1 receptora antes da preparação do perfil,
sem protocolo/serviço/canal novo.
Aprovação normativa anterior ou disposição não autoriza este UPDATE operacional.

Host TCORP-TOBIAS, PowerShell5.1Desktop AMD64, operador original
S-1-5-21-785699550-1440458426-1682014700-1108 elevado legítimo e console original,
sem autoelevação/impersonação. Declaração e confirmação atribuídas OWNER_BINARY_UPDATE
do plano nativo exato, sem resposta automática. Consumo único antes dos efeitos.

## Bytes e originais esperados

Novo Selected/Release exclusivamente DATA:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe
805376bytes, SHA256174db00a075b55433fdc793f23d9c9412f982814eef8c650579a824afc567f8c.
Congelado papacc_server.receiver-path-805376.exe na integração, mesmos bytes. Delta de tamanho zero; conteúdo diferente.
Somente o caminho build acima será passado ao updater. Adquirir referência original
DATA retida, inspecionar SD, provar identidade/continuidade/hash/conteúdo. Não executar
DATA, exigir ACL instalada para ela ou transformar posse/hash em autoridade.

Canônico anterior C:\Program Files\Papinho\Accelerator\papacc_server.exe:
805376bytes, ID417614100:4194304:339059,
SHA256044f10de34010b83075976ba39599a92381bce483ade9f780960b0fcc73010b1.
Fonte WAIT-STATE-IMAGE-UPDATE-RESULT.json/UPDATE reconciliado, ID417614100:4194304:339059.
Não é lease fresco ou atestação de bytes mapeados. Revalidar no owner.

Executor independente C:\Program Files\Papinho\Accelerator\papacc_update.exe:
104448bytes, ID417614100:30670848:57859,
SHA256084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
Parent instalado ID417614100:8257536:71209, âncora histórica417614100:131072:226316.
Owner/DACL estritos, ancestrais, ausência de reparse, identidade/proteção efetiva e
continuidade obrigatórios; não presumir autoridade por string/ID/token/imagem.
Rejeitar aliases/hardlinks entre DATA/executor/canônico; não atualizar o updater.

DLLs apenas conferir, sem substituir:
libssl-3-x64.dll1321472bytes/pin3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
libcrypto-3-x64.dll7217664bytes/pin09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Bytes do build preservados; instalados exigem reaquisição/validação fresca.

## Disposição anterior e SCM

Sessão WAIT_STATE5bf09792d603754b94e9117cb6514895081f16414c48482e731108ad6ba29618
consumida: receipt1052 operation2/pending1/offset0, timeout1460/212/retained1.
Consulta e6e06c85… consumida e disposição específica
9bd9caf5b87b515d8380958cdcab02e3efad381647a69768853be15eb6feff8c/
decisão2bd42b0e6a7f9b743dce62620ffbe37519940eb84bf4dbd2712a932e5d7a4790:
first0/consumedTrue, runtime8760/criação134381401485994342 e controlador12976/
criação134381401233458253 tiveram saídas observadas; observer_close_knownTrue,
SCM STOPPED observado duas vezes/1067/specific0. Saída fornecida pelo proprietário,
registrada em WAITSTATE-RETAINED-DISPOSITION-RESULT.json. Não foi nova coleta do agente.
Remove impedimento desses dois processos, não prova application close/SCQ close/
bilateral/receipt/reservas/quiescência OS. UNKNOWN material impeditivo mantém DENY.
Todas as autorizações anteriores consumidas; não reutilizar. Sem limpeza/refund.

Exigir SCM STOPPED/PID0/configuração fresca antes dos efeitos e no readback final:
PapinhoAccelerator, NT SERVICE\PapinhoAccelerator, OWN_PROCESS16/DEMAND_START3/
ERROR_NORMAL1/SID_UNRESTRICTED1, comando exato
"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed.
Não modificar configuração/ACL ou limpar1067. STOPPED anterior não é lease permanente.

## Efeitos limitados, fechamento e riscos

Uma chamada do updater com16argv. Tentativa CSPRNG32 original gera dois leaves
exclusivos papacc_server.<hex64>.candidate e papacc_server.<hex64>.previous;
nomes concretos apresentados antes dos efeitos, sem nonce/ID futuro inventado.

1. Revalidar operador/executor/DATA/antigo/DLLs/parent/ancestrais/SCM/recursos/plano.
2. Consumir uma vez antes de uma criação exclusiva protegida BA/SY full e AU RX.
Copiar805376bytes, hash/flush/readback e revalidar originais antes dos renames.
3. Rename canônico->previous exclusivo sem overwrite, RootDirectory=NULL.
4. Rename candidato->canônico sem overwrite; readback de identidade/conteúdo/SD,
referência anterior retida e SCM STOPPED/PID0.
5. Fechar uma vez quando demonstrado. Stop/erro/parcial/close incerto mantém
consumo/objetos/reservas/UNKNOWN, sem retry, rollback, restauração ou limpeza.

Preservar todos os previous/candidatos/originais históricos, inclusive o previous
8af30fe5… e os objetos das sessões WS3M/SQB3. Não adotar/sobrescrever/excluir para
contornar exclusividade. Não tocar DLLs/updater/ProgramData/CA/CURRENT/configuração.

U4 próprio, sem transportar G3/B0/R0: K2027314bytes do mesmo updater inalterado,
Selected3145728. Físico/commit e margens frescas conforme gates existentes.
R=ceil(805376/U)*U com geometria real original, revalidação e overflow checado;
exemploU4096=>806912bytes é cálculo, não consulta atual. Livre caller/actual>=R
antes de criar e restante arredondado antes de escrever; retidos ocupam espaço.

Custos loader/CRT/alocador/SD/hash/provider/SCM/RPC/kernel/cache/filesystem/flush/
rename/log continuam opacos. Não garantir atomicidade, rollback, suficiência,
durabilidade, cancelamento ou latência. Há risco de parcialidade, intervalo sem
EXE canônico e retenção persistente. Divergência/insuficiência/Stop/UNKNOWN impeditivo
nega sem ajustar ACL ou quota. Efeitos persistentes limitados ao novo EXE e anterior retido.

## Procedimento e decisão solicitada

update-receiver-path-image.MANUAL.CANDIDATE.ps1, SHA256 3decf073d85867a007bca9084253f161f56c6f633842abec049b96631c481cdf.
Mesmas primitivas/helpers do wrapper anterior, deltas de nomes/pins/tamanhos/ID.
AST22bindings/16argv/uma chamada futura qualificados offline; nenhum wrapper/helper
nativo executado. Preflight OLD fecha antes de DELETE/rename; close incerto bloqueia.
Owner readquire OLD e revalida; snapshot não substitui continuidade nativa.

Solicita-se aprovação específica dos bytes deste pedido e do candidato805376/pin174db00a075b55433fdc793f23d9c9412f982814eef8c650579a824afc567f8c
para UMA execução MANUAL, reconhecendo os riscos. Depois da decisão real, registrar
OWNER-RECEIVER-PATH-UPDATE-APPROVAL.txt e seu hash, preparar comando com hashes reais.
Esse arquivo ainda não existe; não fabricar autorização nem executar automaticamente.

Após saída manual: reconciliar primeira falha/consumo/criação/dois renames/anterior/
novo canônico/ID/hash/SD/readback/SCM/close. Exit0 sozinho não basta. Depois, preparar
pedido independente de nova sessão E/Q/S para validar a correção do ramo receptor P1;
Start não é autorizado aqui. Se a nova linha localizar apenas uma fase, a causa
interna do receptor ainda poderá exigir prova focal adicional; não prometer resolução.

Sem UPDATE automático, retry, Start/Stop, endpoint/SCQ, SCM/ACL edit, CA/CURRENT,
rede, TPM/NV/Q4, refund, cleanup/rollback/restauração/recuperação ou produção.
Preservar HEAD/WIP/históricos/pins/EXEs/UNKNOWN/DENY/PST0.6.4.
