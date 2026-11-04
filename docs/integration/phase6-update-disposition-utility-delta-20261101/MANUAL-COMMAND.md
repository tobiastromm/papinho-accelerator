# Consulta read-only — uma execução manual autorizada

Windows PowerShell5.1 x64 já aberto como administrador, pelo proprietário.
Colar UMA vez de qualquer diretório. Não repetir em erro ou saída inconclusiva.
Nenhum binário de produto será executado. Enviar a saída integral.

```powershell
& {
    $dispositionRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-update-disposition-utility-delta-20261101\ONE-READONLY-DISPOSITION-REQUEST.CANDIDATE.md'
    $dispositionProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-update-disposition-utility-delta-20261101\observe-disposition.MANUAL.CANDIDATE.ps1'
    if ((Get-FileHash -LiteralPath $dispositionRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne '079f72d1e09f48c0fc5a098735add1df5c3d8976cb0e20b4ca1be46c6b6fb483') { throw 'REQUEST_HASH_DENY_NO_RETRY' }
    if ((Get-FileHash -LiteralPath $dispositionProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne '46b0e72f909d09d93a3f9f921d67900dcff8a02667a2cab93e407957f7abd96e') { throw 'SCRIPT_HASH_DENY_NO_RETRY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $dispositionProcedure -ExpectedScriptSha256 '46b0e72f909d09d93a3f9f921d67900dcff8a02667a2cab93e407957f7abd96e' -ExpectedRequestSha256 '079f72d1e09f48c0fc5a098735add1df5c3d8976cb0e20b4ca1be46c6b6fb483'
}
```

Não executado pelo agente. Não autoriza manutenção, UPDATE ou retry.
