# N0START-20261024-01 — handoff candidato, NÃO EXECUTAR AINDA

Exige aprovação específica do owner para operation-request.json e os efeitos desta página. Implementação somente foi autorizada. Não criar o registro APPROVED antes da decisão real.

Script existente: C:/Projetos/PapinhoAccelerator/tools/qualification/scm-n0-protected-single-powershell.ps1
SHA256: cdda38efeb2fa9cec404b1b1180c2ec420f031f0e41601a79437634760dfea64
Qualificador Selected Release: build/positive-n0-n9-native-lab-cycle-integration-20261024/Selected/Release/papacc_scm_instrumented_lab.exe
SHA256: da7af53d54601fcac5036f3ccfebd7eadb18297499cc5f0a656ce3b74aab70f1

Plano/identidades: [preparação exata](../first-n0-n9-native-lab-start-preparation-20261024/operation-review-and-plan.md), specialized by source-review.md e qualified-pins.json deste pacote. Host/boot236441/operador1108/serviço PapinhoAcceleratorLab20261023/tentativa75330d11cc7e4123ae62eadc9a59c2f5/config/candidate/políticas/inputs permanecem exatos. Novos PID/token/challenge/file IDs só na futura execução. Não reutilizar amostras históricas nem substituir o binário de serviço registrado agora.

## Comando candidato após aprovação e registro real

Uma NOVA janela Windows PowerShell5.1 x64 já elevada; invocar com &, sem powershell.exe -File e sem segundo PowerShell. Conferir script e qualificador independentemente. ScopeProcessBypass apenas se necessário/permitido; não alterar política permanente.

O parâmetro abaixo é variável FUTURA: $OwnerApprovedRecordSha256 deve conter o hash independentemente aprovado do registro REAL em docs/integration/canonical-preflight-busy-diagnosis-20261023/EXACT-OPERATION-APPROVED.json. Esse arquivo está AUSENTE nesta entrega. Não calcular um hash arbitrário e chamá-lo de aprovação. Não copiar operation-request.json como se fosse aprovado.

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -ExpectedScriptSha256 cdda38efeb2fa9cec404b1b1180c2ec420f031f0e41601a79437634760dfea64 -ExactApprovalSha256 $OwnerApprovedRecordSha256
```

Este é somente o comando candidato. Após aprovação e registro real, entregar novamente com o hash literal completo. Preparação não concede execução. Não usar script/binário anterior, --cycle-lab antigo, pins read-only ou PID histórico.

## Confirmações e efeitos

1. PowerShell apresenta operação/tuple/efeitos e exige START_N0_SINGLE_LAB N0START-20261024-01 <attempt> <auxPID> <auxCreation> exatos observados.
2. Owner apresenta plano/hash/token/boot/N7 e BIND_BD_READ_ONLY <attempt> <qualifierPID> <creation> <challenge>. Essa confirmação é SOMENTE binding, não autorização implícita de Start; o plano já distingue a rota positiva e o hash do registro aprovado.
3. Ciclo apresenta os efeitos completos e exige INSTRUMENT_SCM_LAB 75330d11cc7e4123ae62eadc9a59c2f5. Seguem gates factuais, consumo exclusivo, cópia exclusiva da imagem, arm exclusivo, UM rebind ImagePath e UM Start/até UM Stop seguro. Não há retry/overwrite/delete/rollback/kill/ACLremediation. Confirmação lenta/stale/limite excedido pode negar; não repetir automaticamente.

Os efeitos SCM solicitados são somente o ImagePath do serviço existente para papacc_server_lab_host_ntstatus.exe e as duas chamadas limitadas. Startup/conta/SID/DACL/recovery/triggers não serão corrigidos para passar. Qualquer recovery/trigger inesperado ou consulta inconclusiva nega. Copiar imagem e criar arm/intent são efeitos separados a aprovar explicitamente; nenhum input/diretório N0 será recriado.

## Stop, recibos e retenção

Esc/Stop antes de efeitos cancela. Depois, Stop solicitado != aceito != concluído; em START_PENDING pode não aceitar controles. Preservar contexto se vivo/UNKNOWN. Não matar, fechar a janela à força, liberar mutex, apagar ou rearmar. Console desaparecido não é cleanup. Manter efeito parcial, primeiro erro e arquivo até decisão posterior. Uma falha antes do Start pode consumir a tentativa.

Observar IG_SERVICE, IG_PROCESS/IG_HOST, IG_DIAGNOSTIC_RECEIPT/IG_FIRST_RECORD, IG_FINAL ou IG_RETAINED e N0_CYCLE_EXIT. Captura de NTSTATUS pode ser indisponível se falha preceder recorder/sink. IG_FINAL só será aceito pelo launcher após fechamento0 e, se houve Start solicitado, quiescent1; factual original process exit é exigido. F10 somente após retorno factual e aviso explícito. Sem recibo/retorno, reter e informar; nunca repetir.

Host admission5/9GiB, pisos4/8GiB, 60s/final1s, amostragem alvo100ms/gap1s. Native128 e administrativo1024 compartilhado inalterados. Amostras não são máximos; N7, kernel/cache/transitive stack/gaps e I/O síncrono/external contention permanecem risco explícito. Créditos NONE_GRANTED. Listener/rede/CA/cert/bootstrap/CURRENT ausentes; autoridades produtivas DENY; ED11 PARTIAL.
