# Phase 6 — callers originais e admissão de Start

Task: PACC-PHASE6-ORIGINAL-INGRESS-AND-NATIVE-START-ADMISSION.
Estado factual: **PARTIAL; Start UNKNOWN/DENY; pedido NOT_READY**.
Não é fechamento positivo da cadeia nativa. E21 PENDING; ED11/Phase6 PARTIAL;
OS_QUIESCENCE UNKNOWN; PST0.6.4 preservada.

## Mudanças efetivamente integradas

- A CLI de Start fechado chama o bridge do owner quando falta o carrier original.
  O bridge exige owner canônico ativo, invocação/thread próprias e ausência de
  rede/publicação. Sem isso, nega; não inicializa autoridade a partir de argv.
- Novo `personal_scq_first_controller_win32.inc`, incluído na composição nativa
  existente: sobre referências JÁ capturadas no mesmo packet, liga verificação
  de conteúdo, candidato da seleção, fonte administrativa, confirmação SCQ,
  confirmação INDEPENDENTE de Start e preparo do session driver. As duas
  confirmações não consomem os atos. Nenhum endpoint/SCM/KSP é aberto pelo wrapper.
- Fechamento pré-Start no mesmo owner: libera somente seu perfil ainda não
  consumido e sem transporte/UNKNOWN. Fecha os originais pelo mecanismo existente,
  remove o handler de console e só então conclui o owner. Falha retém e impede
  disposição por saída normal da CLI. Atos consumidos e transporte vivo não
  passam por esse fechamento pré-efeito. O fechamento terminal existente também
  remove o handler da nova composição; falha mantém RETAINED/UNKNOWN.
- Stop preexistente nega antes de instalar handler. Entradas já retidas ou com
  close/hash incerto não são reclassificadas como fonte ausente: propagam UNKNOWN.
- Relatório de recursos inclui sizeof do objeto local do plano (1048 bytes),
  explicitamente distinto de stack total, provisão nativa e crédito. Mantém todos
  os seis termos prévios desconhecidos e três obrigações de efeito.

Callers compilados reais: `server_service_win32.c` ->
`papacc_server_launcher_scq_first_controller` ->
`papacc_configuration_personal_scq_first_controller` -> componentes existentes.
Retorno negativo pré-efeito -> `papacc_server_launcher_scq_prestart_close`.
Declarações e ligações Selected/Legacy foram verificadas pelos builds.

## Alcance que continua incompleto

A aquisição inicial no processo controlador frio **não** foi concluída: falta o
producer legítimo do plano operacional e dos handles que esse caller exige.
O início normal da CLI antecede a aquisição do owner. A nova composição NÃO
supre esse fato por uma inicialização permissiva.

O receptor inicial do ServiceMain **não** ganhou uma fonte positiva: o getter
somente empresta fonte já adquirida; packet vazio continua vazio. A decisão
independente de endpoint também está ausente. Não se removeu a guarda nem se
promoveu o carrier a aprovação. A demonstração exata, a ordem dos callers e os
limites normativos estão em [ORIGINAL-INGRESS-CUT.md](ORIGINAL-INGRESS-CUT.md).
Não se provou impossibilidade de toda solução sob os contratos; os atalhos de
metadados/fixtures é que foram rejeitados. Não se declara conclusão por builds.

## Recursos

[RESOURCE-ADMISSION.md](RESOURCE-ADMISSION.md) distingue custos controlados,
transitórios/opacos, gates prévios e provas posteriores. Censo Selected1993744 /
quota3145728; Legacy1825176 / quota2097152. Estado SCM14928; plano local1048;
carrier308; texto622; nove slots de cadeia. Não somar componentes embutidos nem
tratar slots como contagem do processo inteiro.

`resource_pre_missing=63`; `resource_effect_obligations=7`; recursos permanecem
obrigatórios. F8/TO7 não aceitam risco opaco por consequência de B3 ou R0–R9.
Não se escolheu ceiling/margem/duração/freshness por inferência. PID/token/receipt
futuros não são exigidos antes de Start. Não há suficiência nativa demonstrada.

