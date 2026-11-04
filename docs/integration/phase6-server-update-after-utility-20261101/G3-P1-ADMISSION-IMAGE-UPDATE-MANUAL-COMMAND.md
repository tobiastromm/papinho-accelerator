# Uma atualização manual — diagnóstico da admissão

Execução PENDENTE, autorização contextual registrada. Conferir integridade dos
arquivos do pacote e plano nativo. Uma chamada do updater protegido, sem resposta
automática, retry/rollback/limpeza ou Start. O procedimento confere seus pins,
request, executor/DATA/antigo/DLLs e chama os gates nativos existentes.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-g3-p1-admission-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '2858c5fe2c74d63eabad453c00fd45282401c325536ea499b897b9e5aa381b75' -ExpectedRequestSha256 'a7b7bff08dfc7ba41e8463590ecfb17fb12fe66acda8622f2745b8db3e0fea3f'
```

Não repetir em falha/UNKNOWN. Enviar saída integral; depois de readback e ID real,
preparar sessão E/Q/S independente já solicitada. Nenhum teste operacional aqui.
