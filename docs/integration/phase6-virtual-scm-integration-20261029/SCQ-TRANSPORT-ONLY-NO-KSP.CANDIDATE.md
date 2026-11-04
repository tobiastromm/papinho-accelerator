# Especialização focal SCQ — transporte sem KSP

Status: PROPOSED. Task PACC-PHASE6-SCQ-TRANSPORT-ONLY-FOCAL-PROPOSAL.
Direção do proprietário: primeira prova exclusivamente de transporte, aquisição
de originais, fechamento e readiness; preparar esta proposta, sem aceitá-la.
Nenhuma autorização operacional. TO0–TO9 formam um conjunto indivisível.

## Necessidade e precedência delimitada

OHI accepted incorpora OSR-SCQ-ORIGINAL-HANDOFF.CANDIDATE.md
(SHA25634c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561).
Em Handoff/destinatário, exige vínculo à chave MACHINE fixture exata e distingue
o administrador do processo que consulta KSP. A regra3 da alternativa B accepted
(SHA256e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1)
preserva aquela chave. SSO-B/S0–S7 permite preparo/readiness fechado e proíbe KSP
por consequência do startup, mas não dispensou o campo. scqp_static/scqp_key,
prepare/dispatch e original_binding ainda o exigem.

Não há GOVERNANCE CONFLICT: a dependência atual é coerente com o ensaio SCQ/CNG.
Ela é desnecessária para a prova menor de transporte pretendida. Esta proposta
especializa SOMENTE essa dependência para o novo propósito abaixo. Não altera
os bytes dos accepted, não aceita outras cláusulas propostas por referência e
não usa ausência de CA/instalação como autorização. Enquanto não aceita,
prevalece integralmente a exigência atual, inclusive no código fechado.

## TO0 — Escopo exclusivo

Definir SCQ_TRANSPORT_ONLY como especialização privada PERSONAL_LOCAL no MESMO
runtime, owner, receptor OHI e transporte SCQ. Limite: um par original
administrador/runtime virtual, uma tentativa viva, entrega de referências de
leitura, validação bilateral, receipt, prova original de close e readiness
explicitamente diagnóstica. Não é SCQ/CNG, NORMAL, RECOVERY, LAB, NEW_INSTALL ou
CA_INITIALIZE. Não cria serviço/owner/root/ledger/authority provider/IPC genérico.
A seleção do perfil ou de um artefato não concede autoridade.

## TO1 — Exceção exata e não aplicabilidade

Somente para SCQ_TRANSPORT_ONLY, tornar não aplicável a seleção de chave MACHINE
nas passagens OHI Handoff/destinatário e efeitos separados e na regra3 B.
Também fica não aplicável a função de consultar KSP desse ensaio: o destinatário
continua sendo o runtime virtual original com TODA sua validação, mas não executa
consulta KSP. A única dispensa no binding é key; os demais componentes do pedido,
decisão, política, tentativa, imagem, serviço, conta, alvo, processos e referências
originais permanecem obrigatórios.

key não aplicável é uma propriedade do propósito aprovado, não UNKNOWN convertido
em PASS. Não exigir CA ou chave anterior à primeira instalação para testar esse
transporte. Não criar chave, fixture MACHINE ou identidade CA para preencher o
campo. Nos demais perfis, incluindo SCQ/CNG e propósito READ existente, manter
key obrigatório com a validação atual; não estender esta dispensa por inferência.

## TO2 — Discriminação e representação privadas

Propor o propósito privado SCQ_TRANSPORT_ONLY com valor2 no campo purpose já
existente. O propósito READ atual permanece1. Manter version1, profile SCQ2,
schema não secreto1 e os campos/tamanhos do record existentes; não modificar
ROOT/CURRENT/PACC wire, N0 ou formatos persistidos. A extensão é a aceitação
explícita desse novo propósito SOMENTE por construtores/rotas de transporte
fechado; construtores ordinários continuam a rejeitá-lo.

Para propósito2, todos os112 bytes do campo key devem ser zero. Nos backings
locais equivalentes (incluindo WCHAR), exigir igualmente ausência canônica.
Nome não vazio, bytes após terminador, sentinel textual, whitespace ou conteúdo
ambíguo implicam DENY; não ignorar conteúdo para obter PASS. Não usar nome falso,
chave default ou a fixture CurrentUser retida. A apresentação pode dizer
“key: não aplicável — transporte sem KSP”; esse texto não é conteúdo do campo.

