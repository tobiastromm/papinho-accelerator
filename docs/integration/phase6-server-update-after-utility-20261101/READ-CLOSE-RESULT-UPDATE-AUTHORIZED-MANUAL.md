# Um UPDATE manual

Autorização prospectiva real registrada; gates frescos e confirmação nativa obrigatórios. Sem execução automática ou retry.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-read-close-result-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'cf0aff3fffa97f16aa8e367a4fefcdc1dd8efe6480ae5be620c9689efd45e95d' -ExpectedRequestSha256 '57403127d1730fd389ede137c1d7a60ed19a74b42c59bc85c272fa857965e371' -ExpectedDecisionSha256 '51a798d221a0b450c281b1db5d77810ed2222d26544683ab9950d2aa19576cf6'
```

Conferir plano/hash/IDs e enviar saída integral. Não iniciar sessão antes de reconciliar UPDATE.
