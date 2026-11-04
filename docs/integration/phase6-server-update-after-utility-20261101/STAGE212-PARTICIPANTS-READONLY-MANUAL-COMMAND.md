# Consulta manual única autorizada

Executar em outra janela PowerShell elevada, preservando os processos retidos. Não repetir em falha.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-stage212-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '4ff39b81d8ab16eaee50cd9224d5557f67fd0669c881e49bb1851e2db0fe2d8e' -ExpectedRequestSha256 'e21756e47387d4e1500d03f32d4142ad97ba4c76f1d97e844ba9bf9d7f748e65'
```

Confirmação interativa: `OBSERVE_STAGE212_TWO_PARTICIPANTS_ONCE`. Enviar saída integral. Nenhum Stop/kill/Start/UPDATE autorizado.
