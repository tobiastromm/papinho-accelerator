# Sessão TAKE207 única manual autorizada

Windows PowerShell5.1 x64 como administrador original, console interativo.
Conferir planos/IDs/pins e confirmar documentos e E/Q/S separadamente.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-take207-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'bc6aedf1a280b0a00aa7b7df62413ae338541d3a51881a080492fa08daf6a03b' -ExpectedOperationalRequestSha256 'b2ae05445cc49e19b56d997a2c248db575796ce5ee7896108c862ffd6617bd7e' -ExpectedDecisionSha256 'f3a03914a670bf6fd265997519a21fc672e89bc12bc9fd552db61a36ee25beca'
```

Digitar P1 DATA somente no prompt próprio; sem respostas automáticas.
Falha/cancelamento: não repetir, preservar saída/objetos. Se retido, manter
janela aberta; disposições antigas não concedem encerramento desta instância.
Enviar saída completa.
