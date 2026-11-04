# SCQ — fonte administrativa atribuída integrada offline

Task PACC-PHASE6-SCQ-ATTRIBUTED-ADMIN-AUTHORITY, 2026-10-30.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados.
B/OHI/F1–F6 permanecem vigentes; nenhuma nova ADR ou decisão operacional criada.

## Resultado e callers

A fonte está ligada ao receptor no packet canônico. O caller deixou de passar
NULL literal: chama `personal_scq_native_original`, que somente entrega o
original derivado do próprio controlador, depois de confirmação e consumo.
Não existe setter público de approved, grant via CLI/arquivo/IPC ou callback
positivo de fixture no root. As novas entradas privadas são:

- `papacc_configuration_personal_scq_admin_prepare`: vincula o registro original
  da decisão operacional, plano/proteção/conteúdo, operador e referências retidas.
- `papacc_configuration_personal_scq_admin_confirm_console`: apresenta a projeção
  fechada e os caminhos congelados, exige console existente e o texto específico
  OWNER_SCQ + identificador exato, revalidando o contexto antes/depois.
- `papacc_configuration_personal_scq_admin_take`: consome uma vez no controlador,
  antes de qualquer primeiro efeito; não executa nem autoriza Start por si só.
- `personal_scq_native_original` / `scqa_borrow`: vinculam o runtime original fresco
  e entregam o empréstimo limitado ao receptor existente.

`personal_scq_admin_win32.h/.inc` é subestado do owner atual, não novo owner,
serviço, canal ou persistência. O registro privado imutável pertence ao contexto
original da decisão; liga pedido/decisão byte-exatos, tentativa, política, alvo,
key/geração, serviço/conta, referências completas e identidades. Fica vinculado ao
packet e PID/criação/TokenId/AuthenticationId/ModifiedId do controlador original.
Não é registro de confiança global nem formato persistido/restaurável.

A captura reutiliza os readers originais protegidos e verificados por CNG, o
bootstrap token já retido e o processo atual original do controlador. Esse
handle de processo é pseudo-handle local, não uma referência para serializar ou
fechar como aquisição adicional. Referências de arquivo/token continuam sendo
borrows do mesmo packet; só o owner original as fecha após os consumidores.

A fonte revalida TokenUser, PID/criação, stats e enumeração completa de grupos,
SID bounds, Administrators habilitado e não deny-only, grupo Interactive,
ausência de Service/restricting SIDs/IsTokenRestricted, elevação e sessão. O
runtime é validado separadamente pelo adapter existente; SID/PID do administrador
não podem ser os do runtime. Alteração de decisão, política, refs ou contexto
invalida a fonte. O registro não é aceito em outro packet ou nova invocação.

## Confirmação e consumo

O helper de console F1–F6 foi extraído e compartilhado, preservando os bytes
funcionais de NEW_INSTALL (reconstituição e SHA conferidos contra OAS). A
confirmação SCQ é distinta de NEW_INSTALL/CA_INITIALIZE. A apresentação em console
mostra apenas campos tipados reconhecidos: propósito/serviço, IDs/pins, peers,
SID/atributos, referências/digests e caminhos frozen em Unicode. Nenhum arquivo
arbitrário ou conteúdo bruto é impresso. Entrada redirecionada, Escape, Stop,
texto divergente, falha de leitura/escrita ou perda de elegibilidade negam.

Estados: capturado, apresentado, confirmado, tomado, fechado, UNKNOWN e inválido.
O take tem revalidação antes e depois do CAS monotônico. Stop já conhecido nega
antes de consumo; uma falha posterior ao consumo não o desfaz. O uso único do
controlador é separado do CAS de uso do empréstimo pelo destinatário. A falta de
runtime antes do primeiro efeito não fabrica PID/token futuro: o vínculo dinâmico
somente ocorre depois do take e da observação original válida.

Fechamento incerto conserva consumo e referências; não rearma, apaga ou devolve
créditos. pending_blocks/retire/close reconhecem a nova retenção. Copiar estado,
recriar um packet ou restaurar um registro não reconstrói a confirmação original.
Isso não prova persistência nativa, resistência global a rollback ou fencing.

## Estado nativo atual: EMPTY/DENY

`personal_scq_registered_owner_decision()` retorna NULL: não há decisão
operacional SCQ específica aprovada/registrada para esta nova sessão. Essa
seleção nativa não tem entrada por argv, arquivo, IPC, flag ou enum. Nenhum
registro de aprovação foi produzido por esta task.

