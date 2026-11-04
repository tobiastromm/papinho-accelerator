# Sessão TAKE207: Start submetido, falha na etapa 212

Fonte: OWNER-TAKE207-STAGE212-OUTPUT.txt, SHA256 72f740d241a8fcea632a091861759958958ba18758e801e9da270199eb16365e. Saída parcial fornecida pelo proprietário; nenhuma consulta nova realizada.

As três respostas atribuídas constam da saída. Tentativa 97f3549e752dc4de3e7e61d69a3c3d680154522c82a92ad5b45458f1edeee3fa. Originais documentais criados/verificados com fechamento conhecido: pedido 417614100:851968:329474; decisão 417614100:589824:329475. A criação documental não substitui os atos nativos.

Primeira falha registrada: stage=212, error=5, start_submission_phase=1, prestart_close=0, service_close=5023, retained=1. Autorização manual utilizada, sem retry. Controlador original: PID 7580, creation.low 2826307890, creation.high 31288063. A saída não demonstra seu estado atual.

Inferência estrita do código server_service_win32.c: atingir 212 exige retorno de sucesso de StartServiceW, readback imediato START_PENDING/PID não zero e aquisição do processo correspondente em 211. Isso não comprova autenticação bilateral nem RUNNING diagnóstico. A falha pode ocorrer em start_progress ou start_wait, inclusive em uma iteração posterior. Não atribuir retroativamente à ACL, ao token ou a um componente específico.

start_progress revalida recursos/originais, vincula runtime, prepara oferta e progride conexão/referências/transporte. runtime_bind pode negar ao capturar/revalidar processo, SCM, conta/SID, token/grupos/restrições. start_wait também pode devolver erro 5 pela ausência de event/process/carrier válido. Não há subpredicado capturado suficiente para distinguir esses ramos. Nenhuma correção funcional foi demonstrada nesta revisão.

5023 corresponde à recusa deliberada de ps_close enquanto start_effect=1. prestart_close=0 não prova fechamento: esse fechamento não foi tentado no caminho pós-submissão. Controlador/possível runtime, referências, reserva e fechamento permanecem retidos/UNKNOWN. Não usar disposições anteriores de PID 148 ou 15744.

Não executar diretamente service host scq-closed no console: esse lançamento não reproduz o vínculo SCM/conta virtual/argumentos privados do ServiceMain e criaria outra execução. Não repetir sessão nem atualizar apenas para experimentar.

Primeiro gate: consulta específica de disposição atual, somente leitura, autorizada separadamente. Ela não recupera o subpredicado histórico nem atesta handles internos, token ou close. Nenhum build/teste de produto repetido: código operacional inalterado.