Os validadores deverão exigir a mesma discriminação no plano, decisão original,
origem local, associação/dispatch e ambos os pares. Se um backing precisar de um
campo privado de propósito ou construtor próprio, adicioná-lo no owner existente,
com censo/lifetime qualificados; não reutilizar campo de erro/autoridade nem
criar setter permissivo. Um key vazio ou flag closed isolados nunca selecionam
o perfil nem comprovam origem. scqp_key continua inalterado para os perfis que
exigem seletor: a dispensa é uma ramificação tipada, não relaxamento global.

Versões antigas rejeitam propósito2; manter fail-closed. Não converter READ em
TRANSPORT_ONLY, nem o inverso, não editar records/atos já consumidos, não fazer
downgrade para contornar rejeição e não reinterpretar históricos.

## TO3 — Ausência de KSP e de capacidade produtiva

Não permitir NCrypt provider/key acquisition, abertura, assinatura, exportação,
desencriptação, criação/rotação de chave ou sondagem de acesso à CA nesse perfil.
Negar dispatch/probe KSP antes de qualquer chamada ao provider, mesmo se forem
apresentados seletor válido, recibo, contexto closed alterado ou metadados de
outro propósito. Não resolver key ausente em qualquer chave do host.

Resultado de transporte não é negação KSP, custódia MACHINE, confiança CA,
permissão de emissão ou capacidade produtiva. KSP e CA permanecem
NOT_EXECUTED/NOT_DEMONSTRATED; não rotular “KSP DENY demonstrado” por um bloqueio
local. O uso futuro de RNG/hash públicos Windows/CNG para identificação dos
originais não é acesso KSP; deve ter orçamento/lifetime e autorização próprios
na operação. Nesta preparação, todos esses efeitos continuam não executados.

## TO4 — Origem e autorização permanecem

Exigir pedido operacional byte-exato de propósito2, decisão específica original
do proprietário, contexto administrativo elegível e confirmação própria atribuída
do plano. Reutilizar OHI/F1–F6, sem consumir aprovação NEW_INSTALL, CA_INITIALIZE,
SSO-B, LAB, TPM ou sessão histórica. Não fabricar aprovação, intent ou registro
de autoridade. O runtime não assume o papel administrativo.

Os originais imutáveis/protegidos, conteúdo, identidade/descritor/proveniência e
referências retidas são obrigatórios; hash/path/PID/nome/SID/nonce/JSON/DATA não
os substituem. Ambos os pares validam independentemente o escopo e a origem.
Toda projeção é tipada e não secreta; campos desconhecidos/secretos ou referências
com direitos excessivos negam. Não transferir tokens impersonáveis, privilégios
administrativos reutilizáveis, ponteiros locais ou material privado.

## TO5 — Binding fresco, lifetime e consumo

Preservar TokenUser exato, grupos/atributos completos e restrições, contexto de
logon, PID/criação, imagem e SCM originais, alvo congelado e continuidade. Nomes
SCM e service SID em grupos não substituem logon primário. Capturar valores que
só existem durante a sessão por gates frescos; não exigir PID/token futuros.

Consumir o ato controlador antes do primeiro efeito protegido, inclusive eventual
Start/endpoint separadamente autorizados. Um par, uma tentativa viva e sem
reconexão/retry/forwarding/substituição/replay. Validade permanece da mesma
invocação e política/plano protegidos; restart não restaura autoridade por
arquivo, CLI, token ou receipt. Perda de continuidade nega novos efeitos.

## TO6 — Receipt, close e readiness

Preservar a sequência ONB: offer/originais -> receipt SQR1 DATA -> ack no
controller -> produtor SQC1 de close nativo -> entrega e validação dos IDs contra
a entrega original, antes de Running diagnóstico. SQR1 sozinho não comprova
close; SQC1 não concede autoridade. Não executar APIs de HANDLE sobre IDs remotos
recebidos. Leitores dos originais e referências entregues têm lifetimes distintos.

START_PENDING não anuncia prontidão inexistente. Running fechado somente após
fonte original, validação bilateral e provas/limites aplicáveis; deve continuar
explicitamente distinguível da readiness produtiva. Nenhuma transição para
operação/KSP decorre desse Running: exige outro ato e todos os próprios gates.

