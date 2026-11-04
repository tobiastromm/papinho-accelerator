# Sessão manual autorizada — execução pendente

Uma chamada somente, sem retry ou respostas automáticas. O script confere os pins; conferir IDs dos originais com os planos e as três confirmações independentes. Cancelar em divergência. Não fechar à força processos retidos.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-wait-state-diagnostic.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '0a14e8cb99513c8b548979e28b84d039c1611b5a7568b023f0f8321e1c768dee' -ExpectedOperationalRequestSha256 '5bf09792d603754b94e9117cb6514895081f16414c48482e731108ad6ba29618' -ExpectedDecisionSha256 'a9253f2243c30b4de2f6f480a0c5df79dd48ba7c6deed27243963a8e2893e37b'
```

Após a chamada, preservar e fornecer a saída integral. Exit isolado não comprova autenticação, receipt/close ou sucesso diagnóstico. Histórico e quiescência OS continuam UNKNOWN.
