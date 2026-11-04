# Próximo prompt executável — autorização UPDATE

Continue a Phase 6 a partir de READ-FINAL-IO-COMPLETION-INTEGRATION-REPORT.md.

Conferir byte-exatamente ONE-READ-FINAL-IO-COMPLETION-IMAGE-UPDATE-REQUEST.CANDIDATE.md, SHA256 5da25da5938aef7eaa289e320393051fd7dfdb1f5fed05db364b6c94487e1d86. Candidato 882688 bytes, SHA256 e3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7.

Primeiro gate pendente: decisão operacional específica do proprietário para uma execução manual deste UPDATE, sem retry. Sem essa decisão, manter NOT_AUTHORIZED e não criar aprovação nem executar. Havendo decisão real, registrá-la fielmente, conferir contratos/pins e preparar comando manual com hash real da decisão e do procedimento; não inserir placeholders ou executar automaticamente.

Procedimento preparado: update-read-final-io-completion-image.MANUAL.CANDIDATE.ps1, SHA256 c19121fc8f704947d28a64afa5ddce8ec869a1c04e930b61c7aaa738c34ef6d4. Gates frescos de identidade/proteção/continuidade/SCM STOPPED/recursos permanecem obrigatórios.

Após saída manual, reconciliar primeira falha, consumo, criação, dois renames, anterior preservado, novo ID/hash/proteção/readback, SCM e fechamento. Não repetir. Só então preparar sessão READ independente com fixture existente e novos originais/decisão/E/Q/S/P. CLI3 inconclusivo não é PASS. Preservar UNKNOWN, retenção histórica, quiescência OS desconhecida, DENY, históricos, autorizações consumidas e PST0.6.4. Não executar coleta, endpoint, Start/Stop, limpeza ou autoridades produtivas.