Stop, cancelamento, peer perdido, divergência, efeito/IO/fechamento incertos
preservam primeira falha, consumo, buffers/referências/créditos e UNKNOWN no
mesmo owner. Receipts de cada lado separados. Não declarar quiescência global
por um close local, pelo retorno de callback, expiração ou término não observado;
sem cleanup/refund, rearm, double-close, destruição forçada ou recuperação implícita.

## TO7 — Quantitativos, relógios e UNKNOWN

Preservar124/128/1024, quotas de bytes e quaisquer gates aplicáveis de recursos,
headroom, freshness e duração. Não é exceção de envelope opaco, créditos ou
limites. O anexo técnico SCQ deste pacote identifica fontes/medições; sua
incompletude não impede decidir esta especialização normativa, mas impede
pedido/execução positivos. Nenhum número ausente é aprovado por este texto.

Separar janela de autorização, idade de observação, duração total e encerramento.
Cópia de sample não renova validade; revalidar referências antes dos efeitos,
recepção/close e readiness. Deadline expirado nega/solicita Stop, não prova que
IO terminou nem libera reservas. Não somar timeouts locais como limite global,
comparar relógios de domínios diferentes ou importar números Browser/D4–D5/LAB.

Custos Windows/pipe/SCM/token/hash/RNG, compartilhados/transitórios e retidos não
viram zero por excluir KSP. UNKNOWN obrigatório mantém DENY. Limites/risco
quantitativo do ensaio real precisam de fundamentação e aceitação aplicáveis
antes da operação; amostras finitas nunca viram ceilings universais.

## TO8 — Artefato, compatibilidade e efeitos independentes

Seleção do proprietário para PLANEJAMENTO: Selected/Release,
SHA25690123fbadc3d7e4db1e5f48ea2b7fe85f6e78f61f6adee083b00d1288b951874.
Esse binário ainda exige key e rejeita o novo propósito. Não é imagem instalada,
protegida ou atestada. Sua seleção não torna a proposta implementada.

Implementação posterior produzirá hashes derivados reais. Preservar o pin
selecionado como histórico e apresentar o novo artefato para nova seleção
explícita; não substituir silenciosamente nem inventar hash futuro. Preservar
instalações existentes, SAM console, READ/key e N0/T0–T14, sem migração implícita.

Endpoint, disponibilização/lifecycle SCM/Start/Stop, implantação/imagem e eventual
parent exigem escopos operacionais próprios e gates frescos. Parent não é criado
implicitamente, existência de diretório não comprova publicação, N0 não vira C8.
Não alterar NEW_INSTALL/CA_INITIALIZE, CA/issuer/NORMAL/RECOVERY, CURRENT/epoch,
SCM/ACL, rede operacional, TPM/Q4, recuperação ou produção PST0.6.4.

## TO9 — Aceitação e qualificação posterior

Aceitar somente por decisão explícita sobre este texto byte-exato após conferir
compatibilidade. Preservar proposta/histórico e registrar a especialização no
mecanismo normativo existente ANTES de código dependente. Aceitação autoriza
exclusivamente implementação e qualificação offline de TO0–TO9; não execução,
endpoint real, SCM/KSP, instalação ou declaração de Phase6 PASS.

Depois de aceita: implementar propósito/construtores/validação nas camadas
perfil, origem administrativa, handoff/receptor, sessão/transporte, dispatch e
owner/host; preservar seleção normal e rejeições independentes. Qualificar
Selected/Legacy Debug/Release e regressões afetadas, negativos e mutantes de
propósito trocado, key não canônico, READ sem key, cópia/downgrade de ato,
originais ausentes/substituídos, token/imagem/alvo divergentes, replay/concorrência,
Stop/perda de peer, close incerto e UNKNOWN. Provar zero chamadas KSP no novo
perfil inclusive em entradas diretas, sem callbacks positivos fictícios.

RNG/KSP/endpoint/mutações ficam simulados offline. Separar modelo de evidência
nativa. Revisar censo, lifetimes, hashes reais e política; só preparar pedido
operacional exato quando originais, continuação e bindings estiverem completos.
Sem placeholder, comando prematuro ou aprovação em nome do proprietário.

A decisão solicitável é aceitar integralmente TO0–TO9 como especialização focal
de transporte. O pedido atual só autorizou preparar esta proposta. Enquanto
pendente: key obrigatório no código, seletores operacionais vazios, DENY;
E21 PENDING e ED11/Phase6 PARTIAL. Nenhum accepted anterior foi editado.
