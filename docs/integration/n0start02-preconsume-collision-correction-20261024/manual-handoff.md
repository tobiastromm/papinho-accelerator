# N0START-20261024-02 — novo pacote para reaprovação

NÃO executar agora. A tarefa autorizou correção e qualificação offline. A aprovação condicional anterior permanece preservada, sem promover seu hash ou corpo para autorização executável.

- Script: `tools/qualification/scm-n0-protected-single-powershell.ps1`
- SHA256: `573bf1ca9a95dc052c992a0893b47f06f9dd96a92f53d3aa41cdf5bcc25951c2`
- Qualificador: `build/n0start02-preconsume-collision-correction-20261024/Selected/Release/papacc_scm_instrumented_lab.exe`
- SHA256: `b482b959400b41e6e64ecb86858dfd00522a0aa988b36300dc7d70f4d3c33eae`
- Tentativa: `75330d11cc7e4123ae62eadc9a59c2f5`
- Host: `2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff`; boot236441; operadorSID terminado1108; LAB `PapinhoAcceleratorLab20261023`.

[operation-request.json](operation-request.json) fixa configuração, candidato instrumentado, políticas, RAW1, efeitos, condicionantes e riscos. Os inputs ProgramData existentes não são recriados. PID,token,criação,desafio e métricas serão factuais na execução futura. Os números históricos de handles não são admissão fresca.

O gate novo nega colisão antes do consumo pelo handle original do pai. As criações finais permanecem exclusivas; consulta não reserva. Colisão posterior pode consumir a tentativa sem Start e sem rollback. Consulte [revisão](source-review.md), inclusive a limitação do fixture nativo de concorrência bloqueado por1307; ele não foi contado como PASS nativo.

É necessária nova aprovação específica deste pacote/pins. O registro futuro deverá ser revisado e criado exclusivamente em `C:\Projetos\PapinhoAccelerator\docs\integration\n0start02-preconsume-collision-correction-20261024\EXACT-OPERATION-APPROVED.json`. Esse arquivo permanece AUSENTE. Não copiar a aprovação condicional, nem reutilizar registro01 ou pins anteriores. Após decisão real, entregar seu SHA256 literal completo.

Comando apenas candidato, deliberadamente incompleto e NÃO pronto para execução:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -ExpectedScriptSha256 573bf1ca9a95dc052c992a0893b47f06f9dd96a92f53d3aa41cdf5bcc25951c2 -ExactApprovalSha256 $NewOwnerApprovedRecordSha256
```

A variável representa autorização futura; não executar. Um único Windows PowerShell5.1 x64 já elevado, invocação direta, sem segundo PowerShell. Conferir pins por fonte independente antes da execução. Confirmações não podem ser automatizadas: START_N0_SINGLE_LAB N0START-20261024-02 com auxiliar contemporâneo; BIND_BD_READ_ONLY com PID/criação/challenge nativos; INSTRUMENT_SCM_LAB com tentativa/plano positivos exatos apresentados. Não copiar valores históricos.

Antes de efeitos: canonicamente comprovar ausência do claim/launch anterior sob exclusividade, ausência dos dois nomes pelo pai original, identidades/RAW1/proveniência e configuração protegidas, SCM STOPPED/PID0/startup/conta/SID/recovery/triggers esperados, censo e limites atuais. Nenhuma remediação para passar.60s/1s,128 nativos,1024 administrativos,quatro papéis,NONE_GRANTED. Qualquer divergência ou Stop nega; sem retry.

Efeitos a reaprovar: consumo único,imagem exclusiva,sink/arm exclusivo,um rebind ImagePath,um Start e até um Stop seguro quando aceito,recibos e retenção. N7 permanece aceitação residual limitada: associação console e imagem mapeada UNPROVEN,sem autoridade auxiliar. Sem rede/listener/CA/cert/bootstrap/CURRENT/produção. I/O síncrono pode atrasar; custos/picos/kernel/cache/gaps não observados UNKNOWN.

Stop solicitado,aceito,concluído,quiescência e fechamento são fatos distintos. Timeout,janela fechada ou STOPPED isolado não provam cleanup. Sem kill,força de mutex,delete,overwrite ou rollback. Preservar recibos/efeitos parciais/UNKNOWN. F10 somente após retorno e fechamento factuais. Não repetir operação01 nem testar Prepare.

OPERATIONAL_ADMISSION=NOT_YET_GRANTED
START_EXECUTED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=N0START02_EXACT_OPERATION_REAPPROVAL
