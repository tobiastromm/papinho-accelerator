# Especialização focal de U2 — executor protegido e candidato DATA

Status: CANDIDATE / NÃO ACEITA. Data: 2026-11-01.
Autorizada somente a preparação desta proposta. Não autoriza implementação
dependente, execução ou redução de guarda. Escopo exclusivo: um UPDATE
individual PERSONAL_LOCAL BINARY_UPDATE_CLOSED sob U1–U7, no owner existente.
Não é nova ADR, serviço, root, canal ou infraestrutura de distribuição.

## Problema e decisão proposta

O pedido anterior executava o mesmo arquivo do workspace como controller e
payload. Foi negado antes do plano, location108/slot8. A autorização foi
utilizada e não é reutilizável, embora consumed interno0. O subpredicado
histórico não foi exportado; não presumir equivalência com a ACL atual.

Propõe-se separar três papéis tipados no MESMO ato e owner: executor
administrativo protegido, origem do payload exclusivamente DATA e candidato
protegido criado no destino. Nenhum papel é autoridade isoladamente. Exigir
conjuntamente política accepted, autoridade administrativa atribuída, provas
originais abaixo, capacidade, confirmação, consumo e gates U1–U7.

A única redução proposta é não exigir da origem DATA a allowlist estrita de
owner/escritores do executor/instalação. Em contrapartida, impor aquisição
original somente leitura, inspeção do descritor, custódia contínua e hashes,
sem executar/carregar a origem. A guarda do executor NÃO é reduzida. Não
implementar a simples troca de scm_descriptor por IsValidSecurityDescriptor.

## Executor independente: fonte, proteção e continuidade

Priorizar um controller administrativo PapinhoAccelerator já existente,
legitimamente protegido e capaz de executar o ato UPDATE sob o owner canônico.
Sua origem deve preceder e ser independente do payload e do efeito que ele
autoriza. A aprovação operacional futura deve selecionar separadamente sua
revisão/artefato, hash, identidade original esperada e dependências. Proveniência
exige registro de qualificação e disposição legítima de instalação/proteção,
mais aquisição factual atual; não é um caminho, um hash ou um nome de produto.

Antes de admitir qualquer efeito, exigir:

1. Imagem executora e dependências necessárias adquiridas por referências
   originais; ancestrais retidos e validados, ausência de reparse e substituição,
   correspondência ao artefato qualificado e seleção exata. Nenhuma busca de
   DLL, plugin ou código administrativo no diretório do payload/workspace.
2. Guarda estrita de owner/DACL efetiva: proprietário no conjunto já aceito
   para esse objeto; nenhuma escrita/delete/WRITE_DAC/WRITE_OWNER não confiável
   efetivamente aplicável. Preservar a política de ancestrais/anchor-child e
   descritor protegido onde obrigatório. Não acrescentar trustees de build
   nem modificar descritores existentes para produzir PASS.
3. Correlação original entre processo executor, PID/criação, token efetivo,
   imagem e objetos protegidos, reutilizando os adapters originais já existentes.
   Nome de processo, QueryFullProcessImageName ou hash de arquivo isolados não
   provam a correlação. Aquisição e vínculo devem pertencer à mesma invocação
   e ser revalidados até o último efeito e disposição de fechamento.
4. Autoridade do operador estabelecida pelo contexto nativo elegível, declaração
   atribuída e confirmação independente BINARY_UPDATE_CLOSED conforme F1–F6/U1.
   Proteção da imagem não autoriza por si só; SID, elevação e metadados tampouco.
5. Separação factual entre identidade do arquivo executor e da origem DATA.
   Rejeitar alias/hardlink para o mesmo objeto, self-update do executor vivo ou
   tentativa de usar o candidato novo como prova retroativa do executor.

As referências de arquivo/processo/token e a associação de política/plano
permanecem no packet existente. Não serializar handles, criar grants/setters
ou transportar autoridade. Antes do efeito, falha de correlação, perda de
continuidade ou ausência de fonte legítima mantém DENY.

Não se alega atestação universal das páginas já mapeadas nem defesa contra
kernel/administrador comprometido. A prova operacional deve declarar exatamente
se demonstra associação de objeto/imagem/processo e proteção desde a aquisição,
ou continuidade desde a admissão do processo. Não converter snapshot posterior
em prova de um intervalo anterior não observado. Se a continuidade exigida não
puder ser demonstrada pelos adapters, o executor não é admissível; não substituir
a prova por afirmação do próprio candidato. Nenhuma exceção implícita de loader.

## Entrada DATA do workspace

