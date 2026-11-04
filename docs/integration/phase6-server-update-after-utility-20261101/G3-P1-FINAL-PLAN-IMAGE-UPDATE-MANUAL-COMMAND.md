# Comando manual único — UPDATE diagnóstico final G3-P1

Aprovação registrada; execução PENDENTE. Executar uma vez em PowerShell
administrativo x64. Conferir plano nativo, IDs/pins/nomes/K/R antes de confirmar.
Sem respostas automáticas, retry, rollback, Start ou limpeza.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-g3-p1-final-plan-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'dabb0df0426916576e3f66d3cc414c265303bb35a177aa60e4824d7491cbb043' -ExpectedRequestSha256 '43a5bad791d20ee5d48c7bd0efe90e25ca956a139c1f491bc8f7bc986720b4c7'
```

Procedimento confere os hashes antes da chamada. Bypass só no processo filho,
sem mudança persistente. Gates originais/frescos obrigatórios; UNKNOWN que
impeça a operação => STOP. Enviar saída integral; NÃO REPETIR em falha/UNKNOWN.
