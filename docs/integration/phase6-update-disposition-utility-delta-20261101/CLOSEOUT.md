# Disposição UPDATE e manutenção do utilitário — preparação concluída

2026-11-01, main/HEADf341985924d29870d8ff627ed37c8a0874ad6469. Owner existente,
WIP e históricos preservados. Nenhuma consulta da instalação/SCM, execução do
produto, alteração ACL, preparação, UPDATE, retry ou rename nesta tarefa.

## Estado conhecido e o que continua UNKNOWN

Última tentativa: autorização consumida, criação de candidato confirmada,
first0xC0000043/location701, nenhum rename confirmado, close_known0. Wrapper
fechou suas referências, mas cohort do owner permanece UNKNOWN. Saída histórica
4a7e4dbc…9dfd4 preservada na frente anterior; não há nova evidência dos objetos.
Não declarar canônico atual, previous ausente, crédito liberado ou quiescência.

Conflito próprio corrigido offline no mesmo owner: RootDirectory NULL/leaf
simples para rename same-directory, sem mudar shares/lifetimes/guards/readbacks.
Quatro builds zero warnings,64 pares afetados PASS, UPDATE106/634 por configuração,
evidências em phase6-first-rename-sharing-20261101. Não repetir campanha inalterada.

## Primeiro: pedido read-only pronto para autorização

[ONE-READONLY-DISPOSITION-REQUEST.CANDIDATE.md](ONE-READONLY-DISPOSITION-REQUEST.CANDIDATE.md), SHA256
079f72d1e09f48c0fc5a098735add1df5c3d8976cb0e20b4ca1be46c6b6fb483.
READY_FOR_SPECIFIC_MANUAL_AUTHORIZATION, NÃO AUTORIZADO/NÃO EXECUTADO.
Procedimento SHA25646b0e72f909d09d93a3f9f921d67900dcff8a02667a2cab93e407957f7abd96e.

Uma consulta manual só: cadeia/parent original e quatro leaves exatos do plano
consumido (canônico, candidato, previous, utilitário), metadados/IDs/hash/SD/ACEs,
três queries SCM qc/queryex/qsidtype, sem produto executado ou qualquer mutação.
Sem enumeração de outros objetos, tracing ou campanha de contagem de handles.
Reutiliza originais/interop in-memory/OPEN_EXISTING e preserva share READ.
Se conflitante, UNKNOWN/STOP, nunca forçar acesso. Somente erro2 é ausência
snapshot com parent retido;3/5/32 e demais falhas não são ausência.

Qualificação WinPS5.1x64:21 checks PASS em funções isoladas sobre workspace,
incluindo error2 real nativo, demais classes negativas modeladas, hash/SD,
continuidade/substituição e close. Nenhum procedimento completo/alvo/SCM executado.
Teste inicial detectou que SetLastError não ficava registrado ao reaplicar
atributo sobre DefinePInvokeMethod; corrigido definindo métodos/atributos
PInvoke com SetLastError explícito. Falha inicial e resultado final preservados.
Não converter falta de erro capturado em ausência. PInvoke tem apenas open/
file-info/close, sem APIs de efeito.

Fontes técnicas primárias:
- https://learn.microsoft.com/en-us/dotnet/api/system.runtime.interopservices.dllimportattribute.setlasterror
- https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew

Resultado futuro só estabelece disposição de snapshot e close da consulta.
Não resolve automaticamente cohort/créditos anteriores nem quiescência universal.
Não há comando operacional emitido antes da aprovação específica do pedido.

## Segundo: delta exato do utilitário

| Papel | Bytes | SHA256 | Estado |
|---|---:|---|---|
| Utilitário protegido histórico |104448|6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87|última evidência; ID417614100:1507328:540527; não reconsultado|
| Utilitário corrigido workspace Selected/Release |104448|be024e0bf0dbf5ee93fd4dde0bf0124233d0735d59f89488a57736e2a2817510|build qualificado, não instalado/selecionado operacionalmente|
| Server corrigido workspace |758272|841e9f8c2d6c877c9b415e51d3976e6022094bd21b8ace69e2377008a6fcaed4|mesmo owner corrigido; controller de planejamento, não protegido/selecionado para manutenção|

