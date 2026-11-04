# Disposição única manual autorizada

Executar em OUTRA janela Windows PowerShell5.1 x64 como administrador.
Preservar a janela original; não a fechar nem usar Ctrl+C.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\dispose-retained-p1-controller.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '002497df6d4b4851aa24563399124f8a9ad604bae30256bb58cc1329f62c24d2' -ExpectedRequestSha256 '18d4d848249c4a820699d027f1df7216c930905e61a8f91dbfb0babb4b78bff5' -ExpectedDecisionSha256 '370373aeb948e8935fc1ea705694d55978caa0cfa3d8a600d044696c75e706f5'
```

Conferir o plano e digitar somente quando solicitado:

```text
DISPOSE_RETAINED_P1_148_134381137375135845_ONCE
```

Não repetir em falha/cancelamento/timeout. Enviar saída completa desta janela
e as novas linhas da janela original. O wrapper original pode terminar com erro
após abandono; isso é esperado, não concede retry ou sucesso diagnóstico.
Não realizar UPDATE ou nova E/Q/S.
