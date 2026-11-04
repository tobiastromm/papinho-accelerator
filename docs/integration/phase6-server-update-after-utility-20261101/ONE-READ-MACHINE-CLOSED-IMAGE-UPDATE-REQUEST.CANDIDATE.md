# Pedido operacional — imagem READ/MACHINE fechada

CANDIDATE / FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED. 2026-11-03.

Solicita-se selecionar o EXE Selected/Release abaixo e autorizar UMA execução MANUAL de BINARY_UPDATE_CLOSED PERSONAL_LOCAL, U1–U7/U2, exclusivamente para substituir papacc_server.exe. Não é autorização de fixture, endpoint, QUERY/SCQ, P, Start/Stop ou produção. Autorizações anteriores continuam consumidas. Nenhuma operação foi executada nesta preparação.

## Qualificação e norma

K0–K9 accepted em docs/adr/DECISION-personal-scq-read-machine-k0-k9-20261103.md, por incorporação byte-exata de READ-MACHINE-DIAGNOSTIC-SCOPE.CANDIDATE.md, SHA2568489c009dfa62df0835415d4544c0632abd850924a35217b2ec451718147ad38. Não há nova ADR ou ampliação dos demais perfis. Caller frio READ/QUERY, controle administrativo fresco, P separado, resultado, receipt/close e lifecycle foram integrados e qualificados em modelo; não provam capacidade Windows ou isolamento real.

READ-MACHINE-END-TO-END-OFFLINE-QUALIFICATION.json, SHA256 d3622a25bc080ffe5326b20da7cd50b4ba052fe4e8ef03316efcec12fc292a40: quatro configurações Selected/Legacy Debug/Release, zero warnings,156 pares PASS (39 por configuração). Cadeia integrada27 cenários/1473 checks; quatro caminhos positivos fechados, lifecycle de serviço em modelo; READ21 cenários/927 checks, entrada7 e seleção fria8 negativos/positivos. Logs anteriores preservados e oito pins do snapshot anterior reconferidos. Não repetir campanhas inalteradas.

U1–U7 accepted por incorporação do UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md SHA2569fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234; U2 SHA25658493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da. Mesmo owner/updater/primitivas, nenhum subsistema adicional.

## Objetos, executor e DATA

Operador esperado TCORP-TOBIAS, SID S-1-5-21-785699550-1440458426-1682014700-1108, contexto administrativo legítimo e console original PowerShell5.1 Desktop AMD64. Elevação, arquivo, hash e argumentos não concedem autoridade: declaração/confirmação OWNER_BINARY_UPDATE atribuída ao plano original e consumo único antes dos efeitos continuam obrigatórios. Nenhuma resposta automática, autoelevação ou ajuste de permissões.

Destino existente C:\Program Files\Papinho\Accelerator, parent417614100:8257536:71209; âncora histórica417614100:131072:226316. Exigir aquisição/retenção/revalidação nativa, owner/DACL estritos, ancestrais protegidos, ausência de reparse e continuidade. Não escolher destino alternativo para contornar falha.

Executor independente protegido C:\Program Files\Papinho\Accelerator\papacc_update.exe:104448bytes,ID417614100:30670848:57859,SHA256084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a. Capacidade U1–U7/U2 demonstrada no percurso existente, não inferida da presença do arquivo. Conferir identidade/proteção/continuidade/imagem/contexto atuais; não atualizar executor, não executar candidato do workspace.

EXE canônico anterior esperado C:\Program Files\Papinho\Accelerator\papacc_server.exe:809984bytes,ID417614100:1638400:439388,SHA25678886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5. Fonte histórica reconciliada SQF1-CONTENT-DETAIL-IMAGE-UPDATE-RESULT.json; não é lease fresco, readback atual ou atestação de páginas mapeadas. Atualizações anteriores consumidas.

Novo candidato EXCLUSIVAMENTE DATA:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe

**871424bytes; SHA256 6063d3692f22cfd3baa7e5128b5b787ce6ce33ddb362e805897323f39884599a.** Delta61440bytes sobre instalado histórico;26112 sobre snapshot offline845312. Cópia congelada offline papacc_server.read-machine-closed-871424.exe, mesmos bytes, sem autoridade ou instalação. Somente caminho build acima será passado ao updater; qualquer rebuild divergente bloqueia.

DATA será adquirida por referência original retida, descritor inspecionado, identidade/continuidade/conteúdo/hash verificados antes da cópia e consumo. ID futuro da aquisição não é inventado. O workspace não precisa ter ACL do executor e não recebe sua autoridade; executor/originais continuam estritos U2. Rejeitar aliases/hardlinks entre DATA/executor/canônico.

DLLs idênticas, somente conferir: libssl-3-x64.dll1321472bytes/SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed e libcrypto-3-x64.dll7217664bytes/SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044. Pins do build reconferidos offline; instalados dependem dos gates originais frescos. Nenhuma substituição de DLL/updater ou mudança de ProgramData.

## SCM, retenção e gates frescos

PapinhoAccelerator OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1, NT SERVICE\PapinhoAccelerator, SID_UNRESTRICTED1; comando exato:
"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed.

Exigir configuração original e SERVICE_STOPPED/PID0 antes dos efeitos e após readback. Não executar Stop para satisfazer gate. Histórico TRANSPORT_ONLY fechado qualificado não prova estado atual, READ, fixture, KSP ou quiescência. Não exigir PID/token futuro de uma sessão READ durante UPDATE.