O workspace não passa a ser origem administrativa confiável. O executor
adquirirá UM arquivo de payload como dados, sem CreateProcess/LoadLibrary,
interpretação de instruções ou resolução de dependências a partir dele.

Adquirir por referência original read-only, direitos mínimos de leitura de
conteúdo/atributos/descritor e sincronização, sem pedir escrita/delete ou direitos
de token. Validar cadeia/caminho sem reparse conforme os adapters aplicáveis;
identificar volume/file ID, tipo regular, tamanho, owner e DACL observados.
Capturar um descritor completo válido com owner/SID e DACL interpretáveis,
registrar digest e manter referência, não aceitar apenas SD genericamente válido.
DACL ausente/nula, campos inválidos ou semântica não suportada negam.

ALLOW de escrita de terceiros ou owner fora da allowlist administrativa pode
ser observado APENAS nessa entrada DATA. Não concede autoridade nem torna o
arquivo apto a execução. A admissibilidade depende cumulativamente de executor
independente admitido, objeto retido, exclusão efetiva de escritores/deleters
concorrentes pelo mecanismo nativo, tamanho finito e SHA256 selecionado no ato.
Se já houver escritor incompatível, falhar a aquisição: sem esperar/repetir,
alterar ACL ou compartilhamento para forçar êxito. Compartilhamento não é defesa
universal contra escrita por kernel/admin ou mapeamento previamente existente;
integridade da cópia deve ser verificada independentemente.

Calcular SHA256 e tamanho a partir do handle retido antes de apresentar o plano.
Não reabrir pelo path para copiar. Revalidar identidade/tamanho/atributos e
descritor capturado antes do consumo e dos efeitos dependentes; alteração ou
UNKNOWN nega. O hash selecionado vem do plano atribuído, nunca de um digest
autodeclarado pelo próprio arquivo. Rejeitar alias com executor, original antigo,
DLLs ou candidato. Nenhuma exceção para request, decisão, READ ou SCQ.

## Associação, criação e substituição

O plano imutável deve separar executor, fonte DATA, EXE anterior e candidato,
com suas identidades/proveniência, hashes, tamanhos, política, serviço, destino,
nomes exclusivos, tentativa/lifetime e efeitos. Não inventar identidade do
candidato antes da criação. Confirmação ligada a esse plano e operador; consumo
irrevogável antes da primeira criação, não apenas antes de renomear.

Reutilizar U3/U5: um candidato FILE_CREATE exclusivo relativo ao parent
protegido original, com descritor novo protegido e guarda estrita. Copiar
somente o tamanho congelado, buffer limitado existente, I/O parcial governado,
hash independente do candidato, flush e readback de bytes/tamanho/identidade/
proteção. Exigir hash final igual ao selecionado antes de qualquer rename.
Correspondência dos bytes não promove a fonte DATA a executor administrativo.

Preservar os dois renames exclusivos do EXE, sem overwrite, e readback U6.
DLLs idênticas são somente adquiridas/validadas; não copiá-las. Preservar
diretórios, ProgramData, dados, CA/CURRENT e configuração SCM. Exigir serviço
original parado/PID0 e configuração exata em gates frescos. Esse snapshot não
impede Start por outro administrador nem prova quiescência OS universal.

## Stop, falhas e recursos

Revalidar executor, fonte, plano, token/contexto, destino, SCM, recursos e Stop
antes das submissões dependentes. Perda de prova, replay/concorrência, mudança
de plano, hash/identidade/SD divergente ou Stop nega novos efeitos. Não assumir
cancelamento imediato de I/O submetido. Retenção e fechamento seguem U5/U6:
primeira falha preservada; objetos/referências/créditos incertos ficam retidos;
sem liberar por timeout, rearmar por restart ou transformar UNKNOWN em PASS.

Após falha entre renames pode faltar EXE canônico. Reter anterior e candidato
na disposição observada; não restaurar, excluir, completar automaticamente ou
repetir. Retenção do anterior não autoriza futura limpeza.

U4 permanece apenas no seu domínio de custos opacos já aceitos. Referências
adicionais do executor, coexistência e frames devem entrar no censo K final;
nenhuma quota aumenta. R continua o EXE novo arredondado no volume factual,
com gates existentes. Não reutilizar K histórico sem verificar o delta, nem
importar B3/R3/G3 ou limites de handles de outros domínios. Custos opacos,
latência, cancelamento e quiescência OS permanecem UNKNOWN nos limites U4.

## Compatibilidade e precedência mínima proposta

