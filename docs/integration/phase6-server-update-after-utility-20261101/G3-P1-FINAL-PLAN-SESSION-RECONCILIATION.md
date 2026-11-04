# Reconciliação — rejeição demonstrada na geometria do SID runtime

2026-11-02. G3_P1_FINAL_PLAN_SESSION_DENIED_UNBOUND_SID_GEOMETRY.
Autorização bd8bd383...d0744447 utilizada, sem retry, apesar de não alcançar
confirmações/consumos internos E/Q/S. Nenhuma sessão reexecutada pelo agente.

Duas cópias documentais verificadas e fechadas: pedido417614100:1114112:290114,
decisão417614100:1048576:290115. firstNONE/consumedTrue/created3/verified3/
close_knownTrue. Preservar essas cópias e todos os históricos, sem recriar/limpar.

## Primeira rejeição demonstrada nesta sessão

Entrada113<=128 e última query pré-imagem118<188; não são pico completo ou
admissão READY. Imagem: raw-open NTSTATUS0/IOSBInformation1/handle retornado;
última query SD2calls/status0/needed108/capacity108. Cadeia alcançou marcador
EXISTING_TARGET_CANDIDATE. Não há negação de handles demonstrada nessa fronteira.

SCQ_COLD_FINAL_PLAN attempted1/passed_mask1/rejected2/unbound_reason3/field0:
scqp_static PASS; scqp_unbound(runtime) DENY. A classificação reason3 é:
`sid_bytes<8 || sid_bytes>68 || sid[1]>SID_MAX_SUB_AUTHORITIES || 8+4*sid[1]!=sid_bytes`.
Logo a forma/tamanho declarados do SID não satisfazem o contrato. A instrumentação
não emitiu valores ou qual disjunto falhou; não inventar tamanho/count ou
atribuir SID malformado ao Windows. reason3 antecede IsValidSid/GetLengthSid,
padding/autoridade e não é prova de falha desses testes posteriores.

window_known0/from0/until0/now0 significam NÃO AVALIADO por curto-circuito,
não janela expirada ou prazo zero. Nenhum PID/token futuro é exigido nesta
fase. O produtor atual chama LookupAccountNameW com buffer68 e copia o tamanho
retornado para runtime.sid_bytes; a semântica desse parâmetro e a construção
do SID precisam ser confrontadas offline antes de corrigir qualquer guarda.

Confirmações/consumos E/Q/S, endpoint/Start e autenticação bilateral não foram
alcançados pelo caminho qualificado. Static não é admissão administrativa ou
readiness. O erro13 de sessões anteriores continua UNKNOWN: esta evidência
não comprova retroativamente a mesma causa.

Fechamento documental conhecido. Fechamento nativo completo não explicitamente
reportado; retorno/exit2 não o transforma em conhecido. Histórico de retenção/
quiescência OS UNKNOWN. Estado SCM atual não consultado; não inferir lease
fresco de STOPPED apenas porque este fluxo não executou Start.

## Próximo passo mínimo

Investigar offline o produtor da seleção SID do runtime e o contrato Win32
LookupAccountNameW/pcbSid versus tamanho semântico do SID. Distinguir capacidade
do buffer, retorno da API e tamanho validado do objeto. Reproduzir em modelo
as situações permitidas pela API; corrigir só um defeito de construção comprovado,
sem aceitar SID inválido/truncado, normalizar bytes para obter PASS, dispensar
origem/authority80/count6/padding ou inventar PID/token futuros.

Reutilizar testes/owner existentes. Não repetir a sessão nem coletar novamente.
Se faltar dado factual irredutível, delimitar exatamente, sem presumir a API
incorreta ou emitir retry. Nenhuma alteração funcional realizada nesta reconciliação.
Evidência transcrita em OWNER-G3-P1-FINAL-PLAN-SESSION-RECEIVED-EXCERPT.txt,
dados em G3-P1-FINAL-PLAN-SESSION-RESULT.json. Não são bytes originais do console.
Preservar G3-P1, demais G3, objetos/EXEs/HEAD-WIP/pins/reservas/PST0.6.4,
UNKNOWN e DENY produtivo. Sem operação protegida, build ou teste nesta task.
