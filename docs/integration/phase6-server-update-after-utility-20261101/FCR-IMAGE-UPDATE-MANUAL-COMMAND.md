# UPDATE manual — imagem FCR

Uma execução no PowerShell administrativo original. Conferir plano/artefatos e digitar a confirmação nativa específica. Não repetir em falha e enviar saída integral. Não executar FCA antiga ou FCR por este comando.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-fcr-validation-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'e84571f67e4b0d45e95a38bbf37d920e9a61f9dee485e63fdeb54e2049f9e0eb' -ExpectedRequestSha256 '61500898ae1c7eda08e85f340dd0ecd2b557fd9bdb50ee34736ec7a60260cbd5' -ExpectedDecisionSha256 'd788857e053f65bea93a745a4ab87bc132a1275c885b6c9eb2bcd4f11b404ea3'
```
