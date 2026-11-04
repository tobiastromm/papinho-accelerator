# Fechamento offline — seleção P1 do receptor

Defeito demonstrado: ingress_continue exigia scq_profile_data.pre_readiness para
selecionar paths_selected. O perfil está vazio nessa etapa; pre_readiness só é
definido posteriormente por first_prepare, após aquisição dos inputs. O ramo
portanto selecionava paths_new para QUERY P1, contrariando P0–P7 nas duas pontas.
Isso não demonstra retroativamente a causa do timeout observado.

Correção no owner existente: scqb_existing_target_query seleciona aquisição pelo
estado fechado pré-preparação: papel2/sessão e perfil vazios/runtime virtual
closed_preparation1/TRANSPORT_ONLY/transport phase3/capture_query1/capture_valid7/
resource_scope P1. Falta desse estado no ramo TO QUERY nega sem fallback para
NEW_INSTALL. Outros ramos preservados. Não é autoridade ou admissão. Identidade,
proveniência, continuidade, OBJ_DONT_REPARSE, proteção, G3, consumo, Stop,
bilateral, receipt/close, Q6 e UNKNOWN continuam obrigatórios.
Base accepted: decisão P0–P7 P0/P2/P3/P4 e SQB3/Q6; nenhuma norma nova necessária.

Teste usa predicado de produção com perfil zero antes de first_prepare e negativos
de purpose/papel/runtime/captura/escopo; modo legado não recebe P1. O cold_chain
continua modelando conteúdo/SD via capture_input_model, não executa o ingress
nativo inteiro. O defeito estava nessa composição anteriormente não exercitada.
Quatro builds sem warnings; 19 regressões afetadas por configuração, 76 pares PASS.
Logs receiver-path-*. Nenhuma coleta, UPDATE ou E/Q/S executada pelo agente.

Candidato 805376 bytes; SHA256 174db00a075b55433fdc793f23d9c9412f982814eef8c650579a824afc567f8c; congelado papacc_server.receiver-path-805376.exe.

Imagem instalada permanece805376/ID417614100:4194304:339059.
WAIT_STATE consumida: receipt operation2/pending1/offset0/total1052, legs1.
Bootstrap concluído localmente não prova leitura/validação/admissão pelo receptor.
Primeira rejeição do receptor UNKNOWN. Controlador12976/criação134381401233458253;
PID runtime não impresso; SCM atual não consultado. Não presumir término.

Primeiro gate: preparar e autorizar consulta read-only específica da disposição
SCM/controlador/runtime, usando mecanismos existentes, sem executar automaticamente.
Não preparar UPDATE executável com retenção atual inconclusiva. Se necessária,
disposição exige decisão própria; antigas autorizações consumidas não servem.
Depois, preparar UPDATE U1–U7/U2 e nova sessão E/Q/S independentes, sem reconstruir
infraestrutura. Históricos, originais, reservas, PST0.6.4, application close e
quiescência OS UNKNOWN, DENY produtivo preservados. Sem limpeza/refund/Stop.
