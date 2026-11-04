# Handoff manual autorizado — uma coleta read-only

Task PACC-PHASE6-STAGING-NATIVE-OBSERVATION-AUTHORIZED.

Pedido original preservado, SHA256
`9ffe2080ea29403555f64dff5d3b0f372aab4613f2054ef4c43efeb60765b00e`.
Autorização documental: OWNER-OBSERVATION-AUTHORIZATION.json, SHA256
`e16494d395e19fbbfaac833e7a9aa6470d62a69dcbe577a0f45237ad9fece90b`.
Fonte: decisão explícita do proprietário na task acima, uma única coleta manual
conforme o pedido integral. Nenhuma aprovação histórica foi reutilizada.

## Estado e pré-condições

Autorização registrada; coleta NÃO executada pelo agente; resultado/consumo
pendentes de evidência manual. Não há autorização de staging ou instalação.
O registro é documental e não um grant carregado pelo executável. Não se afirma
que o script implemente um ledger ou impeça tecnicamente uma segunda invocação.
A restrição de uso único vem do ato e deste procedimento; não repetir.

Windows PowerShell5.1 Desktop x64 já elevado, pelo proprietário legitimamente
administrador. Contexto/SID/token/parent/recursos serão observados e revalidados
na invocação; sua elegibilidade futura não foi demonstrada pela conferência de
bytes. Não preencher com SID de sandbox, conta SCM futura ou dado histórico.
Não mudar ExecutionPolicy, elevar automaticamente ou modificar permissões.
Se a política/contexto impedir a execução, STOP e enviar o erro; sem tentativa
alternativa. Se já invocou a coleta, NÃO executar novamente.

## Comando definitivo

Executar o bloco em ordem uma única vez no console especificado. A primeira
checagem protege o próprio script ANTES de sua chamada. O script confere de novo
seu hash e os tamanhos/hashes dos três artefatos ANTES da única chamada nativa.
Não há placeholders, retries, instalação, redirecionamento ou comandos adicionais.

```powershell
$coletor = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-staging-native-capacity-20261031\collect-staging-resources.MANUAL.ps1'
if ((Get-FileHash -LiteralPath $coletor -Algorithm SHA256).Hash -ne '1935b7dd349193f288f30b3e9ee99f0f88389d8b3fa1f0e0473e4df3cb498f67') { throw 'SCRIPT_HASH_DENY' }
& $coletor -ExpectedScriptSha256 1935b7dd349193f288f30b3e9ee99f0f88389d8b3fa1f0e0473e4df3cb498f67
```

O wrapper só invoca o servidor fixado com `--qualification-staging-observe`.
Os pins foram reconferidos neste handoff: exe552.960 bytes/74f430e0..., DLL SSL
1.321.472 bytes/3fb3cd78..., DLL crypto7.217.664 bytes/09eec573.... Valores completos
permanecem no pedido e no script aprovados; não substituir o pacote/rebuildar
antes da coleta sem reconciliar os pins. Total9.092.096 bytes.

## Saída e STOP

Enviar a saída integral do console, inclusive erro do precheck/wrapper, frames
CONTROLLER_RESOURCE, STAGING_SOURCE, STAGING_RESOURCES, primeira localização,
predicado nativo, close/retidos e STAGING_OBSERVATION_NATIVE_EXIT. O script não
grava arquivo de evidência. Não executar outra vez para obter uma saída melhor.

Exit0 significa somente coleta concluída. UNKNOWN/máscaras remanescentes e
APPLY/AUTHORITY DENY não são dispensados. Pin, contexto ou proteção divergente,
query falha, guard124, memória/disco insuficiente, overflow, NT pending, Stop ou
close incerto: STOP, sem retry ou mudança de guarda. Interrupção/ausência de
receipt completo não demonstra ausência de execução ou consumo reutilizável.

Read-only significa nenhuma mutação de instalação solicitada: o preflight abre
originais FILE_OPEN e consulta o volume. Solicita os direitos descritos no
pedido, inclusive ADD_SUBDIRECTORY para verificar contexto, sem utilizá-los
para criar. Loader/cache/metadados de acesso internos do OS continuam fora de
qualificação; não são declarados inexistentes.

Sem RNG/confirmar/take de staging, SCM/ACL, Start/Stop, endpoint, CA/CURRENT,
KSP/ownerAuth, TPM/Q4, rede, cleanup/refund ou recuperação. E21 PENDING;
ED11/Phase6 PARTIAL. Próximo gate é a saída manual, não nova aprovação desta
mesma coleta. Staging exigirá evidência suficiente e pedido separado.
