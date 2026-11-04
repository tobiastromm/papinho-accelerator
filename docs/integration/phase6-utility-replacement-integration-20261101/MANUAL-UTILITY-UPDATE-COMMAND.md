# Uma manutenção manual autorizada — papacc_update.exe

Abrir Windows PowerShell5.1 x64 como administrador, pelo proprietário previsto.
Colar o bloco completo UMA vez, de qualquer diretório. Não repetir em erro,
cancelamento ou resultado inconclusivo. Não há rollback automático.

```powershell
& {
    $utilityUpdateRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-utility-replacement-integration-20261101\ONE-UTILITY-UPDATE-REQUEST.CANDIDATE.md'
    $utilityUpdateProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-utility-replacement-integration-20261101\update-utility.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath $utilityUpdateRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne 'a52db75dc6b18f52fffa9207fbc946b0e0ed867c5c9df0617cfe648ec71742b2') { throw 'REQUEST_HASH_DENY_NO_RETRY' }
    if ((Get-FileHash -LiteralPath $utilityUpdateProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne 'cf8ea1c17316bbb8809b5e88982dcd5362a0ed0fd794acc2cb33a180bc09847b') { throw 'SCRIPT_HASH_DENY_NO_RETRY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $utilityUpdateProcedure -ExpectedScriptSha256 'cf8ea1c17316bbb8809b5e88982dcd5362a0ed0fd794acc2cb33a180bc09847b' -ExpectedRequestSha256 'a52db75dc6b18f52fffa9207fbc946b0e0ed867c5c9df0617cfe648ec71742b2'
}
```

Conferir plano nativo: UTILITY_UPDATE_CLOSED, parent/IDs, controller server e
utility DATA, cinco pins, K/R, SCM parado, nomes papacc_update.<tentativa>.candidate
e .previous. Somente se tudo coincidir, digitar a linha OWNER_UTILITY_UPDATE
com o nonce mostrado nessa execução. Nenhuma resposta é fornecida automaticamente.
Cancelar em divergência; não ajustar ACL/direitos/nomes para prosseguir.

Enviar a saída integral para reconciliação. Não executar o utilitário resultante,
UPDATE do servidor, limpeza do candidato anterior ou Start por reflexo.
Esse comando NÃO foi executado pelo agente. Retenção histórica/OS UNKNOWN
permanece preservada e não autoriza retry.
