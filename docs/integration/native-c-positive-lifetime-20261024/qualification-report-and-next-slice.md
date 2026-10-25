# Native C — qualificação isolada de lifetimes positivos

O fixture final passou 52 execuções nativas isoladas, em Selected/Legacy Debug/Release, com zero warnings nos quatro builds finais /W4 /WX. A viabilidade operacional do LAB continua NÃO DEMONSTRADA. A forma de lifetimes conserva três referências adicionais antes do primeiro monitoramento e seis após a validação live; não há margem comprovada sobre a observação histórica126/128.

## Escopo e integridade

Branch main, HEAD `913c90cf2328f7939d8f5b927640601903ac6e91`. A comparação integral de10.331 arquivos prévios confirmou zero alterações e zero ausências antes do append da documentação viva. Código operacional, launcher, pins, contratos accepted, dossiê e evidências anteriores permanecem byte-exatos. Preimagem da live doc e relatório de preservação acompanham este pacote.

Autoridades: Model E, ED11 B0–B6 revisão4, N0–N9, L0–L9, H0–H9 e obrigações Stop/UNKNOWN/retenção. Nenhuma nova decisão normativa ou admissão foi criada. O plano, auditoria A–J, ownership matrix, viabilidade N0START03 e relatório A1 são a base do experimento. A direção futura C2 permanece inalterada: esta medição não implementa C2 nem promove o LAB.

O executável standalone inclui os bytes funcionais de `scm-aux-reservation-core.inc` e `scm-aux-reservation-bridge.inc`, sem modificá-los. Contexto e Stop do harness não constituem owner/autoridade de admissão. O ledger do fixture não é registry de produto; os dois slots A1 continuam exatamente dois. Selected/Legacy são compilações identificadas do mesmo helper agnóstico de perfil, não reconstrução ou qualificação do runtime completo do produto. Windows10.0.19045 AMD64, MSVC19.51.36256, Python3.14.7 existente usado somente como observador externo; hashes em tool-and-source-identities.json.

Todos os arquivos são .dat sintéticos em diretórios exclusivos sob build/native-c-positive-lifetime-20261024/runs. Não houve uso de caminhos LAB, mutex canônico, intent operacional, imagem/sink/arm de produto ou diretório protegido. Objetos do fixture e seus arquivos persistem para inspeção; nenhum delete, rollback, overwrite ou remediação de ACL foi realizado. Não há chamada SCM. O worker é este mesmo fixture, sem console adicional, sem rede, e apenas sinaliza prontidão e espera seu evento Stop. Nenhum serviço foi executado.

## Medição e limites

`phase-ledger-factual.csv` contém1.296 amostras; o observador externo confirmou exatamente os mesmos totais em todas1.296, durante barreira ACK por stdin/stdout já existentes. Seu único handle de consulta por execução pertence à tabela Python, é fechado após o término factual e nunca é somado à tabela do fixture. Os128 GetProcessHandleCount repetidos por execução terminaram com total igual ao inicial. Isso não torna observação/CRT/logging/PSS gratuitos em CPU, memória ou picos entre leituras. A barreira modifica o timing; este não é teste de latency/freshness do ciclo operacional.

[GetProcessHandleCount](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesshandlecount) fornece a contagem do processo, não atribuição por objeto nem pico universal. O ledger complementar registra os valores próprios, slot/geração, aquisição após retorno da API, marca de última chamada de uso, tentativa e sucesso de close com QPC/tick. Essas marcas não são timestamps internos de criação/destruição no kernel nem limite de operações transitórias não observadas. `reference-lifetimes-factual.csv` e `a1-real-objects-events.csv` preservam os eventos.

Tipos e identidades: File/Directory com [GetFileType](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfiletype), volume/File ID/links/no-reparse; Process/Thread com PID/TID/criação e handle original; Token com TokenStatistics; Event/Mutex/Toolhelp por proveniência da API criadora documentada. Não foi usada enumeração privada NtQueryObject. Borrow/alias não cria novo handle e não é contado como aquisição. Roots são cinco handles do MESMO diretório sintético, e os papéis administrativos usam self como substituto. A revalidação RAW nativa via GetSecurityInfo(mask7) usa o mesmo objeto aberto; ela NÃO qualifica a proteção N0–N9 real, assinatura Windows ou associação de console. SDDL/ACL não foram normalizadas nem alteradas.

O ledger e seus buffers, stdio, ACK e processo observador têm custo. Eles não foram incorporados ao Accelerator. O fixture não reproduz todo o trabalho de hash, assinatura, censo autenticado, RPC/SCM, stack-VA walk, runtime/managers ou conta virtual do LAB. Custos omitidos e diferenças de contexto permanecem UNKNOWN; não aplicar seus totais absolutos como quota do produto.

## Ledger factual A–J