Por isso prepare/confirm/take permanecem NOT_SUPPORTED; o receptor não obtém
original válido e o KSP não é alcançado. O root foi testado inclusive com
content_verified, controller_consumed e metadata sintética de fonte: continua
DENY. Um modelo de decisão explícito só existe na unidade isolada; não pode ser
selecionado pela composição nativa.

A implementação permite ligar um registro original legítimo quando ele existir;
não afirma que há autorização, autenticação bilateral nativa ou capacidade
operacional disponível. O empréstimo local em modelo não é entrega cross-process.
Nenhum ponteiro é serializado como referência remota. Endpoint real permanece
não criado, não conectado e não testado. N0/T0–T14/same_token intactos.

## Qualificação

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings. Allowlist de
11 CTests/configuração: 44 execuções PASS, incluindo presenter e o novo source.

Fonte administrativa: 33 cenários / 549 verificações por configuração, com
fluxo positivo até o receptor, registro ausente/divergente/consumido, propósito
errado, outro controlador, clone/packet novo, PID/criação/token divergentes,
refs/decisão substituídas, enumeração incompleta, admin desabilitado/deny-only,
restrição/sessão inválidas, replay, Stop, prazo, thread estrangeira e UNKNOWN.
Console compartilhado: sete casos (correto, Escape, texto errado, leitura/escrita
falhas, Stop e overflow). Regressões de inputs 248, identidade163, perfil3338 e
receptor24 preservadas no alcance de cada log.

Win32 de arquivo/processo/token/SCM/console e a decisão positiva são modelados.
SID/SD/thread e o hashing CNG da regressão de inputs usam Windows real.
Não houve confirmação em console real, decisão nativa positiva, endpoint, sessão
SCM/CNG, instalação, CA, CURRENT, Start/Stop, SCM/ACL, rede operacional,
TPM/NV/Q4, limpeza/refund ou recuperação. O último UNKNOWN do source mantém
backings em armazenamento estático do fixture até sua saída; não declara cleanup.

Source embutido768 bytes (original720); registro privado de scope832 bytes é
backing separado, ausente na composição nativa atual. Crescimento fixo768 bytes.
Known managed Selected1.977.016/3.145.728 e Legacy1.808.448/2.097.152.
Grupos/buffers/stack/provider/kernel e transporte futuro têm custos próprios;
nenhum crédito ou capacidade decorre desses layouts. 124/128/1024 preservados.

[Qualificação](ASA-QUALIFICATION.json), [fontes](ASA-SOURCE-HASHES.json),
[artefatos](ASA-ARTIFACT-HASHES.json). Logs ASA-*-build/ctest/LastTest/layout.
Chave fixture persistente anterior não aberta; receipt e históricos preservados.

## Bindings e próximo gate

| Preparável antes da sessão | Dependente de observação fresca na sessão |
|---|---|
| Hashes dos artefatos qualificados, serviço/conta e propósito exatos | Processo/token/console do controlador e sua continuidade |
| Layout resolvido/frozen, perfil PERSONAL_LOCAL e escopo separado de efeitos | Identidade/proteção/ausência ou existência do parent e dos alvos originais |
| Pedido/decisão após aprovação específica, pins e scope tipado exatos | Arquivos originais/SD/conteúdo e ligação ao contexto vivo aprovado |
| Formato privado/validadores e consumo/fechamento | Registro vinculado ao packet original, confirmação local e consumo fresco |
| Plano para um runtime SCM virtual | PID/criação/token/grupos/SCM/imagem do runtime depois do efeito autorizado |
| Caps accepted e critérios DENY/UNKNOWN | Headroom, participantes, custos retidos/compartilhados, KSP e fechamento reais |

O primeiro fato de autoridade ausente é a decisão operacional específica original.
A fonte técnica não está mais ausente, mas seu registro nativo está vazio.
Além disso, a entrega bilateral/driver nativos ainda não foram demonstrados;
não se afirma que falta apenas uma aprovação para executar. Devem ser preparados
no mecanismo existente sem criar endpoint nesta autorização. Os bindings reais e
os orçamentos obrigatórios não podem ser completados com observações históricas.

Não foi emitido pedido operacional executável ou comando com placeholders.
Endpoint, sessão SCM/CNG, implantação/Start/Stop e parent devem ser delimitados
como efeitos/autorização separados. NEW_INSTALL/CA_INITIALIZE seguem cerimônias
próprias após qualificação factual, sem retomar TPM/Q4 ou bootstrap C8 por inferência.

[Próximo prompt](NEXT-ASA-SCQ-SESSION-DRIVER-OFFLINE.md).
E17/E21 sem promoção; E18/E19/E20 independentes, E22 pendente.
ED11/Phase6 PARTIAL; autoridades DENY; PST0.6.4 preservada.
