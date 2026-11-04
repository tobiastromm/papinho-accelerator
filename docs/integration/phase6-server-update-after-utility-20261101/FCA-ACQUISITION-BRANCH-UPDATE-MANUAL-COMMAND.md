# Comando manual UPDATE aquisição FCA — autorizado e pendente

Uma execução no Windows PowerShell administrativo original. Conferir plano nativo e responder somente à confirmação exibida, sem automação. Não executar FCA antiga/novo ensaio ou repetir em falha. Enviar saída completa para reconciliação.

& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-fca-acquisition-branch-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'd6013467022e83a25edb0ebe01dd90bd25942d8f602f2aaed208ee438fb9a115' -ExpectedRequestSha256 '5fb51f54b9832364c19cd79ea4f1189c24a54a43258aecb2720b9982bc575355' -ExpectedDecisionSha256 'fde7370fc844e77d678d62c777bb616e72d5b6c3ac3e5f61f846e5e3c05490cd'