Valores da coluna correspondente ao produto são a contagem REAL de referências explícitas dos objetos substitutos, agrupadas conforme o ledger estático; não são contagem do Accelerator real.

| Fase | Correspondentes ao ledger | Extras exclusivos do fixture | Fato observado |
|---|---:|---:|---|
| A baseline / comuns |0 →9|0|Seis arquivos, token, aprovação sintética e mutex sem nome.|
| B handoff / bound |34 →27|0|Onze de contexto, cinco originais, sete de binding; par A1 fecha e cinco originais fecham.|
| C censo |28 →27|0|Toolhelp próprio fecha; snapshots PSS separados abaixo.|
| D intent .dat |28|0|CREATE_NEW, write/flush, mantido até J; não há intent operacional.|
| E cópia .dat |29|0|Writer fecha antes de reopen; identidade igual, sem dois handles simultâneos de writer/reader.|
| F sink .dat |30 →29|0|Fecha antes do primeiro monitoramento.|
| G criação / supervisão |30|3 →1|Processo próprio + evento Stop; ready event e handle inicial de thread fecham após prontidão. Nenhum SCM.|
| H primeiro monitoramento |30|1|+3 persistentes relativamente à base27; PSS separado.|
| H live / retorno |34 transitório →33|1|Token+token duplicado+snapshot de threads permanecem até J; thread temporário fecha.|
| I Stop sintético |33|1|Evento solicitado, processo original sinalizado e exit0; referências do pai ainda permanecem.|
| J fechamento |0|0|Closes próprios comprovados no caminho limpo; não prova liberação global do Windows.|

Pico amostrado explícito:34 correspondentes ao ledger,35 incluindo o evento Stop no intervalo live. Durante CreateProcess há três extras (Stop, ready e thread inicial), mas esse ponto soma33, abaixo35. Snapshots HPSS/marker NÃO são somados arbitrariamente como um HANDLE cada. Totais brutos amostrados do processo atingiram97–103 no caminho limpo, com baseline52–58. Esses valores incluem recursos fora do ledger explícito e pertencem somente ao fixture.

Verificação externa posterior confirmou conteúdo idêntico origem/cópia em32 cenários com processo sintético. Não há adoção de imagem real nem autenticação de binário mapeado. A presença de arquivo persistente após close não é contada como handle vivo.

## PSS e overhead