Delta funcional: função de rename muda root não nulo para NULL, valida bit/slot
e leaf simples; nenhuma mudança de share masks/packet/censo. K_UPDATE existente
2027314 Selected/1858746 Legacy. Isso não é K de ato de manutenção ainda não
implementado. DLLs de build permanecem idênticas. Não foram lidos bytes atuais
do utilitário protegido para comparação de offsets; não inventar ranges binários.
Manifesto de builds anterior e hashes atuais de workspace conferidos; nenhum
novo build/relink do produto feito nesta tarefa.

## Terceiro: menor caminho governado e lacuna normativa

U1 accepted restringe efeito a papacc_server.exe. U2 exige executor protegido
independente e proíbe autossubstituição do executor vivo. Preparação accepted
029a611b…07fb é primeira criação exclusiva papacc_update.exe, existente nega,
sem rename/overwrite; bootstrap daquele ato não se transporta. Portanto não há
permissão vigente de substituir utilitário, mesmo com uma nova autorização de
execução dos pedidos anteriores. Não relaxar U2 nem renomear arquivos para
contornar o escopo. Não há conflito entre accepted; há efeito novo não coberto.

Menor solução recomendada: mesmo controller server de build e owner existentes,
ato independente restrito à manutenção de UM utilitário, com bootstrap
explicitamente escolhido somente nesse ato. Candidato DATA, criação exclusiva
protegida, cópia/hash/flush/readback, preservar utilitário antigo em previous e
dois renames same-directory sem overwrite; não executar o utilitário-alvo para
se atualizar. Server/candidato anterior/DLLs/SCM/ProgramData não são tocados.
Isso reutiliza primitivas e não cria updater-do-updater, serviço, distribuição
ou root. Controller protegido adequado já existente continua não demonstrado;
PowerShell protegido sozinho não é implementação desse owner.

Única proposta focal preparada, NÃO ACEITA e sem implementação dependente:
[UTILITY-REPLACEMENT-SCOPE.CANDIDATE.md](UTILITY-REPLACEMENT-SCOPE.CANDIDATE.md), SHA256
00d0cd8dca8ebea3d221a5e0d9b09281ab42a300b93d3dbd52a294ca804003b6.
Delimita exatamente alvo/purpose utilitário, bootstrap e custos opacos próprios;
preserva U2 para UPDATE servidor. Contrapõe alternativa estrita que conserva
DENY sem executor adequado. Não solicita escolher hash final antes do código
necessário; integração futura pode mudar esses bytes, exigindo qualificação
e seleção final única. Aprovação normativa NÃO autoriza operação.

## Gates e caminho mínimo

Primeira operação autorizável: uma observação read-only, somente após aprovação
do pedido exato e ação manual. Em paralelo documental, proprietário pode decidir
sobre especialização focal, sem produzir efeitos. Depois de aceitação válida,
integrar/qualificar manutenção no owner e finalizar artefatos/pedido operacional
próprios, condicionados à reconciliação factual e preservação das incertezas.
Nenhum retry de UPDATE preparado enquanto retenção/disposição não resolvidas.

Read-only→reconciliação dos objetos, sem limpeza/crédito presumido→decisão focal
de manutenção→integração/qualificação→pedido operacional próprio do utilitário→
evidência de executor corrigido→UPDATE server e primeiro Start sob atos separados.
Nenhuma etapa cria infraestrutura paralela. Custos opacos/OS_QUIESCENCE UNKNOWN;
E21 PENDING, ED11/Phase6 PARTIAL, produção DENY/PST0.6.4 preservado.
