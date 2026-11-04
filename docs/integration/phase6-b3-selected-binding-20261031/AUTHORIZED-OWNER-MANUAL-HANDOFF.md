# Handoff autorizado — primeira execução manual de staging limitado

Task PACC-PHASE6-FIRST-LIMITED-STAGING-AUTHORIZATION.

Autorização específica registrada em OWNER-STAGING-EXECUTION-AUTHORIZATION.json, SHA256 `fa7c3c1da45159a562057d354ada43df10746b9a9e376ad02d9923b56ce5b11f`.
Pedido original aprovado SHA256 `a9280bc14623dfdf23e4c67caf65b4baa58d0109c34afd348ae7223e5c173d16`; bytes preservados. Script SHA256 `88dfb7e8f0de05ca745aecc7fe39d6a9082532260fad2d917aa794c222e4502e`. Pacote/manifesto e seleção B3 conferidos integralmente, sem divergência. Nenhum staging/coleta foi executado pelo agente; uso e efeitos ainda aguardam saída manual.

## Procedimento único

Ler previamente o pedido aprovado e abrir Windows PowerShell5.1 Desktop x64 já elevado. Não usar PowerShell7, janela remota/redirecionada ou elevação automática. Colar TODO o bloco uma única vez. Ele confere hash do script/registro ANTES da chamada; o script confere pedido/manifesto e os três arquivos ANTES de chamar o native uma vez. Se necessário para o script unsigned, Bypass fica somente em Process e o valor salvo é restaurado no finally; não muda User/Machine/GPO.

Não há nonce a preencher antecipadamente. O programa apresentará operador/plano/pins/destino/limites e a frase completa `OWNER_QUALIFICATION_STAGING` seguida do desafio fresco. Digitar exatamente a frase mostrada SOMENTE se os dados corresponderem ao pedido e à administração legítima. Confirmação ausente, expiração ou divergência nega. Os120s incluem apresentação; não rearmar se expirar.
```powershell
& {
    $ErrorActionPreference = 'Stop'
    $stagingScript = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-b3-selected-binding-20261031\run-selected-staging.MANUAL.ps1'
    $stagingScriptSha256 = '88dfb7e8f0de05ca745aecc7fe39d6a9082532260fad2d917aa794c222e4502e'
    $stagingApproval = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-b3-selected-binding-20261031\OWNER-STAGING-EXECUTION-AUTHORIZATION.json'
    $stagingApprovalSha256 = 'fa7c3c1da45159a562057d354ada43df10746b9a9e376ad02d9923b56ce5b11f'

    if ($PSVersionTable.PSVersion.Major -ne 5 -or $PSVersionTable.PSEdition -ne 'Desktop' -or -not [Environment]::Is64BitProcess) {
        throw 'POWERSHELL_51_X64_REQUIRED'
    }
    $stagingIdentity = [Security.Principal.WindowsIdentity]::GetCurrent()
    try {
        if (-not ([Security.Principal.WindowsPrincipal]::new($stagingIdentity)).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
            throw 'ALREADY_ELEVATED_OWNER_REQUIRED'
        }
    } finally {
        $stagingIdentity.Dispose()
    }
    if ((Get-FileHash -LiteralPath $stagingScript -Algorithm SHA256 -ErrorAction Stop).Hash.ToLowerInvariant() -cne $stagingScriptSha256) {
        throw 'SCRIPT_HASH_DENY'
    }
    if ((Get-FileHash -LiteralPath $stagingApproval -Algorithm SHA256 -ErrorAction Stop).Hash.ToLowerInvariant() -cne $stagingApprovalSha256) {
        throw 'APPROVAL_HASH_DENY'
    }

    $stagingSavedPolicy = Get-ExecutionPolicy -Scope Process
    try {
        if ((Get-ExecutionPolicy) -notin @('Bypass', 'Unrestricted')) {
            Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force -ErrorAction Stop
            if ((Get-ExecutionPolicy) -ne 'Bypass') { throw 'EXECUTION_POLICY_DENY' }
        }
        & $stagingScript -ExpectedScriptSha256 $stagingScriptSha256 -ExpectedApprovalSha256 $stagingApprovalSha256
    } finally {
        if ((Get-ExecutionPolicy -Scope Process) -ne $stagingSavedPolicy) {
            Set-ExecutionPolicy -Scope Process -ExecutionPolicy $stagingSavedPolicy -Force -ErrorAction Stop
        }
    }
}

```

## STOP e evidência posterior

Em qualquer falha de hash/contexto/política/identidade/proteção/recurso/IO/clock/fechamento: não repetir, não baixar pisos, não mudar permissões ou destino. Enviar a saída completa do console, inclusive primeira falha, `SELECTED_STAGING_RESULT`, `STAGING_RESOURCES`, `STAGING_SELECTED_LIMITS` e `MANUAL_SELECTED_STAGING_EXIT`. Não exportar conteúdo secreto; o protocolo só solicita metadata/pins/nonce não secreto. Não criar log operacional adicional ou redirecionar a confirmação.

Uma aprovação manual é usada ao iniciar o procedimento, mesmo se efeitos0/consumed0; consumed nativo distingue submissão mutante. O arquivo de aprovação original fica imutável: a reconciliação futura será separada, sem reescrever esse hash nem rearme. Não há alegação de enforcement persistente entre processos por novo ledger.

Gates frescos AINDA NÃO OBSERVADOS: token/host/boot/contexto, originais/SD/volume/unidades/espaço, READY e recursos correntes,500ms/120s e close. Preparação/hashes em disco não são atestação de imagem mapeada, capacidade futura ou sucesso do efeito. Código e68 CTests anteriores não foram repetidos por não haver mudança nesta preparação.

Só o pedido aprovado autoriza efeitos: até dois diretórios e três arquivos novos, criação exclusiva e proteção na criação, sem adoção ou ACL em objeto existente. Custo opaco/riscos B0–B8 permanecem; falha parcial pode reter objetos/handles/backings e bloquear outra tentativa. Sem cleanup/refund, SCM/Start/Stop, CA/CURRENT, endpoint/rede, TPM/NV/Q4 ou promoção produtiva.

Estado: AUTHORIZED_OWNER_MANUAL_ONCE / EXECUTION_PENDING; E21 PENDING; ED11/Phase6 PARTIAL. PST0.6.4/históricos/pins/UNKNOWN/reservas preservados. Depois de resultado suficiente, apenas preparar qualificação/pedido independente para registro SCM parado; não executar registro.
