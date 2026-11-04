# SCQ — adapter Win32 qualificado offline

TASK: PACC-PHASE6-SDR-NATIVE-ENDPOINT-ADAPTER-OFFLINE
Data: 2026-10-30. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservado.
Estado: QUALIFIED_OFFLINE; autenticação bilateral nativa NOT_DEMONSTRATED.
Autoridade DENY; ED11/Phase6 PARTIAL. Nenhum pedido operacional emitido.

## Integração e callers

`apps/server/personal_scq_transport_win32.h/.inc` implementam o adapter privado
Win32 dentro de `PAPACC_SCQ_SESSION_DRIVER`, no packet canônico existente.
Não há novo owner, serviço, loader de aprovação ou IPC genérico. N0/T0–T14,
perfil/record SCQ1048 e receipt1052 permanecem sem alteração.

- `scqt_create/connect/connected/open_local`: fábrica exclusiva local, uma
  instância, namespace privado aleatório CNG, DACL protegida desde criação,
  SYSTEM/administrador/runtime exatos, sem herança; cliente anônimo, sem
  impersonation. Não adotar colisão, reconectar ou repetir conexão.
- `scqt_prepare`: custódia limitada de referências de endpoint/evento já
  adquiridas pelo custodian original; handles isolados não provam sua origem.
- `scqt_current`: correlação kernel do peer com processos retidos, criação,
  fontes/token/grupos/restrições/SCM/perfil/imagem governados existentes.
  PID, nome, SID resolvido, nonce ou frame isolados não autenticam o peer.
- `scqt_deliver_refs`: somente o controlador abre a referência adicional
  PROCESS_DUP_HANDLE ao runtime, correlacionada ao processo original ainda
  retido; duplica exatamente os três arquivos com direitos0x00120089.
  Esse direito de processo NÃO é entregue ao runtime. Não duplicar tokens,
  chaves, segredos, handles de administração ou autoridade reutilizável.
- `scqt_accept_refs`: destinatário captura referências locais do handoff
  original, duplica direitos mínimos e valida identidades/roles/atributos.
  Valores da tabela de handles remota não são interpretados como handles locais
  nem serializados no frame. A aquisição local não fabrica a fonte OHI.
- `scqt_start_io/poll/apply`: IO overlapped parcial, buffer estável, conclusão
  factual, uma transmissão por perna, consumo existente anterior aos efeitos.
  Aplicação continua usando `scqs_receive/ack` e o receptor original existente.
- `scqt_stop/close`: cancelamento não comprova conclusão; IO pendente e falhas
  preservam OVERLAPPED, buffers, referências, primeira falha e UNKNOWN.

Callers de produto: `papacc_configuration_personal_scq_transport_data/poll/stop`
no owner; fechamento via `scqs_close` antes de liberar fontes/inputs; perda do
peer solicita cancelamento. Reutilização do buffer por APIs DATA é negada durante
IO; output parcial não é exportado. Guards de retire/pending existentes continuam
bloqueando estado não terminal. Testes native/model do root confirmam DENY sem
registro. Não há entrada que injete endpoint/approval a partir do chamador.

As fábricas são compiladas e chamadas somente com APIs substituídas no ensaio.
Sua ativação no produto NÃO está ligada a uma decisão operacional válida:
`personal_scq_registered_owner_decision` continua literalmente NULL, e o root
não possui caminho para criar/conectar endpoint. O pump nativo permanece DENY.
Uma futura ativação exige binding original e autorização própria de endpoint;
uma autorização genérica SCQ não dispensa esse gate.

## Fechamento e limite do ack

Receipt/ack DATA preserva sua semântica: pode validar um eco sem comprovar execução
ou autenticação do destinatário. Nenhuma API nova concede KSP/admissão por ack.
A fonte original continua independente dos bytes recebidos.

Referências duplicadas remotamente são acompanhadas no controlador. Não executar
DUPLICATE_CLOSE_SOURCE contra números remotos que possam ter sido reutilizados.
Sem backend de recibo nativo autenticado de fechamento, somente o término observado
do processo destinatário original retido prova a destruição de sua tabela de
handles. Esse término é apenas modelado nos positivos; não há Stop real executado
ou autorizado. Ack DATA com destinatário vivo => fechamento UNKNOWN/retido.
Um fechamento incerto não é repetido e não libera consumo/quota.

A prova de origem entre processos reais, a aquisição do original opaco na ponta
receptora e o recibo bilateral autenticado não são demonstrados pelo modelo.
O adapter exige essa fonte local legítima e nega sua ausência; não a reconstrói
por ponteiro, flag, recibo ou metadados remotos. O ensaio usa dois packets no mesmo
processo, explicitamente MODELO, não uma prova cross-principal real.

## Qualificação final

