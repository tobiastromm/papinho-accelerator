# N0START-20261024-01 — aprovação registrada e comando manual final

OWNER_FINAL_PROCEDURE_REVIEW=REQUIRED_BEFORE_EXECUTION
AUTOMATIC_CODEX_EXECUTION=DENY

A aprovação real do owner foi registrada exclusivamente em [EXACT-OPERATION-APPROVED.json](../canonical-preflight-busy-diagnosis-20261023/EXACT-OPERATION-APPROVED.json), sem reescrever o request/handoff anterior. Registro6204 bytes, hash SHA256 `c5b44691086e205995ac75bbd9eb2599b3943ed4dfc742463984eb3e34535628`.21 pins do manifest conferem; HEAD/main913c90cf2328f7939d8f5b927640601903ac6e91 preservados. O registro não é medição atual nem admissão operacional, e sua criação não executa o ciclo. [Verificação](owner-operation-approval-verification.json). Nenhum código, binário, input protegido ou política foi alterado. Não se repetem builds/testes já qualificados sem divergência.

## Conferência final pelo owner

Conferir esta página e o [handoff completo preservado](manual-handoff.md), incluindo efeitos, riscos e retenção. Este documento completa somente o hash antes pendente do comando; não amplia o escopo. Não executar antes dessa conferência. Não criar outro PowerShell a partir do prompt atual. Usar uma NOVA janela Windows PowerShell5.1 x64 já elevada, ainda sem helper PaccFileAcl carregado. Operador1108 e boot236441 continuam obrigatórios; divergência causa DENY, sem repin/retry.

Hashes esperados:

- Script: cdda38efeb2fa9cec404b1b1180c2ec420f031f0e41601a79437634760dfea64
- Qualificador Selected Release: da7af53d54601fcac5036f3ccfebd7eadb18297499cc5f0a656ce3b74aab70f1
- Registro aprovado: c5b44691086e205995ac75bbd9eb2599b3943ed4dfc742463984eb3e34535628

Se a política da sessão bloquear o script e o uso estiver permitido, apenas nesta janela:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
```

Após a conferência final, uma única invocação manual:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -ExpectedScriptSha256 cdda38efeb2fa9cec404b1b1180c2ec420f031f0e41601a79437634760dfea64 -ExactApprovalSha256 c5b44691086e205995ac75bbd9eb2599b3943ed4dfc742463984eb3e34535628
```

Não executar via powershell.exe -File: o script deve permanecer no único processo administrativo já elevado. Não usar modo Prepare antes, não repetir N0PF, não iniciar ETW/WPR ou executar o ciclo antigo.

## Confirmações durante a execução

1. Copiar exatamente a linha START_N0_SINGLE_LAB N0START-20261024-01 com tentativa/PID/criação auxiliar que o script apresentar naquele momento.
2. Copiar exatamente a linha BIND_BD_READ_ONLY com PID/criação do qualificador e desafio nativo contemporâneo. Esta é confirmação do binding, não outra tentativa ou autoridade do auxiliar. Plano positivo/hash do registro ficam vinculados.
3. Após conferir os efeitos apresentados, confirmar literalmente:

```text
INSTRUMENT_SCM_LAB 75330d11cc7e4123ae62eadc9a59c2f5
```

Não reutilizar PIDs/desafios históricos. Confirmar prontamente e somente o plano correto; stale/erro/limite produz DENY, sem tentativa de renovar para passar. Métricas120/1018 são históricas; recursos atuais precisam passar novamente.

Efeitos condicionais aprovados: consumo único do intent existente, imagem e arm exclusivos, um rebind ImagePath somente no LAB, um Start e até um Stop quando aceito. Nenhum input ou diretório N0 será recriado. Não alterar startup/conta/SID/recovery/triggers/ACL para passar. Ausência/colisão/divergência ou recovery automático nega. Pode consumir e deixar efeitos parciais antes do Start; preservar sem rollback/delete/retry.

## Retorno e incerteza

Enviar a saída completa até IG_FINAL/IG_RETAINED e N0_CYCLE_EXIT, incluindo IG_SERVICE, IG_DIAGNOSTIC_RECEIPT e IG_FIRST_RECORD quando disponíveis. Não inferir sucesso por silêncio, timeout, janela fechada, PID0 ou STOPPED isolado. Não matar nem liberar referências. F10 somente se o launcher confirmar o retorno factual e apresentar o aviso. Ausência desse aviso ou UNKNOWN exige preservar janela/contexto e evidências; não repetir. Truncamento da saída continua drenando e marca resultado não limpo; nenhum retry.

START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
PROTECTED_OBJECTS_CREATED=NO
INTENT_CONSUMED=NO
OPERATIONAL_ADMISSION=NOT_YET_GRANTED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_MANUAL_FIRST_POSITIVE_N0_N9_LAB_EXECUTION_AFTER_FINAL_PROCEDURE_REVIEW
