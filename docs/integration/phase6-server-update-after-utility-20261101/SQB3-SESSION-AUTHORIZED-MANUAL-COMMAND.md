# Sessão SQB3 autorizada — execução manual pendente

Pedido byte-exato aprovado `e7627530423dd073c96e836ba22032862b4da44f965302fff87e11fe7569c0d2`; seis artefatos/deltas conferidos,
preservando CRLF/LF originais nas comparações. Decisão efetivamente recebida em
OWNER-SQB3-THREE-ACTS-APPROVAL.txt, SHA256 `d3c4ce1dc22b8c9b01ec8c0db2dc453c98a8d489e7556407dc41ccb74305b8af`.
Nenhuma operação real nem build/teste inalterado executado nesta preparação.

Uma única execução no Windows PowerShell administrativo original:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-sqb3-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '1a035ccd7be46a2d9de61ef3cbd7fd529b183e18597f1dcb6b4f492c49fcd38f' -ExpectedOperationalRequestSha256 'e7627530423dd073c96e836ba22032862b4da44f965302fff87e11fe7569c0d2' -ExpectedDecisionSha256 'd3c4ce1dc22b8c9b01ec8c0db2dc453c98a8d489e7556407dc41ccb74305b8af'
```

Conferir cada plano. Confirmar manualmente as duas cópias DATA e comparar seus IDs
criados com os planos nativos. No prompt P1 DATA, usar somente a tupla aprovada
exibida pelo driver; não é confirmação E/Q/S. Confirmar separadamente as três
linhas nativas OWNER_SCQ/OWNER_SCQ_PRIVATE_ENDPOINT/OWNER_SCQ_DIAGNOSTIC_START,
se todos os campos corresponderem. Nenhuma resposta ou nonce automático.

Sem retry se negar/cancelar/falhar. Se retiver, não fechar console/Stop/kill/limpar
por inferência desta autorização. Enviar saída integral. Nenhum RUNNING/bilateral/
receipt/SQC3/custódia terminal/saída finita presumido. Histórico/quiescência OS UNKNOWN.
