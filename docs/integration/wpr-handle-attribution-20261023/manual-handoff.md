# WPR Handle attribution — preparation and manual handoff

TASK=PACC-PHASE6-WPR-HANDLE-ATTRIBUTION-1
2026-10-23. Preparação concluída; captura/atribuição pendentes. Sem alteração funcional ou normativa.

## Estado e autoridades

main / HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Revisados recommendation.md da revisão Windows, decisão accepted B0–B6 revisão4, specification1.0 §11.2/11.3, decisões de preservação incremental/perfil Windows, launcher, censo, observer e collector de console. Preservar Model E: primeira falha, ownership, Stop distinto de quiescência e retenção UNKNOWN; nenhuma liberação por ausência de processo. ED08 permanece independente.

B4 não admite automaticamente o observador externo. Esta captura será uma operação diagnóstica separada, sem admissão operacional, crédito ou consumo da tentativa. A autorização futura deve abranger explicitamente os recursos ETW e, se escolhido o experimento abaixo, UMA repetição read-only do preflight. A autorização desta tarefa é preparação apenas.

## Identidade da ferramenta e pins

WPR: C:\Windows\System32\wpr.exe,321536bytes, FileVersion10.0.19041.6093 (WinBuild.160101.0800), Microsoft Corporation. SHA2568c3d49a83ebd8138ffa15da93d25ba1fc44acdec3cb9d729b6f43b55a5ef7ca5.

Get-AuthenticodeSignature retornou Status0/assinatura verificada, tipo Catalog2, signer CN=Microsoft Windows, O=Microsoft Corporation, L=Redmond, S=Washington, C=US; thumbprint3B77DB29AC72AA6B5880ECB2ED5EC1EC6601D847. É prova do arquivo verificado nesta consulta, não de imagem de processo futuro. Revalidar hash/identidade antes da captura.

Pins preservados:

- launcher scm-instrumented-start-uac.ps1:60a6dd761fee00143cdf88d7fcf629ad1a47986f41875ab1e8020c641b10bca0.
- qualificador build/console-proof-20261023/Selected/Release/papacc_scm_instrumented_lab.exe:e76df9084e62e761bd24e818074c4aed1dcff520b63aa38660275400ed153a51.
- scm-instrumented-cycle-lab.inc:1718c5db36a76bd329cff04459cbfab58da5300e36df33c70593d740afde5f61.
- decisão ED11:e7031bd29c9bd174359ffcc3c9a021a6ad25a0256a5cf03b40c0866d3a5776b7.

WPA não localizado anteriormente no PATH/caminho convencional; não implica ausência global. Usar estação de análise com WPA compatível já qualificado, ou decidir separadamente a instalação necessária. Nenhuma instalação efetuada.

## Perfil efetivo e limites

Consulta local `wpr -profiledetails Handle.Light -filemode`: Handle.Light.File. Keywords CpuConfig, Handle, Loader, ProcessThread. Dois coletores: System Collector e Event Collector, cada um20 buffers de1024KiB. Total configurado40MiB; não é custo total máximo do ETW/kernel nem crédito. Providers auxiliares incluem BrokerInfrastructure, Kernel-EventTracing, WPR-Control, ProcessStateManager e PerfTrack. Nenhum stackwalk anunciado neste perfil; stacks NÃO garantidas. Não selecionar Verbose automaticamente para obter stacks.

A CLI local confirma start, recordtempto, filemode, skipPdbGen e instancename (último argumento). São consultas de ajuda, não execução de captura. Iniciar exige operador administrador e autorização específica. Falha/permissão insuficiente => preservar erro e STOP; não alterar políticas/ACL.

Modo arquivo tem crescimento sem teto nativo de tamanho. Janela proposta30segundos de atividade é limite operacional escolhido, não limite físico, watchdog ou garantia de conclusão de I/O. O owner deve aceitar explicitamente esse custo residual ou manter STOP. Registrar espaço livre antes/depois e tamanho final; não inventar crédito nem cota máxima. Não usar o perfil Memory padrão como alternativa silenciosa. Tracing de kernel pode conter nomes/caminhos/atividade de outros processos; guardar evidência local externa ao repositório e restringir sua distribuição, sem alterar ACL nesta tarefa.

## Experimento único proposto

Uma captura Handle.Light, iniciada ANTES do nascimento do qualificador, envolvendo no máximo UMA execução manual `Prepare` do launcher pinado. Serviço não iniciado; manter rejeição170 quando o censo rejeitar. A autorização deve mencionar ambas as operações: trace externo e repetição read-only. Capturar baseline anterior, nascimento, sequência de criação/duplicação/fechamento, instante aproximado do IG_PROCESS e término factual. Identidades atuais por PID+criação; PIDs históricos não são alvos atuais.

A janela30s serve apenas se o owner conseguir preparar a invocação e confirmação antes de iniciá-la. UAC/espera humana pode excedê-la: interromper a coleta adquirida, preservar resultado incompleto, sem retry automático. O encerramento do trace é independente do serviço e não significa StopService. Não pedir fechamento de janela/qualificador retido como forma de liberar referências.

