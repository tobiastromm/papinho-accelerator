# Critério final de suficiência — staging binário

Task PACC-PHASE6-STAGING-SUFFICIENCY-CRITERION. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Análise estática/documental, sem execução de produto ou alteração de código. D0–D7/hash5cad769d... e binárioeee02c93... reconferidos. Não reabre Stop/FS/producer SCM; resultados anteriores preservados.

## Resultado inequívoco

O critério de admissão pode ser definido e verificado; os seus parâmetros de suficiência ainda não estão todos fundamentados. Não há uma observação read-only que, sozinha, complete os limites de custos opacos da escrita futura ou escolha legitimamente a margem do host. Portanto NÃO há pedido operacional pronto e NÃO se recomenda repetir132/free-space/tracing como substituto dessa lacuna. Não falta provar antecipadamente que a operação terá sucesso; falta fundamentar o recurso/risco admissível para iniciá-la.

## Critério de admissão

Admitir somente na conjunção abaixo, imediatamente antes do consumo:

1. Ato específico válido/atribuído, owner/invocação/operador/alvo/plano/pins originais e imutáveis; nenhuma retenção conflitante/Stop; política aplicável identificada e consumível uma vez. D3/D5, F3/F5/F6. Dados ou autorização normativa isolados não bastam.
2. Recursos fixos/controlados cabem nas quotas accepted aplicáveis, com multiplicidades reais, automáticos/transientes e retidos contabilizados. Custos compartilhados aninhados uma vez. W1–W4, F8, D3/D4. Não importar quota de LAB/TPM/SCQ.
3. Identidade/proteção dos ancestrais e volume adquiridas originalmente/revalidadas; ausência de reparse/substituição e direitos suficientes; criação planejada exclusiva sem adoção. D4/D5 e F3. São fatos de preflight, não promessa de resultado futuro.
4. Para cada recurso sujeito a insuficiência, comparação de disponibilidade fresca com custo futuro ainda não cobrado e margem fundamentada, usando aritmética checada. Handles: H_atual + H_novos_controlados + H_transitorios_previstos deve caber no orçamento de processo justificado; H_atual inclui herança/internos, não baseline descontável. Memória/commit: considerar parcelas ainda a adquirir e custo retido incremental sem somar PrivateUsage/WS como métricas disjuntas. Espaço: caller disponível e limite global compatíveis com projeção de payload e provisão FS fundamentada. D3/D4 e decisão incremental20261012.
5. Todos os termos relevantes são justificados por fonte aplicável, bound ou política operacional legitimamente escolhida e evidência suficiente. Se só houver estimativa observada, identificá-la como tal e explicitar o residual; não tratá-la como teto. A política atual não tem provisão justificada para o residual material: DENY. Não inserir zero,132 ou constante LAB para satisfazer as comparações.
6. Caminho de execução/erro já qualificado: antes de cada efeito dependente, ownership/Stop/identidades e gates aplicáveis; resultados de criação/escrita/flush/readback verificados durante a execução; erro/UNKNOWN bloqueia efeitos novos e conserva cohort. Close só comprovado depois, sem refund/timeout/retry. F5/F6, W5, D4. Não exigir sucesso futuro como input da admissão.

É um critério necessário e composto, não uma API/setter de aprovação. A existência da fórmula não cria valor para nenhuma incógnita. Host margin não é simplesmente available>0; quantidade de handles livres não é derivável de um contador de uso sem política/limite aplicável.

## Custos fundamentados

| Classe | Evidência reutilizada | Limite da afirmação |
|---|---|---|
| Root/packet | Gerido Selected1.988.704/Legacy1.820.136 e quotas correspondentes; staging7360 aninhado | Bound de composição gerida, não teto de memória nativa/host |
| Cópia/hash |4096 e1024/contexto existentes, arquivos serializados, progresso positivo/checked bytes | Não aloca pacote inteiro; stack/CRT/alocador transitivo não limitado por esses dois buffers |
| Referências | Até8 ancestrais +3fontes +2diretórios +3destinos; token/volume/contexto adicionais |16slots não significam16handles totais. STD emprestados não são crédito ou aquisição fechável |
| Plano de efeitos | Até5 criações exclusivas;3destinos sequenciais, flush/readback por arquivo no caminho saudável | Não limita quantas chamadas parciais serão necessárias, tempo de IO ou metadata/kernel |
| Payload | Três arquivos, tamanhos/hashes do pacote; soma e arredondamento por unidade | Não é reserva, teto de diretórios/metadata nem ausência de concorrência externa |
| Retenção | Um cohort original, sem operação adicional de recuperação; sem descarte de backings em pending/close incerto | Custo já comprometido não some; resultado de close futuro não é comprovado antecipadamente |

Nenhum novo ceiling ou quota selecionado. Zero alteração de toolchain/backend/target ou guarda operacional.

## Cada custo opaco: produtor, fase, observabilidade e garantia

