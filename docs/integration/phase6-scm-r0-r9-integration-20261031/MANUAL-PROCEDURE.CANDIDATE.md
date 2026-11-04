# Procedimento candidato — não executar sem aprovação operacional específica

O pedido ONE-STOPPED-SCM-REQUEST.CANDIDATE.md ainda NÃO está autorizado. O comando abaixo está preparado para uso futuro somente após aprovação explícita do pedido SHA256 `7eb9e6912a3d2c4b3a4408d176bfcd356e511da3a8bfc240942ce82a7ad03a79`. Não é uma autorização nem uma instrução de execução imediata.

Usar console Windows PowerShell 5.1 x64 já elevado, no host/operador do pedido. Colar UMA vez o bloco completo após aquela aprovação; hashes são conferidos antes de carregar o script. Bypass limita-se ao processo filho, sem política persistente. Se GPO/política superior negar, STOP sem contorno. Não usar execução anterior ou outro binário/script como substituto.

```powershell
& {
    $scmRequest = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-scm-r0-r9-integration-20261031\ONE-STOPPED-SCM-REQUEST.CANDIDATE.md'
    $scmProcedure = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-scm-r0-r9-integration-20261031\register-stopped.MANUAL.ps1'
    if ((Get-FileHash -LiteralPath $scmRequest -Algorithm SHA256 -ErrorAction Stop).Hash -ne '7eb9e6912a3d2c4b3a4408d176bfcd356e511da3a8bfc240942ce82a7ad03a79') { throw 'REQUEST_HASH_DENY' }
    if ((Get-FileHash -LiteralPath $scmProcedure -Algorithm SHA256 -ErrorAction Stop).Hash -ne '55102d036d8add96c334df7d2fee18443aeae1f65aa58916b443280e56889571') { throw 'SCRIPT_HASH_DENY' }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $scmProcedure -ExpectedScriptSha256 '55102d036d8add96c334df7d2fee18443aeae1f65aa58916b443280e56889571'
}
```

O wrapper verifica host, SID/contexto, known folder e seis arquivos/pins antes de invocar UMA vez o controlador. São verificações somente leitura; originais/identidade/proteção/freshness e ausência do serviço continuam gates nativos obrigatórios. A confirmação exata é manual no console; nunca fornecida por este bloco.

Conferir operador, alvo, comando, conta, início, SID type e os três hashes staged contra o pedido. O presenter usa escaping de console: uma barra invertida pode aparecer como `\x5c`; isso não muda os bytes do caminho Win32. Não confirmar uma divergência real. Cancelamento ou erro termina a única tentativa, sem retry automático ou ampliação de permissões.

Após qualquer resultado, enviar somente a saída textual não secreta completa para reconciliação. Não repetir, iniciar/parar, remover serviço parcial, alterar ACL nem executar status/coleta adicional por conta desta autorização. Exit0 isolado não é prova de sucesso: exigir resultado nativo correlacionado, readback parado e fechamento controlado conhecido. OS_QUIESCENCE permanece UNKNOWN.
