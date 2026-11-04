# UPDATE manual P1 3 GiB — uma tentativa

Autorização específica registrada, EXECUÇÃO PENDENTE. PowerShell5.1Desktop x64
administrativo legítimo, sem autoelevação. Conferir pins antes da chamada e
cada campo do plano nativo/IDs/hashes/efeitos/leaves/K/R antes de confirmar.
Nenhuma resposta automática. Bypass apenas no processo filho desta chamada.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-G3-P1-3GIB-IMAGE-UPDATE-REQUEST.CANDIDATE.md' = 'dad56b876cdbf34d004bfe56ca89037298260e85abe17db3bb42832860d1f3af'
        'OWNER-G3-P1-3GIB-UPDATE-APPROVAL.txt' = '90163176a985c2506fa96f6249789baed844e503c852fc9f2521aa706cabb9eb'
        'G3-P1-3GIB-IMAGE-UPDATE-AUTHORIZATION.md' = 'd8471af6aea060112939d3b8fdba152a500c2bf6a45742903978253d250938d2'
        'update-g3-p1-3gib-image.MANUAL.CANDIDATE.ps1' = '83adb8e5309086d6d9a149045d080a3bc69c277f0fb1ee609f9e75225dbd4c67'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('THREE_GIB_UPDATE_PIN_DENY: ' + $name) }
    }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File (Join-Path $folder 'update-g3-p1-3gib-image.MANUAL.CANDIDATE.ps1') -ExpectedScriptSha256 '83adb8e5309086d6d9a149045d080a3bc69c277f0fb1ee609f9e75225dbd4c67' -ExpectedRequestSha256 'dad56b876cdbf34d004bfe56ca89037298260e85abe17db3bb42832860d1f3af'
    if ($LASTEXITCODE -ne 0) { throw 'THREE_GIB_UPDATE_FAILED_OR_UNKNOWN_NO_RETRY' }
}

```

Enviar saída integral; não repetir em falha/cancelamento/UNKNOWN. Não iniciar
serviço. Após UPDATE/readback/ID novo, preparar pedido E/Q/S independente com
3GiB e aprovação específica, sem reutilizar decisões antigas. Retenção
histórica/OS UNKNOWN, produtivo DENY; preservar todos os objetos/EXEs/pins/
retidos/originais/reservas/PST0.6.4, sem limpeza/refund/rollback.
