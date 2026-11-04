# SCQ — session driver integrado e qualificado offline

Task PACC-PHASE6-ASA-SCQ-SESSION-DRIVER-OFFLINE, 2026-10-30.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados.
B/OHI/F1–F6 accepted intactos; nenhum registro ou autorização operacional criado.

## Integração realizada

`personal_scq_session_win32.h/.inc` adiciona ao packet existente um driver de
sequência e custódia de buffer, sem novo owner, serviço, root ou IPC. Reutiliza
scq_inputs, fonte administrativa, perfil privado e receptor existentes.

Callers canônicos em `server_configuration_personal_native_win32.inc`:

- session_acquire_originals encadeia captura dos três handles originais,
  verificação de conteúdo/SD e preparo da fonte administrativa. Não abre paths
  fornecidos por caller nem obtém autoridade por frame/receipt.
- session_prepare liga os objetos existentes depois da confirmação SCQ explícita.
  Se apenas capturou a fonte, deixa o driver desarmado até essa confirmação;
  não confirma ou reapresenta automaticamente.
- session_take_data faz revalidação, take monotônico do controlador e take DATA
  do perfil, uma vez. Não executa Start nem concede esse efeito.
- session_offer_data deriva observação runtime/imagem dos adapters existentes,
  vincula o destinatário real e prepara o frame no buffer do próprio packet.
- session_receive_data usa somente original fornecido pelo gateway nativo de
  origem; não o reconstrói do frame. Liga o empréstimo ao receptor e ao uso
  separado do destinatário, antes de qualquer body protegido.
- session_receipt_data / session_ack_data validam exclusivamente DATA.
  session_output_data fornece borrow local const do buffer tipado.
- session_peer_lost e close/retire/pending_blocks preservam a retenção.

Todos os passos nativos que exigem decisão/origem continuam negados pelo
registro operacional vazio. O gateway nativo de originais permanece obrigatório;
nenhuma API nova recebe um ponteiro original de CLI/arquivo/IPC. Não há dispatch
CNG/KSP, Start/Stop ou operação de serviço no driver.

## Sequência, identidades e lifetime

A composição do modelo é: originais/decisão/confirmacão → take do controlador →
take DATA → observação/vínculo runtime → offer → receive com original local
validado → consumo do empréstimo → receipt DATA → ack DATA → fechamento separado
do destinatário e do controlador.

Controlador e runtime são validados separadamente. O snapshot runtime é
confrontado com o adapter retido: PID/criação, SID, stats de token, enumeração,
atributos e service SID type. O receptor confronta também identidade nativa dos
três originais e identidade/token/grupos/restrições do administrador com sua
projeção independente. N0 não é roteado ao perfil SCQ. Direitos de arquivo seguem
os três papéis e READ/query exatos do perfil vigente. Nenhum token impersonável,
autoridade administrativa reutilizável ou ponteiro/HANDLE local entra no frame.

O caso sem runtime mantém apenas SID esperado no frame do controlador. O teste
confirma runtime/dispatch/profile do destinatário ainda inexistentes e consumo
anterior à fronteira de efeito MODELADA; só depois obtém PID/criação/token do
modelo. Não é criação de processo ou SCM real.

O buffer1052 bytes pertence ao driver: comporta o frame1048 ou o receipt1052.
Ele é reutilizado somente depois da validação correspondente. Erro após take,
perda do peer, borrow pendente ou fechamento incerto conservam originais e buffer
como UNKNOWN. Não há retry/rearm/reset ou refund. Fechar o controlador antes do
último borrow do destinatário também mantém UNKNOWN.

O close chamado pelo root usa quiescência DATA porque NÃO existe backend de IO
nativo instalado nesta implementação. Isso não prova Stop de serviço, término de
IO externo, fechamento de endpoint ou quiescência global. Ativar um futuro
backend exige integrar suas referências/IO pendente antes de usar esse close.

## Receipt não é autenticação

Um teste constrói SQR1 + frame conhecido sem executar o destinatário. O ack DATA
pode validar esses bytes, mas o receptor permanece sem execução/consumo e nenhum
efeito é chamado. Portanto phase4/OK/receipt não constituem ack autenticado,
permissão KSP ou prova de entrega nativa. Não existe campo/setter para promover
esse resultado a autenticação. O driver não chama o body SCQ.

O original passado entre os dois packets do teste é um objeto local em um
único processo MODELADO. Não é uma técnica de handoff cross-process. O adapter
nativo futuro deverá adquirir prova local legítima em cada ponta, sem serializar
aquele ponteiro ou tratar valores de handles de outro processo como locais.

## Qualificação

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings.
Allowlist focal: 12 CTests/configuração, 48 execuções PASS. Não houve campanha
genérica que pudesse criar endpoints ou tocar a chave persistente.

