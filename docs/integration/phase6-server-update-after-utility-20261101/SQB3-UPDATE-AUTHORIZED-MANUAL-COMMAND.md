# UPDATE SQB3 autorizado — única execução manual pendente

Pedido original aprovado e conferido: `f9a85a2f700d3cae1e18ca2b6fded91843791e2e9203a0028c30099379b98986`.
Decisão efetivamente emitida: OWNER-SQB3-UPDATE-APPROVAL.txt,
SHA256 `0219b426fb7358ed953c5c9c48725b58860fa202c01a4bfb93342599294c8645`.
Candidato804352bytes/pin11cbef6508b1b06fb56031b22b93dccc6ae3e05df22528190f3cf7215636d815.
Conferidos wrapper/candidato congelado/DLLs do build/U1–U7/U2; sem testes/builds
inalterados repetidos. Nenhuma consulta/efeito protegido executado pelo agente.

Executar UMA vez no Windows PowerShell administrativo original:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-sqb3-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '31533c135d19cef6d36badca8ed51fcf446fb61b9a80638c04ade71dbf461db5' -ExpectedRequestSha256 'f9a85a2f700d3cae1e18ca2b6fded91843791e2e9203a0028c30099379b98986' -ExpectedDecisionSha256 '0219b426fb7358ed953c5c9c48725b58860fa202c01a4bfb93342599294c8645'
```

O wrapper confere hashes antes da chamada. O owner deve readquirir os originais,
verificar proteção/identidade/SCM STOPPED e K/R frescos e apresentar plano exato.
Confirmar manualmente somente a linha OWNER_BINARY_UPDATE exibida, se coincidir
com o pedido, os IDs/pins e os dois leaves exclusivos apresentados. Não há resposta
automática, nonce antecipado, retry, rollback ou Start.

Enviar saída integral para reconciliar primeira falha, consumo, criação, dois
renames, anterior retido, novo canônico/ID/hash/SD/readback, SCM e fechamento.
Qualquer falha ou UNKNOWN não autoriza repetir/limpar/restaurar. UPDATE não é
registrado como concluído antes dessa evidência. Histórico/quiescência OS UNKNOWN.