| API/componente | Fase e necessidade | Read-only pode observar? | Garantia existente e evidência que falta |
|---|---|---|---|
| Loader/CRT e SHGetKnownFolderPath/CoTaskMemFree | Entrada/resolução obrigatória ProgramFiles, antes do primeiro efeito | Totais por fase sim; amostras109→132 já existem | Conhecido incremento líquido no intervalo, não ownership/pico futuro. Falta provisão nativa/margem aplicável, não mais confirmação de132 |
| GetSecurityInfo e alocação de SD | Preparação e revalidação de originais/criados, antes e durante | Comprimento/custo observado de originais, sim | Liberação/lifetime governados; validação<=65536 ocorre depois da alocação. Falta bound/provisão de alocador/stack, que comprimento final não demonstra |
| Conversão SDDL/SID | Preparação em memória antes do efeito | Pode medir resultado e recursos da preparação | Número de objetos conhecido e LocalFree governado; não há custo de alocador/OS máximo demonstrado |
| NtCreateFile FILE_OPEN | Ancestrais/fontes/token/volume antes do efeito | Sim, com referências originais e close | Originais/proteção e erros verificáveis; recursos internos/kernel não limitados pelo16slots |
| NtCreateFile FILE_CREATE | Dois diretórios/três arquivos; é o próprio primeiro efeito e seguintes | Não sem executar criação | Exclusividade, retorno/disposition e retenção em incerto já qualificados; falta provisão FS/native para iniciar, não prova de sucesso futuro |
| ReadFile/WriteFile/FlushFileBuffers e cache/FS | Leitura/hash antes; escrita/flush depois da criação | Apenas leitura/hash prévia; não custos da escrita futura | Buffers e progresso, retorno, erro e barreiras Stop qualificados; não oferecem teto de cache/metadata/latência. Falta garantia/provisão de suficiência ativa |
| Fechamento/retention | A partir de qualquer aquisição; disposição final depois do efeito | Close de preparação só no seu alcance | Retenção monotônica protege lifetime, não prova quiescência global nem quantifica charges kernel. Custo futuro fica no mesmo envelope; retorno real futuro é outcome |

A API identificar o produtor não transfere ownership de recursos internos ao stager. Não fechar caches/handles por varredura, subtrair herança ou deslocar custo para helper. O custo da eventual observação também precisaria ser contabilizado.

## O que uma observação mínima resolveria — e o que não resolveria

Preflight fresco de originais/volume consegue resolver identidade/proteção/reparse/direitos, unidade de alocação, espaço caller/global, snapshots e close da preparação. Mecanismos estão no owner existente. Não exige serviço SCM futuro ou peer. Deve preservar referências, clocks/erros, pins e limites da política que efetivamente autorizar essa observação.

Não resolve provisão nativa/FS de criação/escrita/flush ainda não executados, margem aceitável ou teto universal. O guard conservador atual corta antes desses dados; este relatório não o remove. Sem política capaz de fundamentar um caminho que alcance o objetivo, emitir outro pedido read-only seria repetir uma operação inconclusiva. Não emitido.

Uma observação de efeito ativo só pode qualificar aquela instância/condições e resultados, sob autorização própria; também não estabelece teto universal por amostra. Tracing de ownership não escolhe margem nem prova reserva de FS. Logo a primeira lacuna restante não é um segredo, PID futuro ou campanha que simplesmente ainda não foi rodada.

## Fronteira factual versus decisão exclusiva

Os contratos accepted exigem capacidade suficiente e negam UNKNOWN material; não fornecem neste domínio um allowance nativo/FS ou margem de host numericamente justificados. APIs e amostras disponíveis não preenchem isso. Se a exigência mantida for bound demonstrado, conservar DENY até existir uma fonte real desse bound/provisão. Se o proprietário desejar um ensaio de qualificação com risco residual explícito e falha governada em vez de tal bound prévio, isso exige decisão focal específica sobre o nível de garantia, não uma medição rotulada PASS. D0–D7 sozinha não contém essa aceitação.

Nenhuma escolha é feita aqui. Nenhuma exceção de UNKNOWN é integrada. Não é necessária uma nova família de ADRs; o ponto de decisão é único: garantia exigida para o custo opaco no ato delimitado. Um ensaio de risco ainda precisaria critérios de admissão, orçamento/margem justificados, recursos de observação e autoridade operacional próprios; dizer apenas “aceito o risco” não inventa parâmetros ou habilita código.

## Código/testes/artefatos

Nenhuma mudança de código necessária à conclusão desta tarefa: gate já nega as parcelas sem fonte e a barreira/Stop não é reaberta. Builds/testes não repetidos. Reutilizados quatro builds sem warnings/68 CTests da integração anterior, incluindo51cenários/152checks de staging e31casos/117checks de recursos por configuração, somente no alcance offline já documentado. HASHES.json registra as fontes atuais, decisões e artefato; exe555008bytes/eee02c930d2a59c8b80e43e544b6d71d34b1a3add91f3b4bed46f41c662791d6 permanece não selecionado para operação. HEAD/WIP/pins/históricos e autorizações consumidas preservados.

## Caminho mínimo

Resolver a fonte de provisão/margem OU a decisão focal de nível de garantia, com consequências explícitas; qualificar somente a alteração legítima decorrente. Depois obter os fatos frescos do alvo por preflight capaz de alcançá-los, se ainda necessários. Preparar pedido de staging apenas com política/artefatos/alvo/gates completos e aprovação específica; depois staging autorizado/readback, pedido separado de registro SCM parado. Não agrupar Start/Stop, SCQ/CNG, CA/CURRENT ou NEW_INSTALL operacional.

Pedido operacional NOT_READY, nenhum comando emitido. Staging DENY; E21 PENDING; ED11/Phase6 PARTIAL. Sem coleta, tracing, staging, diretório/conta/CA operacional, SCM/ACL, rede, endpoint, CURRENT, TPM/NV/Q4, cleanup/refund ou recuperação. Chave fixture, UNKNOWN, reservas e PST0.6.4 preservados.
