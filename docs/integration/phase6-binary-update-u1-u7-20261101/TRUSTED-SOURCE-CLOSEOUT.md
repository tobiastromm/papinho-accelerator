# Origem do UPDATE — conclusão focal

2026-11-01. Estado: BLOCKED_SOURCE_PROVISIONING / DENY. Não é nova proposta
normativa, aprovação ou pedido operacional. Branch main, HEAD
f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Causa e alcance da evidência

location108 é `staged_update_object(8,1)`: o novo EXE no workspace.
O código abre a fonte antes de consultar SCM, preparar o plano, solicitar a
confirmação e consumir/criar. A saída histórica não distingue o subpredicado
interno. Permanecem possíveis metadados/tipo/reparse/volume e a conjunção do
descritor/digest; não se afirma que o owner do snapshot seja o owner daquele
handle. `native_predicate=0` não resolve essa ausência.

SOURCE-ACL-SNAPSHOT.json é evidência já existente do workspace do agente,
não coleta nova no host. Owner observado termina em -1003 de outra autoridade
SID, diferente do operador selecionado. ALLOW AU Modify/Synchronize
0x001301bf e duas outras ACEs de escrita são herdadas e aplicáveis ao arquivo,
sem INHERIT_ONLY. Sob `scm_descriptor` o owner não admitido já rejeita; AU
Modify é uma rejeição independente. DACL não protegida, isoladamente, não
rejeita slot8: o código dispensa expressamente esse bit para a fonte.

Não foi feito AccessCheck com token operacional; classificar a DACL não é
provar todos os acessos efetivos de todos os participantes. DELETE no arquivo
é relevante; substituição também depende de DELETE_CHILD/WRITE_DAC/WRITE_OWNER
nos ancestrais. Não há evidência original retida dos ancestrais da fonte,
reparse de toda a cadeia ou continuidade do handle histórico. Esses campos
continuam UNKNOWN, sem nova consulta operacional. A abertura existente usa
nome nativo absoluto para a fonte: não confundir a validação dos ancestrais
do destino com aquisição relativa de toda a cadeia da fonte.

Não há defeito demonstrado que permita relaxar owner/DACL. Hash aprovado,
FileShare.Read e elevação não tornam um controller substituível confiável.
O snapshot não prova que a ACL Windows esteja errada.

## Aplicabilidade e reutilização

- U1: ato BINARY_UPDATE_CLOSED independente, política fora dos candidatos;
  arquivos/pins são DATA. Isso não torna confiável o programa elevado que
  interpreta a política e recebe a confirmação.
- U2: owner/DACL efetiva, ancestrais, ausência de reparse, identidade e
  continuidade de cada objeto permanecem. Não exige um nome de distribuição
  específico nem que toda origem seja exterior ao workspace. Um workspace
  que comprovasse essas propriedades poderia satisfazer a guarda; a fonte
  observada não as demonstra.
- U3/U5: o candidato protegido no destino só pode ser criado depois da
  confirmação/consumo. Usá-lo como preparação anterior inverteria a ordem;
  ele não pode fornecer retroativamente confiança ao controller.
- U4: reduz garantia sobre custos opacos; não reduz U2.
- F3/F4/F5: referências, tentativa e confirmação protegidas não são strings,
  hashes nem autorização restaurável. F0 não converte NEW_INSTALL em UPDATE.
- B0/B3/B8: o ensaio anterior teve pacote/alvo/tentativa próprios. Sua
  autorização está utilizada; não é autorização de distribuição genérica.
  D0–D7 delimita recursos, não cria um novo efeito de preparação.

`staging_apply_composition` cria exclusivamente Papinho e Accelerator sob o
ProgramFiles original e os três leaves fixos. `staging_acl` permite fonte DATA
com SD válido, retido e revalidado; valida estritamente os objetos que cria.
Isso explica por que staging de bytes e origem protegida UPDATE são requisitos
distintos. Não há caller existente para criar uma distribuição alternativa
no parent instalado. Chamar novamente o stager colidiria com o alvo existente;
não pode ser transformado em adoção ou cópia genérica.

