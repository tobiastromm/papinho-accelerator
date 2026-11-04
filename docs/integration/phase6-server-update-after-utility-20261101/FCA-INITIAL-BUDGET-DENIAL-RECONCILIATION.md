# FCA aquisição — rejeição do orçamento inicial, não da aquisição
Pedidoe3c11dbcdc45ed4f4919946ae0cbe04306080f78acd48882b4550f6c991a8ca5 executado manualmente uma vez. Não repetir.
Saída original fornecida no chat: stage1/operation_result0, budget_first1816/sample_stage0, nenhuma SD call, prepared0/flagszero/close1, wrapperCloseTrue/exit2. Tentativa manual usada; confirmação e consumo nativo de efeito não observados. Flagszero não são consulta independente de ausência.

## Primeiro gate demonstrado
stage1 corresponde exclusivamente a papacc_ca_key_first_budget_begin, antes de console/layout/acquisition.
operation_result0 é inicialização do último PAPACC_RESULT capturado no launcher; o orçamento retorna DWORD em outra chamada, portanto NÃO significa que o orçamento passou.
server_ca_key_win32.c: begin coleta amostra, valida Stop/QPC/freshness/overflow e nega1816 se:
- handles iniciais >128; OU
- RAM física disponível <7239172096bytes; OU
- commit disponível <3221225472bytes (3GiB).
O coletor registra estágio nãozero ao falhar uma API/validade de amostra. sample_stage0 com budget_first1816 é consistente com a disjunção de pisos no begin, não prova excesso de handles. Os três valores originais não são emitidos por este caminho.
Não importar contagens169–171 do updater: outro processo/instante/domínio. Não usar RAM do TaskManager como commit, nem snapshot antigo como gate atual.
A instrumentação26ramos não foi alcançada. Nenhuma nova evidência explica o INVALID_STATE da aquisição na tentativa anterior. Não se conclui correção ou regressão dela.

## Estado e limites
Nenhum defeito funcional ou insuficiência de quota foi demonstrado. Não alterar floors/guards, não presumir vazamento ou tornar UNKNOWN PASS.
Instalado continua912384bytes/417614100:2555904:1094956/SHA256f4d91013a34da09053dcb800dcd909f82ba544384f1430d9c22584d1d24ea82b segundo UPDATE reconciliado. Sem novo UPDATE, build/teste ou coleta neste turno. Produto/owners/predicados e configurações preservados.
Configuração417614100:2490368:315690 e Papinho417614100:1310720:452619 não devem ser recriados. Mantidos históricos/pins/EXEs/consumos/PST0.6.4, retenção histórica/quiescência OS UNKNOWN e readiness DENY.

## Primeira lacuna e menor passo admissível
Faltam os valores da amostra ORIGINAL do orçamento inicial; a saída não os permite reconstruir. Não existe autorização para repetir o ato só para imprimir esses valores.
Antes de qualquer nova operação, avaliar offline se evidência já existente conserva essa amostra. Se não, preparar somente uma observação prospectiva read-only dos mesmos três campos pelo mecanismo de amostragem existente, uma vez, sem UPDATE/FCA/SCM/endpoint/Start/chave/diretório. Essa observação terá processo/instante próprios e não demonstrará causa histórica ou capacidade futura do owner; tal limitação precisa constar antes de pedir autorização.
Não iniciar nova campanha de recursos, não atualizar EXE novamente apenas por esta negação de admissão e não criar infraestrutura. Qualquer próxima FCA requer nova decisão específica e não pode prometer ultrapassar o gate com base na observação.
Próximo prompt NEXT-FCA-INITIAL-BUDGET-EVIDENCE-OFFLINE.md.