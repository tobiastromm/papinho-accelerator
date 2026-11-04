# UPDATE FCA — decisão registrada, execução manual pendente

O proprietário respondeu literalmente **“autorizo o update”**, durante a conclusão do pacote corrente FCA. A autorização foi associada exclusivamente ao EXE Selected/Release já congelado: 905728 bytes, SHA256 `1ee37a465e6820a5d5967e25630673aadb81d1972a734fb9bf4046ef3b19b2e6`.

O pedido final foi emitido depois dessa resposta. Não se registra aprovação histórica do hash pelo proprietário: ele não o citou. A associação contextual, os efeitos permitidos e as condições estão explicitados na decisão efetivamente emitida, sem substituir a conferência byte-exata e a confirmação nativa independente antes dos efeitos.

| Artefato | SHA256 |
|---|---|
| ONE-FCA-FIRST-PREPARATION-IMAGE-UPDATE-REQUEST.CANDIDATE.md | `9f856436cfbd90d857540f6bebfd6cf16765469822491ce6bd2d094e7b8458a7` |
| OWNER-FCA-FIRST-PREPARATION-UPDATE-APPROVAL.txt | `15dcd9bff58a47565bc6d1e914e3198bfb015b28d763e4a8b6aa074e0c29240e` |
| update-fca-first-preparation-image.MANUAL.CANDIDATE.ps1 | `0a31f85c452585c79ccffee949208dc0d6966fc79dfd0e917435ee957aa3fc10` |

Uma única execução **manual** BINARY_UPDATE_CLOSED U1–U7/U2. Não executada automaticamente. Consumo pendente da tentativa; após falha/inconclusão, não repetir mesmo se consumed nativo for zero/UNKNOWN. As autorizações anteriores permanecem utilizadas.

Efeitos máximos: um candidato protegido, cópia/verificação/flush, dois renames exclusivos, anterior preservado, novo readback e fechamento factual. Nenhuma alteração de DLL/updater/SCM/ACL existente, CA/CURRENT, criação FCA, fixture, READ/E/Q/S/P, endpoint ou Start/Stop. Riscos U4 de parcialidade/custos opacos/retenção/ausência de rollback permanecem.

Primeiro gate restante: conferência dos pins e gates nativos frescos na única chamada manual. O comando está em `FCA-FIRST-PREPARATION-UPDATE-MANUAL-COMMAND.md`. Não declarar UPDATE concluído por esta decisão ou por exit code isolado. Histórico, quiescência OS e retenção UNKNOWN, HEAD/WIP/pins/EXEs anteriores e PST 0.6.4 preservados.
