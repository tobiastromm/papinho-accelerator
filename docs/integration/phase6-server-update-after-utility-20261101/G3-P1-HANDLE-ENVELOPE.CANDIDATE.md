# Revisão focal G3 — envelope de handles do primeiro Start P1

2026-11-02, America/Sao_Paulo. PROPOSED / NOT_ACCEPTED / NOT_IMPLEMENTED /
NO_OPERATIONAL_AUTHORITY. Uma proposta completa subordinada a G3/G4/P5,
sem nova família normativa, serviço, owner ou infraestrutura. Não altera os
documentos accepted, o código, o instalado, quotas atuais ou atos consumidos.

## Decisão recomendada

Recomenda-se, exclusivamente para UMA nova sessão PERSONAL_LOCAL P1
TRANSPORT_ONLY de endpoint/SCQ/primeiro Start diagnóstico fechado:

| Campo de handles, por participante | Atual | Proposta |
|---|---:|---:|
| Máximo total observado | 128 | **192** |
| Guarda antes de nova aquisição: negar com count>= | 124 | **188** |
| Admissão inicial completa antes dos originais/QUERY | sujeita à guarda124 | **count<=128** |
| Distância entre guarda e máximo | 4 | **4**, preservada como tolerância |

Controlador e runtime têm tabelas DIFERENTES; cada um observa sua contagem
TOTAL, sem descontar console/CRT/loader/herdados/alias físico/token/SCM.
Não somar handles do processo pai ao filho nem subtrair recursos iniciais para
obter PASS. As contagens iniciais não são fatos históricos reutilizáveis.

Essa recomendação é uma mudança explícita de contenção de risco. A elevação
do cap é64handles por participante,50% do antigo128; não é correção cosmética
nem constatação de que192 seja limite de saúde do Windows. Duas tabelas em192
representam até384referências contadas pelo orçamento, não384objetos físicos
distintos ou reserva kernel. Sem aprovação, continuam124/128 e DENY.

## Por que o orçamento vigente não fecha o percurso

A guarda é aplicada corretamente. Não há conflito entre as normas accepted:
elas exigem124/128. A insuficiência é do dimensionamento para a composição
atual, não autorização para ignorar essas cláusulas. A API Windows mede objetos
executivos do processo; não apresenta124 ou128 como teto de segurança/saúde.

O modelo já conhecido do trecho de aquisição tem coexistência:
raw3+cadeia5+duplicatas3+readers3 =14referências, antes do fechamento de raw.
Com baseline113: após raw116; após cadeia121; após cópias124; a primeira
aquisição de reader é negada pela guarda124. O pico hipotético127 caberia no
cap128, mas não pode ser atingido mantendo a reserva pré-aquisição de4.
Esse subtrecho exige B<=110 sem outros custos. Não é o custo de toda a sessão.

Na execução nova, a consulta antes da imagem já encontrou124, com pedido e
decisão retidos. Ainda restariam imagem1+cadeia5+cópias3+readers3=12referências
nesse trecho. Mantidos os demais recursos constantes, o cenário chegaria a136,
acima do cap128. Recursos internos poderiam variar/encerrar, mas não se pode
planejar dependendo de uma liberação não demonstrada. Não inferir baseline122:
as queries anteriores foram sobrescritas e não estão disponíveis.

As evidências demonstram negações e um cenário de margem insuficiente. Não
demonstram um vazamento, teto nativo ou contagem futura invariável. A troca de
API SD avançou o percurso mas não fechou a admissão. Eu deveria ter confrontado
o custo completo com o envelope antes de propor sucessivas operações; essa
falha de planejamento não muda a validade das guardas executadas.

## Censo do percurso completo, não somente até a imagem

Escopo auditado: controlador frio, originais/cadeia/duplicatas/readers,
preparação/entrega privada/QUERY, identidade de runtime e peer, receipt/close,
espera terminal e Stop no owner existente. Um packet/sessão por participante;
loops de I/O reutilizam evento/pipe/backing, não criam nova sessão.

O quadro é uma soma CONSERVADORA de posições de referências locais pertinentes.
Não afirma que todas sejam ocupadas simultaneamente, adicionais ao baseline ou
necessárias em ambos os papéis. Algumas são aliases/pseudohandles ou exclusivas
de um papel. Contá-las como capacidade separada aqui superestima o custo
controlado, sem descontar nada da contagem nativa real. O censo não cobre os
handles que as APIs/loader/RPC/provider/kernel possam manter internamente.

