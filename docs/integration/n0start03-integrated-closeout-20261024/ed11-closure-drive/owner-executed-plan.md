# PapinhoAccelerator — ED11 Final Closure Drive

TASK=PACC-PHASE6-ED11-FINAL-CLOSURE-DRIVE-20261024
PROJECT=C:\Projetos\PapinhoAccelerator
MODE=AUTONOMOUS_TECHNICAL_CLOSEOUT_WITH_EXPLICIT_OPERATIONAL_GATES

## Objetivo
Encerrar o máximo possível do ED11 de forma concentrada e factual, sem novos ciclos de preparação documental redundante. Trabalhar no projeto canônico e preservar o WIP e a evidência histórica. Não declarar PASS nem encerrar Phase 6 sem evidência dos gates aplicáveis.

## Baseline verificada no relatório recebido
- Qualificador Selected Release SHA256: deeff3b9911ea720ebf5df2c31a1ae10657a19a7d64b6e37fee48029aee1944f
- Launcher SHA256: daf6c078e08474cbb112198a02dcbc90efe441344a530f34e8ea8017b287c8ee
- Pedido DRAFT SHA256: 6355099741186d2a0443a292b0dbe10953b4f23591dcfdd1e5d4590d16e1d717
- Selected Release PASS zero warnings; 15 testes offline PASS anteriores, revalidados sem reexecução.
- Registro EXACT-FRESH-READONLY-APPROVED.json ausente; execução ainda não autorizada.
- ED11 PARTIAL; positive path NOT_DEMONSTRATED; CURRENT/signature/authoritative recovery DENY.

## Trabalho autônomo autorizado nesta tarefa
1. Ler INTEGRATED-CLOSEOUT.md, READONLY-OPERATION-REQUEST.DRAFT.json, o ADR e as autoridades N0–N9/L0–L9/H0–H9/RAW1/ED11/Model E e os recibos anteriores. Não presumir que o relatório sozinho substitui esses artefatos.
2. Construir uma matriz mínima e completa de gates restantes do ED11: gate, evidência atual, lacuna factual, teste ou medição que a fecharia, dependência operacional, condição de STOP. Separar explicitamente fechamento ED11 de fechamento global Phase 6, incluindo outros slices abertos.
3. Analisar a lacuna do pico de handles e do orçamento de 128 nativos/1024 shared-admin, incluindo janela pré-PSS, captura, OpenThread, VirtualQueryEx, self-token condicionado a positive&&n0, e fechamento/retention. Reutilizar a instrumentação existente; se necessário, implementar a menor instrumentação **observacional e opt-in**, compatível com o ambiente e sem alterar a política, os limites ou a semântica do caminho positivo. Medir em testes offline/controlados tudo o que não requer autoridade operacional. Não inferir pico nativo de fixture.
4. Corrigir autonomamente defeitos técnicos demonstrados dentro desse escopo, fazer rebuild/relink, atualizar hashes/pins derivados e repetir regressões afetadas. Não criar novas árvores de produto nem duplicar owners, launchers ou mecanismos de aprovação. Parar apenas se uma correção exigir alterar uma decisão normativa ou ultrapassar uma fronteira operacional protegida.
5. Preparar de modo completo o próximo procedimento operacional com as etapas, verificações e critérios de aceitação que efetivamente faltam. Não multiplicar pacotes candidatos sem motivo; atualizar o pacote canônico e apontar o relatório principal.
6. Caso uma observação operacional seja necessária, distinguir rigorosamente: (a) proposta pronta; (b) decisão explícita do proprietário; (c) registro aprovado exato e seu SHA256; (d) execução manual; (e) reconciliação factual. Não converter (a) em (b).
7. Após receber evidências de execução manual em rodada posterior, analisar e fechar autonomamente todos os gates que essas evidências suportarem; caso o read-only passe mas o positivo permaneça não demonstrado, preparar a menor qualificação positiva separada, sem declarar autorização para ela.

## Limites obrigatórios
- Este prompt NÃO é autorização para registrar aprovação, executar Prepare nativo, preflight, StartService, StopService, Cycle, mutações SCM/ACL, arm/consume de intent, criar imagem/sink, rebind, elevar quotas, publicar CURRENT, assinar ou fazer recuperação autoritativa.
- Não gerar aprovação em nome do proprietário, não reutilizar a histórica e não preencher hash de aprovação com hash de DRAFT/candidato.
- Não alterar os guards, a semântica fail-closed, UNKNOWN, retention, ownership, Model E, ou o predicado positive&&n0 para fabricar uma medição favorável.
- Não executar um Start positivo sem autorização operacional distinta e gates comprovados.
- Preservar o estado de recursos e a primeira falha; não forçar cleanup, kill, rollback ou release de mutex em caso incerto.
- STOP somente na primeira divergência de segurança/autoridade/contrato ou quando uma operação protegida exigir consentimento específico; divergências técnicas corrigíveis dentro do escopo devem ser resolvidas e retestadas.

## Entrega única e objetiva
- ED11_GATE_MATRIX com PASS/PARTIAL/DENY/NOT_RUN e evidência rastreável.
- TECHNICAL_WORK_DONE, BUILDS, TESTS, ARTIFACT_SHA256, FIRST_UNRESOLVED_FACTUAL_GATE.
- READONLY_OBSERVATION_APPROVAL_REQUIRED=YES (até aprovação explícita).
- POSITIVE_PATH_QUALIFICATION=NOT_DEMONSTRATED até prova factual.
- ED11_TOTAL_STATUS=PASS apenas se todas as condições normativas forem efetivamente satisfeitas; senão PARTIAL.
- PHASE6_TOTAL_STATUS separado de ED11; não confundir os dois.
- NEXT=um único passo concreto de maior valor para fechar o gate restante, com procedimento pronto, sem nova microtask documental.

Prioridade: concluir o trabalho técnico, reduzir a distância para a prova operacional e não criar mais uma rodada de revisão de pins sem mudança factual.
