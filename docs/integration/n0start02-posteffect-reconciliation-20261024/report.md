# N0START02 — efeitos reconciliados e crescimento administrativo

## Resultado e fronteira

A tentativa foi consumida. Imagem,sink/arm e ImagePath instrumentado permanecem presentes. Não houve Start/Stop na tentativa,conforme recibo. Quiescência continua UNPROVEN; close0 atesta referências próprias do qualificador,não quiescência do serviço ou ausência de efeitos persistentes. Nenhum rollback ocorreu. A aprovação02 é histórica e não reutilizável.

HEAD/main913c90cf2328f7939d8f5b927640601903ac6e91,pins fontes/binários/políticas e registro aprovado SHA256556167c630bf870797dbd2b970c6d5a5f2177cc06d731b1986c4aa30dbe18456 conferidos. RAW1,N0–N9,ED11,Model E/Stop/quiescência e retenção existentes preservados. Sem alteração funcional ou normativa.

## Efeitos factuais atuais

Consultas sequenciais somente por leitura: Get-Item/Get-FileHash/Get-Acl,fsutil file queryfileid,Get-Content do intent,ReadAllBytes do sink e sc qc/queryex/qfailure/qfailureflag/qtriggerinfo/qsidtype. Não são transação atômica nem admissão operacional. O sandbox não podia ler o intent; consulta read-only autorizada fora dele conseguiu,sem mudança de permissões. SDDL consultada é representação descritiva,não digest RAW1 nem reserialização aplicada.

- Intent103 bytes,IDhex0000000000000000002000000023fb51,SHA2569e54d105d484c2201b5b280f9354038e9373f355f137c7e8a0debbca1ca45895. Conteúdo exato registra tentativa75330d11cc7e4123ae62eadc9a59c2f5 CONSUMED/NO_RETRY/NO_RECOVERY/UNKNOWN_UNTIL_FACTUAL_CLOSE. Owneroperador1108,DACL protegida SYSTEM/Administrators/operadorFA. Nenhuma reutilização ou remoção.
- Imagem434688 bytes,IDhex0000000000000000000a00000024dfd7,SHA256eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b coincide com fonte instrumentada aprovada. AtributoArchive,sem ReparsePoint. DACL protegida,owneroperador,grantsFA apenasSYSTEM/BA;demaisACEs observadas0x1200a9 de leitura/execução. Grupo observado S-1-5-21-896877160-3262526087-2538948522-513. Isso não declara imagem mapeada autenticada.
- Sink1024 bytes,IDhex0000000000000000001000000024e0ad,coincide com high1048576/low2416813 do recibo;SHA256240d658c0cd8fdc2fdd6980f81c3a6419b1c8427876b766a664d686ebdd661e0. DACL protegida com SYSTEM/BA/operadorFA e SIDserviço0x120083. Validação offline seguindo PSRF_FRAME/psrf_checksum/psrf_valid existentes:magic,attempt,version1,kind0,length0,checksum e texto zero PASS. Somente arm;sem append final,nem NTSTATUS da falha psr_prepare.
- SCM ImagePath permanece "C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release\papacc_server_lab_host_ntstatus.exe" service host. DEMAND_START,conta virtualLAB,SIDUNRESTRICTED,STOPPED/PID0,Win32Exit1077,specific0. Recovery sem ações listadas/reset0/mensagem e comando vazios/non-crashFALSE,triggersnão registrados. Não inferir quiescência a partir desse estado.
- PIDs históricos20740/21328/488 não apareceram na consulta atual. Essa ausência momentânea não é recibo de quiescência nem atribuição de identidade a processo novo. Fechamento histórico do qualificador e retorno/saída do launcher são fatos separados dos objetos persistentes.

## Crescimento e localização da rejeição

| Processo/papel | Inicial | Final | Delta |
|---|---:|---:|---:|
| PowerShell administrativo |751|759|+8|
| Auxiliar administrativo |271|273|+2|
| Conjunto administrativo |1022|1032|+10|
| Qualificador separado |122|124|+2|

Orçamento compartilhado1024: margem inicial2; amostra final excede8. Qualificador124/128 não é somado ao orçamento administrativo. Essas são amostras sequenciais,sem saldo por objeto,timestamp de cada criação/close ou prova de pico/simultaneidade atômica. Não provam leak ou custo mínimo inevitável.

