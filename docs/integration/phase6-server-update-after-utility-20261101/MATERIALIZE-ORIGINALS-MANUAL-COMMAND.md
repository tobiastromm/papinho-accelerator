# Histórico do comando manual — autorização CONSUMIDA; NÃO REPETIR

Pedido candidato de materialização:
ad44e1b0e754819c8795ea65acbb3a70e6cae9de826470a06217e9a79e01e399.
O proprietário autorizou UMA execução manual na task
PACC-PHASE6-ONE-ORIGINAL-PAIR-MATERIALIZATION-AUTHORIZATION.
Registro: ORIGINAL-PAIR-MATERIALIZATION-AUTHORIZATION.md. Execução única
concluída pelo proprietário e reconciliada em
ORIGINAL-PAIR-MATERIALIZATION-RECONCILIATION.md. O bloco abaixo permanece
somente como histórico. NÃO EXECUTAR novamente. Nenhum retry está autorizado.

Após autorização explícita desse pedido, uma única execução manual em console
próprio/elevado do operador esperado. Qualquer erro interrompe, sem retry.
Conferência de hashes antecede a chamada; Bypass fica somente no processo filho,
sem mudança persistente de ExecutionPolicy, direitos ou ACLs existentes.

```powershell
& {
    $pairFolder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pairPins = @{
        'ONE-ORIGINAL-PAIR-MATERIALIZATION-REQUEST.CANDIDATE.md' = 'ad44e1b0e754819c8795ea65acbb3a70e6cae9de826470a06217e9a79e01e399'
        'FIRST-START-SESSION-REQUEST.DRAFT.md' = '312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d'
        'OWNER-FIRST-START-APPROVAL.txt' = '7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a'
        'first-start-originals.cs' = '18719507fafe6ea6fb81f73dc688410ce7531dfb6074c331de123e0441878f5f'
        'prepare-first-start-originals.PREPARATION-ONLY.MANUAL.ps1' = '9e4c8cecc4d1e5bf58808100a6085ce717270936a9314856051af7530e126516'
    }
    foreach ($pairName in $pairPins.Keys) {
        $pairPath = Join-Path $pairFolder $pairName
        if ((Get-FileHash -LiteralPath $pairPath -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pairPins[$pairName]) {
            throw ('PAIR_PIN_DENY: ' + $pairName)
        }
    }
    $pairProcedure = Join-Path $pairFolder 'prepare-first-start-originals.PREPARATION-ONLY.MANUAL.ps1'
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File $pairProcedure -ExpectedScriptSha256 '9e4c8cecc4d1e5bf58808100a6085ce717270936a9314856051af7530e126516' -ExpectedRequestSha256 '312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d' -ExpectedDecisionSha256 '7f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a'
}
```

O script para na confirmação atribuída, sem resposta automática. Nenhum comando
de endpoint/SCQ/Start é emitido ou chamado. Enviar saída integral para reconciliação.
Resultado esperado para considerar apenas a materialização: primeira falha NONE,
consumed=True, created_mask=3, verified_mask=3, close_known=True e duas identidades
com readback. São máscaras, não contagens; first diferente de NONE ou close incerto
impede sucesso mesmo com bits. Exit0 não prova autoridade operacional ou quiescência.

Se houver criação parcial/UNKNOWN, preservar todos os objetos e a autorização
utilizada; não apagar, adotar, substituir, restaurar ou repetir. Mesmo se bem-sucedido,
o original da decisão continua de PREPARAÇÃO, sem autoridade endpoint/SCQ/Start.
