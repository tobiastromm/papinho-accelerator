# Preparação fixture R0–R4 — autorizada, execução manual pendente

2026-11-03. Pedido c504edee8142647c3a5e2661feb8d46985baf4dbe28acb1b45d3cbe3c6bc026e aprovado pelo proprietário; decisão REAL registrada em OWNER-MACHINE-FIXTURE-R0-R4-PREPARATION-APPROVAL.txt/SHA2567c3ee69b26eb89088b5f07e6fbd8d151a77d07f65af0295c962fe47ca87bf2ab. Pedido original preservado, pins do pacote/fontes/normas/resultados reconferidos offline, sem repetir builds/testes inalterados ou consultar objetos operacionais.

Execute UMA VEZ no WindowsPowerShell administrativo original:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\prepare-machine-fixture-r0-r4.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '88c0c088e1dd53c17bcee731156f8013c82053cbaa008057bc637494106e5c51' -ExpectedOperationalRequestSha256 'c504edee8142647c3a5e2661feb8d46985baf4dbe28acb1b45d3cbe3c6bc026e' -ExpectedDecisionSha256 '7c3ee69b26eb89088b5f07e6fbd8d151a77d07f65af0295c962fe47ca87bf2ab'
```

Não executado pelo agente. Bypass só nesta instância; não é necessário alterar política global. Wrapper verifica hashes de si/pedido/decisão/procedimento-fonte/normas/binários antes das chamadas e exige contexto original. Uma invocação D1 e, somente se fechada com sucesso, uma invocação F. Confirmações atribuídas e consumos separados, nenhuma resposta automática.

Confira pedido histórico ID417614100:6815744:9313/hashfc91..., imagem871936/ID417614100:10223616:73344/SHAa6b8ee1ac5ef3e7b6908686f3690ce2a6ca6c4171812a201b7d3024ea6f2ab8a e novo ID/hash da decisão da saída D1 contra o plano F. Cópia única nova em MACHINE-TEST-ONLY-FIXTURE-R0-R4-DECISION.ORIGINAL.txt; todas as cópias/pedidos/decisões anteriores preservadas. Plano F deve mostrar limites atuais privado/working16777216, entrada128/guarda188/cap192/commit3GiB/físico7239172096/fresh100/global60000, escopo TEST_ONLY e autoridade legítima original. Divergência => STOP/DENY sem contorno.

D1 autoriza só a nova cópia auxiliar da decisão; F só a criação exclusiva da fixture TEST_ONLY e controle administrativo público positivo da mesma chave conforme pedido. Não executa Start/Stop, READ/E/Q/S/P/endpoint/SCQ, UPDATE, SCM/ACL existentes, CA/CURRENT, rede, TPM/NV/Q4 ou produção. Não adota nome de chave anterior ou autorizações usadas. Nome/publicSHA/ID futuro da cópia somente por saída real.

Não repetir o comando após qualquer resultado, mesmo se D1 concluir e F negar. Preserve toda a saída e objetos parciais, backing/owner/reservas se pending ou closeUNKNOWN; sem delete/cleanup/rollback/refund/kill, ajustes de quota/ACL ou nova sessão implícita. Gates negados não são dispensados pela autorização.16MiB não garante suficiência ou PASS.

Envie a saída integral para reconciliação conforme NEXT-MACHINE-FIXTURE-R0-R4-PREPARATION-RECONCILIATION.md. Snapshot inicial pendente em MACHINE-FIXTURE-R0-R4-PREPARATION-AUTHORIZATION-STATE.json não é prova de consumo/criação e será preservado. Histórico/application-close/retenção/reservas/quiescênciaOS UNKNOWN e PST0.6.4 permanecem; readiness produtiva DENY.