| Target | Build | CTests permitidos | Adapter |
|---|---|---:|---|
| Selected Debug | PASS, zero warnings | 13 PASS | 53 cenários / 1131 checks |
| Selected Release | PASS, zero warnings | 13 PASS | 53 cenários / 1131 checks |
| Legacy Debug | PASS, zero warnings | 13 PASS | 53 cenários / 1131 checks |
| Legacy Release | PASS, zero warnings | 13 PASS | 53 cenários / 1131 checks |

52 execuções CTest na allowlist `NTA-CTEST-PATTERN.txt`; suíte completa não rodada.
Regressão SDR:35 cenários/711 checks/config preservados. Também exercitados source,
perfil privado, N0 scope sem endpoint, inputs, runtime, probe, presenter e roots
native/model/readonly. `git diff --check` sem erros de whitespace.

Negativos: ausência de fonte/decisão (incluindo regressões), destinatário/token/
criação divergentes, direitos excessivos, alias/substituição, forwarding/cópia,
replay e duplicação de consumo/perna, thread estrangeira real, Stop, peer perdido,
IO parcial/pendente/truncado, cancelamento/conclusão, entrega interrompida depois
da primeira duplicação, falha de criação/conexão/abertura, namespace remoto,
DACL/direitos, fechamento incerto e falta de prova de fechamento remoto.
50 fixtures UNKNOWN mantêm backing estático e SIDs até o fim do processo.

Reais: compilação/link Win32, estruturas OVERLAPPED, helpers de SID/SD/ACL,
BCryptGenRandom e thread isolada para testar exclusividade. Modelados: processos,
tokens/SCM, fonte/decisão positiva, endpoints, eventos, duplicações, IO, cancelamento,
queries de pipe e fechamento. Nenhuma API de endpoint real foi invocada, inclusive
em fixture. Sem sessão SCM/CNG ou nova chave persistente. Chave fixture anterior
não aberta; receipt de retenção permanece com hash histórico.

## Recursos e hashes

Contexto do adapter664 bytes embutidos; driver total1800 (antes1136), delta664.
O buffer existente1052 é reutilizado com exclusividade. Selected managed1978816 /
limite3145728; Legacy1810248 / limite2097152, medidos pelo layout compilado.
Isso não comprova headroom físico/commit nem capacidade de IO/provider/kernel.
128 próprios, guard124 e orçamento administrativo1024 não foram alterados.

O controlador pode deter endpoint/evento/referência adicional de processo e três
referências na tabela do destinatário. O destinatário mantém suas duplicatas,
originais de entrada e endpoint/evento segundo os owners existentes. Contar essas
referências no escopo correto; não converter slots, aliases ou fechamento modelado
em créditos. Buffers kernel de pipe, KSP, queries, stacks, picos e retenção nativa
continuam não medidos/UNKNOWN; nenhuma operação foi admitida.

Hashes literais das fontes: `NTA-SOURCE-HASHES.json`. Binários por configuração,
logs, guards e resultados: `NTA-QUALIFICATION.json`. Manifesto final do pacote:
`NTA-ARTIFACT-HASHES.json`. Históricos SDR/ASA e pins N0 permanecem preservados;
seus resultados anteriores não foram regravados como se fossem desta compilação.

## Primeiro gate e pedido operacional

Pedido específico de endpoint: NOT_READY / NOT_EMITTED, sem comando/placeholders.
Primeira dependência factual: par ORIGINAL controlador/runtime virtual SCM e seu
vínculo à imagem/serviço/alvo/decisão operacional ainda não adquirido neste pacote.
O registro operacional está vazio; a qualificação offline não instala esse runtime,
não concede custódia e não cria uma tentativa operacional. Metadata histórica não
preenche identidades frescas. Não pedir outro teste same-user para chamar isso
prova bilateral do runtime virtual.

Menor prova real necessária: um controlador administrativo original e o runtime
SCM original NT SERVICE\PapinhoAccelerator, uma conexão privada exclusiva,
validação independente dos dois peers/originais/direitos, consumo anterior aos
efeitos, entrega só de referências não secretas, frame/receipt e evidências
separadas de fechamento, com Stop/UNKNOWN/124/128/1024. Sem CNG/CA/CURRENT nessa
prova de transporte. Endpoint, sessão SCM/CNG, eventual implantação/Start/Stop e
parent têm autorizações independentes; não supor que o runtime já exista ou que
a aprovação de endpoint permita criá-lo.

Nenhuma nova decisão normativa demonstrada: B/OHI continuam aplicáveis. A próxima
fronteira é factual/operacional, não reaprovação de ADR ou repetição dos modelos.
E17/E18/E21 não promovidos; E19/E20 independentes; E22 pendente. Phase6 PARTIAL e
PST0.6.4 preservados. Caminho NEW_INSTALL: bindings originais + autorizações
operacionais separadas -> prova bilateral -> sessão factual SCM/CNG e custódia
MACHINE -> cerimônia NEW_INSTALL autorizada/publicação governada -> continuidade.
Não retomar TPM/Q4/C8 histórico por inferência.

[Próximo prompt](NEXT-NTA-ORIGINAL-PAIR-AND-ENDPOINT-REQUEST.md).
