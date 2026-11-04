# Disposição WS3M autorizada — execução manual pendente

Pedido aprovado byte-exato: `ffd24d8f4bdcf272b819b61c43c352bff467d1a7caeaf07f8e2e528873357bfe`.
Decisão efetivamente emitida registrada em OWNER-WS3M-PARTICIPANTS-DISPOSITION-APPROVAL.txt,
SHA256 `4268e4f2181030606e0e4ae2d5b1fe1901ef28cdac07eee60367ae266d2749bf`.
Fonte, modelo, log e wrapper conferidos contra os pins da qualificação52cenários.
Nenhum método nativo foi invocado nesta preparação; nenhum build/teste inalterado repetido.

Executar UMA vez em Windows PowerShell administrativo original, sem redirecionamento:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\dispose-ws3m-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '472c31542b8da80f33b7bce3238a51f63cc17395f70cc04e96d8f6edc36be233' -ExpectedRequestSha256 'ffd24d8f4bdcf272b819b61c43c352bff467d1a7caeaf07f8e2e528873357bfe' -ExpectedDecisionSha256 '4268e4f2181030606e0e4ae2d5b1fe1901ef28cdac07eee60367ae266d2749bf'
```

Somente se os dois objetos/identidades exibidos coincidirem com o pedido, digitar
manualmente `DISPOSE_WS3M_19860_19836_ONCE` no prompt nativo. Não há resposta automática.
Se houver divergência, cancelar. Falha/UNKNOWN não autoriza repetir nem atingir outro PID.
Enviar a saída integral para reconciliar consumo, submissões, saídas, SCM e close.
Até essa saída, execução e consumo permanecem pendentes, não presumidos.

Não autoriza UPDATE/Start/limpeza. Application close, bilateral original, retenção
histórica e quiescência OS permanecem UNKNOWN mesmo após eventual saída.