| Grupo | Posições orçadas | Fonte / lifetime |
|---|---:|---|
| Originais frios raw[3] | 3 | personal_native_win32.h / cold_acquisition/cold_start; até transferência/close factual |
| Cadeia original | 9 | personal_scq_inputs_win32.h / image_chain;8diretórios+imagem, filhos protegidos e ancestrais retidos |
| Cópias originais e readers | 6 | inputs.file[3]+reader[3]; coexistem durante verificação, mantidos até close |
| Transporte local | 14 | transport.pipe/event/files[3]/delivery_process/capture_controller/capture_token/capture_refs[3]/delivered_inputs[3]; inclui aliases e ramos de papéis conservadoramente |
| Identidade de runtime | 3 | runtime_identity.process/token/check_token, último temporário |
| Referências do operador no receptor | 2 | recipient.value.operator_process/operator_token; autenticação/proveniência |
| Views do operador na fonte | 2 | admin.value.operator_process/operator_token; podem ser pseudohandle/alias já contado |
| Adaptador SCM normal | 2 | server_service native.image/start_runtime; não importar PS_RUNTIME de LAB |
| Ponte de espera de console | 1 | server_console, evento original emprestado; sem nova referência por empréstimo |
| Token primário original | 1 | bootstrap_token, pode já estar no baseline |
| Token temporário ou inesperado | 1 | check_token/unexpected_thread; falha/impersonation interrompe antes de avançar |
| **Soma conservadora controlada** | **44** | Orçamento por participante/packet; não pico executivo universal |

SC_HANDLE manager/service são contextos com CloseServiceHandle próprio; não
atribuir automaticamente um executive HANDLE por SC_HANDLE. Seus internals
RPC permanecem no custo opaco. BCRYPT_HASH_HANDLE/ALG_HANDLE, SD/LocalAlloc,
SERVICE_STATUS_HANDLE e ponteiros também não equivalem por definição a uma
unidade GetProcessHandleCount. Não transformar nome de tipo em contagem física.

remote_files[3] do emissor são identificadores da tabela DESTINO; não somá-los
à tabela do emissor. As três referências reais são contadas no runtime receptor,
inclusive antes de seus slots locais serem preenchidos. delivered_inputs e
capture_refs podem representar custódia transferida, não nova abertura; o
orçamento14 deliberadamente não depende de eliminar essa sobrecontagem.

Ao fechar uma referência uma vez pode terminar uma sobreposição, mas somente
quando ownership/lifetime/continuidade estejam provados. Nenhuma redução por
fechamento antecipado é premissa desta proposta. Close incerto retém e bloqueia
novas aquisições; não acumular novas coortes ou refazer a mesma tentativa.
SCM/RPC, autenticação bilateral, recursos transitórios, Stop/espera/close e
retenção não foram excluídos da análise: slots próprios estão acima; internals
ficam explicitamente opacos. O censo deve ser revalidado na integração; se
outro slot/material de caminho normal não estiver coberto, registrar divergência
e parar, nunca aumentar44 ou o cap silenciosamente.

## Derivação quantitativa e natureza de cada número

| Parcela | Valor | Natureza / fundamento |
|---|---:|---|
| B: maior contagem inicial aceita | 128 | NOVA escolha administrativa de entrada; conserva o antigo cap como filtro inicial, não presume baseline do host |
| C: posições controladas conservadoras | 44 | Soma do censo fonte acima; inclui sobrecontagens de aliases/baseline/ramos, não inferência de amostra |
| U: tolerância de crescimento opaco além do censo | 16 | Escolha de risco; duas vezes a variação localizada+8 como escala deliberada, NÃO inferência causal, estatística ou teto dos internos |
| T: distância guarda-cap | 4 | Escolha de preservar a folga vigente; NÃO máximo demonstrado de custo interno de uma chamada |
| **Cap=B+C+U+T** | **192** | Composição do orçamento proposto |
| **Guarda=Cap-T** | **188** | Antes de novas referências; negar count>=188 |

Para a maior entrada permitida:128+44=172;16unidades de tolerância até188,
mais4até192. Para entrada113, o mesmo censo+U chega173, antes da folga final;
isso é aritmética condicional, não previsão de que o processo real ficará173.
Não somar B+44 como handles fisicamente novos demonstrados: trata-se de uma
reserva conservadora de planejamento, com contagem total nativa preservada.

U16 é uma tolerância limitada que o proprietário pode aceitar ou rejeitar.
Não foi observado o runtime dessa sessão nem há máximo dos custos RPC/loader/
provider/kernel. O salto+23 do percurso Shell antigo não é ignorado como zero
nem importado como custo do P1 atual; aquele caminho não é executado no P1.
Outras inicializações podem exceder16. A proposta permite negar quando isso
ocorrer, não promete chegar a RUNNING ou eliminar qualquer nova falha.