## Qualificação e limites

Quatro builds Selected/Legacy Debug/Release finais PASS, /W4 /WX, zero warnings.
23 regressões afetadas por configuração, **92 pares distintos PASS**. Não é soma
das campanhas históricas. Detalhes completos, pins e censos em
[EVIDENCE.json](EVIDENCE.json) e nos logs `*-final-build.log`, `*-tests.log` e
`*-detail.log` desta pasta.

Novo teste `scq_first_controller_test`: 20 cenários/194 verificações por
configuração, usando o corpo real da composição, construtor da seleção e
máquinas reais de fonte/confirmação/session. As fronteiras de aquisição/hash,
token/path e entrada de console são MODELADAS; não demonstram atribuição nativa.
Inclui fonte ausente, falha de conteúdo/retida, aquisição original divergente,
console/SCQ/Start cancelados, Stop, preparo de sessão negado, referência ausente,
thread/identidade/descriptor divergentes, ato consumido e replay. Um carrier
válido no packet frio permanece sem fonte. O sucesso modelado prepara sem
consumir, sem transporte e sem RUNNING.

A regressão SCM virtual agora verifica que a CLI chama preparo e fechamento uma
vez na falta de fonte e não chama SCM/Start. O teste canonical-empty exercita os
bridges nativos vazios. Regressões de ServiceMain/primeiro ingress, fonte, carrier,
bootstrap, replay/peer/receipt/close, Stop, transporte e lifecycle permanecem
negativas onde a origem falta. Modelos de efeitos nativos/SCM/RPC/endpoint são
compilados apenas nos testes; não há callback permissivo no produto. Helpers de
SID/SD e hashing público em memória dos modelos são distintos de KSP, RNG,
autenticação bilateral ou consulta operacional. Nenhum processo SCM/endpoint
real foi iniciado/conectado.

Incidentes de qualificação preservados: caminho cmd com separador inadequado
falhou antes do build; corrigido. Novo modelo recebeu C4701 por variável carrier
não inicializada; armazenamento inicializado, sem conceder fonte/autoridade.
Build final limpo. Reruns por correções não são contados como novos pares.

## Imagem e efeitos

[STAGED-IMAGE-DELTA.md](STAGED-IMAGE-DELTA.md): novo EXE617984 bytes,
944535ef159686b4d274fdb6af1c18d9d9dcf2ba045c49c56cc1a7b52ea3f497.
DLLs de workspace inalteradas. Staged histórico c0f27f94… não consultado nem
alterado. Atualização será necessária depois de estabilizar a origem/admissão;
nenhum artefato foi selecionado para operação agora.

Nenhuma consulta ou mutação SCM, Start/Stop, endpoint, staging/update, ACL,
CA/CURRENT, rede operacional, TPM/Q4, cleanup/refund ou recuperação. HEAD/branch
main f341985924d29870d8ff627ed37c8a0874ad6469 preservados; WIP/históricos/pins,
reservas e autorizações consumidas preservados. Nenhum accepted alterado/criado.
Hashes incorporados OHI/B/SSO-B/TO e candidato técnico de owner conferidos.
Future directions relacionadas continuam sem mudança de aplicabilidade.

## Primeiro gate e caminho mínimo

Não existe pedido operacional byte-exato completo ou comando de Start. Primeiro,
fechar a origem da seleção inicial/controller e da admissão inicial de endpoint
no receptor; depois fundamentar a suficiência própria exigida por TO7 (ou obter
uma decisão específica de risco, sem herdar staging/registro). Uma amostra não
resolve provisão opaca. Então estabilizar/fixar artefato, preparar UMA atualização
governada e pedidos independentes de endpoint e Start diagnóstico, com gates
frescos tardios de PID/token/SCM e fechamento. Não repetir staging/CreateService,
coletas ou aprovações consumidas. Nada disso concede readiness produtiva ou
conclui NEW_INSTALL/CA/CURRENT.

[Próxima tarefa focal](NEXT-BOOTSTRAP-SOURCE-AND-START-ASSURANCE.md).
