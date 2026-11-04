# Achados offline — primeira falha do receptor

Sessão RECEIVER-PATH consumida mantém receipt1052 pending, timeout1460/212.
Defeito P1 corrigido não resolveu o bloqueio. Não se demonstrou outra causa.

Fronteiras não cobertas pelo modelo cold_chain: capture_input_model substitui
captura dos originais/cadeia/SD/conteúdo/readers e copia digests do plano.
Chamadas reais antes do receipt: runtime/policy/resources/pipe/read SQB3;
NtQueryObject rights/type e correlação; input-capture/chain/SD/hash/readers;
observação runtime/imagem; bind_query/deadline/first_prepare/promote/receive/receipt.
Concluir bootstrap local no sender não demonstra passar qualquer dessas etapas.

Erro original n->error/unresolved/first permanece em memória do receptor. O ator
retém e service ps_park_retained estaciona sem close/STOPPED. A projeção do console
é somente do controlador. Não há retorno negativo no SQR1 (magic+plano exato),
que não pode ser modificado silenciosamente. Q2/SQR1 e errataQ6 proíbem extensão
sem decisão. Nenhum dump/debugger/memória/eventlog/arquivo/novo canal foi usado.
Não encontrado PDB/map papacc_server no escopo Selected/Release enumerado offline;
não presumir que é possível ler n->error por endereço histórico ou objeto root.

Menor especialização proposta: frame negativo SQF1 no mesmo pipe, sem autoridade,
suprimido quando não seguro. Uma única proposta focal, não implementada:
RECEIVER-FIRST-FAILURE-SAME-PIPE.CANDIDATE.md SHA256 96b2d26f83cecc7688eeb9e3543a6b19d6660978b31b62ae175fe604f41a3643.
Sem aprovação, DENY/UNKNOWN e nenhuma mudança dependente de protocolo.
Não houve código de produto alterado/build/teste novo ou consulta nativa nesta
investigação; quatro builds/76pares anteriores não comprovam essa fronteira.
Processos atuais não consultados; qualquer disposição requer identidades/gates
atuais e autorização própria. Preservar históricos/retidos/PST0.6.4/UNKNOWN.