| Cláusula | Disposição proposta |
|---|---|
| U1 | Preservada: ato independente e candidatos DATA; distinguir explicitamente executor e entrada |
| U2 | Especializar SOMENTE a exigência de allowlist estrita de owner/escritores da origem DATA; exigir o vínculo independente do executor e todos os gates acima |
| U2 executor/originais/destino | Integralmente preservada; nenhuma redução de proteção ou continuidade |
| U3/U5/U6 | Preservadas: um arquivo exclusivo, cópia/flush, dois renames, readback, Stop e retenção |
| U4 | Sem nova redução de garantia; recalcular composição controlada caso cresça |
| U7 | Preservada: aceitação, implementação offline e execução são atos distintos |
| F1–F6 via U1 | Preservadas: ator fora do candidato, declaração/confirmação atribuídas e consumo; proteção técnica não é aprovação |
| Demais perfis/contratos | Sem extensão a staging, registro, Start, endpoint, SCQ, READ, request/decisão, CA/CURRENT ou recuperação |

Não editar textos accepted anteriores. Caso aprovada byte-exata, registrar
somente esta precedência focal em documento normativo designado antes do código.
Até lá, a guarda atual permanece intacta. Nenhum conflito com os contratos
delimitados é pretendido; obrigação superior incompatível identificada impede
implementação dependente e deve ser reportada, sem dispensas por inferência.

## Alternativa simples: controller protegido já existente

É a alternativa preferida se disponível: executar o UPDATE pelo controller
protegido e capaz já instalado, passando o novo EXE somente como DATA. Não
precisa pasta adicional, serviço, root, canal, distribuição ou cópia preliminar.

Evidência atual: o EXE staged conhecido é c0f27f…806ecf/571392 bytes. O pedido
histórico ONE-BINARY-UPDATE-REQUEST.CANDIDATE.md afirma que ele não executa essa
atualização; a implementação/caller UPDATE foi qualificada posteriormente no
EXE42d468…29ca do workspace. Não foi demonstrado outro controller protegido
capaz, nem foi feita nova consulta/execução do instalado para tentar descobri-lo.
PowerShell/conhost protegidos não são automaticamente controllers deste owner.

Portanto a alternativa é arquiteturalmente menor, mas sua disponibilidade
atual NÃO está comprovada. Esta proposta não instala um controller, não autoriza
provisionamento e não inventa uma fonte de confiança. Se faltar executor capaz
e independentemente protegido, manter DENY e apresentar essa dependência
específica; a aceitação normativa não a elimina. Não oferecer execução do
workspace como fallback nem criar infraestrutura para ocultar a lacuna.

## Qualificação e próximo passo técnico

Após eventual aceitação exata, e somente então: integrar no owner existente
papéis tipados/referências de executor e DATA, vinculando-os ao ato/plano.
Manter caminho estrito intacto até admissão conjunta, sem flag permissiva do
caller. Reutilizar captura original de imagem/processo/token onde aplicável,
FILE_OPEN/read share, criação/cópia/hash/flush/rename/readback e fechamento.
Não importar permissões operacionais de SCQ por reutilizar seus adapters.

Qualificar Selected/Legacy Debug/Release e regressões afetadas com APIs mutantes
simuladas. Testar executor ausente/substituído/não protegido; controller=DATA
ou hardlink; dependência do workspace; token/contexto divergente; DATA com owner
estrangeiro/ALLOW de escrita somente quando todos os gates cumulativos passam;
DACL ausente/inválida; escritor aberto; reparse; troca de path/objeto; mudança
de bytes/SD após captura; hash copiado divergente; confirmação ausente; replay,
consumo duplicado, Stop/concorrência, falhas de flush/rename/readback/close e
retenção UNKNOWN. Testar que nenhuma ausência é satisfeita por fixture/grant,
hash ou SD válido isolado; demais guardas estritas devem continuar negando.

Somente com executor concretamente demonstrado e integração qualificada,
congelar artefatos e preparar um único novo pedido operacional independente.
Não emitir comando prematuro, não reutilizar baa501…4726. Não executar qualquer
preparação, UPDATE, coleta, staging, ACL/SCM, Start/Stop, endpoint, CA/CURRENT,
rede, TPM/NV/Q4, exclusão, rollback, limpeza/refund ou recuperação.

Risco residual: limita-se ao host/admin legítimo nos contratos existentes,
custos opacos U4, lacunas de atestação explicitamente negadas quando materiais
e resultados parciais já delimitados. O histórico de build não prova o processo
futuro; fixture não prova imagem original; aprovação desta norma não autoriza
operar. DENY; E21 PENDING; ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN.
HEAD/WIP, pins/históricos, EXE anterior, atos utilizados e PST0.6.4 preservados.
