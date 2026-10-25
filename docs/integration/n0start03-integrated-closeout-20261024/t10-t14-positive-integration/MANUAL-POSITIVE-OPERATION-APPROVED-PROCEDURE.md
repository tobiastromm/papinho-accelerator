# Operação manual N0START-03 — aprovação exata registrada

Pedido SHA256 `218f012c98d1f65e6216fb21023113d266d252a0acd5cd17cee201509ee44efb`. Aprovação original do owner: “aprovo”, em resposta ao pedido específico dessa única operação manual. Registro exclusivo EXACT-POSITIVE-OPERATION-APPROVED.json, SHA256 `ce2bd1bc2d1fc8bc289481fcead37aad92b9e0a88ffca626673e2dd37040bfa7`. Operação NÃO executada pelo Codex. A aprovação não concede orçamento nem dispensa gates frescos.

Usar um único Windows PowerShell5.1x64 já elevado, no console interativo autorizado, sem iniciar outro PowerShell. Executar uma única vez:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -Profile N0START03Positive -ExpectedScriptSha256 09558b27272995b2c98b5cee9cddc95d777ffde0f0a054b987cc8d7082aefddc -ExactApprovalSha256 ce2bd1bc2d1fc8bc289481fcead37aad92b9e0a88ffca626673e2dd37040bfa7
```

Confirmar somente as linhas exatas apresentadas nesta execução: START_N0_SINGLE_LAB com PID/criação atuais do auxiliar; desafio B+D com PID/criação/nonce atuais; INSTRUMENT_SCM_LAB 0b1de869e11493b7cb08fb329578f6ce. Não reutilizar valores históricos.

Escape solicita Stop governado, condicionado ao estado/controls aceitos. Em falha, timeout ou UNKNOWN, não repetir, apagar, restaurar ou matar processos. Preservar efeitos e saída completa; F10 somente conforme recibo factual. Trazer stdout/stderr e recibo final para reconciliação. Fechamento próprio não equivale a quiescência global.

Efeitos possíveis: consumo único, imagem/sink exclusivos, rebind LAB, um Start e até um Stop seguro/legal; podem permanecer efeitos parciais antes de Start. Nenhuma alteração ACL/quota/CURRENT/autoridade produtiva. ED11/Phase6 PARTIAL.
