# Staging de qualificação — reconferência e primeira lacuna

Task: PACC-PHASE6-QUALIFICATION-STAGING-READINESS. Data: 2026-10-31.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP preservado.

## Resultado

Os três arquivos foram relidos e seus tamanhos/SHA256 conferem com o inventário
anterior: 9.073.152 bytes. Não houve rebuild, alteração dos payloads ou nova
consulta ao destino. Verificação por caminho nesta observação não constitui
aquisição de originais de uma operação futura ou atestação de imagem carregada.
SDK normal/API3 0.6.5 exclusivamente qualificador; produção PST0.6.4 preservada.

**Pedido NOT_READY. Primeira lacuna: falta a composição de staging qualificada,
no controlador existente, que retenha originais e proteção dos dois diretórios
novos e três arquivos até cópia, revalidação e fechamento.** Não se exige serviço,
CA, root C8 ou identidade de filho futuro para resolver essa lacuna. Não foi
demonstrada necessidade de nova decisão normativa para iniciar sua implementação
offline já autorizada. Esta implementação não foi concluída nesta tarefa.

## Localização técnica

- `apps/server/server_configuration_personal_native_win32.inc`,
  `scm_open_relative`: parent original e OBJ_DONT_REPARSE, mas FILE_OPEN e leitura;
  não cria/copia pacote.
- `apps/server/server_configuration_bootstrap_win32.inc`,
  `bootstrap_create_child`: FILE_CREATE relativo exclusivo, porém um único filho
  no packet bootstrap. Não compõe a árvore binária, descritor explícito na
  criação, três cópias verificadas e conclusão agregada. Seu worker não deve
  receber confirmação NEW_INSTALL reinterpretada como autorização de staging.
- `tools/qualification/scm-runtime-cycle-lab.inc`, `cycle_open_relative` e o
  bloco de cópia/flush: técnicas reutilizáveis, mas paths, pins, lab_aux_gate,
  referências e lifetime específicos do LAB. Não é stager PERSONAL_LOCAL.
- `apps/server/server_launcher_scm_prepare_win32.inc`, `scm_descriptor`:
  predicado owner/DACL de aplicação, não instalador nem prova de proteção dos
  ancestrais Windows. Não presumir owner de aplicação em ProgramFiles.

New-Item/Copy-Item por path não demonstra essas garantias. Expor FILE_CREATE
isoladamente ou chamar o ciclo LAB também não resolve autoridade/lifetime.
Nenhum desses atalhos foi implementado ou executado.

## Operador e alvo

Metadados atuais do processo: `TCORP-TOBIAS\CodexSandboxOffline`, SID
`S-1-5-21-896877160-3262526087-2538948522-1003`. Não é o SID1108 histórico nem
custodiante designado para staging. Não houve consulta elevada. O operador de
uma futura tentativa será o proprietário local elegível, adquirido/confirmado
por gate fresco; não substituir essa aquisição por qualquer dos SIDs históricos.

Destino pretendido `%ProgramFiles%\Papinho\Accelerator`, no plano
`C:\Program Files\Papinho\Accelerator`. Resolver FOLDERID_ProgramFiles, congelar
e comparar antes de confirmação. Reter/revalidar ancestrais originais sem reparse,
sem alterar seus owners/ACLs. Observação anterior provider-not-found não prova
ausência atual ou legitimidade. Existência inesperada de Papinho/Accelerator
implica STOP, sem adoção. ProgramData e root de dados não são pré-condições.

## Recursos e qualificação

Estático: três arquivos/9.073.152 bytes e até dois componentes novos; não é espaço
alocado, reserva de disco ou pico de memória. Censo anterior Selected1.981.344 +
13.060 separados SCM; Legacy1.812.776 +13.060: controlador de registro, não staging.
Baseline/pico nativo do stager, buffers transitórios, hash/CRT, filesystem/kernel,
headroom físico/commit/disco e retenção continuam UNKNOWN, nunca zero presumido.
124/128/1024 conservam as aplicabilidades accepted, sem franquia fictícia para
auxiliares. Samples saudáveis não concedem capacidade nem ceilings universais.

A observação por fases do controlador será útil depois de staging aprovado; sua
entrada após loader/hash não cobre baseline integral, custos SCM ou reserva de
capacidade. F4 permanece por evento/contexto; não importar TTL SCQ/LAB/Browser.

Reutilizados quatro builds/52 CTests/cinco mutantes do relatório anterior, sem
reexecução ou atribuição ao staging. Nenhum código alterado: não existe teste
novo de cópia ou prova nativa de capacidade alegado neste closeout.

## Entregas e sequência

`STAGING-SCOPE.DRAFT.md`: escopo/protocolo a completar, não pedido executável.
`PACKAGE-RECHECK.json`: reconferência. `EVIDENCE-HASHES.json`: hashes das evidências.
`NEXT-QUALIFIED-STAGING-COMPOSITION.md`: próximo patch focal, sem nova auditoria.

Sequência: composição protegida e qualificação offline; fechar pedido byte-exato;
aprovação específica; staging governado; coleta própria do controlador; capacidade
de registro; pedido independente de registro parado; Start diagnóstico/SCQ
separadamente; NEW_INSTALL/CA_INITIALIZE/publicação sob autoridades próprias.

Nenhum efeito protegido, cópia, ACL, SCM, endpoint, Start/Stop, CA/CURRENT,
TPM/Q4 ou cleanup executado. Nenhuma aprovação criada. Somente documentos de
trabalho do repositório foram escritos. E21 PENDING; ED11/Phase6 PARTIAL/DENY.
