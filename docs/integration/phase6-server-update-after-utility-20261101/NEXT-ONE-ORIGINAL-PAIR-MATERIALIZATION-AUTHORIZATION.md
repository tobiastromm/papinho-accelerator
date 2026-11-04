# Próximo prompt — autorização manual da materialização restrita

Ler PREPARATION-ONLY-CLOSEOUT.md, ORIGINALS-PREPARATION-DECISION.md e
ONE-ORIGINAL-PAIR-MATERIALIZATION-REQUEST.CANDIDATE.md nesta pasta.
SHA256 do pedido operacional de materialização:
ad44e1b0e754819c8795ea65acbb3a70e6cae9de826470a06217e9a79e01e399.

Somente se houver autorização EXPLÍCITA do proprietário para UMA execução
manual desse pedido: conferir bytes, escopo, fontes, hashes e procedimentos,
registrar a autorização separadamente e apresentar o comando de
MATERIALIZE-ORIGINALS-MANUAL-COMMAND.md. Não executar automaticamente ou repetir.
Sem essa autorização, permanecer aguardando a decisão, sem novas ADRs/campanhas.

Não modificar OWNER-FIRST-START-APPROVAL.txt: é a fonte pinada da decisão de
PREPARAÇÃO. O novo aceite da materialização deve ter registro separado. Não
transformar nenhum desses atos em decisão operacional de endpoint/SCQ/Start.
Os ORIGINAL devem permanecer inexistentes até a execução manual autorizada.

Após saída manual: reconciliar primeira falha, consumo, dois IDs, bytes/pins,
SD, readback, efeitos confirmados e close/retention. Falha parcial/UNKNOWN:
preservar objetos e consumo, sem retry, delete, adoção ou recuperação. Não
inferir quiescência OS, liberação do cohort antigo ou sucesso por exit0.

Depois da materialização suficiente, preparar decisão operacional independente
dos atos endpoint/SCQ/Start com referências do propósito correto; não promover
ou sobrescrever a decisão de preparação. Reutilizar caller frio/owner/QUERY/SQB2/
G3/SSO-B/SCQ e não exigir PID/token futuro. Nenhum Start/Stop, endpoint, UPDATE,
SCM/ACL, CA/CURRENT, rede, TPM/NV/Q4, limpeza/refund ou autoridade produtiva por
este prompt. Preservar HEAD/WIP/históricos/pins/EXEs/UNKNOWN/reservas/PST0.6.4.