O caller mantém cycle_stage=EXPLICIT_INSTRUMENTED_REBIND até START_ONCE. Já houve ChangeServiceConfig bem-sucedido,readback,recovery,status e censo antes das últimas linhas de processos. Em seguida ig_observe imprime as amostras e chama papacc_server_service_admission_bd_metrics,que valida estado/inventário e ps_bd_costs antes do retorno ao ig_metric do caller. O custo1032 necessariamente nega1816. O diagnóstico genérico não captura a instrução interna exata ou uma API nativa falha; o código/valores identificam o gate suficiente para rejeitar. Não chamar1816 de falha de ChangeServiceConfig. Não há IG_FRESHNESS de timeout nesta execução.

Primeiro instante do crescimento: UNKNOWN dentro do intervalo entre cada par de amostras. Para o PowerShell,GetProcessHandleCount precede PssCaptureSnapshot na própria coleta inicial. Depois há emissão de saída,demais coletas,consumo,cópia,sink,rebind/readbacks/censo e segunda coleta. Para o auxiliar,sua coleta ocorre depois de outros participantes. A ordem não isola uma operação responsável.

## Ledger estático de referências

| Recurso/operação | Owner da tabela | Lifetime demonstrável no código | Relação com delta |
|---|---|---|---|
| Pins de script/exe/políticas/inputs/aprovação e locks de ancestrais |PowerShell|Todos adquiridos antes do filho e da primeira amostra;Dispose após retorno/final|Baseline,sem nova chamada Pin/Lock no loop;nenhum dos+8 identificado individualmente|
| Process/StandardOutput/controle |PowerShell|Antes da primeira amostra até EOF/WaitForExit/recibo|Obrigatórios para supervisão/Stop/retorno;nenhuma remoção precoce justificada|
| ReadLine/Write-Host/.NET/console |PowerShell e infraestrutura,não atribuição automática|Ativos entre amostras;internos não inventariados|Hipótese de crescimento/transitório,sem prova de8objetos|
| Intent consumido |Qualificador|CREATE_NEW conservado até cleanup canônico|Referência nativa,fora do orçamento administrativo|
| Writer da imagem,reabertura readonly |Qualificador|Writerfecha;readonly original verificado conservado até cleanup|Compatível com+1nativo,sem atribuição por evento|
| Sink/arm |Qualificador|psrf_create escreve/flush/fecha antes de IG_DIAGNOSTIC_CREATED;descriptorLocalFree|Não fica aberto pelo preparador na amostra final;arquivo persiste|
| SCM manager/service/rebind |Qualificador|HandlesSCM já adquiridos antes da primeira amostra;rebind usa mesmo handle|Sem OpenSCM explícito novo no PowerShell;efeitos internosUNKNOWN|
| Snapshot PSS,marker,thread handles |Qualificador e mecanismos internos do Windows|Threadfecha por iteração,marker/snapshotfecham por processo;primeira contagem antecede captura|Uso obrigatórioED11;efeitos remotos/internos não atribuídos por estas amostras|
| Auxiliar |Auxiliar|Fonte executada pelo Windows não instrumentada nesta tentativa|+2observados;tipo,necessidade e lifetimeUNKNOWN|

Os+2do qualificador são compatíveis com intent+imagem retidos,mas saldo líquido não prova que foram exatamente esses handles e nenhum outro. Tampouco duplicatas guardadas no qualificador devem ser contadas como handles da tabela do PowerShell ou auxiliar.

## Correção mínima e próxima evidência

Não foi demonstrada redução segura. Não preparar patch para fechar pins,controle,pipe ou referências de identidade antes da amostra final. Não atribuir o aumento ao sink,cópia,rebind ou PSS sem evidência de ownership/lifetime. Não aumentar1024 nem declarar1032 como mínimo normativo necessário.

Primeira prova ausente: duas tabelas de handles/eventos por objeto,associadas à criação nativa dos processos administrativos e aos limites das operações desta execução. Não existe captura de handles desta tentativa no material fornecido; o ETL antigo do qualificador não a substitui. Os processos históricos não podem ser recriados como prova retroativa.

Próximo slice proposto,dependente de autorização: experimento isolado sem SCM/LAB/intent,com supervisão e sequência representativas,comparação externa das tabelas PowerShell/auxiliar por ferramentas Windows/Sysinternals já qualificadas,discriminando saída/console e coleta obrigatória. Primeiro testar com fixture,sem nova arquitetura/recorder nem replay desta operação. Uma redução só será proposta após provar objetos,owner e lifetime e revisar Stop/retenção. Nenhum teste ou build de código foi necessário nesta tarefa read-only;nenhuma captura ETW ou operação positiva ocorreu.

A reconciliação de efeitos está concluída,mas a tarefa completa de atribuição causal permanece incompleta. CURRENT_TASK_COMPLETED=NO reflete essa prova ausente,sem bloquear a preservação factual já obtida. O intent consumido e a aprovação histórica continuam sem permissão de retry,rearme ou rollback.
