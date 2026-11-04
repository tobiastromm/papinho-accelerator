# Pedido da nova sessão PERSONAL_LOCAL — endpoint / SCQ / Start diagnóstico

Status: CANDIDATE / FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED.
Este documento fixa o pedido candidato. Não registra decisão do proprietário,
não é grant e não pode ser usado como decisão original. Não reutiliza atos
consumidos de staging, registro SCM, manutenção ou UPDATE.

## Operador, alvo e imagem

Operador pretendido: S-1-5-21-785699550-1440458426-1682014700-1108,
host TCORP-TOBIAS; elegibilidade, token/contexto e identidade exigem gates
nativos frescos, sem inferir autoridade desses metadados.

Serviço existente PapinhoAccelerator, conta NT SERVICE\PapinhoAccelerator,
OWN_PROCESS, DEMAND_START, SID UNRESTRICTED, configuração exata:
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Exigir STOPPED/PID0 e readback fresco antes do Start, sem reconfigurar o serviço.

Controller e imagem do runtime: canônico instalado
C:\Program Files\Papinho\Accelerator\papacc_server.exe,
762880 bytes, objeto esperado417614100:1900544:539611, SHA256
3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b.
Parent esperado417614100:8257536:71209. Adquirir/revalidar originais, proteção,
ancestrais, ausência de reparse/substituição e continuidade, não só nomes/pins.
DLLs não são alteradas: ssl SHA256
3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed,
crypto SHA256
09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.

## Três atos independentes pretendidos

1. Endpoint: uma abertura/conexão privada da tentativa, somente QUERY/SQB2 SCQ
   existente; sem listener genérico, segunda conexão, forwarding ou replay.
2. SCQ: TRANSPORT_ONLY; originais não secretos em empréstimo restrito apenas
   após admissão e autenticação bilateral; sem key/KSP ou acesso produtivo.
3. Start: um Start diagnóstico fechado do serviço exato, usando o carrier
   existente; não permite Start produtivo, reconfiguração, restart ou retry.

Cada ato continua exigindo apresentação e confirmação atribuída independente
no owner existente. Aprovação futura do pedido não elimina essas confirmações
ou os consumos nativos anteriores aos respectivos efeitos. Não executar um
Stop SCM separado por inferência; distinguir fechamento finito existente do
diagnóstico e qualquer comando ControlService que exigiria autoridade própria.

## Originais administrativos: contrato de disponibilização ainda pendente

Pedido original pretendido: este documento, depois de congelado e aprovado,
em cópia byte-exata protegida no diretório documental já existente, leaf
FIRST-START-SESSION-REQUEST.ORIGINAL.md.
Decisão original pretendida: texto EXATO de aprovação operacional futura
efetivamente emitida pelo proprietário, ligado ao SHA256 do pedido congelado,
leaf FIRST-START-SESSION-DECISION.ORIGINAL.txt no mesmo diretório:
C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101.

Nenhum dos dois arquivos ORIGINAL foi criado por esta preparação. Não há
decisão presumida, conteúdo de decisão gerado pelo agente ou identidade de
objeto predita. A seleção de caminhos não demonstra sua existência/proteção.
Sua disponibilização deve ser exclusiva, sem substituir arquivos existentes,
com owner do operador e DACL protegida desde a criação, sem alterar ACL do
workspace/ancestrais. Exigir ausência de alias/hardlink/reparse, conferir
conteúdo e pin e reter as referências originais FILE_SHARE_READ durante o
fluxo. Não tratar uma cópia protegida como origem autônoma de autoridade.

O proprietário confirmou que ainda não existem originais desta sessão. A
disponibilização exclusiva desses dois documentos fica incluída como efeito
PREPARATÓRIO solicitado neste rascunho, distinto dos três atos do ensaio.
Não modifica ACL de objeto existente, não cria diretório e não permite adoção,
overwrite, limpeza ou retry. A composição específica reutiliza criação exclusiva,
cópia/hash/flush/readback/fechamento, no adaptador first-start-originals.cs;
não é emissor de grants nem altera o owner do produto. A sequência e a política
de SD foram qualificadas em memória; APIs nativas de criação/identidade/I/O são
fronteiras ainda não exercidas. Uma falha pode deixar um ou dois documentos
parciais/protegidos; preservar, sem apagar ou repetir. A criação exige rights
ADD_FILE/TRAVERSE no parent, sem WRITE_DAC/WRITE_OWNER em objeto existente.
Criação/ACL desses arquivos não está autorizada pelo prompt de preparação.
Não usar o placement RAW1 de outro domínio sem comprovar aplicabilidade.
Identidades nativas e pin da decisão só poderão ser conhecidos depois de
existirem os objetos e a decisão verdadeira. São bindings futuros obrigatórios,
não campos a preencher por inferência nem requisito impossível para aprovar
este pedido. Sua aprovação deverá autorizar EXPRESSAMENTE a materialização
documental preparatória e, separadamente, os três atos do ensaio.

