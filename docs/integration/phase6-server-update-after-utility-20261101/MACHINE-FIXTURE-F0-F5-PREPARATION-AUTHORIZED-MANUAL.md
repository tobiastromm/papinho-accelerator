# Preparação F0–F5 — autorizada, execução manual pendente

2026-11-03. Pedido a772ab19934fcfc1b5829e04fe5fba1f36f5973aadbec50058238c8895f5594a aprovado pelo proprietário. Decisão real registrada em OWNER-MACHINE-FIXTURE-F0-F5-PREPARATION-APPROVAL.txt/SHA256e2e47dc1062f26b5952bddc68e720426210d66bb9148419b06562bb1f90118f3. Pedido original preservado; pins do pacote/normas/manifestos/fontes reconferidos offline, sem repetir builds/testes inalterados ou consultar objetos protegidos.

Não executado pelo agente. Executar UMA VEZ no WindowsPowerShell administrativo original:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\prepare-machine-fixture-f0-f5.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '15e3e77a19f8cc4f4528d331c4b40c4d14eb4a117866667a76af8c015f05ad7a' -ExpectedOperationalRequestSha256 'a772ab19934fcfc1b5829e04fe5fba1f36f5973aadbec50058238c8895f5594a' -ExpectedDecisionSha256 'e2e47dc1062f26b5952bddc68e720426210d66bb9148419b06562bb1f90118f3'
```

Bypass só nesta instância; não é necessário alterar política global. O wrapper verifica hashes antes da chamada e usa uma invocação D1 e, somente se fechada com sucesso, uma invocação F. Confirmações manuais atribuídas independentes, sem respostas automáticas. Confira ID do pedido histórico417614100:6815744:9313, imagem417614100:5373952:164063 e novo ID da decisão da saída D1 contra o plano F; confira todos os pins/escopos. Divergência ou gate negado => STOP/DENY sem contorno.

D1 autoriza somente a nova cópia da decisão; F autoriza somente a chave TEST_ONLY/controle administrativo público positivo do pedido aprovado. Não cria par histórico novamente e não executa Start/endpoint/SCQ/P/READ/UPDATE. Nome de chave/publicSHA/ID da cópia somente por saída nativa real.

Não repetir o comando após qualquer resultado, mesmo se D1 concluir e F negar. Preserve toda a saída, arquivos/chave parciais e UNKNOWN. Sem delete/cleanup/rollback/refund/kill ou ajustes de ACL/SCM/quotas. Se houver fechamento incerto, preservar a janela/owner e não interpretar ausência de prompt como aprovação de término.

Enviar saída integral para reconciliação conforme NEXT-MACHINE-FIXTURE-F0-F5-PREPARATION-RECONCILIATION.md. Autorizações anteriores continuam utilizadas, sem reutilização. Histórico/retention/quiescênciaOS/PST0.6.4 preservados. Nenhum diagnóstico produtivo é concedido.
