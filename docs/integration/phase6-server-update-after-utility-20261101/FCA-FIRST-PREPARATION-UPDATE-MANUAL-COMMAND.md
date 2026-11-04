# Única execução manual autorizada — UPDATE da imagem FCA

MANUAL_PENDING / NOT_EXECUTED. Autorização contextual literal do proprietário registrada em `FCA-FIRST-PREPARATION-UPDATE-AUTHORIZATION.md`. A decisão efetiva e os três hashes abaixo foram emitidos/conferidos; nenhum hash foi atribuído falsamente à fala do proprietário.

Executar uma vez no Windows PowerShell como administrador. A opção Bypass vale para o processo chamado. O script confere seus próprios bytes, o pedido, a decisão, normas, executor, DATA, anterior e DLLs antes da chamada nativa. Cancelar se o plano divergir. A confirmação do UPDATE será digitada manualmente a partir do plano efetivamente apresentado; não há resposta automática.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-fca-first-preparation-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '0a31f85c452585c79ccffee949208dc0d6966fc79dfd0e917435ee957aa3fc10' -ExpectedRequestSha256 '9f856436cfbd90d857540f6bebfd6cf16765469822491ce6bd2d094e7b8458a7' -ExpectedDecisionSha256 '15dcd9bff58a47565bc6d1e914e3198bfb015b28d763e4a8b6aa074e0c29240e'
```

Candidato: 905728 bytes, SHA256 `1ee37a465e6820a5d5967e25630673aadb81d1972a734fb9bf4046ef3b19b2e6`. Executor: o `papacc_update.exe` protegido existente. SCM deve estar STOPPED/PID0 e com configuração original. Não chamar Stop para satisfazer o gate.

O efeito permitido é somente a troca governada do EXE, preservando o anterior. Não cria contêiner/chave FCA nem executa Start. Preservar DLLs, updater, originais, todos os `.previous`, autorizações consumidas e UNKNOWN histórico. Risco de falha parcial/intervalo sem canônico/retidos, sem rollback automático.

Após a chamada, enviar a saída integral. Não repetir, restaurar, limpar ou tentar completar manualmente se falhar. Reconciliar conforme `NEXT-FCA-FIRST-PREPARATION-UPDATE-RECONCILIATION.md`; código de saída isolado não prova sucesso/close. Nenhuma execução foi realizada pelo agente.
