# Pedido operacional — UPDATE da imagem SQF1

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma execução MANUAL
BINARY_UPDATE_CLOSED PERSONAL_LOCAL, sem retry; sem endpoint/SCQ/Start/Stop.

## Finalidade, autoridade e limite de prova
Instalar o retorno NEGATIVO SQF1/56bytes no mesmo pipe/owner, limitado P1
PERSONAL_LOCAL TRANSPORT_ONLY. F0–F7 accepted em
DECISION-personal-scq-p1-receiver-first-failure-sqf1-20261102.md, incorporando
96b2d26f83cecc7688eeb9e3543a6b19d6660978b31b62ae175fe604f41a3643.
Não é receipt/close/autoridade/admissão. Preservar SQR1/SQC3 e demais formatos/
perfis, origem/identidade/proteção/bilateral/custódia Q6/G3/Stop/retention.
Envio suprimido se inseguro/pending/Stop/deadline/resources;56bytes separados,
+88bytes/zerohandles novos por pacote. Primeira falha preservada e códigos remotos
são somente DATA para DENY, nunca comando de espera/progresso. Nenhuma quota aumenta.

Quatro builds sem warnings/80pares afetados PASS. Censo Selectedroot375256/
known2002304 sob3145728;Legacyroot206688/known1833736 sob2097152. Custos nativos
opacos UNKNOWN. Teste do corpo real ingress/wait com fronteiras mockadas e injeções;
não é prova de validação nativa ou causa histórica do timeout. Componentes podem
retornar erro composto com native_kind0, sem inferir rawstatus. Pode continuar
sem retorno se recursos/deadline/provas não permitirem enviar. Não garantir sucesso
ou diagnóstico completo da próxima sessão. Ver SQF1-INTEGRATION-REPORT.md e qualificação.

U1–U7/U2 inalterados, mesmo owner/executor protegido. Propostas incorporadas
9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234 e
58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Sem nova arquitetura de UPDATE, serviço/root/canal/CLI, stager ou updater.
Aceitação SQF1/disposição anterior NÃO concedem este UPDATE operacional.
Host TCORP-TOBIAS, PowerShell5.1Desktop AMD64, operador original SID
S-1-5-21-785699550-1440458426-1682014700-1108 legitimamente elevado, console original,
sem autoelevação/impersonação. Declaração/confirmação nativa atribuída OWNER_BINARY_UPDATE,
sem resposta automática. Consumo único antes dos efeitos e plano imutável.

## Bytes e originais esperados

Novo Selected/Release exclusivamente DATA:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe
809472bytes, SHA25681a849dc51bf2e7fd0455c09f30c926329ef4c30196b36747f1429e5fdecee86.
Congelado papacc_server.sqf1-qualified-809472.exe na integração, mesmos bytes. Delta+4096bytes; conteúdo diferente.
Somente o caminho build acima será passado ao updater. Adquirir referência original
DATA retida, inspecionar SD, provar identidade/continuidade/hash/conteúdo. Não executar
DATA, exigir ACL instalada para ela ou transformar posse/hash em autoridade.

Canônico anterior C:\Program Files\Papinho\Accelerator\papacc_server.exe:
805376bytes, ID417614100:2097152:411753,
SHA256174db00a075b55433fdc793f23d9c9412f982814eef8c650579a824afc567f8c.
Fonte RECEIVER-PATH-IMAGE-UPDATE-RESULT.json/UPDATE reconciliado, ID417614100:2097152:411753.
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
Sessão db62ad040b42e49a568fcdd91a816abc2c29a97ed324a93e0935ab75d2587a2f
consumida: receipt1052/operation2/pending1/offset0, timeout1460/212/retained1.
Consulta3e32bbc6… consumida; disposição específica
 e8cb88b09d73af188c69db265b8234959ebaf94ace5535cbaede3bbda5fadae9 /
decisão b468e7f2ac8183da349d28e0118429293291a6e0f2da9293f95dcdb0661170f3
concluída conforme saída do proprietário: first0/consumedTrue, runtime21316/
criação134381413935377225 e controlador7400/criação134381413716812022 tiveram
saídas observadas; observer_close_knownTrue, SCM STOPPED duas vezes/1067/specific0.
Registro atribuído em RECEIVER-PATH-RETAINED-DISPOSITION-RESULT.json; não foi
nova coleta do agente. Remove impedimento desses dois objetos, não prova
SCQ/application close/bilateral/receipt/reservas/quiescência OS histórica.
Todas as autorizações anteriores permanecem consumidas. UNKNOWN material impeditivo
nega o UPDATE; não presumir recuperação nem limpar/refundar para satisfazer gate.

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
Copiar809472bytes, hash/flush/readback e revalidar originais antes dos renames.
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
R=ceil(809472/U)*U com geometria real original, revalidação e overflow checado;
exemploU4096=>811008bytes é cálculo, não consulta atual. Livre caller/actual>=R
antes de criar e restante arredondado antes de escrever; retidos ocupam espaço.

Custos loader/CRT/alocador/SD/hash/provider/SCM/RPC/kernel/cache/filesystem/flush/
rename/log continuam opacos. Não garantir atomicidade, rollback, suficiência,
durabilidade, cancelamento ou latência. Há risco de parcialidade, intervalo sem
EXE canônico e retenção persistente. Divergência/insuficiência/Stop/UNKNOWN impeditivo
nega sem ajustar ACL ou quota. Efeitos persistentes limitados ao novo EXE e anterior retido.

## Procedimento e decisão solicitada

update-sqf1-image.MANUAL.CANDIDATE.ps1, SHA256 f44d31bdc4bed63d92414c0a1e448570294c2c28e6280128cca5b27473b34426.
Mesmas primitivas/helpers do wrapper anterior, deltas de nomes/pins/tamanhos/ID.
AST22bindings/16argv/uma chamada futura qualificados offline; nenhum wrapper/helper
nativo executado. Preflight OLD fecha antes de DELETE/rename; close incerto bloqueia.
Owner readquire OLD e revalida; snapshot não substitui continuidade nativa.

Solicita-se aprovação específica dos bytes deste pedido e do candidato809472/pin81a849dc51bf2e7fd0455c09f30c926329ef4c30196b36747f1429e5fdecee86
para UMA execução MANUAL, reconhecendo os riscos. Depois da decisão real, registrar
OWNER-SQF1-UPDATE-APPROVAL.txt e seu hash, preparar comando com hashes reais.
Esse arquivo ainda não existe; não fabricar autorização nem executar automaticamente.

Após saída manual: reconciliar primeira falha/consumo/criação/dois renames/anterior/
novo canônico/ID/hash/SD/readback/SCM/close. Exit0 sozinho não basta. Depois, preparar
pedido independente de nova sessão E/Q/S para validar o caminho diagnóstico SQF1 no receptor;
Start não é autorizado aqui. Se a nova linha localizar apenas uma fase, a causa
interna do receptor ainda poderá exigir prova focal adicional; não prometer resolução.

Sem UPDATE automático, retry, Start/Stop, endpoint/SCQ, SCM/ACL edit, CA/CURRENT,
rede, TPM/NV/Q4, refund, cleanup/rollback/restauração/recuperação ou produção.
Preservar HEAD/WIP/históricos/pins/EXEs/UNKNOWN/DENY/PST0.6.4.
