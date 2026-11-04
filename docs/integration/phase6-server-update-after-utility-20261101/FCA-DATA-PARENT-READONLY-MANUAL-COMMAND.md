# Comando preparado — não executar antes da autorização específica

Pedido SHA256: `39cf3699b037983e9bfd84959740b170eab502b2c6a1a319f27215f9490e7ae3`.

Uma execução manual read-only; sem retry. Confirmação: `OBSERVE_FCA_DATA_PARENT_ONCE`.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-fca-data-parent.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '5c27d2fdacc319d4c0b6c6d5a0b73b54ee9f4f76fdf12ec6645ac772bdd951be' -ExpectedRequestSha256 '39cf3699b037983e9bfd84959740b170eab502b2c6a1a319f27215f9490e7ae3'
```

Não repetir a chamada após consumo ou falha; enviar toda a saída. Se houver retenção, preservar a janela e os objetos; nenhum kill está autorizado.
