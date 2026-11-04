# Primeiro Start diagnóstico após UPDATE qualificado

Estado: NOT_READY / DENY. E21 PENDING; ED11/Phase 6 PARTIAL.
Revisão offline em 2026-11-01; branch main, HEAD
f341985924d29870d8ff627ed37c8a0874ad6469. Sem execução do produto,
consulta SCM, endpoint, Start/Stop, UPDATE ou alteração de ACL nesta revisão.

## Imagem e SCM: evidência e validade

Reutiliza SERVER-UPDATE-RECONCILIATION.md e a transcrição original
OWNER-MANUAL-SERVER-UPDATE-OUTPUT.txt, cujo SHA256 foi reconferido:
8cbbd6e127d8fbb07187a65f755c1723f36341d64da3920691ac7b533ada4f80.
O readback dessa sessão identifica o canônico instalado de 762880 bytes,
417614100:1900544:539611, SHA256
3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b.
O arquivo Selected/Release do workspace foi novamente hasheado e coincide.
Isso não é nova atestação do objeto instalado ou de páginas mapeadas.

O último estado SCM demonstrado é STOPPED/PID0, serviço PapinhoAccelerator,
conta NT SERVICE\PapinhoAccelerator, OWN_PROCESS, DEMAND_START,
SID UNRESTRICTED e comando exato:
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Esse snapshot não reserva o serviço: identidade/configuração/estado serão
revalidados antes de qualquer Start. DLLs continuam sob os pins históricos:

- libssl-3-x64.dll: 1321472 bytes,
  3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
- libcrypto-3-x64.dll: 7217664 bytes,
  09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.

O EXE anterior permanece no nome .previous registrado na reconciliação.
Nenhuma atualização adicional foi demonstrada necessária nesta revisão.

## Callers existentes e primeira lacuna

server_launcher_scq_cold_start_win32.inc:15 implementa o entrypoint privado
`--owner-manual-first-scq-start`. Exige dois caminhos e respectivos SHA256
(request e decisão), pin da imagem e os oito parâmetros G3. Os argumentos
são DATA; não concedem aprovação. personal_scq_cold_acquisition_win32.inc
adquire os três objetos originais, retém referências, inspeciona identidade
e descritor e exige que o controller execute a imagem canônica instalada.
papacc_update.exe não é o controller desta sessão.

Primeira lacuna concreta: não foi identificado, nos artefatos desta cadeia,
um par específico de request/decisão da NOVA sessão com caminhos definitivos,
bytes/pins, proveniência administrativa e condição de aquisição protegida.
Isso não afirma inexistência de arquivos fora do escopo examinado. As decisões
normativas accepted e autorizações UPDATE/registro consumidas não substituem
esse par. Tampouco se deve produzir uma decisão operacional em nome do owner.

personal_scq_inputs_win32.inc:16 exige owner igual ao SID do operador e DACL
protegida para request/decisão. A exceção Administrators é exclusiva da imagem
TRANSPORT_ONLY, não desses documentos. SHA256 isolado não supre essa guarda.
Não há, no caller frio examinado, criação ou endurecimento desses dois arquivos;
ele abre originais já disponíveis. Portanto ainda não existe comando exato
completo, nem pedido operacional READY emitido nesta revisão.

Essa exigência é coerente com OHI, sua decisão accepted
DECISION-personal-scq-original-handoff-20261030.md e QUERY/G3 (preservação de
request/decisão/READ estritos). A proposta QUERY/G3 incorporada foi reconferida:
5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca.
Não foi identificado conflito normativo ou justificativa para relaxar a guarda.

Depois da aquisição, personal_scq_first_controller_win32.inc:29–31 exige três
confirmações atribuídas independentes: SCQ, endpoint e Start. O prompt atual
não as aprova operacionalmente. A escolha dos originais não dispensa essas
confirmações, os consumos específicos ou a autorização futura do ensaio.

## Gates prévios e tardios

Antes do efeito: imagem canônica/controller original e dependências, ancestrais
e proteção, request/decisão originais, token administrativo completo, SCM parado
e comando exato, namespace, três atos independentes, Stop e G3. O carrier
privado identifica contexto/tentativa; posse não é autoridade.

G3 permanece exatamente: máximo128/guard124 handles completos; crescimento
privado1994944 e working set1994944 bytes POR participante sobre READY próprio;
pisos físico7239172096 e commit9823326208 bytes; freshness100 ms até o efeito;
janela original240000 ms sem renovação; duração global60000 ms compartilhada.
São tolerâncias selecionadas de um ensaio, não máximos nativos comprovados.
Deadline receptor é o menor prazo aplicável, inclusive o SQB2 recebido.
Os picos171/177 de UPDATE não são baselines nem dispensa do guard124/128.
Sem observação fresca suficiente, o caminho deve negar antes do efeito.

Só após Start autorizado: PID/criação do runtime, TokenUser/grupos/restrições,
continuidade processo/SCM/imagem, peer real e autenticação bilateral. QUERY
recebe candidatos não autoritativos; empréstimo só após admissão independente.
Receipt e close originais precedem RUNNING exclusivamente diagnóstico. Ack DATA
não prova autenticação. START_PENDING não prova readiness. O caminho existente
também verifica encerramento finito, processo e SCM STOPPED; não autoriza um
ControlService/Stop adicional por inferência. O pedido futuro deve discriminar
esses efeitos e não prometer cancelamento imediato de I/O.

## UNKNOWN e qualificação

Close conhecido do último UPDATE não resolve o cohort/close/créditos UNKNOWN
da tentativa anterior. Seu candidato retido não será adotado, aberto como fonte,
renomeado ou removido. Não há refund, limpeza ou declaração de quiescência OS.
A futura admissão deve negar se uma obrigação desse cohort conflitar com a nova
operação; presença no filesystem não demonstra liberação de referências.

Cadeia fria QUERY/SQB2/G3 e ligação pré-Start já qualificadas em modelo no
closeout phase6-prestart-continuation-20261101. Última qualificação de artefatos
na phase6-utility-replacement-integration-20261101: quatro builds PASS, zero
warnings, 68 pares CTest afetados PASS. São evidências históricas reutilizadas,
não testes executados nesta revisão. Código não foi alterado; não se repetiram
campanhas inalteradas. Bilateral nativa permanece NÃO DEMONSTRADA.

## Caminho mínimo

Concluir a seleção/proveniência e o procedimento governado de disponibilização
do par original request/decisão, sem inventar aprovação nem criar nova raiz ou
infraestrutura. Somente então fechar seus pins e o pedido byte-exato dos atos
independentes endpoint/SCQ/Start, com gates frescos e ausência de retry.
Após autorização específica, execução manual única; reconciliação posterior
separando QUERY, autenticação, empréstimo, receipt/close, diagnóstico e fechamento.
Nenhuma dessas etapas equivale a NEW_INSTALL produtivo, CA_INITIALIZE ou CURRENT.
