---
status: proposed
date: 2026-10-29
scope: PERSONAL_LOCAL Windows SCM logon only
supersedes: []
---
# Especialização focal — conta virtual no serviço pessoal

Esta proposta requer aprovação explícita de seus bytes. A preferência do proprietário não constitui aceitação, seleção operacional ou autorização de instalação.

## Regra proposta

Especializar somente a seleção de identidade SCM de M5 da DECISION-phase6-personal-mvp-20261022 e a fronteira de hosting futuro de P3/interpretação de identidade da DECISION-personal-ca-preinitialization-20261022: permitir a conta virtual dedicada `NT SERVICE\PapinhoAccelerator` como identidade primária de logon do MESMO runtime/owner pessoal. A opção SAM dedicada do host console permanece vigente. Não criar serviço adicional para obter um SID.

A futura seleção deve vincular nome SCM exato, conta configurada, SID resolvido legitimamente e TokenUser observado por referência original, PID/criação e continuidade. Nome, resolução de SID, configuração SCM, HWND ou hash não substituem token factual. Não fixar neste documento SID derivado ou identidade operacional futura.

Consultar integralmente TokenGroups e restrições/filtragem do token. Uma ausência de duplicação do SID primário nos grupos só pode ser aceita neste perfil se TokenUser corresponder exatamente à identidade virtual selecionada e todas as consultas, identidade/continuidade, isolamento e acesso efetivo exigidos forem demonstrados. Grupo presente desabilitado ou DENY_ONLY não satisfaz qualquer requisito de grupo ENABLED; atributos não são intercambiáveis. Falha, consulta incompleta ou token inesperado nega. Service SID type, SID de serviço e logon primário são fatos distintos, que precisam de configuração esperada explícita e qualificação independente; não se concede dispensa geral de predicados de grupos.

Preservar P4: o runtime não pode abrir/usar/exportar a chave privada da CA. O administrador legitimamente autorizado mantém a custódia MACHINE existente, borrow-only, sem exportação e com lifetime/leases originais. A chave de identidade TLS do runtime, quando aplicável, não é a chave CA. MACHINE e export-policy zero não comprovam acesso efetivo ou privilégio mínimo. Qualificar os tokens reais antes de uso produtivo, inclusive serviço com/sem service SID restrito e falhas de consulta.

Mesma root, CAS, CURRENT/epoch, autoridade NEW_INSTALL e CA_INITIALIZE, confirmação explícita, budgets, ownership, Stop e UNKNOWN permanecem obrigatórios. Nenhum callback permissivo, owner adicional, senha em configuração ou fallback para SYSTEM/LocalService/Administrador. A exceção R0–R9 do LAB não é herdada nem ampliada; registros históricos permanecem imutáveis.

Esta decisão, mesmo aceita futuramente, não cria conta, instala/configura/inicia serviço, altera ACL ou executa CA_INITIALIZE. Esses efeitos exigem procedimento e autorização específicos. Não torna elegível root, chave ou publicação ausentes. PENDING/UNKNOWN não autorizam retry, adoção, recuperação ou nova identidade.

## Rationale e alternativa

A conta virtual evita gerenciamento de senha de usuário local mantendo uma identidade de serviço dedicada. Alternativa sem esta especialização: conservar a conta SAM padrão dedicada explicitamente selecionada e qualificada no escopo já aceito. Não interpretar os dois modelos como tokens equivalentes. A revisão não muda BODY/ROOT/CURRENT, formatos de configuração ou o layout de armazenamento.

## Evidência exigida antes de operação

Após aceitação normativa: testes isolados do binding e predicados negativos; depois, sob autoridade operacional própria, readback original SCM/logon/service SID type, token completo PID/criação, negação efetiva de uso da CA pelo runtime, uso administrativo autorizado, reabertura e lifecycle/Stop/fechamento. Não declarar esses fatos demonstrados pelo modelo offline.
