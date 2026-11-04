# UPDATE manual do presenter — uma tentativa

Autorizado contextualmente. EXECUÇÃO PENDENTE. Executar uma vez no PowerShell administrativo legítimo; conferir o plano nativo antes de confirmar. Sem Start/retry/rollback/limpeza.

```powershell
& {
    $ErrorActionPreference = 'Stop'
    $folder = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101'
    $pins = @{
        'ONE-G3-P1-PRESENTER-IMAGE-UPDATE-REQUEST.CANDIDATE.md' = '70799ac571a47b88aae381dc5a5089a0f9f7c30d2afcdd2734d34d07adaf12b8'
        'OWNER-G3-P1-PRESENTER-UPDATE-APPROVAL.txt' = '0adc24244d7cd0da29d21109596b45fb1fa2bc78ad29a40e5d9951866618a22d'
        'G3-P1-PRESENTER-IMAGE-UPDATE-AUTHORIZATION.md' = 'b351ae99456543af1735446f8ac1c4a82d6bcbc862c6471c993824b115610dc2'
        'update-g3-p1-presenter-image.MANUAL.CANDIDATE.ps1' = '9c031a0da99aded2f60bb57d939f55fff05db4708ba7be74f9506e8f5b3dcae8'
    }
    foreach ($name in $pins.Keys) {
        if ((Get-FileHash -LiteralPath (Join-Path $folder $name) -Algorithm SHA256 -ErrorAction Stop).Hash -ne $pins[$name]) { throw ('PRESENTER_UPDATE_PIN_DENY: ' + $name) }
    }
    & 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File (Join-Path $folder 'update-g3-p1-presenter-image.MANUAL.CANDIDATE.ps1') -ExpectedScriptSha256 '9c031a0da99aded2f60bb57d939f55fff05db4708ba7be74f9506e8f5b3dcae8' -ExpectedRequestSha256 '70799ac571a47b88aae381dc5a5089a0f9f7c30d2afcdd2734d34d07adaf12b8'
    if ($LASTEXITCODE -ne 0) { throw 'PRESENTER_UPDATE_FAILED_OR_UNKNOWN_NO_RETRY' }
}
```

Enviar saída integral para reconciliação, sem repetir em falha/UNKNOWN. Nova sessão E/Q/S depende do novo ID canônico verificado; nenhuma identidade ou confirmação antecipada.
