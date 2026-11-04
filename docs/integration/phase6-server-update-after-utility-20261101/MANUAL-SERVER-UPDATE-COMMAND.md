# UMA execução manual autorizada — EXE principal

Windows PowerShell5.1 x64 já aberto como administrador pelo proprietário.
Colar o bloco completo UMA vez, de qualquer diretório. Não repetir em erro,
cancelamento ou resultado inconclusivo. Não há rollback automático.

```powershell
& {
    $serverUpdateRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\ONE-SERVER-UPDATE-REQUEST.CANDIDATE.md'
    $serverUpdateProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-server.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath $serverUpdateRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne 'a1c5febb1899d513c5b53686192f1e08e1d040c171e910fcd09faae8a5c1a3c7') { throw 'REQUEST_HASH_DENY_NO_RETRY' }
    if ((Get-FileHash -LiteralPath $serverUpdateProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne 'fee3b1c84d85ad57285020b16e15b97f1f938b0ac15e80b468968f2e7273f597') { throw 'SCRIPT_HASH_DENY_NO_RETRY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $serverUpdateProcedure -ExpectedScriptSha256 'fee3b1c84d85ad57285020b16e15b97f1f938b0ac15e80b468968f2e7273f597' -ExpectedRequestSha256 'a1c5febb1899d513c5b53686192f1e08e1d040c171e910fcd09faae8a5c1a3c7'
}
```

Conferir plano nativo BINARY_UPDATE_CLOSED: executor protegido com ID/pin novo,
server DATA e antigo, DLLs/pins, parent, K/R, SCM parado e os dois leaves próprios.
Somente se coincidir integralmente com o pedido, digitar OWNER_BINARY_UPDATE
com o nonce mostrado nessa execução. Nenhuma resposta é fornecida automaticamente.
Cancelar em divergência. Não ajustar ACL/nomes/direitos/share para prosseguir.

Enviar saída completa. Não limpar candidato anterior, restaurar, repetir ou
iniciar serviço depois por reflexo. Retenção histórica e OS_QUIESCENCE UNKNOWN
continuam preservadas. Esse comando NÃO foi executado pelo agente.
