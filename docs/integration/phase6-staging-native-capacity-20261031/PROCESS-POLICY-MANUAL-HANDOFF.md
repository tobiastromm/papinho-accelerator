# Handoff após bloqueio de assinatura — exceção Process explicitamente autorizada

O proprietário respondeu `autorizo` à pergunta específica sobre Bypass somente
na sessão atual e uma única execução manual. Registro independente preservando
os anteriores: OWNER-EXECUTION-POLICY-EXCEPTION.json, SHA256
`38fc35af5134638e3be600129ba8cf6d73055ad5900e30bfd76d91c2021f3162`.

O PSSecurityException/UnauthorizedAccess relatado é negação de carregamento do
script antes de seu corpo: nessa tentativa relatada não começou a coleta nativa.
Isso não demonstra ausência de outra invocação não relatada. Se já iniciou a
coleta nativa por outro meio, não executar este bloco novamente.

Esta exceção especializa somente a condição de ExecutionPolicy da operação
read-only9ffe2080... para o processo atual; não altera ADR, política de usuário/
máquina, GPO, script, pacote ou escopo da coleta. Não autoriza staging ou retry
automático. Nenhum comando foi executado pelo agente; consumo/evidência seguem
pendentes da saída manual.

Em Windows PowerShell5.1 Desktop x64 já elevado, colar o bloco inteiro uma vez:

```powershell
& {
    $stagingCollector = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-staging-native-capacity-20261031\collect-staging-resources.MANUAL.ps1'
    $stagingScriptSha256 = '1935b7dd349193f288f30b3e9ee99f0f88389d8b3fa1f0e0473e4df3cb498f67'
    if ((Get-FileHash -LiteralPath $stagingCollector -Algorithm SHA256 -ErrorAction Stop).Hash -ne $stagingScriptSha256) { throw 'SCRIPT_HASH_DENY' }
    $stagingPreviousPolicy = Get-ExecutionPolicy -Scope Process
    try {
        Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force -ErrorAction Stop
        if ((Get-ExecutionPolicy) -ne 'Bypass') { throw 'EXECUTION_POLICY_DENY' }
        & $stagingCollector -ExpectedScriptSha256 $stagingScriptSha256
    } finally {
        Set-ExecutionPolicy -Scope Process -ExecutionPolicy $stagingPreviousPolicy -Force -ErrorAction Stop
    }
}
```

Hash do script antes da chamada; pins do exe/DLLs conferidos de novo pelo próprio
script antes da única chamada nativa. O finally restaura o valor Process salvo.
Não há mudança de escopo CurrentUser/LocalMachine nem tentativa de vencer uma
política superior; effective policy diferente de Bypass implica STOP.

Enviar a saída completa, inclusive erro de política/restauração, se ocorrer.
Falha/UNKNOWN não permite repetição. Exit0 é coleta apenas, não capacidade ou
admissão. Todos os limites do pedido original, guards frescos e DENY de staging
permanecem. E21 PENDING; ED11/Phase6 PARTIAL. Sem novas assinaturas, certificados,
serviços, contas, diretórios operacionais, ACL, CA/CURRENT, endpoint, rede,
KSP/ownerAuth, TPM/Q4, cleanup/refund ou recuperação.