Não há fórmula que converta esse número em bytes kernel/segurança universal.
Mais handles podem pressionar host/serviço antes de uma amostra; nenhuma margem
opaca é reserva física. Mantém-se G1/G5 e a necessidade de escolha explícita de
risco. Não recomendo dobrar automaticamente para256 ou usar1024 de outro domínio:
nenhum deles foi necessário para compor este orçamento e seriam riscos distintos.

## Regra proposta de admissão e consumo do orçamento

Somente se essa alternativa for aceita integralmente:

- Na entrada fria P1, usar a query completa existente anterior ao primeiro
  original, DEPOIS das inicializações necessárias já efetuadas; exigir B<=128.
  Não mover medição para antes do CRT/token/console necessário nem subtrair
  essas referências. O censo completo44 já reserva as fases subsequentes.
- Runtime: mesma regra B<=128 na primeira admissão local QUERY, após seu
  contexto original estar disponível. Não exigir PID/token antes de Start.
  O processo pode exceder antes de ser observável: G5 continua risco, não PASS.
- Depois, exigir total<=192 em cada observação de recursos e count<188 antes
  de cada nova referência/efeito criador aplicável, mantendo freshness até
  efeito. Não usar B como franquia a descontar nem renovar orçamento por fase.
- Duplicação cross-process também conta na tabela destino. Antes de acrescentar
  referências remotas, aplicar gate fresco de contagem do destinatário pela
  referência original de processo já retida, sem reabrir por PID ou novo canal.
  Falta de referência/dado legítimo=>DENY. A primeira observação do runtime e
  validação posterior de ambas as tabelas permanecem obrigatórias. Isso não
  garante ausência de alteração concorrente entre query e DuplicateHandle.
- Nenhum guard/state de recursos ausente permite fallback permissivo no P1.
  Fonte/objeto/SD/Stop/deadline/UNKNOWN material continuam negando. O ajuste de
  limite não restaura origem administrativa, confirmação ou capacidade plena.
- Retenção fica no MESMO orçamento/owner; nenhuma nova sessão/coorte simultânea,
  crédito automático, fechamento interno ou refund por timeout. Se houver
  excesso, negar novos efeitos e preservar UNKNOWN, sem kill/cleanup implícito.

## Especialização normativa exata necessária

Não se trata de corrigir interpretação de124. Aprovar B autoriza APENAS a
precedência focal dos números/regra de entrada acima sobre:

| Fonte accepted | Passagem afetada |
|---|---|
| FIRST-DIAGNOSTIC-START-ASSURANCE.CANDIDATE.md incorporado por decisãoG0–G9 | G4, preservação124/128 nos participantes desta sessão; G3 exige orçamento específico |
| INITIAL-QUERY-AND-G3.CANDIDATE.md incorporado pela decisãoQUERY/G3 | B3 guardas124/128 e linha Handles da seleçãoG3; mesma exigência de contagem/gate antes de adquirir |
| EXISTING-PROGRAMFILES-ORIGINAL-BINDING.CANDIDATE.md incorporado por P0–P7 | P5 guard124/cap128 e proibição de aumentar quotas, SOMENTE para esses números neste ensaio |
| STARTUP-SOURCE-AND-CLOSED-READINESS.CANDIDATE.md / SSO-B | S6, preservação numérica124/128 somente no domínio P1 ora delimitado |
| SCQ-TRANSPORT-ONLY-NO-KSP.CANDIDATE.md | TO7, mesmo termo numérico no P1; demais exigências e G1 aceito intactos |
| ONA-SCQ-CROSS-PRINCIPAL-DELIVERY.CANDIDATE.md | Regra7, mesmo termo numérico124/128 para os dois participantes P1 |

O registro accepted futuro deve incorporar explicitamente essa precedência
antes da implementação. Não editar autonomamente accepted antigos; preservá-los
e ao candidato original. Não criar família de ADRs ou decisões por microcorreção.
Não há conflito entre accepted atuais; a proposta é uma alteração focal sujeita
à decisão do proprietário. Sem essa decisão, nenhuma implementação dependente.

Escopo EXCLUSIVO: uma nova sessão P1, PERSONAL_LOCAL, TRANSPORT_ONLY, existente
instalação já qualificada, serviço PapinhoAccelerator/NT SERVICE\PapinhoAccelerator,
comando diagnóstico fechado, um controlador e um runtime, mesmos E/Q/S,
QUERY/SQB2/OHI/SSO-B/SCQ/owner. Não estender a READ, NEW_INSTALL, CA, KSP,
N0, LAB/BD, UPDATE/staging/SCM registration ou Start produtivo.1024 e as quotas
dos outros domínios NÃO mudam. Uma eventual invocação no perfil errado deve
continuar128/124 ou DENY conforme seu contrato, nunca192 por argumento CLI.

## Demais valores G3 e garantias inalterados

