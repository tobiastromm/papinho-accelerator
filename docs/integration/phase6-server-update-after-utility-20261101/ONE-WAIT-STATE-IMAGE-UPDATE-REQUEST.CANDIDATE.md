# Pedido operacional — UPDATE da projeção original do estado da espera

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma execução MANUAL BINARY_UPDATE_CLOSED
PERSONAL_LOCAL, sem retry. Não autoriza Start ou uma nova sessão E/Q/S.

## Finalidade e limite da prova

Instalar somente o delta diagnóstico em personal_scq_start_wait_win32.inc:
no retorno original WAIT_TIMEOUT/1460, imprimir fase/operação/pending/offset/total/
legs e máscaras de presença já existentes no pacote, antes de Stop/retenção.
Nenhum novo handle, consulta nativa, relógio, acesso ao peer, buffer persistente,
wire, autoridade, guard, quota ou mudança de comportamento da espera.
Não imprimir handles, ponteiros, tokens, SID, nonce, paths ou payload.

Isso permite distinguir a última operação registrada (conexão/bootstrap/receipt/
close/retorno) em eventual NOVA sessão separadamente autorizada. Não corrige por si
o timeout, não recupera o estado de processos já terminados e não captura diretamente
a primeira rejeição interna do receptor. pending e máscaras são estado do owner,
não nova consulta ao kernel nem comprovação de entrega/autenticação/close.
Valores0 não provam ausência de problema. A causa histórica continua UNKNOWN.

Quatro builds Selected/Legacy Debug/Release sem warnings e quatro regressões
focais PASS,39cenários por configuração: os33anteriores e6novos estados de espera.
Testam retorno1460 preservado, nenhuma consulta extra/mutação do pacote e não
impressão de handles. Qualificação em WAIT-STATE-OFFLINE-QUALIFICATION.json.
Não repetida campanha inalterada de76pares SQB3; seus13arquivos vinculados permanecem
idênticos. Formatos SQB3/SQC3/Q6 e limites G3-P1 permanecem como aceitos.

## Contratos e autoridade

Reutilizar integralmente U1–U7/U2 accepted, mesmo owner e papacc_update.exe protegido.
Propostas incorporadas9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234
e58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Nenhuma nova decisão de arquitetura ou exceção de autoridade. A instrumentação é
projeção local da primeira espera original, sem protocolo/serviço/canal novo.
Aprovação normativa anterior ou disposição não autoriza este UPDATE operacional.

Host TCORP-TOBIAS, PowerShell5.1Desktop AMD64, operador original
S-1-5-21-785699550-1440458426-1682014700-1108 elevado legítimo e console original,
sem autoelevação/impersonação. Declaração e confirmação atribuídas OWNER_BINARY_UPDATE
do plano nativo exato, sem resposta automática. Consumo único antes dos efeitos.

## Bytes e originais esperados

Novo Selected/Release exclusivamente DATA:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe
805376bytes, SHA256044f10de34010b83075976ba39599a92381bce483ade9f780960b0fcc73010b1.
Congelado papacc_server.wait-state-805376.exe na integração, mesmos bytes. Delta+1024.
Somente o caminho build acima será passado ao updater. Adquirir referência original
DATA retida, inspecionar SD, provar identidade/continuidade/hash/conteúdo. Não executar
DATA, exigir ACL instalada para ela ou transformar posse/hash em autoridade.

Canônico anterior C:\Program Files\Papinho\Accelerator\papacc_server.exe:
804352bytes, ID417614100:5242880:349380,
SHA25611cbef6508b1b06fb56031b22b93dccc6ae3e05df22528190f3cf7215636d815.
Fonte SQB3-IMAGE-UPDATE-RESULT.json/saída original de UPDATE reconciliada.
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

Sessão SQB3e7627530…/tentativa771a9ad8… utilizada, falhou1460/212/retained1.
Disposição e97078576eb59492f0f53429978c37ac48f187dcfaf018f9d3310838aceb98b4/
decisão2650110428e8a2bb52b392ecaf494364eebeb06b278e76cf6ff4ce12ac8af26f
foi executada manualmente e reconciliada: first0/consumedTrue, runtime21260 e
controller4184 fixos tiveram saída observada, observer_close_knownTrue e SCM
STOPPED/PID0 nos gates,1067/specific0. Fonte SQB3-RETAINED-DISPOSITION-RESULT.json.
Autorizações de sessão/disposição/UPDATE anteriores consumidas, sem retry.

Esse fato remove o impedimento daqueles dois participantes, mas não prova close
SCQ ou resolve bilateral/application close/retidos/reservas/quiescência OS históricos.
UNKNOWN preservado; material impeditivo implica DENY. Não limpar para satisfazer gate.

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

update-wait-state-image.MANUAL.CANDIDATE.ps1, SHA256 b436ee39a4d675fd0a8470117e9cbd0a048a25319858deab835c2f3a71713e6d.
Mesmas primitivas/helpers do wrapper anterior, deltas de nomes/pins/tamanhos/ID.
AST22bindings/16argv/uma chamada futura qualificados offline; nenhum wrapper/helper
nativo executado. Preflight OLD fecha antes de DELETE/rename; close incerto bloqueia.
Owner readquire OLD e revalida; snapshot não substitui continuidade nativa.

Solicita-se aprovação específica dos bytes deste pedido e do candidato805376/pin044f10de…
para UMA execução MANUAL, reconhecendo os riscos. Depois da decisão real, registrar
OWNER-WAIT-STATE-UPDATE-APPROVAL.txt e seu hash, preparar comando com hashes reais.
Esse arquivo ainda não existe; não fabricar autorização nem executar automaticamente.

Após saída manual: reconciliar primeira falha/consumo/criação/dois renames/anterior/
novo canônico/ID/hash/SD/readback/SCM/close. Exit0 sozinho não basta. Depois, preparar
pedido independente de nova sessão E/Q/S para obter os campos originais faltantes;
Start não é autorizado aqui. Se a nova linha localizar apenas uma fase, a causa
interna do receptor ainda poderá exigir prova focal adicional; não prometer resolução.

Sem UPDATE automático, retry, Start/Stop, endpoint/SCQ, SCM/ACL edit, CA/CURRENT,
rede, TPM/NV/Q4, refund, cleanup/rollback/restauração/recuperação ou produção.
Preservar HEAD/WIP/históricos/pins/EXEs/UNKNOWN/DENY/PST0.6.4.