Se não for autorizada repetição do preflight, uma captura ociosa não atribui os handles do qualificador já encerrado. Não produzir processo fictício como substituto equivalente. Manter atribuição pendente.

## Comandos manuais preparados — NÃO executar sem aprovação específica

Usar console elevado dedicado ao diagnóstico. Antes: verificar operador/host/boot e hashes; consultar sessões existentes, não cancelar ou tomar posse de trace alheio. Se houver conflito de logger/kernel collector, STOP mesmo com nome próprio. Nenhum `wpr -cancel`, kill ou flush alheio.

A identidade externa deste experimento é WPRH-20261023-01, distinta da tentativa operacional reservada. Diretório abaixo deve estar AUSENTE. Se já existir, STOP; não apagar, reutilizar ou inventar próxima tentativa automaticamente.

```powershell
$wprHandleRoot = 'C:\Projetos\_tmp\PapinhoAccelerator\wpr-handle-attribution-20261023-01'
if (Test-Path -LiteralPath $wprHandleRoot) { throw 'EVIDENCE_ALREADY_EXISTS_STOP' }
# Somente após aprovação: criar diretório diagnóstico externo, não arquivo operacional.
New-Item -ItemType Directory -Path $wprHandleRoot -ErrorAction Stop | Out-Null
& 'C:\Windows\System32\wpr.exe' -status
# Revisar saída e ownership. Não prosseguir sobre sessão/conflito incerto.
& 'C:\Windows\System32\wpr.exe' -start Handle.Light -filemode -recordtempto $wprHandleRoot -instancename WPRH-20261023-01
# Registrar código imediatamente. Só após sucesso factual, prosseguir.
$wprHandleStartCode = $LASTEXITCODE
```

Se start falhar, não emitir stop/cancel por presunção; consultar somente estado para determinar aquisição parcial. Preservar códigos e UNKNOWN. Se start passar, guardar horário/identidade da sessão. O qualificador continua sem autorização positiva.