64 capturas finais, com as mesmas flags de threads/contexto do observer: THREADS|THREAD_CONTEXT e CONTEXT_CONTROL, em self e no worker. [PssCaptureSnapshot](https://learn.microsoft.com/en-us/windows/win32/api/processsnapshot/nf-processsnapshot-psscapturesnapshot) exige Windows8.1+; nenhuma compatibilidade PSS com Windows7 é reivindicada. Não foram capturados stacks em bytes, segredos, conteúdo de registradores ou ETW.

Em todas as capturas amostradas: +1 no coletor após capture; +0 adicional após marker; +1 explícito ao abrir o thread de validação, total +2 sobre antes da captura; após PssWalkMarkerFree e [PssFreeSnapshot](https://learn.microsoft.com/en-us/windows/win32/api/processsnapshot/nf-processsnapshot-pssfreesnapshot), delta0. PssFreeSnapshot recebe GetCurrentProcess porque o descritor capturado pertence ao coletor, inclusive quando o alvo é o worker. Cada thread é validado por processo/criação/liveness e fecha uma vez. `pss-overhead-factual.csv` registra cada conjunto.

São custos amostrados da tabela, NÃO teto da API, prova de custo físico zero, restituição de crédito ou atribuição universal do HPSS. GetProcessHandleCount anterior ao PSS continua sem enxergar esse overhead posterior. PSS não foi substituído ou alterado no produto.

Há ainda drift bruto NÃO atribuído ao ledger: +8 já no binding e +10 ao final relativamente à baseline, incluindo +2 durante criação/supervisão. A comparação on/off de PSS não atribui esses resíduos a PSS; todas suas liberações medidas voltaram ao total anterior. Sem rastreio por objeto não se declara leak, Windows cache, vazamento de CRT ou referência dispensável. `residual-observations.csv` mantém essa diferença como observação, não crédito/zero. As quatro primeiras execuções por binário também tinham baseline seis handles maior; isso não é economia provada.

## A1, Stop e negativos

52 casos finais: por configuração seis clean (três PSS off e três on), aquisição falha, close incerto, saturação, Stop, stale, processo terminado e processo vivo. A1 usa arquivos REAIS isolados: reserva antes da API, geração, borrow BUSY, native File ID e checked close. Saturação dos dois slots nega a terceira reserva antes de nova aquisição; fechamento conhecido pode concluir após Stop. Nenhum array/quota operacional cresceu.

Close incerto é fault injection: o callback retorna erro5 SEM chamar CloseHandle sobre o arquivo real. Primeiro erro1816 fornecido sinteticamente pelo caller é preservado (não é nova falha LAB); o valor real permanece UNKNOWN, o segundo permanece OWNED, novas aquisições e segundo close são negados. Nos quatro casos o subprocesso retorna2 como contenção explícita de teste, com onze refs conhecidas ainda retidas. Saída do processo NÃO é recibo de close nativo, quiescência global, refund ou ausência de leak. Nunca se exercitou close inválido em objeto operacional. A1 não libera mutex/dependências nessa condição.

Aquisição ausente retorna2 do Windows, sem handle; o teste negativo termina0 somente porque observou a negação esperada e fechamento conhecido. Stop antes do primeiro efeito sintético impede o arquivo novo. Stale usa uma marca sintética1001ms antiga e observa1460; não aumenta limites60s/1s nem prova tempo de admissão real. Processo vivo permanece com referência até saída factual; processo já encerrado nega validação live. Stop via evento não é SERVICE_CONTROL_STOP: solicitado, aceito por SCM, concluído e quiescência de serviço não são inferidos.

A campanha inicial foi preservada: Selected Release falhou com18 depois da aquisição do snapshot de módulos, antes de Module32FirstW bem-sucedido. Faltava prontidão do worker. O protocolo do fixture foi corrigido com evento ready temporário e fechamento explícito, sem retry de snapshot ou mudança do produto. A campanha v2 passou52 casos; v3 acrescentou somente evidência das identidades/leases A1 e passou os52 casos finais. Logs, diretórios, fontes anteriores e binários anteriores foram preservados. Nada disso repete uma tentativa LAB consumida.

## Viabilidade e menor caminho

A medição confirma no fixture a simultaneidade conhecida: +2 antes da criação do processo, +3 no primeiro monitoramento, +6 depois de live e +7 com thread temporário, relativamente à base bound27. Mantida SOMENTE como hipótese a observação histórica126, as projeções128/129/132 (e133 transitório) continuam. Somar o overhead PSS observado poderia pressionar ainda mais durante a captura, mas não constitui uma projeção factual nem um teto qualificado do LAB.

Portanto, permanecer abaixo128 no fixture97–103 NÃO concede orçamento independente nem torna a operação viável. A base126 não é fresca e o fixture omite custos/validações importantes. O orçamento administrativo1024 também não foi medido por este experimento; não somar os handles do worker, Python ou qualificador ao conjunto PowerShell/auxiliar. Quatro papéis, caps128/1024, headroom, freshness60s/1s, HOST_CREDITS=NONE_GRANTED, reservas reais e UNKNOWN históricos/H0–H9 permanecem.

A candidata mínima já identificada C1 é fechar localmente os três recursos de cycle_live depois de TODOS os últimos usos, com close checado, primeiro erro/freeze/retenção e sem retry. Esta tarefa documenta seus lifetimes e não aplica nem qualifica um patch dessa otimização. Ela pode reduzir o intervalo pós-live em três referências nomeadas, mas NÃO corrige o +3 anterior ao primeiro monitoramento, onde a projeção já é129. Não fechar intent, imagem ou processo obrigatório para fabricar PASS. Não há correção única suficiente demonstrada; o próximo slice deve tratar capacidade conhecida ANTES de consumo, sem remover provas, ampliar quotas ou substituir PSS automaticamente.

CURRENT_TASK_COMPLETED=YES
NATIVE_LIFETIME_FIXTURE=52_FINAL_NATIVE_CASES_PASS_FOUR_CONFIGURATIONS_ZERO_WARNINGS
OWNED_HANDLE_PEAK=34_LEDGER_CORRESPONDING_35_WITH_FIXTURE_STOP_EVENT_SAMPLED
PSS_OVERHEAD=CAPTURE_PLUS1_MARKER_PLUS0_EXPLICIT_THREAD_PLUS1_AFTER_FREE_DELTA0_SAMPLED
POSITIVE_CYCLE_VIABILITY=NOT_DEMONSTRATED
SAFE_REDUCTION=THREE_POSTLIVE_REFS_CANDIDATE_ONLY_INSUFFICIENT_FOR_FIRST_MONITOR
START_EXECUTED=NO
STOP_SERVICE_EXECUTED=NO
PREFLIGHT_EXECUTED=NO
INTENT_OPERATIONAL_CONSUMED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_10331_BASELINE_FILES_UNCHANGED_BEFORE_LIVE_APPEND
FIRST_UNRESOLVED_DIVERGENCE=NO_DEMONSTRATED_CAPACITY_FOR_THREE_CONTROLLED_POSITIVE_REFS_OVER_HISTORICAL_TWO_HANDLE_MARGIN
NEXT=NATIVE_C_POSITIVE_RESOURCE_CORRECTION_OR_OPERATION_REVIEW
