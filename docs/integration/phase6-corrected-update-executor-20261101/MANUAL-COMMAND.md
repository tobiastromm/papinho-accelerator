# Uma execução manual — UPDATE corrigido autorizado

Windows PowerShell 5.1 x64 já aberto como administrador, pelo proprietário
previsto no pedido. Colar o bloco completo uma única vez, de qualquer diretório.
Não repetir em erro/cancelamento/resultado inconclusivo.

```powershell
& {
    $correctedUpdateRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-corrected-update-executor-20261101\ONE-CORRECTED-UPDATE-REQUEST.CANDIDATE.md'
    $correctedUpdateProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-corrected-update-executor-20261101\update-binary.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath $correctedUpdateRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne '557ad499e1435b2bbcde68663462aa3d9581f56b46cefef40f64da8dc5fe744e') { throw 'REQUEST_HASH_DENY_NO_RETRY' }
    if ((Get-FileHash -LiteralPath $correctedUpdateProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne '60834ab0542b72bec33caafae5b5a9bfdc7dc1fad21f05d0d2e06ebdea04118c') { throw 'SCRIPT_HASH_DENY_NO_RETRY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $correctedUpdateProcedure -ExpectedScriptSha256 '60834ab0542b72bec33caafae5b5a9bfdc7dc1fad21f05d0d2e06ebdea04118c' -ExpectedRequestSha256 '557ad499e1435b2bbcde68663462aa3d9581f56b46cefef40f64da8dc5fe744e'
}
```

O procedimento confere os demais pins/contexto e chama somente o executor
protegido. O server do workspace é DATA, não executado. Quando aparecer o plano
nativo, conferir executor, arquivos/IDs/pins, K/R, serviço e os dois leaves.
Somente se tudo coincidir com o pedido, digitar a linha OWNER_BINARY_UPDATE
com o nonce mostrado nessa execução. Nenhuma resposta é automática.

Cancelar em divergência; não alterar permissões ou repetir. Enviar a saída
completa para reconciliação. Mesmo sucesso não autoriza Start/Stop ou endpoint.
Esse comando NÃO foi executado pelo agente. Não há rollback automático.