Comando de estímulo read-only, SOMENTE se essa repetição também for expressamente aprovada:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\scm-instrumented-start-uac.ps1' -Mode Prepare -ExpectedScriptSha256 60a6dd761fee00143cdf88d7fcf629ad1a47986f41875ab1e8020c641b10bca0
```

Não usar Launch/ElevatedRun. O launcher pode continuar criando seus recibos administrativos exclusivos conforme contrato; isso não é arquivo diagnóstico operacional. A rejeição170 é resultado válido do experimento e não deve ser contornada.

No console do diagnóstico, ao concluir o estímulo ou atingir janela30s, consultar e encerrar UMA VEZ a sessão própria, mantendo o alvo intacto:

```powershell
& 'C:\Windows\System32\wpr.exe' -status collectors -details -instancename WPRH-20261023-01
if (Test-Path -LiteralPath ($wprHandleRoot + '\handles.etl')) { throw 'OUTPUT_EXISTS_STOP_NO_OVERWRITE' }
& 'C:\Windows\System32\wpr.exe' -stop ($wprHandleRoot + '\handles.etl') 'PACC WPRH-20261023-01 read-only handle diagnosis' -skipPdbGen -instancename WPRH-20261023-01
$wprHandleStopCode = $LASTEXITCODE
# Consultar estado próprio após retorno; código0 sozinho não prova cleanup universal.
& 'C:\Windows\System32\wpr.exe' -status -instancename WPRH-20261023-01
Get-FileHash -LiteralPath ($wprHandleRoot + '\handles.etl') -Algorithm SHA256
```

Comandos são roteiro para execução sequencial e revisão humana, não bloco automático: falha em qualquer passo bloqueia os posteriores conforme aquisição factual. Se saída já existir inesperadamente durante trace ativo, preservar sessão/arquivo e STOP-new-effects; não abandonar referência nem sobrescrever. Registrar disposição da sessão como UNKNOWN se não verificável. Sem retry de start/stop ou cancel automático.

Confirmação proposta: `CAPTURE_WPR_HANDLES WPRH-20261023-01 ONE_TRACE ONE_READONLY_PREPARE NO_SERVICE_EFFECTS`. Precisa ser aprovada; não cria grant canônico nem altera B4.

## Análise WPA e critérios de interpretação

Abrir ETL offline; registrar versão WPA, integridade/hash, intervalo de cobertura, perdas de eventos/buffers e erros de decodificação. Verificar tabelas efetivamente disponíveis de processos e handles (ou Generic Events conforme versão); ausência de tabela/evento não é zero handles. ProcessThread/Loader e Handle precisam fornecer cobertura útil, não apenas aparecer na configuração.

Selecionar a instância factual do qualificador por criação+imagem+PID, não PID isolado. Manter atores PowerShell, conhost e WPR separados. Agrupar, quando o schema fornecer: processo dono da tabela de handles, tipo de objeto, valor do handle, objeto, timestamp/operação, processo/thread executor, origem/destino de duplicação e stack disponível. Valor de handle e endereço de objeto podem ser reutilizados: não usar como identidade eterna.

Construir saldo apenas com cobertura comprovada: baseline/rundown conhecido + entradas válidas - fechamentos correspondentes, tratando duplicação conforme flags e destino, sem dupla contagem. Handle em processo PowerShell não integra automaticamente GetProcessHandleCount do qualificador. Arquivo/objeto compartilhado pode ter handles distintos em vários processos. Comparar saldo no instante do log com a nova amostra GetProcessHandleCount, declarando desalinhamento temporal. Não exigir repetir147 nem ajustar saldo para atingir esse número.

Light pode não oferecer stacks, handles anteriores ao trace, rundown completo ou evidência suficiente de herança. Sem origem/flags/duplicação observáveis, classificar herança UNKNOWN. Objeto aberto ao final da janela não é automaticamente leak ou retenção contratual; término de processo não inventa recibo do owner. Custos kernel, cache, tabela de handles e picos entre observações permanecem UNKNOWN. Perdas/intervalo incompleto impedem conclusão quantitativa completa mesmo se contagens parecerem plausíveis.

## Classificação atual e oportunidades

| Classe | Evidência atual | Conclusão permitida |
|---|---|---|
| A necessários ao qualificador | Código abre pins/SCM/referências de atores e owner; APIs de runtime/CRT podem abrir outros | Categorias candidatas, contagens atribuídas PENDENTES |
| B PowerShell/console | Amostras históricas724/703/191 handles em processos separados | Não somar esses handles aos147 do qualificador; custos simultâneos existem |
| C herdados | Nenhum ETL de nascimento/duplicação | UNKNOWN; não inferir só de parent ou stdout redirecionado |
| D transitórios | Código usa tokens, Toolhelp, PSS, marker, thread e WinVerifyTrust | Hipóteses de fases, não atribuição medida; custo indireto UNKNOWN |
| E retidos | Recibo close0 e owned capture refs CLOSED reportados | Não demonstra que todos147 eram refs explícitas ou que nenhum objeto permaneceu |
| F sem atribuição | Amostra147 do qualificador | Excesso19 sobre128 é comparação aritmética, não19 handles descartáveis |

Detalhe útil: GetProcessHandleCount ocorre ANTES de PssCaptureSnapshot para a mesma amostra. Não atribuir os147 ao PSS subsequente só porque a rotina o contém. Instrumentação de confiança executada antes pode introduzir estado de bibliotecas; somente trace comprova quais handles/lifetimes.

Correção mínima recomendada AGORA: nenhuma mudança funcional. Após atribuição, avaliar fechamento antecipado apenas de referências temporárias com lifetime comprovado, remoção de coleta redundante em favor de ferramentas externas e isolamento temporal de verificação de proveniência. Não remover pins, mutex, referências de identidade ou retenção para cumprir128. Se baseline necessário exceder128, requer decisão normativa fundamentada; não antecipar aumento.

Alternativa PowerShell único já elevado permanece apenas proposta: três processos de aplicação potenciais (PowerShell, qualificador, serviço) em lugar de quatro, sem prometer redução dos handles internos do qualificador. Infraestrutura console e coletor externo continuam tendo custos. Exige revisão de dois atores distintos atualmente obrigatórios e B4; nenhuma implementação nesta tarefa.

## Resultado

CURRENT_TASK_COMPLETED=YES
WPR_PROFILE=HANDLE_LIGHT_FILE_METADATA_VERIFIED_CAPTURE_COVERAGE_PENDING
TRACE_PREPARED=YES_CONDITIONAL_MANUAL_PLAN
TRACE_EXECUTED=NO
HANDLE_ATTRIBUTION=PENDING_ETL
HANDLE_LIMIT=128_UNCHANGED
CONSOLE_CENSUS=UNCHANGED
ED11_TOTAL_STATUS=PARTIAL
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=147_SAMPLE_UNATTRIBUTED_VERSUS_128_GUARD
NEXT=MANUAL_WPR_HANDLE_CAPTURE_AFTER_APPROVAL

Sem builds/testes de runtime: somente revisão, hashes, assinatura e metadados/ajuda WPR. Sem trace, fixture, launcher, preflight, rede operacional, SCM/ACL, bootstrap, CA, chaves/certificados, CURRENT, quotas, PST/Browser/PACC wire ou produção0.6.4 alterados. Sem clone/worktree/commit/push/tag/release.

Referências primárias: [CLI WPR](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/wpr-command-line-options), [WPR](https://learn.microsoft.com/en-us/windows-hardware/test/wpt/windows-performance-recorder). Ajuda local prevalece para opções efetivas deste binário; sucesso e cobertura só poderão ser afirmados após captura. Documentação Microsoft declara modo arquivo sem teto nativo de crescimento; a janela proposta não elimina essa limitação.