| Parâmetro não relacionado a handles | Valor vigente preservado |
|---|---:|
| Crescimento privado, sobre READY próprio, por participante | 1994944bytes |
| Crescimento working set, sobre READY próprio, por participante | 1994944bytes |
| Piso físico disponível do host | 7239172096bytes |
| Piso commit disponível do host | 9823326208bytes |
| Freshness medida até o efeito | 100ms |
| Janela original sem renovação | 240000ms |
| Duração global compartilhada | 60000ms |

Preservar quota managed3MiB no target aplicável, envelopes de bytes/user/kernel
e demais gates. Não somar WS/private como duas reservas físicas ou transportar
pisos U4/B3/R3. Quotas de threads/processos/endpoints/tentativas não mudam.
Custos opacos continuam UNKNOWN, não máximos demonstrados. Originais/identidade/
proveniência/owner/DACL/ancestrais/reparse/continuidade/imagem/SCM/conta/SID,
três confirmações e consumos independentes, Stop, prazo mínimo, autenticação
bilateral e receipt/close antes de RUNNING diagnóstico permanecem obrigatórios.
Nenhuma readiness produtiva/CA/CURRENT/rede/KSP/TPM/NV/Q4 decorre dessa seleção.

## Integração após eventual aprovação, sem novo subsistema

Reutilizar o campo max_handles e digest quantitativo G3 existentes. Alterar os
valores do perfil P1 aceito, não o tamanho/versão de SCQ1/SQB1/SQB2 ou receipts.
Receptor mantém política local compilada e valida o mesmo digest: peer/argv/
carrier jamais escolhem teto maior. Perfil/binding originais necessários para
entrar no caminho específico; fonte ausente mantém DENY.

Implementação deve cobrir TODOS os pontos, não simples replace global124/128:
cold entry/parser/selected-exact/plan_preimage/cold raw, cadeia/duplicates/readers,
admissãoG3 controlador/QUERY receptor, evento/pipe/peer/delivery remota e Stop/
close. Predicados de outros perfis continuam estritos. Sem setter permissivo,
novo owner ou adiamento de verificação para satisfazer a quota.

Qualificar offline o percurso completo/modelo de coexistência, não só primeiro
open: B128 permitido/B129 negado;187antes da aquisição permitido/188negado;
192cap permitido sem criação/193negado; queryfalha/UNKNOWN; destinatário cheio
antes de DuplicateHandle; mistura de perfis/políticas/artefatos; fonte ausente,
Stop/replay/consumo duplicado; receipt/close/retention; READ/NEW_INSTALL/BD/LAB
inalterados; demais G3 idênticos. Não transformar modelo em teto nativo.
Se o censo44 ou compatibilidade normativa divergir factualmente, STOP e
apresentar SOMENTE essa divergência antes de mudar número/escopo.

Depois de toda integração/qualificação offline: congelar UMA imagem final,
conferir delta e preparar UPDATE U1–U7/U2 pelo updater protegido existente.
Só após UPDATE manual autorizado/reconciliado preparar nova sessão E/Q/S e
novos originais legítimos. Não repetir campanhas, coletas ou autorizações
consumidas. Não otimizar provider/API/criptografia para evitar pedir essa decisão.

## Alternativas e decisão solicitada

**A — não aceitar a revisão:** manter124/128 e DENY. Não continuar tentativas
operacionais sob o mesmo orçamento insuficientemente dimensionado; seria
necessária redução legítima/demonstração nova, não fechamento arbitrário.

**B — recomendada:** aprovar integralmente esta revisão focal de handles
192/188/entrada128 e suas condições, reconhecendo as parcelas de risco U16/T4
e a ausência de garantia de completar Start. Autoriza SOMENTE registro normativo
e implementação/qualificação offline no owner existente. Não concede operação.
As demais escolhas G3 e garantias são expressamente mantidas.

O proprietário ainda não selecionou B. A autorização atual permitiu preparar
a proposta, não registrá-la como ACCEPTED ou implementar dependências.

## Evidência e limites desta entrega

G3-P1-HANDLE-ENVELOPE-EVIDENCE.json registra censo44, fontes/hashes e aritmética.
Conferência offline: soma44;128+44+16+4=192;192-4=188; submodelo113+14=127;
cenário do ponto124mais12=136. Não executados produto, builds, testes nativos,
coleta, UPDATE, E/Q/S/Start/Stop, SCM/ACL ou modificação de quota/código operacional.
Não declarar qualificação de implementação ainda inexistente.

Referências primárias:
- https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesshandlecount
- https://learn.microsoft.com/en-us/windows/win32/services/scm-handles
- https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-closeservicehandle

Preservar HEAD/WIP/pins/históricos/EXEs/originais/retidos/atos consumidos,
PST0.6.4/DENY/UNKNOWN/quiescência OS e ausência de cleanup/refund/recuperação.
