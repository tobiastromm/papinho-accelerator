# Reconciliação — primeira sessão independente G3-P1

2026-11-02. G3_P1_SESSION_DENIED_POST_CHAIN_PLAN_VALIDATION.
Autorização e invocação manuais utilizadas, sem retry, mesmo sem alcançar os
consumos internos E/Q/S. Pedido/decisão/driver aprovados preservados.

Duas cópias novas verificadas e fechadas: pedido417614100:4587520:222537;
decisão417614100:1376256:285871. firstNONE/consumedTrue/created3/verified3/
close_knownTrue. Hashes vinculados aos originais aprovados. Não recriar ou limpar.

Entrada113<=128 demonstrada na query existente. Última query pré-imagem118<188,
reference2; não é pico, contagem após cadeia nem prova da admissão G3 completa.
Abertura da imagem NTSTATUS0/IOSBInformation1/handle retornado; última query SD
2calls/status0/needed108/capacity108. Esses dados não provam consumo zero ou
máximo. O marcador EXISTING_TARGET_CANDIDATE mostra root esperado/observado
idênticos e término bem-sucedido de scqi_chain_capture no caminho qualificado.
Não é recertificação do mapping Windows nem autoridade por identidade histórica.

## Primeira falha delimitada por código

WIN32=13 corresponde ao ERROR_INVALID_DATA usado neste código. stage0/mapped0
só dizem que a instrumentação raw-open/SD não capturou uma primeira falha;
não representam aprovação geral. Após o marcador de alvo, scqc_plan termina em:
`scqp_static(plan)&&scqp_unbound(&plan->runtime)&&scqp_window(plan)?0:ERROR_INVALID_DATA`.

Os outros ramos superiores posteriores retornam ACCESS_DENIED5 para
profile/acquire/start-plan, IO_INCOMPLETE996 para close_raw incerto, e
NOT_FOUND1168/ACCESS_DENIED5/IO_INCOMPLETE996/0 no first_controller. Assim,
pelo fluxo fonte qualificado, a negação13 após esse marcador está delimitada
à validação final do plano. Não há evidência para escolher qual subpredicado
ou campo falhou. scqp_static inclui metadados/refs/peer, scqp_unbound exige a
forma correta de runtime ainda não criado, scqp_window verifica prazo real.
Não atribuir a ACL, atributos, token ou expiração sem prova.

Confirmações e consumos E/Q/S, endpoint/Start e autenticação bilateral não foram
alcançados pelo caminho qualificado. A validação completa G3 READY também não.
Não exigir PID/token futuros para satisfazer scqp_unbound; não relaxar guards.

Fechamento documental é conhecido. A saída não traz marcador explícito de
close nativo completo; retorno ao PowerShell/exit2 não o transforma em conhecido.
Retenção histórica e quiescência OS continuam UNKNOWN. Estado SCM atual não foi
consultado; ausência de Start neste fluxo não é lease fresco de STOPPED.

## Continuação mínima

Revisão offline focal dos três predicados finais, dos campos produzidos por
scqc_plan e da cadeia P1. Procurar defeito reproduzível em modelo antes de
propor qualquer nova operação. Se necessário, diagnóstico de primeira causa
somente nesse owner/retorno, sem segredos, sem nova coleta automática, sem
atualização instalada apenas por inferência e sem novo mecanismo administrativo.
Não preparar retry ou nova sessão. Nenhuma correção operacional demonstrada agora.

Evidência: OWNER-G3-P1-SESSION-RECEIVED-EXCERPT.txt (transcrição de trecho da
mensagem do proprietário, não encoding byte-exato do console); dados estruturados
em G3-P1-SESSION-RESULT.json. Nenhuma operação real/coleta/build/teste executada
pelo agente nesta reconciliação. Preservar os novos originais, todos os retidos,
EXEs/pins/históricos/autorizações consumidas/PST0.6.4, G3-P1 e DENY produtivo.
