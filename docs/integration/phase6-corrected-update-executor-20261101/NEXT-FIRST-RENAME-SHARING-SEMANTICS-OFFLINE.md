# Próximo prompt — primeira causa de sharing violation no primeiro rename

Continuar Phase6 no branch/owner existentes. Ler MANUAL-UPDATE-RECONCILIATION.md,
OWNER-MANUAL-UPDATE-OUTPUT.txt, pedido UPDATE aprovado e U1–U7/U2 accepted.
Tentativa utilizada, consumed1/created1/renames0/close_known0, first0xC0000043,
location701, cohort retido UNKNOWN. Não repetir UPDATE nem reutilizar autorização.

Prioridade exclusiva: investigar offline a semântica de compartilhamento da
primeira NtSetInformationFile FileRenameInformation (antigo→previous), incluindo
direitos e share masks dos handles próprios do antigo/parent/candidato/SCM,
RootDirectory do rename, locks do wrapper e lifetimes. Demonstrar precisamente
se existe conflito próprio; não atribuir automaticamente a processo externo ou
ACL. Não retirar proteção/continuidades nem abrir share DELETE amplo para PASS.

Corrigir somente defeito funcional comprovado dentro dos contratos, usando
primitivas/owner existentes. Qualificar regressões afetadas com APIs mutantes
simuladas; não repetir campanhas inalteradas. Não executar renames/UPDATE real
como teste. Não criar outro utilitário/root/serviço/ADR por conveniência.

Se faltar evidência nativa, delimitar a primeira lacuna e a menor consulta
read-only autorizável, sem executá-la se exigir aprovação própria. Não criar
nova solicitação de UPDATE antes de resolver causa, disposição persistente e
cohort UNKNOWN. Não inferir crédito liberado do encerramento do processo.

Entregar causa demonstrada ou lacuna exata, correção/testes se aplicáveis,
estado do candidato/anterior sem presumir readback e caminho mínimo até UPDATE.
Proibidos retry, completar renames, restauração, exclusão/limpeza/refund,
Start/Stop, endpoint, SCM/ACL, CA/CURRENT, rede, TPM/NV/Q4 ou produção.
Preservar objetos/histórico/pins/autorizações consumidas/reservas/PST0.6.4,
OS_QUIESCENCE UNKNOWN. E21 PENDING; ED11/Phase6 PARTIAL; produção DENY.
