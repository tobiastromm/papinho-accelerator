# P0–P7 — integração offline do alvo existente

2026-11-01. INTEGRATION_PARTIAL / OPERATIONAL_DENY.
Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Decisão e precedência

Original EXISTING-PROGRAMFILES-ORIGINAL-BINDING.CANDIDATE.md conferido integralmente:
SHA256 f2dd0651f1a546b47f5edb756dbfa815533a8c883ceda121a20a956af8f0bb28.
Registro anterior ao código: ../../adr/DECISION-personal-scq-existing-target-p0-p7-20261101.md.
Original e status histórico CANDIDATE preservados. Exclusivamente P0–P7 no domínio
PERSONAL_LOCAL/TRANSPORT_ONLY/x64. BDI continua para NEW_INSTALL; OHI/SSO-B,
QUERY/G0–G9/G3/G4, SCQ, identidade, proteção, originais e receipt/close permanecem.
Sem autorização operacional, dispensa de quota ou recertificação do mapping atual.

## Código integrado

personal_install_paths_win32: seleção candidata privada tipada, separada de NEW,
BINARY_ONLY e paths_existing. GetModuleFileNameW só localiza; não prova papel ou
autoridade. Verifica AMD64, forma relativa Papinho/Accelerator, proveniência e
três identidades como DATA. Receptor não importa IDs esperados do caller.
Validadores genéricos negam o modo novo sem prova própria do image_chain.

personal_scq_image_chain_win32: readquisição relativa pelo mecanismo existente,
handles retidos, identidade/SD/owner/DACL/ancestrais/reparse e continuidade.
Guard completo124 antes de cada abertura. Relação ProgramFiles/Papinho/
Accelerator/imagem obtida da cadeia real; divergência nega antes de confirmação.

personal_scq_cold_acquisition e inputs: caminho condicional para seleção P1
tipada, captura original antes do target final, custódia no packet existente,
revalidação e close também para aquisição anterior à inicialização dos inputs.
Falha incerta mantém a coorte, sem retry/recuperação/refund.
Entrada de custódia no owner aceita somente o ramo P0 já validado: role1,
token original atual, cadeia do mesmo owner, perfil TO pré-readiness aguardando
runtime, sem inputs anteriores. Não recaptura token nem altera o ramo estrito
READ/NEW_INSTALL. A revisão encontrou e corrigiu a exigência anterior de modo NEW.

server_configuration_personal_native: recomposição independente somente em
QUERY TRANSPORT_ONLY fechada, pré-readiness e closed_preparation. Demais perfis
mantêm caminho anterior. Target/QUERY são candidatos; autenticação bilateral,
origem administrativa, consumo e receipt/close permanecem indispensáveis.
Sem novos formatos SCQ1/SQB1/SQB2, receipts/close, públicos ou persistidos.

## PXT1 — binding privado canônico

428 bytes, DWORD little-endian, sem padding/pointers/handles:

| Offset | Bytes | Conteúdo |
| --- | --- | --- |
| 0 | 12 | tag 0x31545850, AMD64 0x8664, propósito |
| 12 | 36 | IDs volume/high/low de ProgramFiles, parent binário e imagem |
| 48 | 96 | SHA256 dos três descritores originais retidos |
| 144 | 160 | request, decisão, tentativa, policy/G3, proveniência |
| 304 | 36 | PID e creation/token/authentication/modified do controlador |
| 340 | 72 | tamanho SID e SID em campo fixo de 68 bytes |
| 412 | 16 | valid_from/valid_until originais |

SHA256 reutiliza target opaco. Receptor calcula com aquisições próprias e
projeção original do controlador. Divergência nega; digest não concede autoridade.
Backing de hash/descritores incertos permanece retido, sem presumir quiescência.

## Primeira lacuna: producer operacional

Aquisição/validação e binding nas duas pontas integrados e exercitados em modelo.
Producer administrativo P1 NÃO concluído: server_launcher_scq_cold_start_win32.inc
ainda expõe só o entrypoint anterior e inicializa input.existing como NULL.
Não há caller operacional legítimo fornecendo a seleção tipada da nova sessão.
Sem isso o controlador conserva o percurso anterior; não existe fallback positivo.
Também não há novo pedido/decisão operacional selecionando expressamente o alvo;
atos consumidos ou esta aceitação normativa não substituem esses originais.