O EXE antigo protegido não implementa o UPDATE novo; não é um atualizador
alternativo disponível. O snapshot preservado de build não é distribuição
protegida por ter outro nome ou hash igual. Nenhuma nova confiança foi provada.

## Origem concreta recomendada, ainda não selecionada nem criada

Propor um diretório de artefatos de distribuição, não root administrativa,
dentro do parent protegido já existente:

`%ProgramFiles%\Papinho\Accelerator\distribution-42d468faf367`

Resolver known folder e congelar caminho antes da confirmação. Não selecionar
existência/identidade futura por inferência. Colisão nega, sem adoção ou leaf
alternativo automático. Esse nome é recomendação DATA e requer seleção própria.

Pacote de planejamento, hashes conferidos no workspace nesta tarefa:

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe | 737280 | 42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Total 9276416 bytes. Incluir DLLs evita executar o controller novo com resolução
de dependências a partir do workspace. Não altera as DLLs canônicas. Ainda é
necessário qualificar o carregamento real; hash on-disk não atesta imagem mapeada.

Preparação mínima teria quatro criações exclusivas (um diretório, três arquivos),
nenhum rename/overwrite/ACL edit em objeto existente. Reutilizar apenas os
primitivos existentes de captura relativa, criação com descritor novo protegido
BA/SY full e AU read/execute, cópia limitada/hash/flush/readback, Stop e retenção.
Manter identidade original e direitos mínimos em todas as fases. Consumo próprio
antes da primeira criação; falha parcial conserva objetos, sem delete/rollback.
Verificar espaço arredondado real e custos próprios; não importar B3 ou U4
automaticamente para essa preparação. Não inventar file IDs ou capacidade.

## Primeira lacuna e pedido autorizável

O primeiro bloqueio é a ausência de uma composição governada de preparação
de distribuição com origem de confiança do EXECUTOR estabelecida antes de
executá-lo elevado. O controller recusado não pode autoatestá-la por copiar
seus próprios bytes. Executar script/Copy-Item elevado com hashes não é um
substituto automaticamente qualificado dos mecanismos de confirmação/custódia.

U1–U7 cobre atualização canônica, B0–B8 o ensaio anterior exclusivo. Não há
autorização vigente para as quatro criações propostas nem caller preparado
com esse escopo. Isso é falta de cobertura para outro efeito, não contradição
entre normas nem necessidade de reaprovar os riscos U1–U7.

Pedido operacional: NOT_READY. Não emitir comando, pedir retry UPDATE ou
apresentar preparação como executável. A menor decisão eventual deve nomear
somente o executor legítimo/proveniência e a preparação exclusiva acima;
deve preceder implementação positiva que dela dependa. Não criar ADR, owner,
root, serviço, ledger ou helper por rotina. Se uma fonte de distribuição já
protegida e legitimamente obtida for disponibilizada, ela evita novas cópias
locais, mas exige a mesma aquisição/revalidação original e nova seleção do
pedido UPDATE; não se afirma que tal fonte exista hoje.

## Qualificação e caminho mínimo

Nenhum código, build, binário ou quota foi alterado. Não há correção funcional
justificada para qualificar nesta tarefa; campanhas históricas não repetidas.
A qualificação anterior não é reapresentada como prova da nova preparação.
Revisão estática dos callers/guardas, hashes dos três arquivos e JSON de
evidências conferidos. Nenhum EXE/SCM/coletor foi invocado.

Próximas fases estritas: estabelecer executor/origem e cobertura do efeito
de distribuição; implementar e qualificar essa preparação no owner existente
se aplicável; selecionar pacote/destino e autorizar preparação separadamente;
reconciliar identidade/proteção; então novo pedido UPDATE U1–U7; só depois
endpoint e Start diagnóstico sob autorizações distintas. Não repetir staging
canônico ou registro SCM.

DENY; E21 PENDING; ED11/Phase6 PARTIAL; SCM atual e quiescência OS UNKNOWN.
Autorização baa501…4726 utilizada, não reutilizável, mesmo consumed interno0.
HEAD/WIP, EXE anterior, pins, históricos, fixture/reservas e PST0.6.4 preservados.