O registro documental da aprovação futura será salvo literalmente, após emissão
real, em OWNER-FIRST-START-APPROVAL.txt nesta mesma pasta (arquivo fonte DATA,
sem autoridade por presença). Essa é a terceira escrita documental solicitada:
registro literal da decisão, depois duas cópias exclusivas protegidas. Não salvar
texto de aprovação antes de sua emissão nem sobrescrever registro preexistente.
Seu SHA256 deve ser conferido antes da cópia; a origem será retida com share de
leitura durante a materialização. Nenhuma cópia renova a decisão ou seus prazos.

Procedimento materializador preparado, NÃO EXECUTADO:
prepare-first-start-originals.MANUAL.ps1, SHA256
1b8cf05bcc52d343f60c71fba600e186042344d142bfe239903074889f8f4a3a.
Adaptador pinado e compilado dos MESMOS bytes verificados: first-start-originals.cs,
SHA256 18719507fafe6ea6fb81f73dc688410ce7531dfb6074c331de123e0441878f5f.
São documentos não secretos limitados a65536 bytes cada por desenho do adaptador;
esse limite de entrada não demonstra ceiling de heap/kernel nem capacidade SCQ.
O script solicita confirmação atribuída somente dessa materialização, cria
exatamente os dois leaves ORIGINAL com DACL estrita desde criação, verifica os
objetos retidos e fecha. NÃO chama o produto, endpoint, SCM ou Start/Stop.
Close incerto interrompe a continuação; exit0 isolado não basta para reconciliar.
Máscaras de criação/verificação indicam efeitos CONFIRMADOS; zero depois de
falha nativa não prova ausência de efeito. Não presumir limpeza ou liberação.

## Parâmetros G3 selecionados, sem alterações

| Parâmetro | Valor |
| --- | --- |
| Handles por participante | máximo128; guard124 antes de aquisições |
| Crescimento privado sobre READY próprio | 1994944 bytes |
| Crescimento working set sobre READY próprio | 1994944 bytes |
| Piso físico do host | 7239172096 bytes |
| Piso commit do host | 9823326208 bytes |
| Freshness até o efeito | 100 ms |
| Janela original sem renovação | 240000 ms |
| Duração global compartilhada | 60000 ms |

Deadline receptor é o menor prazo aplicável, incluindo SQB2. Contagem completa
de handles, sem descontos para herdados/console. Insuficiência ou UNKNOWN
material nega antes do efeito. Não substituir G3 por picos de UPDATE nem por
B0–B8/R0–R9. São tolerâncias de risco selecionadas, não ceilings nativos.

## Sequência e evidências exigidas

Antes: adquirir controller e originais request/decisão/imagem, identificar token
administrativo elegível, confirmar planos independentemente, congelar tentativa
e lifetime, revalidar namespace, G3 e SCM parado imediatamente até cada efeito.
Não renovar prazos por nova leitura ou pelo ingresso do receptor.

Depois de Start: adquirir PID/criação/token/grupos/restrições e continuidade
reais do runtime. QUERY apenas fornece candidatos; não é admissão. Validar
independentemente direitos, imagem, proveniência e destinatários nas duas pontas.
Autenticação bilateral deve preceder empréstimo admitido; receipt e close
originais devem preceder RUNNING somente diagnóstico. Ack DATA não autentica.
Verificar término finito/processo/SCM STOPPED pelo caminho existente.

## Falhas, efeitos e riscos

Tentativa única, sem retry automático ou nova aprovação restaurada por arquivo.
Stop governado, perda do peer, prazo vencido, replay, divergência de identidade,
recursos insuficientes e falhas de receipt/close negam a progressão. Falha parcial
preserva consumo, referências e UNKNOWN; não matar/limpar/recuperar por inferência.
Custos opacos de processo/SCM/IPC/kernel, flutuação entre amostras e latência de
cancelamento permanecem riscos do perfil G0–G9; não há provisão integral provada.

Coorte histórica da tentativa UPDATE falha, candidato retido e EXEs .previous
permanecem intactos. Close recente não resolve esse UNKNOWN ou OS_QUIESCENCE.
Conflito com obrigação histórica impede a nova operação. Não adotar/refundar.

Não autoriza UPDATE/staging/registro SCM/ACL, novos diretórios, CA/CURRENT,
KSP, rede operacional, TPM/NV/Q4, limpeza/refund, recuperação ou produção.
E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4 preservado. Pedido byte-exato para
decisão do proprietário; não é autorização emitida nem comando executável.
Após aprovação verdadeira, registrar seu texto/pin e preparar a chamada manual
exata. Se a materialização ou qualquer gate fresco falhar, não prosseguir.
