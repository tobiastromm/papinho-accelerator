# Uma execução manual autorizada — preparação do utilitário

Executar uma única vez no Windows PowerShell 5.1 x64 já aberto como administrador,
pelo operador/host previstos no pedido. Pode ser colado de qualquer diretório.
Não repetir em erro, cancelamento ou resultado inconclusivo. O script conserva
os demais gates originais e verifica todos os pins antes do controller.

```powershell
& {
    $utilityPrepRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-one-utility-preparation-20261101\ONE-UTILITY-PREPARATION-REQUEST.CANDIDATE.md'
    $utilityPrepProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-one-utility-preparation-20261101\prepare-utility.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath $utilityPrepRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne 'a8dd42aad016b30ef574181a148bf5b7631053cdb1225a5167ed185b606d9489') { throw 'REQUEST_HASH_DENY_NO_RETRY' }
    if ((Get-FileHash -LiteralPath $utilityPrepProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne '3c16e6b50a369b011ee9df9dee8b1276bb5bb781b86d98eaa11a427eebb63df5') { throw 'SCRIPT_HASH_DENY_NO_RETRY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $utilityPrepProcedure -ExpectedScriptSha256 '3c16e6b50a369b011ee9df9dee8b1276bb5bb781b86d98eaa11a427eebb63df5' -ExpectedRequestSha256 'a8dd42aad016b30ef574181a148bf5b7631053cdb1225a5167ed185b606d9489'
}
```

Quando o plano nativo aparecer, conferir antes de responder. Somente se coincidir
integralmente, digitar a linha OWNER_UTILITY_PREPARE com o nonce mostrado nessa
execução. Não há resposta automática. Cancelar em divergência; não corrigir ACL
ou nomes para prosseguir. Enviar a saída completa para reconciliação, sem retry.

Esse comando NÃO foi executado pelo agente. Não permite UPDATE do servidor,
executar o utilitário preparado, Start/Stop ou outras operações.
