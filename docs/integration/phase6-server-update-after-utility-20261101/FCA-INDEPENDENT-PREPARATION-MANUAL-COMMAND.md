# Comando manual FCA autorizado — execução única pendente

Executar no Windows PowerShell administrativo original. Nenhum UPDATE necessário. O wrapper confere hashes/IDs; o owner exige confirmação atribuída nativa. Não repetir em falha, não fornecer resposta automática. Enviar saída completa para reconciliação. Não há Start neste ato.

& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-first-instance-fca-preparation.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '9aa3c67757ebaced2d10bb5fd370e3b2e8e2b5849b22f4f7eefafb47e91d7796' -ExpectedOperationalRequestSha256 '2d7d03574ffcacce1bffd64e3510e7082d4e7c5841c1d74cdf157334957ec8a3' -ExpectedDecisionSha256 '6c19523bc1b188304f79c6bf5289ab7ddcbdb254a74e7df38e8aaf8a3c44e07b'
