# Pedido operacional — uma sessão endpoint / SCQ / primeiro Start diagnóstico

Status: CANDIDATE / FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED.
PERSONAL_LOCAL, um ensaio TRANSPORT_ONLY fechado e finito. Este pedido não
registra aprovação, fonte operacional emitida ou prova bilateral já obtida.

## Sessão nova após UPDATE diagnóstico: diferença documental expressa

O UPDATE diagnóstico foi reconciliado em COLD-DIAGNOSTIC-UPDATE-RECONCILIATION.md.
Este pacote é uma NOVA solicitação, não retry autorizado da sessão25ea...dff4.
A decisão anteriorC6/417614100:6225920:542803, sua fonte e todos os originais
históricos permanecem intactos e não são fontes desta nova aprovação.

O documento-base protegido312999...6798d,417614100:1310720:120518, contém
literalmente a imagem histórica762880bytes/417614100:1900544:539611/pin
3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b.
Não afirmar que esse conteúdo é da imagem nova. Sua reutilização é documental,
sem rearmar ato/nonce/lifetime consumido. A aprovação futura DESTE pacote deve
reconhecer expressamente essa diferença e substituir a seleção da imagem/objeto
do plano antigo exclusivamente pela seleção763904bytes/417614100:1835008:134613/
7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d.
A nova decisão original deve vincular esse pacote e a substituição documental;
não inferir essa intenção somente de hash, presença do arquivo ou teste modelado.
Sem esse escopo explícito na aprovação, STOP: não materializar nem executar.
O plano nativo vivo usará o hash novo e referências originais atuais, com três
confirmações independentes; não usará o ID/hash antigo como seleção operacional.

Conferência atual read-only: hash do canônico/DLLs PASS, proteção BA/SY full e AU
RX, DACL protegida; SCM STOPPED/PID0/exit1077, configuração original. ID novo
provém do readback nativo qualificado do UPDATE, não de um lease permanente.
Exigir reaquisição nativa/continuidade/readback fresco e cancelar plano divergente.
Causa histórica1816, retenção histórica e OS_QUIESCENCE permanecem UNKNOWN.
Não remover referências ou mudar G3 para superar nova negação. A linha
SCQ_COLD_FIRST_CAUSE da imagem instalada apenas distingue eventual evento futuro.

Driver preparado run-cold-image-diagnostic.MANUAL.CANDIDATE.ps1, SHA256
88b9c28775715b92f93b47d377dbebd1ca1c2dc6d033fa66d59e0bf51ef913bf.
Parâmetros de hash da decisão REAL e deste pedido somente após aprovação;
não são placeholders de comando emitido, nem evidência antecipada de aprovação.
O driver rejeita a decisãoC6 e a de preparação7f645..., além do pedido consumido.
Cria somente a decisão auxiliar nova no leaf exclusivo indicado abaixo; não
repete materialização do par. Não foi executado. Não emitir comando até aprovação.

## 1. Três escopos independentes, incorporados byte-exatos

| Ato | Documento nesta pasta | SHA256 |
| --- | --- | --- |
| E: endpoint privado | COLD-IMAGE-ACT-ENDPOINT.REQUEST.CANDIDATE.md | fdb74fbb22b1b50d572b403b05edaf7192e8f5b80ed54ffe7711f393ad2a89ff |
| Q: SCQ TRANSPORT_ONLY | COLD-IMAGE-ACT-SCQ.REQUEST.CANDIDATE.md | d6037fc2e05807299bee8e399ed905980d8eb1c9d38a39dfe9e09f70f490b140 |
| S: um Start diagnóstico | COLD-IMAGE-ACT-START.REQUEST.CANDIDATE.md | 843bebe4074e53fa86b7d18d1709b252916538e1df582691855c797bf37015cf |

O proprietário deverá aprovar EXPRESSAMENTE E, Q e S, com o efeito documental
auxiliar da seção3. Um não deriva autoridade de outro. Aprovação parcial não
habilita a sequência integrada. Além dessa decisão, continuam obrigatórias as
três confirmações nativas atribuídas do owner: OWNER_SCQ, depois
OWNER_SCQ_PRIVATE_ENDPOINT, depois OWNER_SCQ_DIAGNOSTIC_START, para a mesma
tentativa. Nunca responder automaticamente, instalar callback permissivo ou
inferir confirmação do arquivo de aprovação.

