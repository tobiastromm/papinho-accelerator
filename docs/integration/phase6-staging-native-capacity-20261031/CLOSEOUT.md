# Admissão nativa do staging — gates fundamentados e primeira coleta pronta

Task PACC-PHASE6-STAGING-NATIVE-CAPACITY-CLOSURE, 2026-10-31.
main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Resultado

Integrados ao owner existente critérios negativos de memória/handles/disco,
observação por fases e diagnóstico da primeira causa. **Admissão positiva ainda
DENY**: não foi produzida evidência nativa suficiente de envelope, margem ou
retenção. Não há pedido executável de staging. Em seu lugar foi concluído um
único pedido byte-exato da menor coleta factual read-only, pronto para decisão
do proprietário, não aprovado/executado. Não exige processo SCM, CA, C8,
ProgramData ou criação do alvo para medir o controlador e volume existentes.

## Implementação

`server_personal_resource_sample_win32.inc` reutiliza o sampler anterior; os
wrappers de registro conservam validação do ato original. Não há segundo owner.
`server_staging_resources_win32.inc` usa observações e dados de volume retidos
no packet; aritmética verificada, rejeição de insuficiência/query inconsistente,
mask UNKNOWN e relatórios não secretos. Não cria uma fonte de PASS por flags.

`staging_prepare_mode` separa preparação/ato normal da observação sem RNG ou
confirmação. `papacc_server_launcher_staging_observe` é ligado ao seletor
explícito `--qualification-staging-observe` em main antes da composição de
serviço/runtime. Não é executado por default nem por campanhas de build/teste.
O caminho positivo de cópia continua interno/fechado; não é chamado pelo seletor.
No LAB instrumentado a entrada é negada, sem herdar sua autoridade.

ProgramFiles e fontes são originais retidos, não filhos futuros inventados.
NtQueryVolumeInformationFile consulta quota/espaço pelo mesmo handle, sem reabrir
o destino por path. Primeira causa/localização, predicado nativo, referências e
close controlado são distintos de encerramento interno do OS. Query pending/
contraditória conserva buffer/IO_STATUS_BLOCK no packet e bloqueia nova operação.
Corrigido também o registro de primeira falha genérica quando close falha sem
um erro anterior; esse código não é apresentado como NTSTATUS do OS.
Uma checagem antes de cada aquisição impede acumular originais acima do guard
entre amostras; não altera os limites nem declara pico de handles opacos.

`COSTS-AND-ADMISSION.md` especifica os custos demonstrados e UNKNOWN, sem teto
inventado. O crescimento de packet é320 bytes:7.320 total; censos Selected
1.988.664 e Legacy1.820.096, dentro das respectivas quotas geridas, sem concessão
de capacidade nativa. Guard124/cap128/cohort1024 e retenção não são ampliados.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings, /W4 /WX.
16 CTests afetados/configuração, **64 execuções PASS**. Incluem sampler do
registro refatorado, staging, close guard, quotas/layout, tokens e separação dos
hosts. Regressores repetidos porque o sampler/packet compartilhado mudou;
nenhuma campanha histórica TPM/Browser/rede foi retomada.

Novo foco de recursos: **26 casos/79 checks por configuração**; staging anterior
48 cenários/141 checks preservado. Exaustão física/commit, query indisponível,
PageSize inválido, overflow, quota de disco insuficiente, status/tamanho de
volume inconsistente ou pending, concorrência, retenção após falha parcial,
close incerto, zero RNG/take/efeitos em observação e rejeição de admissão por
contadores saudáveis. Três mutantes comportamentais compilados/rejeitados:
omitir disco, omitir exaustão de memória e tratar volume saudável como capacidade.
Restaurados os fontes; ajustes posteriores foram diagnósticos/relatório, com os
builds e64 CTests finais repetidos. Não se repetiram os mutantes antigos do
staging, que permanecem evidência histórica do escopo anterior.

Queries/Win32 mutante/RNG são simulados nos testes. SHA256 e parsing de SD/SID
em memória são reais. O teste adicional de guard de close executa os seis
estados negativos na implementação real, sem aquisição/close de objetos.
Streams CRT de diagnóstico são temporários reais; nenhum payload ou diretório
operacional foi escrito. O wrapper manual teve somente parsing estático, não
execução ou consulta elevada. CLI read-only real não foi executada.

## Artefatos e próximo gate

O rebuild mudou o pacote:3 arquivos, **9.092.096 bytes**, não os9.073.152
históricos. `QUALIFICATION-BUNDLE.json` fixa tamanhos/paths/hashes finais.
Servidor552.960 bytes, SHA256
`74f430e0bd86529396bd267c8ca40548218dc83611ef126c24e72da70388e8ad`.
As DLLs permanecem iguais. Não substituir pins históricos ou tratar isso como
seleção/autorização operacional de staging/release. Produção PST0.6.4 preservada.

`OBSERVATION-REQUEST.CANDIDATE.md` delimita uma única invocação manual e todos os
bytes/efeitos/gates/STOP. `collect-staging-resources.MANUAL.ps1` tem paths/pins
reais e verificação anterior à execução; sem comando com placeholders ou
aprovação fabricada. O script só poderá ser executado após decisão específica.

Primeiro gate restante: **aprovação específica desse pedido de coleta read-only**,
depois reconciliação factual, sem transformar amostras em ceilings. A coleta
não prova custos de writes/metadados ou reserva física; resultado não suficiente
permanece UNKNOWN/DENY. Após capacidade/bindings completos: pedido de staging
separado; depois evidência do controlador de registro e pedido independente de
registro SCM parado. Start/Stop, SCQ, parent de dados e NEW_INSTALL separados.

E21 PENDING; ED11/Phase6 PARTIAL. Sem staging, instalação, SCM/ACL, endpoint,
KSP/ownerAuth, CA/CURRENT, rede operacional, TPM/Q4 ou cleanup/refund. Nenhuma
aprovação criada, nenhum ADR accepted alterado, nenhum segredo solicitado.
