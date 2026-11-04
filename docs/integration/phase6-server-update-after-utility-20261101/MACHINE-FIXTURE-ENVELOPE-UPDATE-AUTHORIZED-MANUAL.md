# UPDATE fixture F0–F5 — autorizado, execução manual pendente

2026-11-03. Pedido byte-exato aprovado pelo proprietário; decisão real registrada em OWNER-MACHINE-FIXTURE-ENVELOPE-UPDATE-APPROVAL.txt, SHA256fff38e9245bd93f0ff271f847a9064460ae9ba79bb0ad62387bcd6a23360feb7. Pedido original preservado. Pins do pacote, candidato871936/SHAe4a36490737eb7e1896997f873e5e69511a7a6a65291b0f44399b4ed18f3b676 e fontes/logs qualificados reconferidos offline; não se repetiram builds/testes inalterados.

Não executado pelo agente. Em WindowsPowerShell administrativo original, executar SOMENTE UMA VEZ:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-machine-fixture-envelope-image.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'f824a051df96cc6568cba1288a0e20dbab763c5ca9e28ee574dca091d87e0b1f' -ExpectedRequestSha256 'ef7432c1b3c4289bcf8a36592f0efc892eb0ce76b1f59c0db58b0ac12fae5183' -ExpectedDecisionSha256 'fff38e9245bd93f0ff271f847a9064460ae9ba79bb0ad62387bcd6a23360feb7'
```

O script verifica seu próprio SHA, pedido, decisão, normas, executáveis, hashes e IDs retidos ANTES da chamada; o owner nativo exige proteção/continuidade/SCM STOPPED/PID0/gates U4 frescos, plano original, declaração atribuída e consumo único antes dos efeitos. Leia o plano e confirme somente se corresponder ao pedido. Nenhuma resposta é automática. Não executar Stop, editar ACL ou contornar negação.

O comando usa Bypass apenas nesta instância; não é necessário alterar a política global. Não executar scripts históricos nem repetir este comando após qualquer resultado. Em falha/divergência/UNKNOWN preservar toda a saída e objetos, sem retry/rollback/limpeza/refund. Copiar a saída integral da única execução para reconciliação.

Somente UPDATE do EXE; sem fixture/cópia protegida/endpoint/SCQ/P/StartStop/CA-CURRENT/rede/TPM-NV-Q4/produção. Preservar EXE anterior/DLLs/updater/SCM, dois originais fixture existentes, históricos/pins/consumos/PST0.6.4. Causa1816/application-close/retenção/reservas/quiescênciaOS UNKNOWN não são resolvidos por esta autorização.

Próximo trabalho: NEXT-MACHINE-FIXTURE-ENVELOPE-UPDATE-RECONCILIATION.md. Estado inicial em MACHINE-FIXTURE-ENVELOPE-UPDATE-AUTHORIZATION-STATE.json não deve ser reescrito retroativamente como execução.
