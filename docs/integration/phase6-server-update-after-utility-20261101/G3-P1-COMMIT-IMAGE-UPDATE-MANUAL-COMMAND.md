# UPDATE manual G3-P1 commit8GiB — uma tentativa

Autorização específica registrada, EXECUÇÃO PENDENTE. Conferir pins antes da
chamada e cada ID/hash/efeito/leaf/K/R no plano nativo antes de confirmar.
Nenhuma resposta automática, autoelevação ou Start. PowerShell5.1Desktop x64
administrativo legítimo. O Bypass vale somente no filho desta chamada.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-G3-P1-COMMIT-IMAGE-UPDATE-REQUEST.CANDIDATE.md' = '246173b9e812fff03444018111ee46a196221b89d3f62c6a6e6795b7530a549b'
        'OWNER-G3-P1-COMMIT-UPDATE-APPROVAL.txt' = 'f7daaf7317487e473c1723f6741bf7037edb45cdf6ee37559e2aeb0759f6b4f7'
        'G3-P1-COMMIT-IMAGE-UPDATE-AUTHORIZATION.md' = '08124b866dada8391044cee99608bca19e74529ff2818b657b699a51b3d898ae'
        'update-g3-p1-commit-image.MANUAL.CANDIDATE.ps1' = '4a20bbbe618fc0a5696c250c1fd037e36268b30756d9d7c5deeec236917066e0'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('COMMIT_UPDATE_PIN_DENY: ' + $name) }
    }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File (Join-Path $folder 'update-g3-p1-commit-image.MANUAL.CANDIDATE.ps1') -ExpectedScriptSha256 '4a20bbbe618fc0a5696c250c1fd037e36268b30756d9d7c5deeec236917066e0' -ExpectedRequestSha256 '246173b9e812fff03444018111ee46a196221b89d3f62c6a6e6795b7530a549b'
    if ($LASTEXITCODE -ne 0) { throw 'COMMIT_UPDATE_FAILED_OR_UNKNOWN_NO_RETRY' }
}

```

Enviar saída integral para reconciliação; não repetir em falha/UNKNOWN. Preservar
anterior/retidos/DLLs/SCM/ProgramData/CA/CURRENT e históricos; sem cleanup ou
rollback. Somente depois de UPDATE qualificado preparar pedido E/Q/S com o
ID real e autorização operacional própria, sem reutilizar decisões consumidas.
