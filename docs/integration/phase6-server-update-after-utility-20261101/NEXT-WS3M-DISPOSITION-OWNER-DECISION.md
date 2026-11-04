# Próximo prompt executável — decisão G5 da disposição WS3M

Pedido preparado: ONE-WS3M-PARTICIPANTS-DISPOSITION.REQUEST.CANDIDATE.md,
SHA256 `ffd24d8f4bdcf272b819b61c43c352bff467d1a7caeaf07f8e2e528873357bfe`.
Aguardar decisão específica do proprietário sobre os bytes exatos. Sem ela,
DENY para execução/registro de autorização. Não tratar “siga” como TerminateProcess.

Se aprovado: conferir byte-exatamente pedido/fonte/wrapper, registrar somente a
decisão efetivamente recebida em OWNER-WS3M-PARTICIPANTS-DISPOSITION-APPROVAL.txt,
calcular SHA256 real e preparar o comando manual de dispose-ws3m-participants.MANUAL.CANDIDATE.ps1,
com todos os hashes reais, sem placeholders/execução automática/respostas automáticas.
A operação é única e limitada aos dois objetos fixos, sem retry ou novos alvos.
Não executar consultas adicionais por conta própria nem reconstruir procedimentos.

Após saída manual: reconciliar primeira falha/consumo/submissão/saída de cada
participante/SCM/fechamento. UNKNOWN continua UNKNOWN. Sem efeitos adicionais.
Se suficiente, avançar ao pedido UPDATE SQB3 condicionado já preparado, mantendo
a autorização operacional de UPDATE separada. Nenhum Start/retry de WS3M.
