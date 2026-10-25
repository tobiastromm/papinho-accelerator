# Operação manual corrigida N0START-03 — aprovação registrada

Pedido SHA256 `64f20a0fcbe94782b3178c9da27ce0c76a989e478802d7e2262d3cef04d0e18b`. Owner respondeu “aprovo” ao pedido específico. Registro exclusivo EXACT-POSITIVE-TIMEOUT-CORRECTION-APPROVED.json, SHA256 `b1d4185c2c581c9c024621bf13adfb9156b516a51da0c074fdfee85c89a8697e`. Nenhuma operação executada pelo Codex; admissão/orçamento frescos ainda dependem dos gates.

Usar um único Windows PowerShell5.1x64 já elevado, processo fresco sem os tipos/módulos nativos carregados da execução anterior. Não matar ou fechar à força processos retidos/UNKNOWN. O launcher não cria outro PowerShell. Executar uma única vez:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -Profile N0START03Positive -ExpectedScriptSha256 10fd6add1c4cd8cdb28845462a219c403b62271b4d5f894ee355279e293b150c -ExactApprovalSha256 b1d4185c2c581c9c024621bf13adfb9156b516a51da0c074fdfee85c89a8697e
```

Confirmar somente os desafios atuais apresentados: START_N0_SINGLE_LAB com tupla atual do auxiliar, desafio B+D com PID/criação/nonce atuais e INSTRUMENT_SCM_LAB 0b1de869e11493b7cb08fb329578f6ce. Não reutilizar valores históricos nem automatizar confirmações. Espera humana conta para freshness; nenhum relógio será renovado para compensá-la.

Em falha/timeout/UNKNOWN, não repetir, apagar, restaurar, matar processos ou forçar mutex. Escape solicita Stop governado quando legalmente aceito; START_PENDING não implica aceitação. Preservar a saída completa, incluindo IG_CENSUS_CONTENT_WINDOW, IG_N0_TIME_ERROR e recibos. F10 somente conforme recibo factual; fechamento local não prova quiescência global.

Efeitos condicionais: consumo único, imagem/sink exclusivos, rebind LAB, um Start e até um Stop seguro. Efeitos parciais podem preceder Start. Limites128/1024/quatro papéis/60s/1s/headroom/RAW1/N7/P3/H/T/NONE_GRANTED preservados. ED11/Phase6 PARTIAL; CURRENT/assinatura/recuperação DENY.