Novo driver: 35 cenários / 711 verificações/configuração. Inclui source/decisão
faltantes, confirmação ausente, controlador/runtime divergentes, service SID type
errado, destinatário incorreto, cópia/forwarding, take/offer/receive/receipt/ack
duplicados, N0, frame/receipt alterados, Stop antes/depois de take, perda do peer
por evento e referência original encerrada, timeout, PID reutilizado, decisão
substituída, fechamento antecipado/incerto e UNKNOWN. Thread estrangeira real
nega antes do consumo. Regressões ASA/OAS/perfil/presenter preservadas.

Há 19 casos retidos no modelo: cada fixture tem backing estático independente,
sem reutilização/descarte dos objetos UNKNOWN; SID backing continua vivo até
saída do teste. Não se apresenta saída de processo como cleanup/quiescência.

Decisão positiva, peers, processo/token/SCM, IO e prova original positivos são
MODELADOS. SID/SD/thread e o hashing CNG da regressão são reais no alcance dos
respectivos testes. Não houve endpoint real criado, conectado ou testado,
sessão SCM/CNG, instalação, diretório/conta/CA, CURRENT, Start/Stop, SCM/ACL,
rede operacional, TPM/NV/Q4, limpeza/refund ou recuperação.

## Recursos e hashes

Driver1136 bytes embutidos, incluindo buffer1052; crescimento fixo1136 bytes no
mesmo owner. Known managed: Selected1.978.152/3.145.728, Legacy1.809.584/2.097.152.
Sem handles/threads/serviços adicionais de transporte no produto nesta etapa.
Custos de provider/kernel, stack, observadores e endpoint futuro são separados
e permanecem sem prova operacional/sem créditos. 124/128/1024 inalterados.
A janela do fixture não se torna freshness aprovada de produto ou do LAB.

[Qualificação](SDR-QUALIFICATION.json), [fontes](SDR-SOURCE-HASHES.json),
[artefatos](SDR-ARTIFACT-HASHES.json); logs SDR-*-build/ctest/LastTest/layout.
N0/T0–T14 e pins históricos intactos; chave fixture anterior não aberta/usada/
excluída, receipt inalterado. HEAD/WIP preservados, PST0.6.4 inalterada.

## Primeiro gate nativo e pedido de endpoint

A composição offline do driver está concluída no alcance acima. A menor prova
bilateral nativa futura precisa de controlador administrativo ORIGINAL e runtime
SCM virtual ORIGINAL, referências independentes nas duas pontas e evidência do
transporte/direitos/lifetime reais. Dois peers same-user não provariam essa
fronteira. A prova de endpoint não é a prova de negação KSP MACHINE.

STATUS DO PEDIDO DE ENDPOINT: NOT_READY / NÃO EMITIDO. Motivos concretos:

1. Não há backend nativo de endpoint/IO pendente/entrega de referências ligado ao
   driver e qualificado. Hoje a entrega é cópia de DATA e o original é local ao
   modelo. Nenhuma autenticação bilateral nativa foi demonstrada.
2. O registro operacional original permanece vazio. Não foi autorizado criar
   endpoint nem ativar os participantes SCM. Não há nesta task observação fresca
   de token/serviço pessoal original disponível; processo futuro não pode ser
   preenchido com identidade histórica ou fixture.
3. Bindings originais/proteção, métricas/headroom e fechamento precisam ser
   delimitados na operação, sem custos/limites inventados. Não basta aprovar
   executar o driver DATA atual como se já tivesse backend autenticado.

Preparável: artefatos/hashes reais, nome SCM/conta/purpose, perfil tipado, layout
resolvido/frozen e escopos separados. Fresco: contexto do controlador, originais,
PID/criação/token/grupos/SCM/imagem do runtime, recursos e conclusão/fechamento.

A eventual criação/conexão de endpoint exige aprovação própria; sessão SCM/CNG,
implantação/Start/Stop e criação de parent, se necessários, exigem efeitos e
aprovações separados. Nenhum comando com placeholders foi emitido e nenhuma
aprovação foi criada em nome do proprietário.

[Próximo prompt técnico](NEXT-SDR-NATIVE-ENDPOINT-ADAPTER-OFFLINE.md).
Caminho até NEW_INSTALL: qualificar o adapter nativo sem efeitos → completar
bindings/pedido exato → obter autorizações separadas → provar SCM/token/KSP e
fechamento factual → executar somente a cerimônia NEW_INSTALL/CA_INITIALIZE
explicitamente autorizada. Sem retomar TPM/Q4 ou exigir bootstrap C8 por inferência.
E17/E18/E21 sem promoção; E19/E20 independentes; E22 pendente.
ED11/Phase6 PARTIAL, autoridade DENY.
