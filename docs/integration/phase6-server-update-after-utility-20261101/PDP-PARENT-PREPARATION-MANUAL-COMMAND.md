# Comando manual — única execução D1/PDP autorizada

Decisão real SHA256: `2e9071afcbea55c423b5c676a574725a7b838bebb6e4b5c5e3d2a1300324c254`. Não executado automaticamente. Dois atos independentes; sem retry, skip, cleanup ou CA/Start.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-pdp-parent-preparation.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '864dc8562ee1f32ea7a622c2f3b6f1ede9dcf86646f868142ce3905ce0fd2c18' -ExpectedOperationalRequestSha256 'e8004335ddadc2badda207e2083b382874c559ccfdfaa60bcb2b2fee8d63c6b0' -ExpectedDecisionSha256 '2e9071afcbea55c423b5c676a574725a7b838bebb6e4b5c5e3d2a1300324c254'
```

D1: digitar somente a linha OWNER_PDP_CONFIG_ORIGINAL exibida. Depois comparar o ID do source no plano nativo com o ID criado em D1, parent, operador e leafPapinho; digitar somente OWNER_NEW_INSTALL_PARENT_PREPARE com o nonce efetivamente exibido. Cancelar em divergência. Não repetir após falha; preservar objetos/janela em retenção e enviar toda a saída.
