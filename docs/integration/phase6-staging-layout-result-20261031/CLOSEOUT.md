# Resultado nativo: corte após layout; correção de escopo binário offline

Fonte: saída manual do proprietário em OWNER-OUTPUT.txt, transcrição não
assinada/sem atestação independente de imagem mapeada. main / HEAD
f341985924d29870d8ff627ed37c8a0874ad6469 e WIP preservados.

## Reconciliação

Coleta diagnóstica a4cbd102... executada uma vez segundo a saída; autorização
594b1f6b... utilizada, inclusive sua exceção Process para a invocação. Não repetir.
O consumed0 do log é o ato STAGING, não uma autorização de coleta reutilizável.
O registro aprovado original permaneceu byte-exato; uso registrado separadamente
em OWNER-USE-RECONCILIATION.json.

- Entrada:109 handles. Query após emissão desse frame: sucesso,109, erro0.
- Após resolução de layout, antes de contexto/token/volume: sample válido com
  **132 handles**, erro1816/stage9 do sampler e localização2.
- Não é query falha: query_complete1/sample_known1 e a contagem132 confirmam o
  guard negativo. O valor observado ultrapassou guard124 **e cap128**.
- Incremento observado:23 handles nesse intervalo, não um pico global ou leak
  demonstrado. A API individual causadora e o custo interno do OS são UNKNOWN.
- PrivateUsage passou de2.826.240 para3.588.096 bytes (+761.856); WorkingSet de
  6.582.272 para8.278.016. Essas métricas não são o limite de memória gerida.
- Mínimos amostrados: físico6.296.461.312, commit8.479.985.664 bytes. Não provaram
  margem admissível nem falha de RAM. Disco não foi consultado; zeros são UNKNOWN.
- Contexto nativo não alcançado, parent/protection0, guard pré-abertura não
  consultado, SID não emitido. first_predicate0 não comprova token elegível.
- Close controlado known1/retained0; efeitos0, consumed0, RNG/confirmar ausentes.
  Quiescência interna do OS permanece UNKNOWN. Mask95 preservada; staging DENY.

O intervalo observado inclui resolução do layout e bookkeeping, não permite
atribuir todos os23 handles especificamente a ProgramFiles ou ProgramData.
Não subtrair caches/framework da contabilidade, elevar limites ou trocar host.

## Correção técnica fundamentada

O stager solicitava o resolvedor completo de primeira instalação: uma consulta
FOLDERID_ProgramFiles e uma FOLDERID_ProgramData. Staging binário não usa parent,
root de dados ou leaf de configuração. Essas dependências já eram não necessárias
no escopo aceito de staging; manter o lookup de ProgramData era composição ampla
demais, não obrigação de NEW_INSTALL anterior ao staging.

Implementado escopo privado BINARY_ONLY no MESMO PAPACC_PERSONAL_INSTALL_PATHS,
sem novo owner/root/serviço/ledger/formato persistido. Usa a MESMA API
SHGetKnownFolderPath, FOLDERID_ProgramFiles e KF_FLAG_DONT_VERIFY, mesmos joins,
validações/Stop e congelamento/revalidação. Não usa env, hardcode ou backend
alternativo para passar no guard. Dados parent/data/leaf ficam vazios e qualquer
substituição é rejeitada; o espaço físico desses campos continua contabilizado.

O helper completo NEW_INSTALL e o existente histórico foram preservados. O
validador genérico de layout completo rejeita BINARY_ONLY; o binário rejeita
perfis completos. O acesso binário nativo exige o cohort original de staging,
self/thread/invocação/phase válidos no owner. Native-current só aceita esse
escopo enquanto tal cohort está vivo. F0/CA/SCQ não ganham autoridade pelo novo
modo. Nenhuma decisão accepted ou bytes de CURRENT/root/manifest foram alterados.

Normas: destino binário pessoal accepted exige known folder/path congelado, não
root de dados para disponibilização de binários. F0–F9/M0–M11 permanecem integrais
para instalação e custódia. A alteração é granularidade de implementação de
DATA já autorizada, não exceção a proteção, quotas ou fonte administrativa.

**Redução real de handles NÃO DEMONSTRADA.** ProgramFiles é obrigatório e sua
inicialização pode continuar custosa. Não afirmar que remover ProgramData resolve
os23 handles ou que o novo perfil cabe128. A próxima coleta continua fail-closed.

## Qualificação e limites

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings;17 CTests
afetados por configuração,68 execuções PASS. Paths:59 checks com APIs de known
folder modeladas; binário consulta somente ProgramFiles em captura/revalidação,
não chama ProgramData mesmo quando esse provider está indisponível, mantém campos
de dados vazios e rejeita replay/Stop/path divergente/contaminação/escopo trocado.
NEW_INSTALL continua com as duas consultas e suas falhas/compatibilidade.

Staging preserva48 cenários/141 checks; recursos30 casos/99 checks. Guard nativo
de close estendido para negar binário sem cohort/invocação e negar binário como
layout completo, sem consulta OS. APIs mutantes/known-folder/contadores são
modelados. Não houve coleta adicional, RNG nativo ou efeito protegido.
Não se repetiram campanhas ou mutantes sem mudança pertinente.

Layout físico inalterado: packet7.360; gerido Selected1.988.704 e Legacy1.820.136.
Sem crédito de buffer/handle/retidos por omitir lookup; custos nativos e stack
transitiva seguem UNKNOWN. Guard124/cap128/cohort1024 não foram alterados.

## Preservação e próximo gate

Antes de build, bytes dos três artefatos aprovados preservados em
build/staging-observation-preserved-20261031/approved-a4cbd102, com hash conferido.
APPROVED-ARCHIVE.json distingue cópia de evidência de identidade original ou
staging operacional. Pedido, script e autorização antigos permanecem imutáveis;
comandos antigos não devem ser executados de novo. O rebuild não herda aprovação.

BINARY-ONLY-BUNDLE.json e o script novo fixam o candidato final para uma coleta
read-only do escopo binário. BINARY-ONLY-REQUEST.CANDIDATE.md está PROPOSTO, não
aprovado/executado; inclui proposta explícita de Process/BYPASS/restauração para
essa invocação, sem reutilizar autorização ou exceção já usadas.

Primeiro gate: aprovação própria da coleta mínima de ProgramFiles e preparação
binária, sem ProgramData, serviço ou diretório a criar. Se continuar excedendo,
preservar DENY e a obrigação de fonte/capacidade, sem prometer outra dispensa.
Só depois de evidência suficiente: pedido de staging; registro SCM parado e
NEW_INSTALL/CA/publicação sob atos próprios. E21 PENDING; ED11/Phase6 PARTIAL.
Sem SCM/ACL, Start/Stop, endpoint, CA/CURRENT, TPM/Q4, segredo, cleanup/refund,
rede operacional, nova autoridade ou alteração da produção PST0.6.4.
