# N0START-20261024-02 — somente candidato, nova aprovação necessária

Não executar agora. Não repetir N0START-20261024-01 nem rodar Prepare para testar o relógio. A autorização atual é de fonte/offline.

Script existente: tools/qualification/scm-n0-protected-single-powershell.ps1
SHA256 e5144e031f0d93aba16a22a8dba0b92b1eeb6b0bb4c03c0ca4ec5a4dec24b1cc
Qualificador: build/n0-n9-final-freshness-ordering-20261024/Selected/Release/papacc_scm_instrumented_lab.exe
SHA256 3b245bae8dbbbec3082e531f151183594d9e11e0ae833528b91193671c609880

Host/boot236441/operador1108/serviço/tentativa/config/candidate/políticas/inputs continuam exatamente os de operation-request.json e das decisões accepted. Nova operação administrativa02 não cria outra tentativa normativa/owner; requer decisão específica sobre elegibilidade do claim histórico não consumido. Dados futuros de token/PID/challenge/file-ID só na execução. Antes de efeitos o owner canônico mantém mutex, valida originais e nega se intent/imagem/sink já existir; não apaga nem adota nada para passar.

Após nova aprovação REAL e sua conferência, o registro deve existir exclusivamente em docs/integration/n0-n9-final-freshness-ordering-20261024/EXACT-OPERATION-APPROVED.json. Seu hash ainda NÃO existe, não deve ser inventado nem calculado do registro anterior.

Comando SOMENTE candidato, após registrar a decisão real e fornecer o hash literal:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -ExpectedScriptSha256 e5144e031f0d93aba16a22a8dba0b92b1eeb6b0bb4c03c0ca4ec5a4dec24b1cc -ExactApprovalSha256 $NewOwnerApprovedRecordSha256
```

A variável é descritiva de autorização FUTURA; não executar/copiar como comando pronto. Após decisão, entregar novamente com hash literal. Usar único NOVO Windows PowerShell5.1 x64 já elevado, invocação &, sem criar segundo PowerShell ou Python. Conferir pins independentemente, sem confiar em autoverificação de script substituído.

Três confirmações continuam exatas: START_N0_SINGLE_LAB N0START-20261024-02 com tuple auxiliar contemporâneo; BIND_BD_READ_ONLY com PID/criação/challenge factual; INSTRUMENT_SCM_LAB 75330d11cc7e4123ae62eadc9a59c2f5 com plano positivo completo. Não usar valores históricos ou automatizar confirmação. Confirmações/revalidação completa antecedem a amostra; depois não há espera manual planejada para consumir. Dossiê60s/metric1s permanecem; lentidão pode negar, sem refresh/retry.

Efeitos a aprovar novamente: intent canônico exclusivo antes de efeitos; imagem instrumentada exclusiva; sink/arm1024+append1024; um rebind ImagePath do LAB; um Start e até um Stop seguro quando aceito; coleta/retention. Startup/conta/SID/recovery/triggers/ACL não serão corrigidos. Sem listener/rede/CA/cert/bootstrap/CURRENT. Sem quota/crédito adicional. N7, kernel/cache/transitive stack/picos/gaps/host contention e I/O síncrono permanecem riscos explícitos. Pode consumir sem chegar a Start; sem rollback/delete/overwrite/retry/kill/força de mutex.

Solicitação de Stop, aceitação, conclusão, quiescência e fechamento são fatos distintos. START_PENDING pode não aceitar controles. Falha/timeout/ausência de recibo/UNKNOWN: preservar contexto/evidências, não repetir nem fechar à força. F10 só após aviso factual de retorno do qualificador. Enviar saída completa com IG_FRESHNESS (se houver), primeiro IG_DIAGNOSTIC, IG_FINAL/IG_RETAINED, IG_FIRST_RECORD e N0_CYCLE_EXIT. Sem inferência por silêncio, PID0, STOPPED isolado ou fechamento da janela.

OPERATIONAL_ADMISSION=NOT_YET_GRANTED
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