## 2. Identidades, imagem e configuração congeladas

Host pretendido TCORP-TOBIAS; operador
S-1-5-21-785699550-1440458426-1682014700-1108, proprietário/administrador legítimo
declarado em confirmação atribuída e contexto nativo elegível, sem impersonação.
O controller é processo original do canônico instalado; não usar workspace,
papacc_update.exe, LAB, token transferido ou processo substituto.

Canônico C:\Program Files\Papinho\Accelerator\papacc_server.exe,
763904 bytes, objeto esperado417614100:1835008:134613,
SHA2567d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d.
Parent esperado417614100:8257536:71209. IDs provêm do UPDATE qualificado, não
constituem nova consulta ou lease. Conferir objeto/imagem, conteúdo, SD e
ancestrais originais novamente. O presenter nativo imprime IDs/pins das
referências; cancelar qualquer discrepância do objeto aprovado.

DLLs, não alteradas:
libssl-3-x64.dll1321472bytes,
SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
libcrypto-3-x64.dll7217664bytes,
SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Pins em disco não são atestação de páginas mapeadas. Preservar SD/identidade/
ausência de reparse/substituição e continuidade, não só comparar strings.

Serviço PapinhoAccelerator, conta virtual NT SERVICE\PapinhoAccelerator,
WIN32_OWN_PROCESS0x10, DEMAND_START3, ERROR_NORMAL1, SID UNRESTRICTED1,
comando `"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Antes de Start: readback SCM exato, SERVICE_STOPPED/PID0 e originalidade da
imagem. Nenhum CreateService/registro, reconfiguração ou edição de ACL.
SID virtual é resolvido e vinculado ao serviço original; TokenUser efetivo,
grupos/restrições, PID/criação e continuidade do runtime só após ele existir.

## 3. Originais reutilizados e decisão operacional ainda não emitida

Pasta documental existente:
C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101.

Request do plano nativo será o objeto já materializado
FIRST-START-SESSION-REQUEST.ORIGINAL.md,417614100:1310720:120518,9517bytes,
SHA256312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d.
Este pacote incorpora esse pedido como plano base, restringindo os efeitos
atuais a E/Q/S e à nova decisão auxiliar abaixo. Não repetir seus efeitos
documentais já consumidos. Reaquisição e continuidade obrigatórias.

FIRST-START-SESSION-DECISION.ORIGINAL.txt,
417614100:1310720:120520/pin7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a,
permanece evidência de PREPARAÇÃO. Não é fonte operacional; não será promovido,
reescrito, apagado, entregue como aprovação operacional ou recriado.

A nova decisão deverá ser texto REAL do proprietário aprovando este pacote
byte-exato e seus três atos, não decisão gerada pelo agente. Após emissão:

1. Preservar literalmente a mensagem em OWNER-COLD-IMAGE-THREE-ACTS-APPROVAL.txt,
   fonte documental DATA nesta pasta, sem overwrite. Conferir/reconciliar escopo
   e calcular SHA256 dos bytes reais. Hash/presença não dão autoridade.
2. Uma cópia exclusiva para FIRST-COLD-IMAGE-OPERATIONAL-DECISION.ORIGINAL.txt,
   no mesmo parent, owner do operador/DACL protegida desde criação, escrita,
   hash/flush/readback/ID/SD, fechamento e retenção UNKNOWN. Sem editar ACL,
   sobrescrever/adotar leaf existente, copiar novamente request ou decisão de
   preparação, rename/delete/rollback ou diretório novo.
3. Somente com prova suficiente dessa cópia e fechamento conhecido, o caller
   frio reutiliza o request protegido e adquire a decisão operacional nova.
   Consumo/confirmadores do produto permanecem independentes do ato documental.

São efeitos documentais AUXILIARES solicitados neste mesmo pedido: registro
literal da decisão futura e UMA criação protegida nova; não são quarto canal,
owner, serviço, ledger ou grant reutilizável. A autorização do pacote deverá
incluí-los explicitamente, dispensando uma rodada separada de autorização
apenas para fornecer a decisão que o próprio pacote exige. Nada será criado
antes da emissão verdadeira e da autorização explícita deste escopo.

Composição preparada no adaptador administrativo existente, revisão restrita
cold-image-operational-decision.cs,
SHA25695b5df17c83fc646b4aff2b29b2c7d4d96c5581549c695235f8c73b6eaf77f5e.
Wrapper preparado prepare-cold-image-decision.MANUAL.CANDIDATE.ps1,
SHA25613c6a92f76eca42c36afe251aa087b8a7ab5257ac723def0a09d4a5e7b471f27.
O wrapper cria SOMENTE a decisão, não chama produto/SCM/endpoint e rejeita o
pin da decisão de preparação. O request é só leitura, conferido pelo ID exato
e SD estrito. O adaptador anterior18719507…8f5f e o par já criado são preservados.
Compilação C# em memória W4/warnings-as-errors,39 verificações com I/O injetado
e5 casos da guarda DATA por AST: PASS. Isso não prova aprovação do proprietário,
nova criação nativa ou autenticação bilateral. Wrapper manual não foi invocado.

SHA256/ID da decisão futura não são inventados antes de sua emissão/criação.
São bindings documentais frescos antes dos efeitos E/Q/S. O comando manual
exato será fechado DEPOIS de receber a decisão real e seu pin, sem placeholders
e sem exigir aprovação normativa adicional para preencher esse dado factual.
Arquivos apenas preservam evidência; não restauram autoridade após uso/restart.

## 4. Tentativa, consumo e G3 exato

Uma tentativa da sessão, nonce original de32bytes produzido por CSPRNG do owner
e nunca escolhido por CLI/fixture/carrier. SHA256/CSPRNG sem acesso a chave são
permitidos somente para integridade/tentativa do mecanismo existente; KSP,
chaves privadas, segredos, TPM ou tokens impersonáveis são proibidos.

| Parâmetro por participante, salvo pisos do host | Valor selecionado |
| --- | --- |
| Handles completos | cap128, guard124 antes de aquisições |
| Crescimento privado sobre READY próprio | 1994944 bytes |
| Crescimento working set sobre READY próprio | 1994944 bytes |
| Piso físico disponível do host | 7239172096 bytes |
| Piso commit disponível do host | 9823326208 bytes |
| Freshness medida até o efeito | 100 ms |
| Janela original sem renovação | 240000 ms |
| Duração global compartilhada | 60000 ms |

Origem: seleção accepted QUERY/G3, sem mudança de números, unidades ou domínio.
São tolerâncias administrativas de risco, NÃO máximos nativos demonstrados.
READY é observado separadamente no controller e receptor; não transferir baseline.
Janela original começa no plano original vivo; global começa no READY controller,
limitado ao vencimento original. SQB2 receptor limita pelo menor prazo recebido/
original/global; nunca renova em ingresso, poll, reaquisição ou confirmação.
Contar todos os handles, inclusive herdados/console/loader. Não importar picos
de UPDATE, LAB/Browser/TPM ou limites B0–B8/R0–R9. Preservar124/128/1024 nos
domínios correspondentes. UNKNOWN material/insuficiência/freshness vencida => DENY.

Documento auxiliar limitado a65536bytes, referências de fontes/ancestrais e
um destino, sem usar a contagem do PowerShell como baseline do controller SCQ.
Não se concede ao materializador provisão nativa integral nem quota arbitrária.

## 5. Sequência da sessão e provas posteriores

Antes de E/Q/S: decisão operacional real/originais, controller administrativo
original e contexto/token completos, imagem/SD/ancestrais, destino, serviço
parado/configuração, namespace e política G3 exatos. Três confirmações deliberadas
do plano, com cancelamento ante qualquer discrepância. O owner efetua consumo
de cada ato antes de seu efeito; consulta que falhe depois do consumo não rearma.

Backend instalado já integrado: entrypoint privado --owner-manual-first-scq-start,
requisição e decisão por referências/pins, imagem exata e oito valores G3.
Argv não contém aprovação, PID/token/HANDLE futuro ou segredo. Serviço iniciado
apenas pelo owner SCM após source/G3/namespace revalidados. Não usar service start
direto para contornar aquisição/confirmadores nem fornecer respostas automáticas.

Após Start autorizado: observar START_PENDING/PID real, reter processo original,
token/criação/TokenUser/grupos/restrições e serviço/imagem, correlacionar peers
pelas APIs nativas independentes e continuidade. QUERY é candidata; não é fonte
administrativa ou readiness. Autenticação bilateral antes do empréstimo admitido;
receipt e close originais antes de RUNNING diagnóstico. Ack DATA não autentica.
Nenhuma dessas provas é marcada PASS antes da execução.

Sessão finita: o caminho exige término do runtime, exit0, SCM STOPPED/PID0,
readback e fechamento próprios. Encerramento natural do diagnóstico é distinto
de ControlService Stop, que NÃO está autorizado. Estado persistente produtivo,
CA/CURRENT, emissor ou listener produtivo continuam negados.

## 6. Riscos, STOP e UNKNOWN

Reconhecer G0–G9: custos opacos/transitórios de loader/CRT/processo/RPC/pipe/
provider/kernel podem exceder orçamento, afetar o host ou ocorrer entre amostras.
Não existe garantia contra exaustão, retorno de API no deadline, cancelamento
imediato de I/O ou quiescência OS. A criação do runtime antecede sua primeira
medição; isso é risco residual, não conformidade presumida.

Falha na cópia da decisão, source/identidade/SD/SCM divergentes, qualquer ato
ausente, endpoint ocupado/incorreto, replay/forwarding/substituição, quota ou
margem insuficiente, deadline/freshness vencidos, Stop, peer perdido ou receipt/
close incompleto => negar novos efeitos. Preservar primeira causa, consumo,
referências e reservas UNKNOWN. Não retry, reconectar, reiniciar ou adotar
objetos. Não interromper/terminar processo para fabricar fechamento; retenção
pode estacionar o owner indefinidamente. O prazo limita elegibilidade para
efeitos, não autoriza kill/cleanup. Eventual Stop SCM exige autorização própria.

Retenção histórica do UPDATE falho não foi resolvida pelo UPDATE bem-sucedido,
pela cópia documental ou por este pedido. Não tocar em candidatos retidos,
EXEs .previous, papacc_update.exe, DLLs ou ProgramData. Se obrigação retida
conflitar com esta tentativa, STOP; não presumir crédito/refund ou recuperação.

## 7. Quantidade, evidência e reconciliação

Uma decisão auxiliar nova; no máximo uma criação exclusiva de namespace, uma
abertura receptora local, uma entrega governada e uma StartServiceW. Poll de
I/O pendente da mesma operação não é retry. Uma autorização manual para a
sequência, sem nova tentativa após uso/falha; consumos nativos dos atos são
separados. Não há prova de ledger durável/unicidade global/fencing por arquivo.

Reconciliar saída integral, fontes/pins, IDs/SD/readback da decisão auxiliar,
planos e confirmações individuais, primeira causa, consumos, peer/token/imagem/
SCM, receipt/close, término e retenção. O marcador final do caminho pinado
SCQ_START=DIAGNOSTIC_CLOSED_QUALIFIED exige suas verificações; SERVICE=REQUEST_SUBMITTED,
retorno ao prompt ou exit0 isolados não provam esse resultado. Falha parcial
pode deixar a decisão e/ou processo/endpoint em estado UNKNOWN: preservar.
Não reclassificar cenários modelados como autenticação Windows demonstrada.

## 8. Aprovação solicitada e exclusões

Solicita-se aprovação explícita DESTE pacote e dos atos E/Q/S individualizados,
incluindo somente os dois efeitos documentais auxiliares da seção3, uma execução
MANUAL, sem automação pelo agente/retries. Aprovação da norma ou documentos
históricos não substitui esse ato. Após aprovação, registrar literalmente seu
texto/pin, preparar os comandos exatos e aguardar execução manual pelo owner.
O aceite deve reconhecer o pedido base312999dd…6798d incorporado como request
original do plano nativo e a NOVA decisão operacional como fonte do propósito
E/Q/S. Aprovar apenas preparação/cópia não autoriza os atos; não completar um
escopo ausente por inferência ou pelo fato de a guarda DATA do wrapper passar.

Sem UPDATE/staging, re-materialização do par, registro/reconfiguração SCM,
ACL de objetos existentes, diretórios/contas novos, Start adicional ou
ControlService Stop, rede operacional, KSP/CA/CURRENT,
TPM/NV/Q4, restauração/delete/cleanup/refund, recuperação ou autoridade produtiva.
Este pedido não altera NEW_INSTALL ou PST0.6.4. E21 PENDING;
ED11/Phase6 PARTIAL; readiness produtiva DENY; retenção histórica/OS UNKNOWN.
