# Consulta única manual autorizada

Executar em OUTRA janela Windows PowerShell, preservando a janela retida.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-g3-p1-3gib-retained.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'ec4824cd671066221dfdde93b9e5b7045ad6f99b109f1b6e7b6c42874729dd1c' -ExpectedRequestSha256 '559b7944e57b0ddf0191d5acbc5bf05f5e82c9da6e4c437e18400f8eab704d92'
```

Confirmar OBSERVE_RETAINED_G3_P1_ONCE no prompt. Enviar saída completa.
Não repetir em falha/cancelamento; não fechar a janela retida.