Preservar todos os .previous, candidatos, originais e efeitos históricos; inclui anterior retido da tentativa984306131079e5b4e267734fbab4340f106a38dbd5d9f4f9acc25e216604eb1c e candidatos de tentativas falhas. Nenhum objeto histórico é adotado, limpo, excluído ou renomeado por esta autorização. Retenção histórica, application-close histórico, reservas e quiescência OS continuam UNKNOWN. Estado impeditivo, objeto/execução ainda ativa, identidade/SCM incoerentes, colisão ou continuidade inconclusiva => STOP/DENY, sem tratar UNKNOWN como resolvido ou trocar nomes para contornar conflito.

Antes da chamada: hashes do pedido/decisão efetivamente emitida/procedimento/executáveis; host/contexto e originais; tamanho/identidade/hash/SD/ancestrais; três binários instalados/DLLs; geometria/recursos/SCM atuais. Antes de efeitos: gates nativos originais repetidos/revalidados e plano imutável apresentado; confirmação manual atribuída e consumo. Não transportar G3/K7/B0/R0 para U4.

## Efeitos persistentes estritamente delimitados

Uma chamada do executor,16 argv existentes, CSPRNG32 e tentativa originais. Os dois leaves exclusivos concretos de candidate/previous são produzidos e apresentados pelo owner ANTES dos efeitos, não inventados neste documento.

1. Revalidar originais/executor/DATA/parent/DLLs/SCM/recursos; consumir uma autorização antes dos efeitos.
2. Uma criação FILE_CREATE exclusiva protegida BA/SY full e AU RX, copiar871424bytes da DATA retida, hash, flush e readback; revalidar referências antes dos renames.
3. Rename do canônico anterior para previous exclusivo, sem overwrite, RootDirectory=NULL, mantendo referências originais e guards.
4. Rename do candidato protegido para canônico, sem overwrite; readback de identidade/conteúdo/SHA256/proteção, antigo retido e SCM STOPPED/PID0.
5. Fechamento uma vez quando demonstrado. Stop do owner cancela a operação; não é ControlService/Stop. Falha parcial, chamada pendente ou fechamento incerto preservam objetos/coorte/backing/reservas e UNKNOWN; nenhum retry, rollback, restauração, delete, refund ou limpeza.

Nenhum arquivo auxiliar documental protegido será criado pelo UPDATE. Nenhuma edição de ACL existente, SCM, CA/CURRENT, root/serviço/canal, fixture/KSP, endpoint, Start/Stop, rede, TPM/NV/Q4 ou autoridade produtiva. ACL inicial do único candidato está delimitada acima.

## Custos e riscos reconhecíveis

Mesmo updater protegido: K2027314bytes, quota3145728, piso físico/commit próprio K com margem fresca. Evidência histórica do seu plano em OWNER-SQF1-CONTENT-DETAIL-UPDATE-OUTPUT.txt; não usar getter K2033362 da imagem servidor nova para fingir que updater foi recompilado.

R=ceil(871424/U)*U com geometria real validada e overflow verificado. Exemplo U4096 => R872448bytes, cálculo e não amostra atual. Caller-free/actual-free >= R antes da criação e restante arredondado antes da escrita, contabilizando retidos existentes. Nenhuma reserva é presumida devolvida.

Loader/CRT/wrapper/alocador/SD/hash/provider/SCM/RPC/cache/filesystem/flush/rename/kernel têm custos/latência opacos. Margens não demonstram máximos nativos ou suficiência. Pode ocorrer substituição parcial persistente e intervalo sem canônico; não há atomicidade, durabilidade de metadata ou cancelamento síncrono garantidos, nem rollback automático. Instalar esta imagem não comprova isolamento MACHINE, prontidão produtiva ou causa histórica de falhas.

## Procedimento manual preparado, ainda não autorizado

update-read-machine-closed-image.MANUAL.CANDIDATE.ps1, SHA256 d7dc0c5fce801e1e21cf26dfc178118384251e9b59f4edaee3b9aa2b322146a0. AST e bindings estáticos PASS; helpers read-only reutilizados,16 argv/uma chamada; procedimento NÃO executado. Preflight do antigo fecha antes do DELETE nativo; sem ampliar FILE_SHARE_DELETE. O owner readquire o antigo nativamente; snapshot não substitui continuidade. Close incerto bloqueia chamada.

O arquivo OWNER-READ-MACHINE-CLOSED-UPDATE-APPROVAL.txt NÃO existe. Somente após decisão operacional específica efetivamente emitida: registrá-la fielmente, conferir seu hash real e preparar comando com hashes reais do procedimento/pedido/decisão. Não fabricar aprovação, preencher hash fictício, fornecer comando com placeholders ou executar automaticamente.

Decisão solicitada: selecionar este EXE871424/SHA2566063d3692f22cfd3baa7e5128b5b787ce6ce33ddb362e805897323f39884599a e autorizar UMA execução manual EXCLUSIVAMENTE deste pedido, reconhecendo custos opacos, parcialidade, retenção, ausência de rollback e gates frescos. Sem decisão específica, UPDATE permanece DENY.

Após saída: conferir primeira falha/consumo/criação/dois renames/antigo/canônico/ID/hash/SD/readback/SCM/fechamento/retenção. Exit0 isolado não prova fechamento. Sem retry. Depois, tratar preparação fixture com assurance própria e originais próprios; obter nome/SHA público factual, só então preparar nova sessão READ E/Q/S/P independente. Não reutilizar TO/decisões consumidas nem exigir PID/token futuros antecipadamente. Preservar HEAD/WIP, históricos/pins/EXEs/DLLs, UNKNOWN e PST0.6.4.
