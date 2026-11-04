# Etapa 212 — diagnóstico integrado e qualificado offline

## Resultado e limite factual

main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP e históricos preservados. A sessão TAKE207 chegou ao StartServiceW, START_PENDING e aquisição do processo; retornou5 em212. Consulta posterior: SCM START_PENDING/PID18000; controlador7580/criação134381210166495538 ativo. Consulta utilizada sem retry. Não foi executada nenhuma consulta nativa, SCM/endpoint/Start/Stop/UPDATE/encerramento pelo agente nesta integração.

A saída histórica não distingue launcher/progress/wait nem os predicados internos. Não se demonstrou defeito funcional em token, DACL, originais, SCM, transporte ou quotas. Nenhuma correção permissiva foi aplicada. A causa histórica continua UNKNOWN; instrumentação futura não a prova retroativamente.

## Integração no mesmo owner

Extraídos os corpos existentes para personal_scq_start_progress_win32.inc e personal_scq_start_wait_win32.inc, incluídos nos mesmos translation units. Mantidos assinaturas, retornos, chamadas, short-circuit dos gates, ordem dos efeitos e ausência de retries. Não houve mudança de packet, wire, bootstrap, receipt/close ou fontes administrativas.

SCQ_START_PROGRESS_LAUNCHER_DENY distingue a guarda de owner/thread. SCQ_START_PROGRESS_FIRST_CAUSE distingue:

1. Registro owner/endpoint, fonte atual ou processo ausente.
2. Recurso original negado; projeta a amostra já capturada pelo mesmo coletor, sem consulta extra.
3. Start não consumido, fase/pipe/original divergente.
4. runtime_bind negado: retorno PAPACC_RESULT preservado separadamente do5 externo.
5. Oferta original negada: PAPACC_RESULT preservado.
6. Entrada de conexão negada.
7. Poll original negado.
8. Segunda entrada de transporte negada.
9. Entrega de referências negada.
10. Continuação negada.

IO_PENDING/MORE_DATA permanecem progressão, sem falso diagnóstico de falha. Stop mantém a chamada e o retorno existentes. Zeros de campos não alcançados não provam sucesso.

SCQ_START_WAIT_FIRST_CAUSE distingue referências locais ausentes, carrier original negado (causa original preservada), deadline, falha PAPACC_RESULT da espera, timeout e término/observação divergente. Nenhum relógio ou wait foi acrescentado; a mesma chamada existente é observada uma vez.

O adapter de identidade emite SCQ_START_RUNTIME_FIRST_CAUSE somente para closed_preparation=1 e somente ao registrar o primeiro erro: site numérico da fonte qualificada e código original. SCQ_START_RUNTIME_CAPTURE_RETURN identifica também retornos anteriores à aquisição. O site é interpretado usando o SHA256 e mapa em STAGE212-OFFLINE-QUALIFICATION.json; não é um identificador de protocolo público. Nenhum SID/token/PID/handle/caminho/segredo é impresso por essa projeção. Demais perfis mantêm o comportamento de diagnóstico anterior.

A projeção não altera first_error, fase, política de close ou ownership. Nenhum getter/query/reabertura/autoridade é criado; nenhuma contagem é excluída. Print em caminho de falha também possui custo nativo opaco; não constitui capacidade ou prova de entrega do diagnóstico.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings, mesma receita MSVC x64/Ninja /W4 /WX e configurações/backends/SDK existentes. Não houve troca de toolchain ou migração PST. 24 testes por configuração,96 pares PASS. Logs e hashes em STAGE212-OFFLINE-QUALIFICATION.json.

33 cenários novos dos corpos reais progress/wait, com todas as fronteiras processo/token/SCM/pipe/clock/wait substituídas por modelos: fonte ausente/divergente, recurso, consumo, identidade do original, runtime/offer negados, Stop, connect/poll/references/continue, progressão normal, timeout e término do peer. Contadores verificam short-circuit e ausência de chamadas posteriores ao erro.

18 cenários novos do adapter real com fronteiras nativas simuladas: falhas de consulta/token/elevação/grupos/restrições/conta/SID/SCM, primeiro erro/site preservados, replay sem substituir primeira causa, caso válido sem falso diagnóstico e outros perfis sem ampliação. Testes anteriores de identidade, fonte, READ/NEW_INSTALL, P1, recursos/G3, namespace, confirmação, retenção, host e censo também passaram.

O primeiro build do novo harness revelou macros herdadas da fixture; corrigidas somente no teste, sem alteração de guards do produto. Censo persistente: delta0; ROOT375104/FIXED_GRAPH1576712 na configuração Selected. Stack/CRT/kernel/RPC/custos de IO continuam opacos.

## Artefato

Selected/Release:797696bytes, SHA256 bf7d0793428f5f751e87b007b28d676aa10235e6decc36d33bdd1de434508d60. Congelado em papacc_server.stage212-797696.exe. Delta observado contra a imagem instalada qualificada804352: -6656bytes; tamanho não prova equivalência, causa ou capacidade. DLLs do build possuem hashes idênticos aos pins existentes. Imagem instalada/updater/SCM/ProgramData/originais não alterados ou consultados nesta qualificação.

## Primeiro gate e caminho mínimo

A instância antiga continua retida no último fato observado, e o runtime só é identificado como candidato SCM PID18000: criação/token/referências originais não demonstrados externamente. Não há autoridade de Stop/kill/disposição nem base para reutilizar permissões antigas de PID148/15744. Não preparar retry enquanto disposição atual inconclusiva.

Primeiro gate operacional: obter escopo específico para caracterizar e tratar a disposição dos participantes atuais, sem atribuir identidade apenas pelo PID e sem afirmar close/quiescência por término de processo. A consulta anterior não resolve essa obrigação e está consumida. Sem esse gate, UPDATE e nova sessão permanecem bloqueados. Não há pedido UPDATE pronto nesta entrega: o serviço foi observado pendente e a operação exige STOPPED.

Depois de disposição governada e provas suficientes: UPDATE U1–U7/U2 com o executor protegido existente e autorização própria; novos originais/atos E/Q/S e sessão única autorizada. Preservar UNKNOWN histórico, sem limpeza/refund/rollback. Não há necessidade de novo serviço, owner, protocolo, root ou decisão normativa para este diagnóstico técnico.