A revisão automática rejeitou um patch combinado que ampliava a CLI com IDs/
proveniência e trocava o caminho genérico do receptor: risco de extrapolar
TRANSPORT_ONLY e de regressão de autoridade/guards. Esse patch não foi aplicado.
Aplicou-se somente a alternativa delimitada no receptor por quatro condições
explícitas e adaptadores tipados sem novo ingresso CLI. Não contornar a rejeição
por script/comando indireto. A lacuna do caller fica exposta.

## Builds e testes

Selected/Legacy Debug/Release: quatro builds PASS, /W4 /WX, zero warnings nos
builds finais. Logs finais p0-<perfil>-<config>-custody-build.log.
11 CTests afetados/configuração: 44 pares PASS; -custody-tests.log.
As verificações anteriores foram sucedidas por essa campanha final porque a
auditoria revelou a necessidade de corrigir a admissão dos readers.

Novo --existing-target: 17 cenários/95 checks; arquivos/NT modelados. Cobre
válido, ID/SD/reparse/cadeia ausente/arquitetura/campo de outro perfil, Stop,
READ, consulta insuficiente, target divergente, recomposição do receptor,
request/proveniência/substituição e close UNKNOWN sem retry. O cenário17 modela
raw3+cadeia5+duplicatas3 e verifica negação por quota ANTES de abrir reader em124.
Revisão encontrou falta da contagem antes de ReOpenFile: corrigida no perfil TO
com cadeia original, estágio de primeira causa31; demais perfis preservados.
Regressões existentes
cobrem consumo/replay, G3, bilateral incompleta, receipt/close e readiness.
Sem consulta real do alvo, RNG, SCM, endpoint ou Start. Modelo não prova sessão nativa.

## Recursos

Guard124/cap128/G3 intactos, contagem integral. Nenhuma economia real medida.
Observações consumidas112/112/112/113/136 permanecem históricas; +23 não atribuído
a COM/Shell/vazamento. Não fechar internos do OS.
Censo sizeof Selected KNOWN=2002000 (antes2001440), Legacy=1833432
(antes1832872): +560 bytes por perfil no owner existente. Não é consumo privado/
WS ou margem física/commit medida. Provider/kernel continuam opacos.

Submodelo de coexistência até custódia: B=total imediatamente após token,
incluindo herdados/console/CRT e demais referências presentes. Cadeia de quatro
diretórios e imagem: raw3 => B+3; cadeia5 => B+8; duplicatas3 => B+11.
Antes de cada aquisição a contagem deve ser<124. A API session_acquire_originals
também abre readers3 ANTES de retornar ao caller que fecha raw3: pico B+14.
Depois do fechamento conhecido dos raw fica B+11. Não fechar originais retidos
antecipadamente. B=113 seria apenas exemplo algébrico: B+11=124 já impediria
o PRIMEIRO reader pela guarda; não é previsão da nova sessão. Para completar
apenas essas 14 aquisições, sem outros custos, o último pre-open exige B+13<124,
portanto B<=110. Hashes, SCM, peers e sobreposições posteriores não são
subtraídos: G3 e contagem integral nos respectivos gates. Esse submodelo não é
ceiling do processo ou suficiência de toda a sessão.

## Imagem e próximo gate

Workspace Selected/Release: 771072 bytes,
SHA256 3e2cf25038e6435194293a06bacf86adbb4d4f3af3f90688f60ddd00797093c3.
Não instalado/selecionado operacionalmente; pode mudar ao concluir o caller.
Imagem instalada histórica: 763904 bytes, ID417614100:1835008:134613,
SHA2567d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d.
Nenhum readback fresco imagem/SCM. DLLs/configuração/EXEs anteriores intactos.
Atualização será necessária, mas não congelar UPDATE com caller funcional pendente.

Primeiro gate: ligar seleção P1 legítima no entrypoint existente sem promover
argumentos a autoridade, reutilizando origem/custódia/confirmadores. Depois:
estabilizar artefato/censo, preparar UPDATE específico e nova sessão E/Q/S com
originais e gates frescos. Sem nova coleta genérica exigida por este relatório.
Start NOT_READY. Retenção histórica, causa histórica1816 e OS UNKNOWN;
E21 PENDING; ED11/Phase6 PARTIAL/DENY; PST0.6.4 e atos consumidos preservados.
